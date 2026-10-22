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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <gmp.h>
#ଅନ୍ତର୍ଭାଵ <regex.h>
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "long-options.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"
#ଅନ୍ତର୍ଭାଵ "strnumcmp.h"

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
ଗଣନା
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    ପଦସମଷ୍ଟି_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    ପଦସମଷ୍ଟି_FAILURE
  };

/* The kinds of value we can have.  */
ଗଣନା valtype
{
  integer,
  string
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା valtype TYPE;

/* A value is.... */
ସଂରଚନା valinfo
{
  TYPE type;			/* Which kind. */
  ସଂଘ
  {				/* The value itself. */
    mpz_t i;
    ଅକ୍ଷର *s;
  } u;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା valinfo ମୂଲ୍ୟ_2;

/* The arguments given to the program, minus the program name.  */
ସ୍ଥିର ଅକ୍ଷର **args;

ସ୍ଥିର ମୂଲ୍ୟ_2 *eval (bool);
ସ୍ଥିର bool nomoreargs (ଶୂନ୍ୟ);
ସ୍ଥିର bool null (ମୂଲ୍ୟ_2 *v);
ସ୍ଥିର ଶୂନ୍ୟ printv (ମୂଲ୍ୟ_2 *v);


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
ସ୍ଥିର size_t
mbs_logical_cspn (ଅକ୍ଷର ଅଚଳ *s, ଅକ୍ଷର ଅଚଳ *accept)
{
  ଯଦି (accept[0] == '\0')
    ଫେରନ୍ତୁ 0;

  /* General case.  */
  ଯଦି (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      ପାଇଁ (ଅକ୍ଷର ଅଚଳ *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ଯଦି (g.len == 1)
            {
              ଯଦି (mbschr (accept, *p))
                ଫେରନ୍ତୁ idx;
            }
          ଅନ୍ୟଥା
            ପାଇଁ (ଅକ୍ଷର ଅଚଳ *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ଯଦି (mcel_eq (g, h))
                  ଫେରନ୍ତୁ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  ଅନ୍ୟଥା
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ଯଦି (s[i])
        ଫେରନ୍ତୁ i + 1;
    }

  /* not found */
  ଫେରନ୍ତୁ 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ସ୍ଥିର ଅକ୍ଷର *
mbs_logical_substr (ଅକ୍ଷର ଅଚଳ *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  ଅକ୍ଷର ଅଚଳ *substart = s;
  idx_t sublen = 0;
  ଯଦି (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  ଅନ୍ୟଥା ଯଦି (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  ଅନ୍ୟଥା
    ପାଇଁ (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ଯଦି (pos <= idx)
          {
            ଯଦି (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  ଫେରନ୍ତୁ ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ସ୍ଥିର size_t
mbs_offset_to_chars (ଅକ୍ଷର ଅଚଳ *s, size_t ofs)
{
  size_t c = 0;
  ପାଇଁ (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ଫେରନ୍ତୁ c;
}



ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
Print the ମୂଲ୍ୟ of EXPRESSION to standard output.  A blank ଧାଡ଼ି below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 ଯଦି it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ଯଦି neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even ଯଦି it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             ମୂଲ୍ୟ of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted ପାଇଁ shells.\n\
Comparisons are arithmetic ଯଦି both ARGs are numbers, ଅନ୍ୟଥା lexicographical.\n\
Pattern matches ଫେରନ୍ତୁ the string matched between \\( and \\) or null; ଯଦି\n\
\\( and \\) are not used, they ଫେରନ୍ତୁ the ସଂଖ୍ୟା of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 ଯଦି EXPRESSION is neither null nor 0, 1 ଯଦି EXPRESSION is null\n\
or 0, 2 ଯଦି EXPRESSION is syntactically invalid, and 3 ଯଦି an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ପଦସମଷ୍ଟି_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (ଅକ୍ଷର ଅଚଳ *) ଖାଲି);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ଯଦି (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ଯଦି (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (ପଦସମଷ୍ଟି_INVALID);
    }

  args = argv + 1;

  ମୂଲ୍ୟ_2 *v = eval (true);
  ଯଦି (!nomoreargs ())
    error (ପଦସମଷ୍ଟି_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
int_ମୂଲ୍ୟ (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
  ମୂଲ୍ୟ_2 *v = xmalloc (ଆକାର *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  ଫେରନ୍ତୁ v;
}

/* Return a VALUE for S.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
str_ମୂଲ୍ୟ (ଅକ୍ଷର ଅଚଳ *s)
{
  ମୂଲ୍ୟ_2 *v = xmalloc (ଆକାର *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ଫେରନ୍ତୁ v;
}

/* Free VALUE V, including structure components.  */

ସ୍ଥିର ଶୂନ୍ୟ
freev (ମୂଲ୍ୟ_2 *v)
{
  ଯଦି (v->type == string)
    ମୁକ୍ତ (v->u.s);
  ଅନ୍ୟଥା
    mpz_clear (v->u.i);
  ମୁକ୍ତ (v);
}

/* Print VALUE V.  */

ସ୍ଥିର ଶୂନ୍ୟ
printv (ମୂଲ୍ୟ_2 *v)
{
  ବାଛ (v->type)
    {
    ସ୍ଥିତି integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      ଭାଙ୍ଗ;
    ସ୍ଥିତି string:
      puts (v->u.s);
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
null (ମୂଲ୍ୟ_2 *v)
{
  ବାଛ (v->type)
    {
    ସ୍ଥିତି integer:
      ଫେରନ୍ତୁ mpz_sgn (v->u.i) == 0;
    ସ୍ଥିତି string:
      {
        ଅକ୍ଷର ଅଚଳ *cp = v->u.s;
        ଯଦି (*cp == '\0')
          ଫେରନ୍ତୁ true;

        cp += (*cp == '-');

        କର
          {
            ଯଦି (*cp != '0')
              ଫେରନ୍ତୁ false;
          }
        ଯେତେବେଳେ (*++cp);

        ଫେରନ୍ତୁ true;
      }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
looks_like_integer (ଅକ୍ଷର ଅଚଳ *cp)
{
  cp += (*cp == '-');

  କର
    ଯଦି (! c_isdigit (*cp))
      ଫେରନ୍ତୁ false;
  ଯେତେବେଳେ (*++cp);

  ଫେରନ୍ତୁ true;
}

/* Coerce V to a string value (can't fail).  */

ସ୍ଥିର ଶୂନ୍ୟ
tostring (ମୂଲ୍ୟ_2 *v)
{
  ବାଛ (v->type)
    {
    ସ୍ଥିତି integer:
      {
        ଅକ୍ଷର *s = mpz_get_str (ଖାଲି, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      ଭାଙ୍ଗ;
    ସ୍ଥିତି string:
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ସ୍ଥିର bool
toarith (ମୂଲ୍ୟ_2 *v)
{
  ବାଛ (v->type)
    {
    ସ୍ଥିତି integer:
      ଫେରନ୍ତୁ true;
    ସ୍ଥିତି string:
      {
        ଅକ୍ଷର *s = v->u.s;

        ଯଦି (! looks_like_integer (s))
          ଫେରନ୍ତୁ false;
        ଯଦି (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (ପଦସମଷ୍ଟି_FAILURE, ERANGE, "%s", (s));
        ମୁକ୍ତ (s);
        v->type = integer;
        ଫେରନ୍ତୁ true;
      }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ସ୍ଥିର size_t
getsize (mpz_t i)
{
  ଯଦି (mpz_sgn (i) < 0)
    ଫେରନ୍ତୁ SIZE_MAX;
  ଯଦି (mpz_fits_ulong_p (i))
    {
      unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା ul = mpz_get_ui (i);
      ଯଦି (ul < SIZE_MAX)
        ଫେରନ୍ତୁ ul;
    }
  ଫେରନ୍ତୁ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ସ୍ଥିର bool
nextarg (ଅକ୍ଷର ଅଚଳ *str)
{
  ଯଦି (*args == ଖାଲି)
    ଫେରନ୍ତୁ false;
  ଅନ୍ୟଥା
    {
      bool r = streq (*args, str);
      args += r;
      ଫେରନ୍ତୁ r;
    }
}

/* Return true if there no more tokens.  */

ସ୍ଥିର bool
nomoreargs (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ସ୍ଥିର ଶୂନ୍ୟ
require_more_args (ଶୂନ୍ୟ)
{
  ଯଦି (nomoreargs ())
    error (ପଦସମଷ୍ଟି_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ସ୍ଥିର ଶୂନ୍ୟ
trace (ଅକ୍ଷର ଅଚଳ *fxn)
{
  printf ("%s:", fxn);
  ପାଇଁ (ଅକ୍ଷର **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
docolon (ମୂଲ୍ୟ_2 *sv, ମୂଲ୍ୟ_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ସଂରଚନା re_registers re_regs;
  re_regs.ଅଙ୍କ_regs = 0;
  re_regs.start = ଖାଲି;
  re_regs.end = ଖାଲି;

  ସଂରଚନା re_pattern_buffer re_buffer;
  ଅକ୍ଷର fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = ଖାଲି;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = ଖାଲି;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  ଅକ୍ଷର ଅଚଳ *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  ଯଦି (errmsg)
    error (ପଦସମଷ୍ଟି_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  ମୂଲ୍ୟ_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  ଯଦି (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ଯଦି (re_buffer.re_nsub > 0)
        {
          ଯଦି (re_regs.end[1] < 0)
            v = str_ମୂଲ୍ୟ ("");
          ଅନ୍ୟଥା
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_ମୂଲ୍ୟ (sv->u.s + re_regs.start[1]);
            }
        }
      ଅନ୍ୟଥା
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_ମୂଲ୍ୟ (i);
        }
    }
  ଅନ୍ୟଥା ଯଦି (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ଯଦି (re_buffer.re_nsub > 0)
        v = str_ମୂଲ୍ୟ ("");
      ଅନ୍ୟଥା
        v = int_ମୂଲ୍ୟ (0);
    }
  ଅନ୍ୟଥା
    error (ପଦସମଷ୍ଟି_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ଯଦି (0 < re_regs.ଅଙ୍କ_regs)
    {
      ମୁକ୍ତ (re_regs.start);
      ମୁକ୍ତ (re_regs.end);
    }
  re_buffer.fastmap = ଖାଲି;
  regfree (&re_buffer);
  ଫେରନ୍ତୁ v;
}

/* Handle bare operands and ( expr ) syntax.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  ଯଦି (nextarg ("("))
    {
      ମୂଲ୍ୟ_2 *v = eval (evaluate);
      ଯଦି (nomoreargs ())
        error (ପଦସମଷ୍ଟି_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      ଯଦି (!nextarg (")"))
        error (ପଦସମଷ୍ଟି_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      ଫେରନ୍ତୁ v;
    }

  ଯଦି (nextarg (")"))
    error (ପଦସମଷ୍ଟି_INVALID, 0, _("syntax error: unexpected ')'"));

  ଫେରନ୍ତୁ str_ମୂଲ୍ୟ (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  ଯଦି (nextarg ("+"))
    {
      require_more_args ();
      ଫେରନ୍ତୁ str_ମୂଲ୍ୟ (*args++);
    }
  ଅନ୍ୟଥା ଯଦି (nextarg ("length"))
    {
      ମୂଲ୍ୟ_2 *r = eval6 (evaluate);
      tostring (r);
      ମୂଲ୍ୟ_2 *v = int_ମୂଲ୍ୟ (mbslen (r->u.s));
      freev (r);
      ଫେରନ୍ତୁ v;
    }
  ଅନ୍ୟଥା ଯଦି (nextarg ("match"))
    {
      ମୂଲ୍ୟ_2 *l = eval6 (evaluate);
      ମୂଲ୍ୟ_2 *r = eval6 (evaluate);
      ମୂଲ୍ୟ_2 *v;
      ଯଦି (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      ଅନ୍ୟଥା
        v = l;
      freev (r);
      ଫେରନ୍ତୁ v;
    }
  ଅନ୍ୟଥା ଯଦି (nextarg ("index"))
    {
      ମୂଲ୍ୟ_2 *l = eval6 (evaluate);
      ମୂଲ୍ୟ_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      ମୂଲ୍ୟ_2 *v = int_ମୂଲ୍ୟ (pos);
      freev (l);
      freev (r);
      ଫେରନ୍ତୁ v;
    }
  ଅନ୍ୟଥା ଯଦି (nextarg ("substr"))
    {
      ମୂଲ୍ୟ_2 *l = eval6 (evaluate);
      ମୂଲ୍ୟ_2 *i1 = eval6 (evaluate);
      ମୂଲ୍ୟ_2 *i2 = eval6 (evaluate);
      tostring (l);

      ମୂଲ୍ୟ_2 *v;
      ଯଦି (!toarith (i1) || !toarith (i2))
        v = str_ମୂଲ୍ୟ ("");
      ଅନ୍ୟଥା
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          ଅକ୍ଷର *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_ମୂଲ୍ୟ (s);
          ମୁକ୍ତ (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      ଫେରନ୍ତୁ v;
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  ମୂଲ୍ୟ_2 *l = eval6 (evaluate);
  ଯେତେବେଳେ (true)
    {
      ଯଦି (nextarg (":"))
        {
          ମୂଲ୍ୟ_2 *r = eval6 (evaluate);
          ଯଦି (evaluate)
            {
              ମୂଲ୍ୟ_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ l;
    }
}

/* Handle *, /, % operators.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  ମୂଲ୍ୟ_2 *l = eval5 (evaluate);
  ଯେତେବେଳେ (true)
    {
      ଗଣନା { multiply, divide, mod } fxn;

      ଯଦି (nextarg ("*"))
        fxn = multiply;
      ଅନ୍ୟଥା ଯଦି (nextarg ("/"))
        fxn = divide;
      ଅନ୍ୟଥା ଯଦି (nextarg ("%"))
        fxn = mod;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ l;
      ମୂଲ୍ୟ_2 *r = eval5 (evaluate);
      ଯଦି (evaluate)
        {
          ଯଦି (!toarith (l) || !toarith (r))
            error (ପଦସମଷ୍ଟି_INVALID, 0, _("non-integer argument"));
          ଯଦି (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (ପଦସମଷ୍ଟି_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  ମୂଲ୍ୟ_2 *l = eval4 (evaluate);
  ଯେତେବେଳେ (true)
    {
      ଗଣନା { plus, minus } fxn;

      ଯଦି (nextarg ("+"))
        fxn = plus;
      ଅନ୍ୟଥା ଯଦି (nextarg ("-"))
        fxn = minus;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ l;
      ମୂଲ୍ୟ_2 *r = eval4 (evaluate);
      ଯଦି (evaluate)
        {
          ଯଦି (!toarith (l) || !toarith (r))
            error (ପଦସମଷ୍ଟି_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  ମୂଲ୍ୟ_2 *l = eval3 (evaluate);
  ଯେତେବେଳେ (true)
    {
      ଗଣନା
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ଯଦି (nextarg ("<"))
        fxn = less_than;
      ଅନ୍ୟଥା ଯଦି (nextarg ("<="))
        fxn = less_equal;
      ଅନ୍ୟଥା ଯଦି (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      ଅନ୍ୟଥା ଯଦି (nextarg ("!="))
        fxn = not_equal;
      ଅନ୍ୟଥା ଯଦି (nextarg (">="))
        fxn = greater_equal;
      ଅନ୍ୟଥା ଯଦି (nextarg (">"))
        fxn = greater_than;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ l;
      ମୂଲ୍ୟ_2 *r = eval3 (evaluate);

      bool val = false;
      ଯଦି (evaluate)
        {
          tostring (l);
          tostring (r);

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp;
          ଯଦି (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          ଅନ୍ୟଥା
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ଯଦି (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (ପଦସମଷ୍ଟି_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          ବାଛ (fxn)
            {
            ସ୍ଥିତି less_than:     val = (cmp <  0); ଭାଙ୍ଗ;
            ସ୍ଥିତି less_equal:    val = (cmp <= 0); ଭାଙ୍ଗ;
            ସ୍ଥିତି equal:         val = (cmp == 0); ଭାଙ୍ଗ;
            ସ୍ଥିତି not_equal:     val = (cmp != 0); ଭାଙ୍ଗ;
            ସ୍ଥିତି greater_equal: val = (cmp >= 0); ଭାଙ୍ଗ;
            ସ୍ଥିତି greater_than:  val = (cmp >  0); ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_ମୂଲ୍ୟ (val);
    }
}

/* Handle &.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  ମୂଲ୍ୟ_2 *l = eval2 (evaluate);
  ଯେତେବେଳେ (true)
    {
      ଯଦି (nextarg ("&"))
        {
          ମୂଲ୍ୟ_2 *r = eval2 (evaluate && !null (l));
          ଯଦି (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_ମୂଲ୍ୟ (0);
            }
          ଅନ୍ୟଥା
            freev (r);
        }
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ l;
    }
}

/* Handle |.  */

ସ୍ଥିର ମୂଲ୍ୟ_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  ମୂଲ୍ୟ_2 *l = eval1 (evaluate);
  ଯେତେବେଳେ (true)
    {
      ଯଦି (nextarg ("|"))
        {
          ମୂଲ୍ୟ_2 *r = eval1 (evaluate && null (l));
          ଯଦି (null (l))
            {
              freev (l);
              l = r;
              ଯଦି (null (l))
                {
                  freev (l);
                  l = int_ମୂଲ୍ୟ (0);
                }
            }
          ଅନ୍ୟଥା
            freev (r);
        }
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ l;
    }
}
