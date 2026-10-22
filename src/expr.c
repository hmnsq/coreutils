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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <gmp.h>
#ಅನ್ತರ್ಭಾವ <regex.h>
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "strnumcmp.h"

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
ಗಣನಾ
  {
    /* Invalid expression: e.g., its form does not conform to the
       grammar for expressions.  Our grammar is an extension of the
       POSIX grammar.  */
    ಅಭಿವ್ಯಕ್ತಿ_INVALID = 2,

    /* An internal error occurred, e.g., arithmetic overflow, storage
       exhaustion.  */
    ಅಭಿವ್ಯಕ್ತಿ_FAILURE
  };

/* The kinds of value we can have.  */
ಗಣನಾ valtype
{
  integer,
  string
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ valtype TYPE;

/* A value is.... */
ಸಂರಚನಾ valinfo
{
  TYPE type;			/* Which kind. */
  ಸಙ್ಘ
  {				/* The value itself. */
    mpz_t i;
    ವರ್ಣ *s;
  } u;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ valinfo ಮೌಲ್ಯ_2;

/* The arguments given to the program, minus the program name.  */
ಸ್ಥಿರ ವರ್ಣ **args;

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *eval (bool);
ಸ್ಥಿರ bool nomoreargs (ಶೂನ್ಯ);
ಸ್ಥಿರ bool null (ಮೌಲ್ಯ_2 *v);
ಸ್ಥಿರ ಶೂನ್ಯ printv (ಮೌಲ್ಯ_2 *v);


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
ಸ್ಥಿರ size_t
mbs_logical_cspn (ವರ್ಣ ಧ್ರುವ *s, ವರ್ಣ ಧ್ರುವ *accept)
{
  ಯದಿ (accept[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;

  /* General case.  */
  ಯದಿ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ಯದಿ (g.len == 1)
            {
              ಯದಿ (mbschr (accept, *p))
                ಪ್ರತ್ಯಾಗಮ idx;
            }
          ಅನ್ಯಥಾ
            ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ಯದಿ (mcel_eq (g, h))
                  ಪ್ರತ್ಯಾಗಮ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  ಅನ್ಯಥಾ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ಯದಿ (s[i])
        ಪ್ರತ್ಯಾಗಮ i + 1;
    }

  /* not found */
  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ಸ್ಥಿರ ವರ್ಣ *
mbs_logical_substr (ವರ್ಣ ಧ್ರುವ *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? strlen (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  ವರ್ಣ ಧ್ರುವ *substart = s;
  idx_t sublen = 0;
  ಯದಿ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  ಅನ್ಯಥಾ ಯದಿ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತಿ (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_bytes = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ಯದಿ (pos <= idx)
          {
            ಯದಿ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_bytes;
          }

        s += char_bytes;
      }

  ಪ್ರತ್ಯಾಗಮ ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ಸ್ಥಿರ size_t
mbs_offset_to_chars (ವರ್ಣ ಧ್ರುವ *s, size_t ofs)
{
  size_t c = 0;
  ಪ್ರತಿ (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ಪ್ರತ್ಯಾಗಮ c;
}



ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
Print the ಮೌಲ್ಯ of EXPRESSION to standard output.  A blank ಸಾಲು below\n\
separates increasing precedence groups.  EXPRESSION may be:\n\
\n\
  ARG1 | ARG2       ARG1 ಯದಿ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ಯದಿ neither argument is null or 0, otherwise 0\n\
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
  + TOKEN                    interpret TOKEN as a string, even ಯದಿ it is a\n\
                               keyword like 'match' or an operator like '/'\n\
\n\
  ( EXPRESSION )             ಮೌಲ್ಯ of EXPRESSION\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need to be escaped or quoted ಪ್ರತಿ shells.\n\
Comparisons are arithmetic ಯದಿ both ARGs are numbers, ಅನ್ಯಥಾ lexicographical.\n\
Pattern matches ಪ್ರತ್ಯಾಗಮ the string matched between \\( and \\) or null; ಯದಿ\n\
\\( and \\) are not used, they ಪ್ರತ್ಯಾಗಮ the ಸಂಖ್ಯೆ of characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit status is 0 ಯದಿ EXPRESSION is neither null nor 0, 1 ಯದಿ EXPRESSION is null\n\
or 0, 2 ಯದಿ EXPRESSION is syntactically invalid, and 3 ಯದಿ an error occurred.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ಅಭಿವ್ಯಕ್ತಿ_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, PROGRAM_NAME, PACKAGE_NAME, VERSION,
                      usage, AUTHORS, (ವರ್ಣ ಧ್ರುವ *) ರಿಕ್ತ);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ಯದಿ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ಯದಿ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (ಅಭಿವ್ಯಕ್ತಿ_INVALID);
    }

  args = argv + 1;

  ಮೌಲ್ಯ_2 *v = eval (true);
  ಯದಿ (!nomoreargs ())
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *args));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
int_ಮೌಲ್ಯ (unsigned long ಪೂರ್ಣಾಙ್ಕ i)
{
  ಮೌಲ್ಯ_2 *v = xmalloc (ಪರಿಮಾಣ *v);
  v->type = integer;
  mpz_init_set_ui (v->u.i, i);
  ಪ್ರತ್ಯಾಗಮ v;
}

/* Return a VALUE for S.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
str_ಮೌಲ್ಯ (ವರ್ಣ ಧ್ರುವ *s)
{
  ಮೌಲ್ಯ_2 *v = xmalloc (ಪರಿಮಾಣ *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ಪ್ರತ್ಯಾಗಮ v;
}

/* Free VALUE V, including structure components.  */

ಸ್ಥಿರ ಶೂನ್ಯ
freev (ಮೌಲ್ಯ_2 *v)
{
  ಯದಿ (v->type == string)
    ಮುಕ್ತ (v->u.s);
  ಅನ್ಯಥಾ
    mpz_clear (v->u.i);
  ಮುಕ್ತ (v);
}

/* Print VALUE V.  */

ಸ್ಥಿರ ಶೂನ್ಯ
printv (ಮೌಲ್ಯ_2 *v)
{
  ವಿಕಲ್ಪ (v->type)
    {
    ಪಕ್ಷ integer:
      mpz_out_str (stdout, 10, v->u.i);
      putchar ('\n');
      ಭಙ್ಗ;
    ಪಕ್ಷ string:
      puts (v->u.s);
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ bool
null (ಮೌಲ್ಯ_2 *v)
{
  ವಿಕಲ್ಪ (v->type)
    {
    ಪಕ್ಷ integer:
      ಪ್ರತ್ಯಾಗಮ mpz_sgn (v->u.i) == 0;
    ಪಕ್ಷ string:
      {
        ವರ್ಣ ಧ್ರುವ *cp = v->u.s;
        ಯದಿ (*cp == '\0')
          ಪ್ರತ್ಯಾಗಮ true;

        cp += (*cp == '-');

        ಕುರು
          {
            ಯದಿ (*cp != '0')
              ಪ್ರತ್ಯಾಗಮ false;
          }
        ಯಾವತ್ (*++cp);

        ಪ್ರತ್ಯಾಗಮ true;
      }
    ಸ್ವಭಾವ:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ bool
looks_like_integer (ವರ್ಣ ಧ್ರುವ *cp)
{
  cp += (*cp == '-');

  ಕುರು
    ಯದಿ (! c_isdigit (*cp))
      ಪ್ರತ್ಯಾಗಮ false;
  ಯಾವತ್ (*++cp);

  ಪ್ರತ್ಯಾಗಮ true;
}

/* Coerce V to a string value (can't fail).  */

ಸ್ಥಿರ ಶೂನ್ಯ
tostring (ಮೌಲ್ಯ_2 *v)
{
  ವಿಕಲ್ಪ (v->type)
    {
    ಪಕ್ಷ integer:
      {
        ವರ್ಣ *s = mpz_get_str (ರಿಕ್ತ, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      ಭಙ್ಗ;
    ಪಕ್ಷ string:
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ಸ್ಥಿರ bool
toarith (ಮೌಲ್ಯ_2 *v)
{
  ವಿಕಲ್ಪ (v->type)
    {
    ಪಕ್ಷ integer:
      ಪ್ರತ್ಯಾಗಮ true;
    ಪಕ್ಷ string:
      {
        ವರ್ಣ *s = v->u.s;

        ಯದಿ (! looks_like_integer (s))
          ಪ್ರತ್ಯಾಗಮ false;
        ಯದಿ (mpz_init_set_str (v->u.i, s, 10) != 0)
          error (ಅಭಿವ್ಯಕ್ತಿ_FAILURE, ERANGE, "%s", (s));
        ಮುಕ್ತ (s);
        v->type = integer;
        ಪ್ರತ್ಯಾಗಮ true;
      }
    ಸ್ವಭಾವ:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ಸ್ಥಿರ size_t
getsize (mpz_t i)
{
  ಯದಿ (mpz_sgn (i) < 0)
    ಪ್ರತ್ಯಾಗಮ SIZE_MAX;
  ಯದಿ (mpz_fits_ulong_p (i))
    {
      unsigned long ಪೂರ್ಣಾಙ್ಕ ul = mpz_get_ui (i);
      ಯದಿ (ul < SIZE_MAX)
        ಪ್ರತ್ಯಾಗಮ ul;
    }
  ಪ್ರತ್ಯಾಗಮ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ಸ್ಥಿರ bool
nextarg (ವರ್ಣ ಧ್ರುವ *str)
{
  ಯದಿ (*args == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;
  ಅನ್ಯಥಾ
    {
      bool r = streq (*args, str);
      args += r;
      ಪ್ರತ್ಯಾಗಮ r;
    }
}

/* Return true if there no more tokens.  */

ಸ್ಥಿರ bool
nomoreargs (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ !*args;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ಸ್ಥಿರ ಶೂನ್ಯ
require_more_args (ಶೂನ್ಯ)
{
  ಯದಿ (nomoreargs ())
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(args - 1)));
}


#ifdef EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ಸ್ಥಿರ ಶೂನ್ಯ
trace (ವರ್ಣ ಧ್ರುವ *fxn)
{
  printf ("%s:", fxn);
  ಪ್ರತಿ (ವರ್ಣ **a = args; *a; a++)
    printf (" %s", *a);
  putchar ('\n');
}
#endif

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
docolon (ಮೌಲ್ಯ_2 *sv, ಮೌಲ್ಯ_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ಸಂರಚನಾ re_registers re_regs;
  re_regs.ಅಂಕೆ_regs = 0;
  re_regs.start = ರಿಕ್ತ;
  re_regs.end = ರಿಕ್ತ;

  ಸಂರಚನಾ re_pattern_buffer re_buffer;
  ವರ್ಣ fastmap[UCHAR_MAX + 1];
  re_buffer.buffer = ರಿಕ್ತ;
  re_buffer.allocated = 0;
  re_buffer.fastmap = fastmap;
  re_buffer.translate = ರಿಕ್ತ;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_EMPTY_RANGES);
  ವರ್ಣ ಧ್ರುವ *errmsg = re_compile_pattern (pv->u.s, strlen (pv->u.s),
                                           &re_buffer);
  ಯದಿ (errmsg)
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, "%s", (errmsg));
  re_buffer.newline_anchor = 0;

  ಮೌಲ್ಯ_2 *v;
  regoff_t matchlen = re_match (&re_buffer, sv->u.s, strlen (sv->u.s), 0,
                                &re_regs);
  ಯದಿ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ಯದಿ (re_buffer.re_nsub > 0)
        {
          ಯದಿ (re_regs.end[1] < 0)
            v = str_ಮೌಲ್ಯ ("");
          ಅನ್ಯಥಾ
            {
              sv->u.s[re_regs.end[1]] = '\0';
              v = str_ಮೌಲ್ಯ (sv->u.s + re_regs.start[1]);
            }
        }
      ಅನ್ಯಥಾ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_offset_to_chars (sv->u.s, matchlen));
          v = int_ಮೌಲ್ಯ (i);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ಯದಿ (re_buffer.re_nsub > 0)
        v = str_ಮೌಲ್ಯ ("");
      ಅನ್ಯಥಾ
        v = int_ಮೌಲ್ಯ (0);
    }
  ಅನ್ಯಥಾ
    error (ಅಭಿವ್ಯಕ್ತಿ_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ಯದಿ (0 < re_regs.ಅಂಕೆ_regs)
    {
      ಮುಕ್ತ (re_regs.start);
      ಮುಕ್ತ (re_regs.end);
    }
  re_buffer.fastmap = ರಿಕ್ತ;
  regfree (&re_buffer);
  ಪ್ರತ್ಯಾಗಮ v;
}

/* Handle bare operands and ( expr ) syntax.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval7 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval7");
#endif
  require_more_args ();

  ಯದಿ (nextarg ("("))
    {
      ಮೌಲ್ಯ_2 *v = eval (evaluate);
      ಯದಿ (nomoreargs ())
        error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(args - 1)));
      ಯದಿ (!nextarg (")"))
        error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *args));
      ಪ್ರತ್ಯಾಗಮ v;
    }

  ಯದಿ (nextarg (")"))
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: unexpected ')'"));

  ಪ್ರತ್ಯಾಗಮ str_ಮೌಲ್ಯ (*args++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval6 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval6");
#endif
  ಯದಿ (nextarg ("+"))
    {
      require_more_args ();
      ಪ್ರತ್ಯಾಗಮ str_ಮೌಲ್ಯ (*args++);
    }
  ಅನ್ಯಥಾ ಯದಿ (nextarg ("length"))
    {
      ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
      tostring (r);
      ಮೌಲ್ಯ_2 *v = int_ಮೌಲ್ಯ (mbslen (r->u.s));
      freev (r);
      ಪ್ರತ್ಯಾಗಮ v;
    }
  ಅನ್ಯಥಾ ಯದಿ (nextarg ("match"))
    {
      ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *v;
      ಯದಿ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      ಅನ್ಯಥಾ
        v = l;
      freev (r);
      ಪ್ರತ್ಯಾಗಮ v;
    }
  ಅನ್ಯಥಾ ಯದಿ (nextarg ("index"))
    {
      ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      ಮೌಲ್ಯ_2 *v = int_ಮೌಲ್ಯ (pos);
      freev (l);
      freev (r);
      ಪ್ರತ್ಯಾಗಮ v;
    }
  ಅನ್ಯಥಾ ಯದಿ (nextarg ("substr"))
    {
      ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *i1 = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *i2 = eval6 (evaluate);
      tostring (l);

      ಮೌಲ್ಯ_2 *v;
      ಯದಿ (!toarith (i1) || !toarith (i2))
        v = str_ಮೌಲ್ಯ ("");
      ಅನ್ಯಥಾ
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          ವರ್ಣ *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_ಮೌಲ್ಯ (s);
          ಮುಕ್ತ (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      ಪ್ರತ್ಯಾಗಮ v;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval5 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval5");
#endif
  ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
  ಯಾವತ್ (true)
    {
      ಯದಿ (nextarg (":"))
        {
          ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
          ಯದಿ (evaluate)
            {
              ಮೌಲ್ಯ_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ l;
    }
}

/* Handle *, /, % operators.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval4 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval4");
#endif
  ಮೌಲ್ಯ_2 *l = eval5 (evaluate);
  ಯಾವತ್ (true)
    {
      ಗಣನಾ { multiply, divide, mod } fxn;

      ಯದಿ (nextarg ("*"))
        fxn = multiply;
      ಅನ್ಯಥಾ ಯದಿ (nextarg ("/"))
        fxn = divide;
      ಅನ್ಯಥಾ ಯದಿ (nextarg ("%"))
        fxn = mod;
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ l;
      ಮೌಲ್ಯ_2 *r = eval5 (evaluate);
      ಯದಿ (evaluate)
        {
          ಯದಿ (!toarith (l) || !toarith (r))
            error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("non-integer argument"));
          ಯದಿ (fxn != multiply && mpz_sgn (r->u.i) == 0)
            error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("division by zero"));
          ((fxn == multiply ? mpz_mul
            : fxn == divide ? mpz_tdiv_q
            : mpz_tdiv_r)
           (l->u.i, l->u.i, r->u.i));
        }
      freev (r);
    }
}

/* Handle +, - operators.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval3 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval3");
#endif
  ಮೌಲ್ಯ_2 *l = eval4 (evaluate);
  ಯಾವತ್ (true)
    {
      ಗಣನಾ { plus, minus } fxn;

      ಯದಿ (nextarg ("+"))
        fxn = plus;
      ಅನ್ಯಥಾ ಯದಿ (nextarg ("-"))
        fxn = minus;
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ l;
      ಮೌಲ್ಯ_2 *r = eval4 (evaluate);
      ಯದಿ (evaluate)
        {
          ಯದಿ (!toarith (l) || !toarith (r))
            error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_add : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval2 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval2");
#endif
  ಮೌಲ್ಯ_2 *l = eval3 (evaluate);
  ಯಾವತ್ (true)
    {
      ಗಣನಾ
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ಯದಿ (nextarg ("<"))
        fxn = less_than;
      ಅನ್ಯಥಾ ಯದಿ (nextarg ("<="))
        fxn = less_equal;
      ಅನ್ಯಥಾ ಯದಿ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      ಅನ್ಯಥಾ ಯದಿ (nextarg ("!="))
        fxn = not_equal;
      ಅನ್ಯಥಾ ಯದಿ (nextarg (">="))
        fxn = greater_equal;
      ಅನ್ಯಥಾ ಯದಿ (nextarg (">"))
        fxn = greater_than;
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ l;
      ಮೌಲ್ಯ_2 *r = eval3 (evaluate);

      bool val = false;
      ಯದಿ (evaluate)
        {
          tostring (l);
          tostring (r);

          ಪೂರ್ಣಾಙ್ಕ cmp;
          ಯದಿ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          ಅನ್ಯಥಾ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ಯದಿ (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          ವಿಕಲ್ಪ (fxn)
            {
            ಪಕ್ಷ less_than:     val = (cmp <  0); ಭಙ್ಗ;
            ಪಕ್ಷ less_equal:    val = (cmp <= 0); ಭಙ್ಗ;
            ಪಕ್ಷ equal:         val = (cmp == 0); ಭಙ್ಗ;
            ಪಕ್ಷ not_equal:     val = (cmp != 0); ಭಙ್ಗ;
            ಪಕ್ಷ greater_equal: val = (cmp >= 0); ಭಙ್ಗ;
            ಪಕ್ಷ greater_than:  val = (cmp >  0); ಭಙ್ಗ;
            ಸ್ವಭಾವ: unreachable ();
            }
        }

      freev (l);
      freev (r);
      l = int_ಮೌಲ್ಯ (val);
    }
}

/* Handle &.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval1 (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval1");
#endif
  ಮೌಲ್ಯ_2 *l = eval2 (evaluate);
  ಯಾವತ್ (true)
    {
      ಯದಿ (nextarg ("&"))
        {
          ಮೌಲ್ಯ_2 *r = eval2 (evaluate && !null (l));
          ಯದಿ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_ಮೌಲ್ಯ (0);
            }
          ಅನ್ಯಥಾ
            freev (r);
        }
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ l;
    }
}

/* Handle |.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval (bool evaluate)
{
#ifdef EVAL_TRACE
  trace ("eval");
#endif
  ಮೌಲ್ಯ_2 *l = eval1 (evaluate);
  ಯಾವತ್ (true)
    {
      ಯದಿ (nextarg ("|"))
        {
          ಮೌಲ್ಯ_2 *r = eval1 (evaluate && null (l));
          ಯದಿ (null (l))
            {
              freev (l);
              l = r;
              ಯದಿ (null (l))
                {
                  freev (l);
                  l = int_ಮೌಲ್ಯ (0);
                }
            }
          ಅನ್ಯಥಾ
            freev (r);
        }
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ l;
    }
}
