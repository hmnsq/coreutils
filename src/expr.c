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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "expr"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Mike Parker"), \
  proper_name ("James Youngman"), \
  proper_name ("Paul Eggert")

/* Exit statuses.  */
ಎಣಿಕೆ
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
ಎಣಿಕೆ valtype
{
  integer,
  string
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ valtype TYPE;

/* A value is.... */
ರಚನೆ valinfo
{
  TYPE type;			/* Which kind. */
  ಒಕ್ಕೂಟ
  {				/* The value itself. */
    mpz_t i;
    ಅಕ್ಷರ *s;
  } u;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ valinfo ಮೌಲ್ಯ_2;

/* The arguments given to the program, minus the program name.  */
ಸ್ಥಿರ ಅಕ್ಷರ **ವಾದಗಳು;

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
mbs_logical_cspn (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *accept)
{
  ಆದರೆ (accept[0] == '\0')
    ಹಿಂತಿರುಗಿ 0;

  /* General case.  */
  ಆದರೆ (MB_CUR_MAX > 1)
    {
      size_t idx = 0;
      ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = s; *p; )
        {
          ++idx;
          mcel_t g = mcel_scanz (p);
          ಆದರೆ (g.len == 1)
            {
              ಆದರೆ (mbschr (accept, *p))
                ಹಿಂತಿರುಗಿ idx;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a = accept; *a; )
              {
                mcel_t h = mcel_scanz (a);
                ಆದರೆ (mcel_eq (g, h))
                  ಹಿಂತಿರುಗಿ idx;
                a += h.len;
              }
          p += g.len;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* single-byte locale,
         convert returned byte offset to 1-based index or zero if not found. */
      size_t i = strcspn (s, accept);
      ಆದರೆ (s[i])
        ಹಿಂತಿರುಗಿ i + 1;
    }

  /* not found */
  ಹಿಂತಿರುಗಿ 0;
}

/* Extract the substring of S, from logical character
   position POS and LEN characters.
   first character position is 1.
   POS and LEN refer to logical characters, not octets.

   Upon exit, sets v->s to the new string.
   The new string might be empty if POS/LEN are invalid. */
ಸ್ಥಿರ ಅಕ್ಷರ *
mbs_logical_substr (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, size_t pos, size_t len)
{
  size_t mb_cur_max = MB_CUR_MAX;
  idx_t llen = mb_cur_max <= 1 ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s) : mbslen (s); /* logical length */

  /* characters to copy */
  size_t vlen = MIN (len, pos <= llen ? llen - pos + 1 : 0);

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *substart = s;
  idx_t sublen = 0;
  ಆದರೆ (pos == 0 || len == SIZE_MAX)
    {
      /* The request is invalid.  Silently yield an empty string.  */
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mb_cur_max <= 1)
    {
      substart += pos - 1;
      sublen = vlen;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (idx_t idx = 1; *s && vlen; idx++)
      {
        idx_t char_ಬೈಟುಗಳು = mcel_scanz (s).len;

        /* Skip until we reach the starting position.  */
        ಆದರೆ (pos <= idx)
          {
            ಆದರೆ (pos == idx)
              substart = s;

            /* Add one character's length in bytes.  */
            vlen--;
            sublen += char_ಬೈಟುಗಳು;
          }

        s += char_ಬೈಟುಗಳು;
      }

  ಹಿಂತಿರುಗಿ ximemdup0 (substart, sublen);
}

/* Return the number of logical characters (possibly multibyte)
   that are in string S in the first OFS octets.

   Example in UTF-8:
   "\xE2\x9D\xA7" is "U+2767 ROTATED FLORAL HEART BULLET".
   In the string below, there are only two characters
   up to the first 4 bytes (The U+2767 which occupies 3 bytes and 'x'):
      mbs_count_to_offset ("\xE2\x9D\xA7xyz", 4) => 2  */
ಸ್ಥಿರ size_t
mbs_ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_chars (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, size_t ofs)
{
  size_t c = 0;
  ಗಾಗಿ (size_t d = 0; d < ofs && s[d]; d += mcel_scanz (s + d).len)
    c++;
  ಹಿಂತಿರುಗಿ c;
}



ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s ಅಭಿವ್ಯಕ್ತಿ\n\
  or:  %s OPTION\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      ಅಕ್ಷರ_ಬರೆ ('\n');
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Print the ಮೌಲ್ಯ ದ ಅಭಿವ್ಯಕ್ತಿ ಗಮ್ಯ standard output.  A blank ಸಾಲು ಕೆಳಗೆ\n\
separates increasing precedence groups.  ಅಭಿವ್ಯಕ್ತಿ may be:\n\
\n\
  ARG1 | ARG2       ARG1 ಆದರೆ it is neither null nor 0, otherwise ARG2\n\
\n\
  ARG1 & ARG2       ARG1 ಆದರೆ neither ವಾದ is null or 0, otherwise 0\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 < ARG2       ARG1 is less than ARG2\n\
  ARG1 <= ARG2      ARG1 is less than or equal ಗಮ್ಯ ARG2\n\
  ARG1 = ARG2       ARG1 is equal ಗಮ್ಯ ARG2\n\
  ARG1 != ARG2      ARG1 is unequal ಗಮ್ಯ ARG2\n\
  ARG1 >= ARG2      ARG1 is greater than or equal ಗಮ್ಯ ARG2\n\
  ARG1 > ARG2       ARG1 is greater than ARG2\n\
"), stdout);
      fputs (_("\
\n\
  ARG1 + ARG2       arithmetic ಮೊತ್ತ ದ ARG1 and ARG2\n\
  ARG1 - ARG2       arithmetic difference ದ ARG1 and ARG2\n\
"), stdout);
      /* Tell xgettext that the "% A" below is not a printf-style
         format string:  xgettext:no-c-format */
      fputs (_("\
\n\
  ARG1 * ARG2       arithmetic ಗುಣಲಬ್ಧ ದ ARG1 and ARG2\n\
  ARG1 / ARG2       arithmetic quotient ದ ARG1 divided by ARG2\n\
  ARG1 % ARG2       arithmetic remainder ದ ARG1 divided by ARG2\n\
"), stdout);
      fputs (_("\
\n\
  STRING : REGEXP   anchored ನಮೂನೆ_2 ಹೊಂದಿಕೆ ದ REGEXP ಒಳ STRING\n\
\n\
  ಹೊಂದಿಕೆ STRING REGEXP        same as STRING : REGEXP\n\
  substr STRING POS ಉದ್ದಳತೆ_2   substring ದ STRING, POS counted from 1\n\
  index STRING CHARS         index ಒಳ STRING where any CHARS is ಸಿಕ್ಕಿತು, or 0\n\
  ಉದ್ದಳತೆ STRING              ಉದ್ದಳತೆ ದ STRING\n\
"), stdout);
      fputs (_("\
  + TOKEN                    interpret TOKEN as a string, even ಆದರೆ it is a\n\
                               keyword like 'match' or an ಕ್ರಿಯಾಕಾರಕ like '/'\n\
\n\
  ( ಅಭಿವ್ಯಕ್ತಿ )             ಮೌಲ್ಯ ದ ಅಭಿವ್ಯಕ್ತಿ\n\
"), stdout);
      fputs (_("\
\n\
Beware that many operators need ಗಮ್ಯ be escaped or quoted ಗಾಗಿ shells.\n\
Comparisons are arithmetic ಆದರೆ both ವಾದಗಳು_2 are numbers, ಇಲ್ಲದಿದ್ದರೆ lexicographical.\n\
ನಮೂನೆ_3 matches ಹಿಂತಿರುಗಿ the string matched between \\( and \\) or null; ಆದರೆ\n\
\\( and \\) are not ಬಳಸಿದ, they ಹಿಂತಿರುಗಿ the ಸಂಖ್ಯೆ ದ characters matched or 0.\n\
"), stdout);
      fputs (_("\
\n\
Exit ಸ್ಥಿತಿ_2 is 0 ಆದರೆ ಅಭಿವ್ಯಕ್ತಿ is neither null nor 0, 1 ಆದರೆ ಅಭಿವ್ಯಕ್ತಿ is null\n\
or 0, 2 ಆದರೆ ಅಭಿವ್ಯಕ್ತಿ is syntactically invalid, and 3 ಆದರೆ an error occurred.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}


ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ಅಭಿವ್ಯಕ್ತಿ_FAILURE);
  atexit (close_stdout);

  parse_long_options (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME, ಆವೃತ್ತಿ_2,
                      usage, AUTHORS, (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

  /* The above handles --help and --version.
     Since there is no other invocation of getopt, handle '--' here.  */
  ಆದರೆ (1 < argc && streq (argv[1], "--"))
    {
      --argc;
      ++argv;
    }

  ಆದರೆ (argc <= 1)
    {
      error (0, 0, _("missing operand"));
      usage (ಅಭಿವ್ಯಕ್ತಿ_INVALID);
    }

  ವಾದಗಳು = argv + 1;

  ಮೌಲ್ಯ_2 *v = eval (true);
  ಆದರೆ (!nomoreargs ())
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: unexpected argument %s"),
           quotearg_n_style (0, locale_quoting_style, *ವಾದಗಳು));

  printv (v);

  main_exit (null (v));
}

/* Return a VALUE for I.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
int_ಮೌಲ್ಯ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ i)
{
  ಮೌಲ್ಯ_2 *v = xmalloc (ಗಾತ್ರ *v);
  v->type = integer;
  mpz_init_ಗಣ_ui (v->u.i, i);
  ಹಿಂತಿರುಗಿ v;
}

/* Return a VALUE for S.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
str_ಮೌಲ್ಯ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  ಮೌಲ್ಯ_2 *v = xmalloc (ಗಾತ್ರ *v);
  v->type = string;
  v->u.s = xstrdup (s);
  ಹಿಂತಿರುಗಿ v;
}

/* Free VALUE V, including structure components.  */

ಸ್ಥಿರ ಶೂನ್ಯ
freev (ಮೌಲ್ಯ_2 *v)
{
  ಆದರೆ (v->type == string)
    ಬಿಡುಗಡೆ (v->u.s);
  ಇಲ್ಲದಿದ್ದರೆ
    mpz_clear (v->u.i);
  ಬಿಡುಗಡೆ (v);
}

/* Print VALUE V.  */

ಸ್ಥಿರ ಶೂನ್ಯ
printv (ಮೌಲ್ಯ_2 *v)
{
  ಆಯ್ಕೆ (v->type)
    {
    ಸ್ಥಿತಿ integer:
      mpz_ಹೊರಹೋಗು_str (stdout, 10, v->u.i);
      ಅಕ್ಷರ_ಬರೆ ('\n');
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ string:
      puts (v->u.s);
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

/* Return true if V is a null-string or zero-number.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
null (ಮೌಲ್ಯ_2 *v)
{
  ಆಯ್ಕೆ (v->type)
    {
    ಸ್ಥಿತಿ integer:
      ಹಿಂತಿರುಗಿ mpz_sgn (v->u.i) == 0;
    ಸ್ಥಿತಿ string:
      {
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp = v->u.s;
        ಆದರೆ (*cp == '\0')
          ಹಿಂತಿರುಗಿ true;

        cp += (*cp == '-');

        ಮಾಡು
          {
            ಆದರೆ (*cp != '0')
              ಹಿಂತಿರುಗಿ false;
          }
        ಆಗ (*++cp);

        ಹಿಂತಿರುಗಿ true;
      }
    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

/* Return true if CP takes the form of an integer.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
looks_like_integer (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *cp)
{
  cp += (*cp == '-');

  ಮಾಡು
    ಆದರೆ (! c_isdigit (*cp))
      ಹಿಂತಿರುಗಿ false;
  ಆಗ (*++cp);

  ಹಿಂತಿರುಗಿ true;
}

/* Coerce V to a string value (can't fail).  */

ಸ್ಥಿರ ಶೂನ್ಯ
tostring (ಮೌಲ್ಯ_2 *v)
{
  ಆಯ್ಕೆ (v->type)
    {
    ಸ್ಥಿತಿ integer:
      {
        ಅಕ್ಷರ *s = mpz_get_str (ಖಾಲಿ, 10, v->u.i);
        mpz_clear (v->u.i);
        v->u.s = s;
        v->type = string;
      }
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ string:
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

/* Coerce V to an integer value.  Return true on success, false on failure.  */

ಸ್ಥಿರ bool
toarith (ಮೌಲ್ಯ_2 *v)
{
  ಆಯ್ಕೆ (v->type)
    {
    ಸ್ಥಿತಿ integer:
      ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ string:
      {
        ಅಕ್ಷರ *s = v->u.s;

        ಆದರೆ (! looks_like_integer (s))
          ಹಿಂತಿರುಗಿ false;
        ಆದರೆ (mpz_init_ಗಣ_str (v->u.i, s, 10) != 0)
          error (ಅಭಿವ್ಯಕ್ತಿ_FAILURE, ERANGE, "%s", (s));
        ಬಿಡುಗಡೆ (s);
        v->type = integer;
        ಹಿಂತಿರುಗಿ true;
      }
    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

/* Extract a size_t value from an integer value I.
   If the value is negative, return SIZE_MAX.
   If the value is too large, return SIZE_MAX - 1.  */
ಸ್ಥಿರ size_t
getsize (mpz_t i)
{
  ಆದರೆ (mpz_sgn (i) < 0)
    ಹಿಂತಿರುಗಿ SIZE_MAX;
  ಆದರೆ (mpz_ಹೊಂದುತ್ತದೆ_ulong_p (i))
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ ul = mpz_get_ui (i);
      ಆದರೆ (ul < SIZE_MAX)
        ಹಿಂತಿರುಗಿ ul;
    }
  ಹಿಂತಿರುಗಿ SIZE_MAX - 1;
}

/* Return true and advance if the next token matches STR exactly.
   STR must not be null.  */

ಸ್ಥಿರ bool
nextarg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಆದರೆ (*ವಾದಗಳು == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      bool r = streq (*ವಾದಗಳು, str);
      ವಾದಗಳು += r;
      ಹಿಂತಿರುಗಿ r;
    }
}

/* Return true if there no more tokens.  */

ಸ್ಥಿರ bool
nomoreargs (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ !*ವಾದಗಳು;
}

/* Report missing operand.
   There is an implicit assumption that there was a previous argument,
   and (args-1) is valid. */
ಸ್ಥಿರ ಶೂನ್ಯ
require_ಇನ್ನಷ್ಟು_ವಾದಗಳು (ಶೂನ್ಯ)
{
  ಆದರೆ (nomoreargs ())
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: missing argument after %s"),
           quotearg_n_style (0, locale_quoting_style, *(ವಾದಗಳು - 1)));
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
/* Print evaluation trace and args remaining.  */

ಸ್ಥಿರ ಶೂನ್ಯ
trace (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fxn)
{
  ಬರೆ ("%s:", fxn);
  ಗಾಗಿ (ಅಕ್ಷರ **a = ವಾದಗಳು; *a; a++)
    ಬರೆ (" %s", *a);
  ಅಕ್ಷರ_ಬರೆ ('\n');
}
#ಆದರೆ_ಅಂತ್ಯ

/* Do the : operator.
   SV is the VALUE for the lhs (the string),
   PV is the VALUE for the rhs (the pattern).  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
docolon (ಮೌಲ್ಯ_2 *sv, ಮೌಲ್ಯ_2 *pv)
{
  tostring (sv);
  tostring (pv);

  ರಚನೆ re_registers re_regs;
  re_regs.ಅಂಕೆ_regs = 0;
  re_regs.ಆರಂಭ = ಖಾಲಿ;
  re_regs.ಕೊನೆ = ಖಾಲಿ;

  ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ re_ಬಫರ್;
  ಅಕ್ಷರ fastmap[UCHAR_MAX + 1];
  re_ಬಫರ್.ಬಫರ್ = ಖಾಲಿ;
  re_ಬಫರ್.allocated = 0;
  re_ಬಫರ್.fastmap = fastmap;
  re_ಬಫರ್.translate = ಖಾಲಿ;
  re_syntax_options = (RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP
                       & ~RE_NO_ಖಾಲಿಗಳು_RANGES);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *errmsg = re_compile_ನಮೂನೆ (pv->u.s, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pv->u.s),
                                           &re_ಬಫರ್);
  ಆದರೆ (errmsg)
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, "%s", (errmsg));
  re_ಬಫರ್.newline_anchor = 0;

  ಮೌಲ್ಯ_2 *v;
  regoff_t matchlen = re_ಹೊಂದಿಕೆ (&re_ಬಫರ್, sv->u.s, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (sv->u.s), 0,
                                &re_regs);
  ಆದರೆ (0 <= matchlen)
    {
      /* Were \(...\) used? */
      ಆದರೆ (re_ಬಫರ್.re_nsub > 0)
        {
          ಆದರೆ (re_regs.ಕೊನೆ[1] < 0)
            v = str_ಮೌಲ್ಯ ("");
          ಇಲ್ಲದಿದ್ದರೆ
            {
              sv->u.s[re_regs.ಕೊನೆ[1]] = '\0';
              v = str_ಮೌಲ್ಯ (sv->u.s + re_regs.ಆರಂಭ[1]);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* In multibyte locales, convert the matched offset (=number of bytes)
             to the number of matched characters. */
          size_t i = (MB_CUR_MAX == 1
                      ? matchlen
                      : mbs_ಸ್ಥಾನಾಂತರ_ಗಮ್ಯ_chars (sv->u.s, matchlen));
          v = int_ಮೌಲ್ಯ (i);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (matchlen == -1)
    {
      /* Match failed -- return the right kind of null.  */
      ಆದರೆ (re_ಬಫರ್.re_nsub > 0)
        v = str_ಮೌಲ್ಯ ("");
      ಇಲ್ಲದಿದ್ದರೆ
        v = int_ಮೌಲ್ಯ (0);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    error (ಅಭಿವ್ಯಕ್ತಿ_FAILURE,
           matchlen == -2 ? errno : EOVERFLOW,
           _("error in regular expression matcher"));

  ಆದರೆ (0 < re_regs.ಅಂಕೆ_regs)
    {
      ಬಿಡುಗಡೆ (re_regs.ಆರಂಭ);
      ಬಿಡುಗಡೆ (re_regs.ಕೊನೆ);
    }
  re_ಬಫರ್.fastmap = ಖಾಲಿ;
  regfree (&re_ಬಫರ್);
  ಹಿಂತಿರುಗಿ v;
}

/* Handle bare operands and ( expr ) syntax.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval7 (bool evaluate)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval7");
#ಆದರೆ_ಅಂತ್ಯ
  require_ಇನ್ನಷ್ಟು_ವಾದಗಳು ();

  ಆದರೆ (nextarg ("("))
    {
      ಮೌಲ್ಯ_2 *v = eval (evaluate);
      ಆದರೆ (nomoreargs ())
        error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: expecting ')' after %s"),
               quotearg_n_style (0, locale_quoting_style, *(ವಾದಗಳು - 1)));
      ಆದರೆ (!nextarg (")"))
        error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: expecting ')' instead of %s"),
               quotearg_n_style (0, locale_quoting_style, *ವಾದಗಳು));
      ಹಿಂತಿರುಗಿ v;
    }

  ಆದರೆ (nextarg (")"))
    error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("syntax error: unexpected ')'"));

  ಹಿಂತಿರುಗಿ str_ಮೌಲ್ಯ (*ವಾದಗಳು++);
}

/* Handle match, substr, index, and length keywords, and quoting "+".  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval6 (bool evaluate)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval6");
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (nextarg ("+"))
    {
      require_ಇನ್ನಷ್ಟು_ವಾದಗಳು ();
      ಹಿಂತಿರುಗಿ str_ಮೌಲ್ಯ (*ವಾದಗಳು++);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("length"))
    {
      ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
      tostring (r);
      ಮೌಲ್ಯ_2 *v = int_ಮೌಲ್ಯ (mbslen (r->u.s));
      freev (r);
      ಹಿಂತಿರುಗಿ v;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("match"))
    {
      ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *v;
      ಆದರೆ (evaluate)
        {
          v = docolon (l, r);
          freev (l);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        v = l;
      freev (r);
      ಹಿಂತಿರುಗಿ v;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("index"))
    {
      ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
      tostring (l);
      tostring (r);
      size_t pos = mbs_logical_cspn (l->u.s, r->u.s);
      ಮೌಲ್ಯ_2 *v = int_ಮೌಲ್ಯ (pos);
      freev (l);
      freev (r);
      ಹಿಂತಿರುಗಿ v;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("substr"))
    {
      ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *i1 = eval6 (evaluate);
      ಮೌಲ್ಯ_2 *i2 = eval6 (evaluate);
      tostring (l);

      ಮೌಲ್ಯ_2 *v;
      ಆದರೆ (!toarith (i1) || !toarith (i2))
        v = str_ಮೌಲ್ಯ ("");
      ಇಲ್ಲದಿದ್ದರೆ
        {
          size_t pos = getsize (i1->u.i);
          size_t len = getsize (i2->u.i);

          ಅಕ್ಷರ *s = mbs_logical_substr (l->u.s, pos, len);
          v = str_ಮೌಲ್ಯ (s);
          ಬಿಡುಗಡೆ (s);
        }
      freev (l);
      freev (i1);
      freev (i2);
      ಹಿಂತಿರುಗಿ v;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ eval7 (evaluate);
}

/* Handle : operator (pattern matching).
   Calls docolon to do the real work.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval5 (bool evaluate)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval5");
#ಆದರೆ_ಅಂತ್ಯ
  ಮೌಲ್ಯ_2 *l = eval6 (evaluate);
  ಆಗ (true)
    {
      ಆದರೆ (nextarg (":"))
        {
          ಮೌಲ್ಯ_2 *r = eval6 (evaluate);
          ಆದರೆ (evaluate)
            {
              ಮೌಲ್ಯ_2 *v = docolon (l, r);
              freev (l);
              l = v;
            }
          freev (r);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ l;
    }
}

/* Handle *, /, % operators.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval4 (bool evaluate)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval4");
#ಆದರೆ_ಅಂತ್ಯ
  ಮೌಲ್ಯ_2 *l = eval5 (evaluate);
  ಆಗ (true)
    {
      ಎಣಿಕೆ { multiply, divide, mod } fxn;

      ಆದರೆ (nextarg ("*"))
        fxn = multiply;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("/"))
        fxn = divide;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("%"))
        fxn = mod;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ l;
      ಮೌಲ್ಯ_2 *r = eval5 (evaluate);
      ಆದರೆ (evaluate)
        {
          ಆದರೆ (!toarith (l) || !toarith (r))
            error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("non-integer argument"));
          ಆದರೆ (fxn != multiply && mpz_sgn (r->u.i) == 0)
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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval3");
#ಆದರೆ_ಅಂತ್ಯ
  ಮೌಲ್ಯ_2 *l = eval4 (evaluate);
  ಆಗ (true)
    {
      ಎಣಿಕೆ { plus, minus } fxn;

      ಆದರೆ (nextarg ("+"))
        fxn = plus;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("-"))
        fxn = minus;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ l;
      ಮೌಲ್ಯ_2 *r = eval4 (evaluate);
      ಆದರೆ (evaluate)
        {
          ಆದರೆ (!toarith (l) || !toarith (r))
            error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0, _("non-integer argument"));
          (fxn == plus ? mpz_ಕೂಡಿಸು : mpz_sub) (l->u.i, l->u.i, r->u.i);
        }
      freev (r);
    }
}

/* Handle comparisons.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval2 (bool evaluate)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval2");
#ಆದರೆ_ಅಂತ್ಯ
  ಮೌಲ್ಯ_2 *l = eval3 (evaluate);
  ಆಗ (true)
    {
      ಎಣಿಕೆ
        {
          less_than, less_equal, equal, not_equal, greater_equal, greater_than
        } fxn;

      ಆದರೆ (nextarg ("<"))
        fxn = less_than;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("<="))
        fxn = less_equal;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("=") || nextarg ("=="))
        fxn = equal;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg ("!="))
        fxn = not_equal;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg (">="))
        fxn = greater_equal;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nextarg (">"))
        fxn = greater_than;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ l;
      ಮೌಲ್ಯ_2 *r = eval3 (evaluate);

      bool val = false;
      ಆದರೆ (evaluate)
        {
          tostring (l);
          tostring (r);

          ಪೂರ್ಣಾಂಕ cmp;
          ಆದರೆ (looks_like_integer (l->u.s) && looks_like_integer (r->u.s))
            cmp = strintcmp (l->u.s, r->u.s);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              errno = 0;
              cmp = strcoll (l->u.s, r->u.s);

              ಆದರೆ (errno)
                {
                  error (0, errno, _("string comparison failed"));
                  error (0, 0, _("set LC_ALL='C' to work around the problem"));
                  error (ಅಭಿವ್ಯಕ್ತಿ_INVALID, 0,
                         _("the strings compared were %s and %s"),
                         quotearg_n_style (0, locale_quoting_style, l->u.s),
                         quotearg_n_style (1, locale_quoting_style, r->u.s));
                }
            }

          ಆಯ್ಕೆ (fxn)
            {
            ಸ್ಥಿತಿ less_than:     val = (cmp <  0); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ less_equal:    val = (cmp <= 0); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ equal:         val = (cmp == 0); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ not_equal:     val = (cmp != 0); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ greater_equal: val = (cmp >= 0); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ greater_than:  val = (cmp >  0); ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ: unreachable ();
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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval1");
#ಆದರೆ_ಅಂತ್ಯ
  ಮೌಲ್ಯ_2 *l = eval2 (evaluate);
  ಆಗ (true)
    {
      ಆದರೆ (nextarg ("&"))
        {
          ಮೌಲ್ಯ_2 *r = eval2 (evaluate && !null (l));
          ಆದರೆ (null (l) || null (r))
            {
              freev (l);
              freev (r);
              l = int_ಮೌಲ್ಯ (0);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            freev (r);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ l;
    }
}

/* Handle |.  */

ಸ್ಥಿರ ಮೌಲ್ಯ_2 *
eval (bool evaluate)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EVAL_TRACE
  trace ("eval");
#ಆದರೆ_ಅಂತ್ಯ
  ಮೌಲ್ಯ_2 *l = eval1 (evaluate);
  ಆಗ (true)
    {
      ಆದರೆ (nextarg ("|"))
        {
          ಮೌಲ್ಯ_2 *r = eval1 (evaluate && null (l));
          ಆದರೆ (null (l))
            {
              freev (l);
              l = r;
              ಆದರೆ (null (l))
                {
                  freev (l);
                  l = int_ಮೌಲ್ಯ (0);
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            freev (r);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ l;
    }
}
