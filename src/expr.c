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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश "system.h"

#समावेश <gmp.h>
#समावेश <regex.h>
#समावेश "c-ctype.h"
#समावेश "long-options.h"
#समावेश "mcel.h"
#समावेश "strnumcmp.h"

/* Various parts of this code assume size_t fits into unsigned long
   int, the widest unsigned type that GMP supports.  */
static_assert (SIZE_MAX <= अच_दीर्घ_उच्च);

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "expr"

#घोषणा AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
क्रमागत
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
क्रमागत valtype
{
  integer,
  string
};
प्रकार क्रमागत valtype TYPE;

/* A value is.... */
काष्ठा valinfo
{
  TYPE type;			/* Which kind. */
  जोड़
  {				/* The value itself. */
    mpz_t i;
    अक्षर *s;
  } u;
};
प्रकार काष्ठा valinfo मूल्य_2;

/* The arguments given to the program, minus the program name.  */
अटल अक्षर **args;

अटल मूल्य_2 *eval (bool);
अटल bool nomoreargs (व्योम);
अटल bool null (मूल्य_2 *v);
अटल व्योम printv (मूल्य_2 *v);


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
अटल माप_प्रकार
mbs_logical_cspn (अक्षर स्थिर *s, अक्षर स्थिर *accept)
{
  अगर (accept[0] == '\0')
    वापस 0;

  /* General case.  */
  अगर (MB_CUR_MAX > 1)
    {
      माप_प्रकार idx = 0;
      क्रम (अक्षर स्थिर *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          अगर (g.len == 1)
            {
              अगर (mbschr (accept, *p))
                वापस idx;
            }
          अन्यथा
            क्रम (अक्षर स्थिर *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                अगर (mcel_eq (g, h))
                  वापस idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  अन्यथा
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      माप_प्रकार i = म_खोज (s, accept);
      अगर (s[i])
        वापस i + 1;
    }

  /* not found */
  वापस 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
अटल अक्षर *
mbs_logical_substr (अक्षर स्थिर *s, माप_प्रकार pos, माप_प्रकार len)
{
  माप_प्रकार mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? म_माप (s) : mbslen (s); /* logical length */

  /* characters to copy */
  माप_प्रकार vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  अक्षर स्थिर *substart = s;
  idx_t sublen = 0;
  अगर (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  अन्यथा अगर (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  अन्यथा
    क्रम (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        अगर (pos <= idx)
          {
            अगर (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  वापस ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
अटल माप_प्रकार
mbs_offset_to_chars (अक्षर स्थिर *s, माप_प्रकार ofs)
{
  माप_प्रकार c = 0;
  क्रम (माप_प्रकार d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  वापस c;
}



व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s EXPRESSION\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);
      अक्षर_दो ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Print the मूल्य of EXPRESSION to standard output.  A blank पंक्ति below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 अगर it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 अगर neither argument is null or 0, otherwise 0\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal to ARG2\n\
  ARG1 = ARG2       ARG1 is equal to ARG2\n\
  ARG1 != ARG2      ARG1 is unequal to ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal to ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
  ARG1 + ARG2       arithmetic योग of ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference of ARG1 and ARG2\n\
"), मानक_निकास);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      ख_माला_दो (_("\
\n\
  ARG1 * ARG2       arithmetic product of ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient of ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder of ARG1 divided by ARG2\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
  STRING : REGEXP   anchored pattern match of REGEXP in STRING\n\
\n\
  match STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS लंबाई_2   substring of STRING, POS counted from 1\n\
  index STRING CHARS         index in STRING where any CHARS is found, or 0\n\
  लंबाई STRING              लंबाई of STRING\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  + TOKEN                    interpret TOKEN as a string, even अगर it is a\n\
                               keyword like 'match' or an चालक like '/'\n\
\n\
  ( EXPRESSION )             मूल्य of EXPRESSION\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Beware that many operators need to be escaped or quoted क्रम shells.\n\
Comparisons are arithmetic अगर both ARGs are numbers, अन्यथा lexicographical.\n\
Pattern matches वापस the string matched between \\( and \\) or null; अगर\n\
\\( and \\) are not used, they वापस the संख्या of characters matched or 0.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Exit status is 0 अगर EXPRESSION is neither null nor 0, 1 अगर EXPRESSION is null\n\
or 0, 2 अगर EXPRESSION is syntactically invalid, and 3 अगर an error occurred.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}


पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (व्यंजक_FAILURE);
  निकास_पर (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (अक्षर स्थिर *) शून्य);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  अगर (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  अगर (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (व्यंजक_INVALID);
    }

  args = argv + 1;

  मूल्य_2 *v = eval (true);
  अगर (!nomoreargs ())
    error (व्यंजक_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

अटल मूल्य_2 *
int_मूल्य (अचिन्हित दीर्घ पूर्णांक i)
{
  मूल्य_2 *v = xmalloc (माप *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  वापस v;
}

/* Return a VALUE for S.  */

अटल मूल्य_2 *
str_मूल्य (अक्षर स्थिर *s)
{
  मूल्य_2 *v = xmalloc (माप *v);
  v->type = string;
  v->u.s = xstrdup (s);
  वापस v;
}

/* Free VALUE V, including structure components.  */

अटल व्योम
freev (मूल्य_2 *v)
{
  अगर (v->type == string)
    मुक्त (v->u.s);
  अन्यथा
    mpz_clear (v->u.i);
  मुक्त (v);
}

/* Print VALUE V.  */

अटल व्योम
printv (मूल्य_2 *v)
{
  चयन (v->type)
    {
    हाल integer:
      mpz_out_str (मानक_निकास, 10, v->u.i);
      अक्षर_दो ('\n');
      अवरोध;
    हाल string:
      माला_दो (v->u.s);
      अवरोध;
    शेष:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
अटल bool
null (मूल्य_2 *v)
{
  चयन (v->type)
    {
    हाल integer:
      वापस mpz_sgn (v->u.i) == 0;
    हाल string:
      {
        अक्षर स्थिर *cp = v->u.s;
        अगर (*cp == '\0')
          वापस true;

        cp += (*cp == '-');

        करो
          {
            अगर (*cp != '0')
              वापस false;
          }
        जबतक (*++cp);

        वापस true;
      }
    शेष:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
अटल bool
looks_like_integer (अक्षर स्थिर *cp)
{
  cp += (*cp == '-');

  करो
    अगर (! c_isdigit (*cp))
      वापस false;
  जबतक (*++cp);

  वापस true;
}

/* Coerce V to a string value (can't fail).  */

अटल व्योम
tostring (मूल्य_2 *v)
{
  चयन (v->type)
    {
    हाल integer:
      {
        अक्षर *s = mpz_get_str (शून्य, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      अवरोध;
    हाल string:
      अवरोध;
    शेष:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

अटल bool
toarith (मूल्य_2 *v)
{
  चयन (v->type)
    {
    हाल integer:
      वापस true;
    हाल string:
      {
        अक्षर *s = v->u.s;

        अगर (! looks_like_integer (s))
          वापस false;
        अगर (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (व्यंजक_FAILURE, दुस्फल, "%s", (s));
        मुक्त (s);
        v->type = integer;
        वापस true;
      }
    शेष:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
अटल माप_प्रकार
getsize (mpz_t i)
{
  अगर (mpz_sgn (i) < 0)
    वापस SIZE_MAX;
  अगर (mpz_fits_ulong_p (i))
    {
      अचिन्हित दीर्घ पूर्णांक ul = mpz_get_ui (i);
      अगर (ul < SIZE_MAX)
        वापस ul;
    }
  वापस SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

अटल bool
nextarg (अक्षर स्थिर *str)
{
  अगर (*args == शून्य)
    वापस false;
  अन्यथा
    {
      bool r = streq (*args, str);
      args += r;
      वापस r;
    }
}

/* Return true if there no more tokens.  */

अटल bool
nomoreargs (व्योम)
{
  वापस !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
अटल व्योम
require_more_args (व्योम)
{
  अगर (nomoreargs ())
    error (व्यंजक_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#अगर_घोषित EVAL_TRACE
/* Print evaluation trace and args remaining.  */

अटल व्योम
trace (अक्षर स्थिर *fxn)
{
  म_लिखो ("%s:", fxn);
  क्रम (अक्षर **a = args; *a; a++)
    म_लिखो (" %s", *a);
  अक्षर_दो ('\n');
}
#पूर्ण_अगर

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

अटल मूल्य_2 *
docolon (मूल्य_2 *sv, मूल्य_2 *pv)
{
  tostring (sv);
  tostring (pv);

  काष्ठा re_registers re_regs;
  re_regs.अंक_regs = 0;
  re_regs.start = शून्य;
  re_regs.end = शून्य;

  काष्ठा re_pattern_बफर re_बफर;
  अक्षर fastmap[अच_अक्षर_उच्च + 1];
  re_बफर.बफर = शून्य;
  re_बफर.allocated = 0;
  re_बफर.fastmap = fastmap;
  re_बफर.translate = शून्य;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  अक्षर स्थिर *errmsg = re_compile_pattern (pv->u.s, म_माप (pv->u.s),
                                           &re_बफर);
  अगर (errmsg)
    error (व्यंजक_INVALID, 0, "%s", (errmsg));
  re_बफर.newline_anchor = 0;

  मूल्य_2 *v;
  regoff_t matchlen = re_match (&re_बफर, sv->u.s, म_माप (sv->u.s), 0,
                                &re_regs);
  अगर (0 <= matchlen)
    {
      /* Were \(...\) used? */
      अगर (re_बफर.re_nsub > 0)
        {
          अगर (re_regs.end[1] < 0)
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
          माप_प्रकार i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_मूल्य (i);
        }
    }
  अन्यथा अगर (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      अगर (re_बफर.re_nsub > 0)
        v = str_मूल्य ("");
      अन्यथा
        v = int_मूल्य (0);
    }
  अन्यथा
    error (व्यंजक_FAILURE,
           matchlen == -2 ? त्रुटि_सं : EOVERFLOW,
           _("error in regular expression matcher"));

  अगर (0 < re_regs.अंक_regs)
    {
      मुक्त (re_regs.start);
      मुक्त (re_regs.end);
    }
  re_बफर.fastmap = शून्य;
  regfree (&re_बफर);
  वापस v;
}

/* Handle bare operands and ( expr ) syntax.  */

अटल मूल्य_2 *
eval7 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval7");
#पूर्ण_अगर
  require_more_args ();

  अगर (nextarg ("("))
    {
      मूल्य_2 *v = eval (evaluate);
      अगर (nomoreargs ())
        error (व्यंजक_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      अगर (!nextarg (")"))
        error (व्यंजक_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      वापस v;
    }

  अगर (nextarg (")"))
    error (व्यंजक_INVALID, 0, _("syntax error: unexpected ')'"));

  वापस str_मूल्य (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

अटल मूल्य_2 *
eval6 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval6");
#पूर्ण_अगर
  अगर (nextarg ("+"))
    {
      require_more_args ();
      वापस str_मूल्य (*args++);
    }
  अन्यथा अगर (nextarg ("length"))
    {
      मूल्य_2 *r = eval6 (evaluate);
      tostring (r);
      मूल्य_2 *v = int_मूल्य (mbslen (r->u.s));
      freev (r);
      वापस v;
    }
  अन्यथा अगर (nextarg ("match"))
    {
      मूल्य_2 *l = eval6 (evaluate);
      मूल्य_2 *r = eval6 (evaluate);
      मूल्य_2 *v;
      अगर (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      अन्यथा
        v = l;
      freev (r);
      वापस v;
    }
  अन्यथा अगर (nextarg ("index"))
    {
      मूल्य_2 *l = eval6 (evaluate);
      मूल्य_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      माप_प्रकार pos = mbs_logical_cspn (l->u.s, r->u.s);
      मूल्य_2 *v = int_मूल्य (pos);
      freev (l);
      freev (r);
      वापस v;
    }
  अन्यथा अगर (nextarg ("substr"))
    {
      मूल्य_2 *l = eval6 (evaluate);
      मूल्य_2 *i1 = eval6 (evaluate);
      मूल्य_2 *i2 = eval6 (evaluate);
      tostring (l);

      मूल्य_2 *v;
      अगर (!toarith (i1) || !toarith (i2))
        v = str_मूल्य ("");
      अन्यथा
        {
          माप_प्रकार pos = getsize (i1->u.i);
          माप_प्रकार len = getsize (i2->u.i);

          अक्षर *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_मूल्य (s);
          मुक्त (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      वापस v;
    }
  अन्यथा
    वापस eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

अटल मूल्य_2 *
eval5 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval5");
#पूर्ण_अगर
  मूल्य_2 *l = eval6 (evaluate);
  जबतक (true)
    {
      अगर (nextarg (":"))
        {
          मूल्य_2 *r = eval6 (evaluate);
          अगर (evaluate)
            {
              मूल्य_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      अन्यथा
        वापस l;
    }
}

/* Handle *, /, % operators.  */

अटल मूल्य_2 *
eval4 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval4");
#पूर्ण_अगर
  मूल्य_2 *l = eval5 (evaluate);
  जबतक (true)
    {
      क्रमागत { multiply, divide, mod } fxn;

      अगर (nextarg ("*"))
        fxn = multiply;
      अन्यथा अगर (nextarg ("/"))
        fxn = divide;
      अन्यथा अगर (nextarg ("%"))
        fxn = mod;
      अन्यथा
        वापस l;
      मूल्य_2 *r = eval5 (evaluate);
      अगर (evaluate)
        {
          अगर (!toarith (l) || !toarith (r))
            error (व्यंजक_INVALID, 0, _("non-integer argument"));
          अगर (fxn != multiply && mpz_sgn (r->u.i) == 0)
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

अटल मूल्य_2 *
eval3 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval3");
#पूर्ण_अगर
  मूल्य_2 *l = eval4 (evaluate);
  जबतक (true)
    {
      क्रमागत { plus, minus } fxn;

      अगर (nextarg ("+"))
        fxn = plus;
      अन्यथा अगर (nextarg ("-"))
        fxn = minus;
      अन्यथा
        वापस l;
      मूल्य_2 *r = eval4 (evaluate);
      अगर (evaluate)
        {
          अगर (!toarith (l) || !toarith (r))
            error (व्यंजक_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

अटल मूल्य_2 *
eval2 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval2");
#पूर्ण_अगर
  मूल्य_2 *l = eval3 (evaluate);
  जबतक (true)
    {
      क्रमागत
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      अगर (nextarg ("<"))
        fxn = less_than;
      अन्यथा अगर (nextarg ("<="))
        fxn = less_equal;
      अन्यथा अगर (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      अन्यथा अगर (nextarg ("!="))
        fxn = not_equal;
      अन्यथा अगर (nextarg (">="))
        fxn = greater_equal;
      अन्यथा अगर (nextarg (">"))
        fxn = greater_than;
      अन्यथा
        वापस l;
      मूल्य_2 *r = eval3 (evaluate);

      bool val = false;
      अगर (evaluate)
        {
          tostring (l);
          tostring (r);

          पूर्णांक cmp;
          अगर (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          अन्यथा
            {
              त्रुटि_सं = 0;
              cmp = strcoll (l->u.s, r->u.s);

              अगर (त्रुटि_सं)
                {
                  error (0, त्रुटि_सं, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (व्यंजक_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          चयन (fxn)
            {
            हाल less_than:     val = (cmp <  0); अवरोध;
            हाल less_equal:    val = (cmp <= 0); अवरोध;
            हाल equal:         val = (cmp == 0); अवरोध;
            हाल not_equal:     val = (cmp != 0); अवरोध;
            हाल greater_equal: val = (cmp >= 0); अवरोध;
            हाल greater_than:  val = (cmp >  0); अवरोध;
            शेष: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_मूल्य (val);
    }
}

/* Handle &.  */

अटल मूल्य_2 *
eval1 (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval1");
#पूर्ण_अगर
  मूल्य_2 *l = eval2 (evaluate);
  जबतक (true)
    {
      अगर (nextarg ("&"))
        {
          मूल्य_2 *r = eval2 (evaluate && !null (l));
          अगर (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_मूल्य (0);
            }
          अन्यथा
            freev (r);
        }
      अन्यथा
        वापस l;
    }
}

/* Handle |.  */

अटल मूल्य_2 *
eval (bool evaluate)
{
#अगर_घोषित EVAL_TRACE
  trace ("eval");
#पूर्ण_अगर
  मूल्य_2 *l = eval1 (evaluate);
  जबतक (true)
    {
      अगर (nextarg ("|"))
        {
          मूल्य_2 *r = eval1 (evaluate && null (l));
          अगर (null (l))
            {
              freev (l);
              l = r;
              अगर (null (l))
                {
                  freev (l);
                  l = int_मूल्य (0);
                }
            }
          अन्यथा
            freev (r);
        }
      अन्यथा
        वापस l;
    }
}
