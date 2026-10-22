/* GNU test program (ksb and mjb) */

/* Modified to run with the GNU shell by bfox. */

/* Copyright (C) 1987-2026 Free Software Foundation, Inc.

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

/* Define TEST_STANDALONE to get the /bin/test version.  Otherwise, you get
   the shell builtin version. */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME (test_ಬಹುಲಕ == TEST_TEST ? "test" : "[")

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "issymlink.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "strnumcmp.h"
#ಅನ್ತರ್ಭಾವ "test.h"

#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಆದರೆ HAVE_SYS_PARAM_H
# include <sys/param.h>
#ಆದರೆ_ಅಂತ್ಯ

/* Exit status for syntax errors, etc.  */
ಎಣಿಕೆ { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
ಎಣಿಕೆ binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#ಆದರೆ defined TEST_STANDALONE
# define test_exit(val) ನಿರ್ಗಮಿಸು (val)
# define test_main_return(val) ಹಿಂತಿರುಗಿ val
#ಇಲ್ಲದಿದ್ದರೆ
   ಸ್ಥಿರ jmp_buf test_exit_buf;
   ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#ಆದರೆ_ಅಂತ್ಯ /* !TEST_STANDALONE */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ pos;		/* The offset of the current argument in ARGV. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ argc;	/* The number of arguments present in ARGV. */
ಸ್ಥಿರ ಅಕ್ಷರ **argv;	/* The argument list. */

ಸ್ಥಿರ bool unary_operator (ಶೂನ್ಯ);
ಸ್ಥಿರ bool ದ್ವಿಮಾನ_operator (bool, ಎಣಿಕೆ binop);
ಸ್ಥಿರ bool two_arguments (ಶೂನ್ಯ);
ಸ್ಥಿರ bool three_arguments (ಶೂನ್ಯ);
ಸ್ಥಿರ bool posixtest (ಪೂರ್ಣಾಂಕ);

ಸ್ಥಿರ bool ಅಭಿವ್ಯಕ್ತಿ_3 (ಶೂನ್ಯ);
ಸ್ಥಿರ bool ಪದ (ಶೂನ್ಯ);
ಸ್ಥಿರ bool and (ಶೂನ್ಯ);
ಸ್ಥಿರ bool or (ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ beyond (ಶೂನ್ಯ);

ATTRIBUTE_FORMAT ((ಬರೆ, 1, 2))
ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
test_syntax_error (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ...)
{
  va_list ap;
  va_ಆರಂಭ (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

ಸ್ಥಿರ ಶೂನ್ಯ
advance (bool f)
{
  ++pos;

  ಆದರೆ (f && pos >= argc)
    beyond ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
unary_advance (ಶೂನ್ಯ)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
beyond (ಶೂನ್ಯ)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಕಂಡುಹಿಡಿ_int (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಖ್ಯೆ_ಆರಂಭ;

  ಗಾಗಿ (p = string; ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*p)); p++)
    ಮುಂದುವರಿ;

  ಆದರೆ (*p == '+')
    {
      p++;
      ಸಂಖ್ಯೆ_ಆರಂಭ = p;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸಂಖ್ಯೆ_ಆರಂಭ = p;
      p += (*p == '-');
    }

  ಆದರೆ (c_isdigit (*p++))
    {
      ಆಗ (c_isdigit (*p))
        p++;
      ಆಗ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*p)))
        p++;
      ಆದರೆ (!*p)
        ಹಿಂತಿರುಗಿ ಸಂಖ್ಯೆ_ಆರಂಭ;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
ಸ್ಥಿರ ರಚನೆ timespec
get_mtime (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  ರಚನೆ stat finfo;
  ಹಿಂತಿರುಗಿ (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
binop (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  ಹಿಂತಿರುಗಿ (  streq (s, "="  ) ? EQ_STRING_BINOP
          : streq (s, "==" ) ? EQ_STRING_BINOP /* an alias for = */
          : streq (s, "!=" ) ? NE_STRING_BINOP
          : streq (s, ">"  ) ? GT_STRING_BINOP
          : streq (s, "<"  ) ? LT_STRING_BINOP
          : streq (s, "-eq") ? EQ_BINOP
          : streq (s, "-ne") ? NE_BINOP
          : streq (s, "-lt") ? LT_BINOP
          : streq (s, "-le") ? LE_BINOP
          : streq (s, "-gt") ? GT_BINOP
          : streq (s, "-ge") ? GE_BINOP
          : streq (s, "-ot") ? OT_BINOP
          : streq (s, "-nt") ? NT_BINOP
          : streq (s, "-ef") ? EF_BINOP
          : -1);
}

/*
 * term - parse a term and return 1 or 0 depending on whether the term
 *	evaluates to true or false, respectively.
 *
 * term ::=
 *	'-'('h'|'d'|'f'|'r'|'s'|'w'|'c'|'b'|'p'|'u'|'g'|'k') filename
 *	'-'('L'|'x') filename
 *	'-t' int
 *	'-'('z'|'n') string
 *	string
 *	string ('!='|'='|>|<) string
 *	<int> '-'(eq|ne|le|lt|ge|gt) <int>
 *	file '-'(nt|ot|ef) file
 *	'(' <expr> ')'
 * int ::=
 *	'-l' string
 *	positive and negative integers
 */
ಸ್ಥಿರ bool
ಪದ (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ;
  bool negated = false;
  ಪೂರ್ಣಾಂಕ bop;

  /* Deal with leading 'not's.  */
  ಆಗ (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  ಆದರೆ (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  ಆದರೆ (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      ಪೂರ್ಣಾಂಕ nargs;

      advance (true);

      ಗಾಗಿ (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        ಆದರೆ (nargs == 4)
          {
            nargs = argc - pos;
            ನಿಲ್ಲಿಸು;
          }

      ಮೌಲ್ಯ = posixtest (nargs);
      ಆದರೆ (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      ಇಲ್ಲದಿದ್ದರೆ
        ಆದರೆ (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    ಮೌಲ್ಯ = ದ್ವಿಮಾನ_operator (true, bop);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    ಮೌಲ್ಯ = ದ್ವಿಮಾನ_operator (false, bop);

  /* It might be a switch type argument.  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    ಮೌಲ್ಯ = unary_operator ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಮೌಲ್ಯ = (argv[pos][0] != '\0');
      advance (false);
    }

  ಹಿಂತಿರುಗಿ negated ^ ಮೌಲ್ಯ;
}

ಸ್ಥಿರ bool
ದ್ವಿಮಾನ_operator (bool l_is_l, ಎಣಿಕೆ binop bop)
{
  ಪೂರ್ಣಾಂಕ op;

  ಆದರೆ (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  ಆದರೆ (r_is_l)
    advance (false);

  pos += 3;

  ಆಯ್ಕೆ (bop)
    {
    ಸ್ಥಿತಿ EQ_BINOP: ಸ್ಥಿತಿ GE_BINOP: ಸ್ಥಿತಿ GT_BINOP:
    ಸ್ಥಿತಿ LE_BINOP: ಸ್ಥಿತಿ LT_BINOP: ಸ್ಥಿತಿ NE_BINOP:
      {
        ಅಕ್ಷರ lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ಅಕ್ಷರ rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *l = (l_is_l
                         ? umaxtostr (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (argv[op - 1]), lbuf)
                         : ಕಂಡುಹಿಡಿ_int (argv[op - 1]));
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *r = (r_is_l
                         ? umaxtostr (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (argv[op + 2]), rbuf)
                         : ಕಂಡುಹಿಡಿ_int (argv[op + 1]));
        ಪೂರ್ಣಾಂಕ cmp = strintcmp (l, r);
        ಆಯ್ಕೆ (+bop)
          {
          ಸ್ಥಿತಿ EQ_BINOP: ಹಿಂತಿರುಗಿ cmp == 0;
          ಸ್ಥಿತಿ GE_BINOP: ಹಿಂತಿರುಗಿ cmp >= 0;
          ಸ್ಥಿತಿ GT_BINOP: ಹಿಂತಿರುಗಿ cmp >  0;
          ಸ್ಥಿತಿ LE_BINOP: ಹಿಂತಿರುಗಿ cmp <= 0;
          ಸ್ಥಿತಿ LT_BINOP: ಹಿಂತಿರುಗಿ cmp <  0;
          ಸ್ಥಿತಿ NE_BINOP: ಹಿಂತಿರುಗಿ cmp != 0;
          }
        unreachable ();
      }

    ಸ್ಥಿತಿ NT_BINOP: ಸ್ಥಿತಿ OT_BINOP:
      {
        ಆದರೆ (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        ಪೂರ್ಣಾಂಕ cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        ಹಿಂತಿರುಗಿ bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    ಸ್ಥಿತಿ EF_BINOP:
      ಆದರೆ (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ರಚನೆ stat st[2];
          ಹಿಂತಿರುಗಿ (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    ಸ್ಥಿತಿ EQ_STRING_BINOP:
    ಸ್ಥಿತಿ NE_STRING_BINOP:
      ಹಿಂತಿರುಗಿ streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    ಸ್ಥಿತಿ GT_STRING_BINOP:
    ಸ್ಥಿತಿ LT_STRING_BINOP:
      {
        ಪೂರ್ಣಾಂಕ cmp = strcoll (argv[op - 1], argv[op + 1]);
        ಹಿಂತಿರುಗಿ bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

ಸ್ಥಿರ bool
unary_operator (ಶೂನ್ಯ)
{
  ರಚನೆ stat stat_buf;

  ಆಯ್ಕೆ (argv[pos][1])
    {
    ಪೂರ್ವನಿಯೋಜಿತ:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    ಸ್ಥಿತಿ 'e':			/* file exists in the file system? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ stat (argv[pos - 1], &stat_buf) == 0;

    ಸ್ಥಿತಿ 'r':			/* file is readable? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ euidaccess (argv[pos - 1], R_OK) == 0;

    ಸ್ಥಿತಿ 'w':			/* File is writable? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ euidaccess (argv[pos - 1], W_OK) == 0;

    ಸ್ಥಿತಿ 'x':			/* File is executable? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ euidaccess (argv[pos - 1], X_OK) == 0;

    ಸ್ಥಿತಿ 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        ಆದರೆ (stat (argv[pos - 1], &stat_buf) != 0)
          ಹಿಂತಿರುಗಿ false;
        ರಚನೆ timespec atime = get_stat_atime (&stat_buf);
        ರಚನೆ timespec mtime = get_stat_mtime (&stat_buf);
        ಹಿಂತಿರುಗಿ (timespec_cmp (mtime, atime) > 0);
      }

    ಸ್ಥಿತಿ 'O':			/* File is owned by you? */
      {
        unary_advance ();
        ಆದರೆ (stat (argv[pos - 1], &stat_buf) != 0)
          ಹಿಂತಿರುಗಿ false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        ಹಿಂತಿರುಗಿ ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    ಸ್ಥಿತಿ 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        ಆದರೆ (stat (argv[pos - 1], &stat_buf) != 0)
          ಹಿಂತಿರುಗಿ false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        ಹಿಂತಿರುಗಿ ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    ಸ್ಥಿತಿ 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_ಬಹುಲಕ));

    ಸ್ಥಿತಿ 'd':			/* File is a directory? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_ಬಹುಲಕ));

    ಸ್ಥಿತಿ 's':			/* File has something in it? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_ಗಾತ್ರ);

    ಸ್ಥಿತಿ 'S':			/* File is a socket? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_ಬಹುಲಕ));

    ಸ್ಥಿತಿ 'c':			/* File is character special? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_ಬಹುಲಕ));

    ಸ್ಥಿತಿ 'b':			/* File is block special? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_ಬಹುಲಕ));

    ಸ್ಥಿತಿ 'p':			/* File is a named pipe? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_ಬಹುಲಕ));

    ಸ್ಥಿತಿ 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    ಸ್ಥಿತಿ 'h':			/* File is a symbolic link? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ issymlink (argv[pos - 1]) == 1;

    ಸ್ಥಿತಿ 'u':			/* File is setuid? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_ಬಹುಲಕ & S_ISUID));

    ಸ್ಥಿತಿ 'g':			/* File is setgid? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_ಬಹುಲಕ & S_ISGID));

    ಸ್ಥಿತಿ 'k':			/* File has sticky bit set? */
      unary_advance ();
      ಹಿಂತಿರುಗಿ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_ಬಹುಲಕ & S_ISVTX));

    ಸ್ಥಿತಿ 't':			/* File (fd) is a terminal? */
      {
        ದೀರ್ಘ ಪೂರ್ಣಾಂಕ fd;
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg;
        unary_advance ();
        arg = ಕಂಡುಹಿಡಿ_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ಖಾಲಿ, 10);
        ಹಿಂತಿರುಗಿ (errno != ERANGE && 0 <= fd && fd <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ && isatty (fd));
      }

    ಸ್ಥಿತಿ 'n':			/* True if arg has some length. */
      unary_advance ();
      ಹಿಂತಿರುಗಿ argv[pos - 1][0] != 0;

    ಸ್ಥಿತಿ 'z':			/* True if arg has no length. */
      unary_advance ();
      ಹಿಂತಿರುಗಿ argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
ಸ್ಥಿರ bool
and (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ = true;

  ಆಗ (true)
    {
      ಮೌಲ್ಯ &= ಪದ ();
      ಆದರೆ (! (pos < argc && streq (argv[pos], "-a")))
        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
ಸ್ಥಿರ bool
or (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ = false;

  ಆಗ (true)
    {
      ಮೌಲ್ಯ |= and ();
      ಆದರೆ (! (pos < argc && streq (argv[pos], "-o")))
        ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_3 (ಶೂನ್ಯ)
{
  ಆದರೆ (pos >= argc)
    beyond ();

  ಹಿಂತಿರುಗಿ or ();		/* Same with this. */
}

ಸ್ಥಿರ bool
one_ವಾದ (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ argv[pos++][0] != '\0';
}

ಸ್ಥಿರ bool
two_arguments (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ;

  ಆದರೆ (streq (argv[pos], "!"))
    {
      advance (false);
      ಮೌಲ್ಯ = ! one_ವಾದ ();
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      ಮೌಲ್ಯ = unary_operator ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    beyond ();
  ಹಿಂತಿರುಗಿ (ಮೌಲ್ಯ);
}

ಸ್ಥಿರ bool
three_arguments (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಂಕ bop = binop (argv[pos + 1]);

  ಆದರೆ (0 <= bop)
    ಮೌಲ್ಯ = ದ್ವಿಮಾನ_operator (false, bop);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (argv[pos], "!"))
    {
      advance (true);
      ಮೌಲ್ಯ = !two_arguments ();
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      ಮೌಲ್ಯ = one_ವಾದ ();
      advance (false);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    ಮೌಲ್ಯ = ಅಭಿವ್ಯಕ್ತಿ_3 ();
  ಇಲ್ಲದಿದ್ದರೆ
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  ಹಿಂತಿರುಗಿ (ಮೌಲ್ಯ);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
ಸ್ಥಿರ bool
posixtest (ಪೂರ್ಣಾಂಕ nargs)
{
  bool ಮೌಲ್ಯ;

  ಆಯ್ಕೆ (nargs)
    {
      ಸ್ಥಿತಿ 1:
        ಮೌಲ್ಯ = one_ವಾದ ();
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 2:
        ಮೌಲ್ಯ = two_arguments ();
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 3:
        ಮೌಲ್ಯ = three_arguments ();
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 4:
        ಆದರೆ (streq (argv[pos], "!"))
          {
            advance (true);
            ಮೌಲ್ಯ = !three_arguments ();
            ನಿಲ್ಲಿಸು;
          }
        ಆದರೆ (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            ಮೌಲ್ಯ = two_arguments ();
            advance (false);
            ನಿಲ್ಲಿಸು;
          }
        FALLTHROUGH;
      ಸ್ಥಿತಿ 5:
      ಪೂರ್ವನಿಯೋಜಿತ:
        affirm (0 < nargs);
        ಮೌಲ್ಯ = ಅಭಿವ್ಯಕ್ತಿ_3 ();
    }

  ಹಿಂತಿರುಗಿ (ಮೌಲ್ಯ);
}

#ಆದರೆ defined TEST_STANDALONE

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      fputs (_("\
Usage: test ಅಭಿವ್ಯಕ್ತಿ\n\
  or:  test\n\
  or:  [ ಅಭಿವ್ಯಕ್ತಿ ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), stdout);
      fputs (_("\
Exit with the ಸ್ಥಿತಿ_2 determined by ಅಭಿವ್ಯಕ್ತಿ.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
An omitted ಅಭಿವ್ಯಕ್ತಿ defaults ಗಮ್ಯ false.  Otherwise,\n\
ಅಭಿವ್ಯಕ್ತಿ is true or false and sets ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2.  It is one ದ:\n\
"), stdout);
      fputs (_("\
\n\
  ( ಅಭಿವ್ಯಕ್ತಿ )               ಅಭಿವ್ಯಕ್ತಿ is true\n\
  ! ಅಭಿವ್ಯಕ್ತಿ                 ಅಭಿವ್ಯಕ್ತಿ is false\n\
  EXPRESSION1 -a EXPRESSION2   both EXPRESSION1 and EXPRESSION2 are true\n\
  EXPRESSION1 -o EXPRESSION2   either EXPRESSION1 or EXPRESSION2 is true\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -n STRING            the ಉದ್ದಳತೆ ದ STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent ಗಮ್ಯ -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the ಉದ್ದಳತೆ ದ STRING is zero\n\
"));
      fputs (_("\
  STRING1 = STRING2    the ಅಕ್ಷರಸರಣಿಗಳು are equal\n\
  STRING1 != STRING2   the ಅಕ್ಷರಸರಣಿಗಳು are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 ಒಳ the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 ಒಳ the current locale\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  INTEGER1 -eq INTEGER2   INTEGER1 is equal ಗಮ್ಯ INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal ಗಮ್ಯ INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -le INTEGER2   INTEGER1 is less than or equal ಗಮ್ಯ INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ne INTEGER2   INTEGER1 is not equal ಗಮ್ಯ INTEGER2\n\
"));
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  FILE1 -ef FILE2   FILE1 and FILE2 have the same device and inode numbers\n\
"));
      oputs (_("\
  FILE1 -nt FILE2   FILE1 is newer (modification date) than FILE2\n\
"));
      oputs (_("\
  FILE1 -ot FILE2   FILE1 is older than FILE2\n\
"));
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -b FILE     FILE exists and is ಖಂಡ special\n\
"));
      oputs (_("\
  -c FILE     FILE exists and is character special\n\
"));
      oputs (_("\
  -d FILE     FILE exists and is a directory\n\
"));
      oputs (_("\
  -e FILE     FILE exists\n\
"));
      oputs (_("\
  -f FILE     FILE exists and is a regular file\n\
"));
      oputs (_("\
  -g FILE     FILE exists and its ಗಣ_2-ಗುಂಪು-ID ಬಿಟ್ is ಗಣ_2\n\
"));
      oputs (_("\
  -G FILE     FILE exists and is owned by the effective ಗುಂಪು ID\n\
"));
      oputs (_("\
  -h FILE     FILE exists and is a symbolic link (same as -L)\n\
"));
      oputs (_("\
  -k FILE     FILE exists and has its sticky ಬಿಟ್ ಗಣ_2\n\
"));
      oputs (_("\
  -L FILE     FILE exists and is a symbolic link (same as -h)\n\
"));
      oputs (_("\
  -N FILE     FILE exists and has been modified since it was ಕೊನೆಯ ಪಠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has ಪಠ access\n\
"));
      oputs (_("\
  -s FILE     FILE exists and has a ಗಾತ್ರ_2 greater than zero\n\
"));
      oputs (_("\
  -S FILE     FILE exists and is a socket\n\
"));
      oputs (_("\
  -t FD       file descriptor FD is opened on a terminal\n\
"));
      oputs (_("\
  -u FILE     FILE exists and its ಗಣ_2-user-ID ಬಿಟ್ is ಗಣ_2\n\
"));
      oputs (_("\
  -w FILE     FILE exists and the user has ಲಿಖ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or ಹುಡುಕು) access\n\
"));
      fputs (_("\
\n\
Except ಗಾಗಿ -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need ಗಮ್ಯ be escaped (e.g., by backslashes) ಗಾಗಿ shells.\n\
INTEGER may also be -l STRING, which evaluates ಗಮ್ಯ the ಉದ್ದಳತೆ ದ STRING.\n\
"), stdout);
      fputs (_("\
\n\
ದ್ವಿಮಾನ_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
or 'test EXPR1 || test EXPR2' instead.\n\
"), stdout);
      fputs (_("\
\n\
'[' honors --help and --ಆವೃತ್ತಿ_3, but 'test' treats them as ಅಕ್ಷರಸರಣಿಗಳು_2.\n\
"), stdout);
      ಬರೆ (USAGE_BUILTIN_WARNING, _("test and/or ["));
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}
#ಆದರೆ_ಅಂತ್ಯ /* TEST_STANDALONE */

#ಆದರೆ !defined TEST_STANDALONE
# define ಮುಖ್ಯ test_command
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Kevin Braunsdorf"), \
  proper_name ("Matthew Bradburn")

/*
 * [:
 *	'[' expr ']'
 * test:
 *	test expr
 */
ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ margc, ಅಕ್ಷರ **margv)
{
  bool ಮೌಲ್ಯ;

#ಆದರೆ !defined TEST_STANDALONE
  ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;

  ಸಂಕೇತ_2 = setjmp (test_exit_buf);

  ಆದರೆ (ಸಂಕೇತ_2)
    ಹಿಂತಿರುಗಿ (test_error_return);
#ಇಲ್ಲದಿದ್ದರೆ /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#ಆದರೆ_ಅಂತ್ಯ /* TEST_STANDALONE */

  argv = margv;

  ಆದರೆ (test_ಬಹುಲಕ == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      ಆದರೆ (margc == 2)
        {
          ಆದರೆ (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          ಆದರೆ (streq (margv[1], "--version"))
            {
              ಆವೃತ್ತಿ_etc (stdout, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME, ಆವೃತ್ತಿ, AUTHORS,
                           (ಅಕ್ಷರ *) ಖಾಲಿ);
              test_main_return (EXIT_SUCCESS);
            }
        }
      ಆದರೆ (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  ಆದರೆ (pos >= argc)
    test_main_return (TEST_FALSE);

  ಮೌಲ್ಯ = posixtest (argc - 1);

  ಆದರೆ (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (ಮೌಲ್ಯ ? TEST_TRUE : TEST_FALSE);
}
