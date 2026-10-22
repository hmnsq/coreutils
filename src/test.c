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

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "issymlink.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "strnumcmp.h"
#ಅನ್ತರ್ಭಾವ "test.h"

#ಅನ್ತರ್ಭಾವ <stdarg.h>

#ಯದಿ HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
ಗಣನಾ { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
ಗಣನಾ binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#ಯದಿ defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) ಪ್ರತ್ಯಾಗಮ val
#ಅನ್ಯಥಾ
   ಸ್ಥಿರ jmp_buf test_exit_buf;
   ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ pos;		/* The offset of the current argument in ARGV. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ argc;	/* The number of arguments present in ARGV. */
ಸ್ಥಿರ ವರ್ಣ **argv;	/* The argument list. */

ಸ್ಥಿರ bool unary_operator (ಶೂನ್ಯ);
ಸ್ಥಿರ bool ದ್ವಿಮಾನ_operator (bool, ಗಣನಾ binop);
ಸ್ಥಿರ bool two_arguments (ಶೂನ್ಯ);
ಸ್ಥಿರ bool three_arguments (ಶೂನ್ಯ);
ಸ್ಥಿರ bool posixtest (ಪೂರ್ಣಾಙ್ಕ);

ಸ್ಥಿರ bool ಅಭಿವ್ಯಕ್ತಿ (ಶೂನ್ಯ);
ಸ್ಥಿರ bool term (ಶೂನ್ಯ);
ಸ್ಥಿರ bool and (ಶೂನ್ಯ);
ಸ್ಥಿರ bool or (ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ beyond (ಶೂನ್ಯ);

ATTRIBUTE_FORMAT ((printf, 1, 2))
ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
test_syntax_error (ವರ್ಣ ಧ್ರುವ *format, ...)
{
  va_list ap;
  va_start (ap, format);
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

  ಯದಿ (f && pos >= argc)
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
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
find_int (ವರ್ಣ ಧ್ರುವ *string)
{
  ವರ್ಣ ಧ್ರುವ *p;
  ವರ್ಣ ಧ್ರುವ *ಸಂಖ್ಯೆ_start;

  ಪ್ರತಿ (p = string; isspace (to_uchar (*p)); p++)
    ಅನುವರ್ತ;

  ಯದಿ (*p == '+')
    {
      p++;
      ಸಂಖ್ಯೆ_start = p;
    }
  ಅನ್ಯಥಾ
    {
      ಸಂಖ್ಯೆ_start = p;
      p += (*p == '-');
    }

  ಯದಿ (c_isdigit (*p++))
    {
      ಯಾವತ್ (c_isdigit (*p))
        p++;
      ಯಾವತ್ (isspace (to_uchar (*p)))
        p++;
      ಯದಿ (!*p)
        ಪ್ರತ್ಯಾಗಮ ಸಂಖ್ಯೆ_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
ಸ್ಥಿರ ಸಂರಚನಾ timespec
get_mtime (ವರ್ಣ ಧ್ರುವ *filename)
{
  ಸಂರಚನಾ stat finfo;
  ಪ್ರತ್ಯಾಗಮ (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
binop (ವರ್ಣ ಧ್ರುವ *s)
{
  ಪ್ರತ್ಯಾಗಮ (  streq (s, "="  ) ? EQ_STRING_BINOP
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
term (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ;
  bool negated = false;
  ಪೂರ್ಣಾಙ್ಕ bop;

  /* Deal with leading 'not's.  */
  ಯಾವತ್ (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  ಯದಿ (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  ಯದಿ (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      ಪೂರ್ಣಾಙ್ಕ nargs;

      advance (true);

      ಪ್ರತಿ (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        ಯದಿ (nargs == 4)
          {
            nargs = argc - pos;
            ಭಙ್ಗ;
          }

      ಮೌಲ್ಯ = posixtest (nargs);
      ಯದಿ (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      ಅನ್ಯಥಾ
        ಯದಿ (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  ಅನ್ಯಥಾ ಯದಿ (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    ಮೌಲ್ಯ = ದ್ವಿಮಾನ_operator (true, bop);
  ಅನ್ಯಥಾ ಯದಿ (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    ಮೌಲ್ಯ = ದ್ವಿಮಾನ_operator (false, bop);

  /* It might be a switch type argument.  */
  ಅನ್ಯಥಾ ಯದಿ (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    ಮೌಲ್ಯ = unary_operator ();
  ಅನ್ಯಥಾ
    {
      ಮೌಲ್ಯ = (argv[pos][0] != '\0');
      advance (false);
    }

  ಪ್ರತ್ಯಾಗಮ negated ^ ಮೌಲ್ಯ;
}

ಸ್ಥಿರ bool
ದ್ವಿಮಾನ_operator (bool l_is_l, ಗಣನಾ binop bop)
{
  ಪೂರ್ಣಾಙ್ಕ op;

  ಯದಿ (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  ಯದಿ (r_is_l)
    advance (false);

  pos += 3;

  ವಿಕಲ್ಪ (bop)
    {
    ಪಕ್ಷ EQ_BINOP: ಪಕ್ಷ GE_BINOP: ಪಕ್ಷ GT_BINOP:
    ಪಕ್ಷ LE_BINOP: ಪಕ್ಷ LT_BINOP: ಪಕ್ಷ NE_BINOP:
      {
        ವರ್ಣ lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ವರ್ಣ rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ವರ್ಣ ಧ್ರುವ *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        ವರ್ಣ ಧ್ರುವ *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        ಪೂರ್ಣಾಙ್ಕ cmp = strintcmp (l, r);
        ವಿಕಲ್ಪ (+bop)
          {
          ಪಕ್ಷ EQ_BINOP: ಪ್ರತ್ಯಾಗಮ cmp == 0;
          ಪಕ್ಷ GE_BINOP: ಪ್ರತ್ಯಾಗಮ cmp >= 0;
          ಪಕ್ಷ GT_BINOP: ಪ್ರತ್ಯಾಗಮ cmp >  0;
          ಪಕ್ಷ LE_BINOP: ಪ್ರತ್ಯಾಗಮ cmp <= 0;
          ಪಕ್ಷ LT_BINOP: ಪ್ರತ್ಯಾಗಮ cmp <  0;
          ಪಕ್ಷ NE_BINOP: ಪ್ರತ್ಯಾಗಮ cmp != 0;
          }
        unreachable ();
      }

    ಪಕ್ಷ NT_BINOP: ಪಕ್ಷ OT_BINOP:
      {
        ಯದಿ (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        ಪೂರ್ಣಾಙ್ಕ cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        ಪ್ರತ್ಯಾಗಮ bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    ಪಕ್ಷ EF_BINOP:
      ಯದಿ (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      ಅನ್ಯಥಾ
        {
          ಸಂರಚನಾ stat st[2];
          ಪ್ರತ್ಯಾಗಮ (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    ಪಕ್ಷ EQ_STRING_BINOP:
    ಪಕ್ಷ NE_STRING_BINOP:
      ಪ್ರತ್ಯಾಗಮ streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    ಪಕ್ಷ GT_STRING_BINOP:
    ಪಕ್ಷ LT_STRING_BINOP:
      {
        ಪೂರ್ಣಾಙ್ಕ cmp = strcoll (argv[op - 1], argv[op + 1]);
        ಪ್ರತ್ಯಾಗಮ bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

ಸ್ಥಿರ bool
unary_operator (ಶೂನ್ಯ)
{
  ಸಂರಚನಾ stat stat_buf;

  ವಿಕಲ್ಪ (argv[pos][1])
    {
    ಸ್ವಭಾವ:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    ಪಕ್ಷ 'e':			/* file exists in the file system? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ stat (argv[pos - 1], &stat_buf) == 0;

    ಪಕ್ಷ 'r':			/* file is readable? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ euidaccess (argv[pos - 1], R_OK) == 0;

    ಪಕ್ಷ 'w':			/* File is writable? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ euidaccess (argv[pos - 1], W_OK) == 0;

    ಪಕ್ಷ 'x':			/* File is executable? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ euidaccess (argv[pos - 1], X_OK) == 0;

    ಪಕ್ಷ 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        ಯದಿ (stat (argv[pos - 1], &stat_buf) != 0)
          ಪ್ರತ್ಯಾಗಮ false;
        ಸಂರಚನಾ timespec atime = get_stat_atime (&stat_buf);
        ಸಂರಚನಾ timespec mtime = get_stat_mtime (&stat_buf);
        ಪ್ರತ್ಯಾಗಮ (timespec_cmp (mtime, atime) > 0);
      }

    ಪಕ್ಷ 'O':			/* File is owned by you? */
      {
        unary_advance ();
        ಯದಿ (stat (argv[pos - 1], &stat_buf) != 0)
          ಪ್ರತ್ಯಾಗಮ false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        ಪ್ರತ್ಯಾಗಮ ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    ಪಕ್ಷ 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        ಯದಿ (stat (argv[pos - 1], &stat_buf) != 0)
          ಪ್ರತ್ಯಾಗಮ false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        ಪ್ರತ್ಯಾಗಮ ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    ಪಕ್ಷ 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    ಪಕ್ಷ 'd':			/* File is a directory? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    ಪಕ್ಷ 's':			/* File has something in it? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    ಪಕ್ಷ 'S':			/* File is a socket? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    ಪಕ್ಷ 'c':			/* File is character special? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    ಪಕ್ಷ 'b':			/* File is block special? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    ಪಕ್ಷ 'p':			/* File is a named pipe? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    ಪಕ್ಷ 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    ಪಕ್ಷ 'h':			/* File is a symbolic link? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ issymlink (argv[pos - 1]) == 1;

    ಪಕ್ಷ 'u':			/* File is setuid? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    ಪಕ್ಷ 'g':			/* File is setgid? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    ಪಕ್ಷ 'k':			/* File has sticky bit set? */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    ಪಕ್ಷ 't':			/* File (fd) is a terminal? */
      {
        long ಪೂರ್ಣಾಙ್ಕ fd;
        ವರ್ಣ ಧ್ರುವ *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ರಿಕ್ತ, 10);
        ಪ್ರತ್ಯಾಗಮ (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    ಪಕ್ಷ 'n':			/* True if arg has some length. */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ argv[pos - 1][0] != 0;

    ಪಕ್ಷ 'z':			/* True if arg has no length. */
      unary_advance ();
      ಪ್ರತ್ಯಾಗಮ argv[pos - 1][0] == '\0';
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

  ಯಾವತ್ (true)
    {
      ಮೌಲ್ಯ &= term ();
      ಯದಿ (! (pos < argc && streq (argv[pos], "-a")))
        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
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

  ಯಾವತ್ (true)
    {
      ಮೌಲ್ಯ |= and ();
      ಯದಿ (! (pos < argc && streq (argv[pos], "-o")))
        ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ (ಶೂನ್ಯ)
{
  ಯದಿ (pos >= argc)
    beyond ();

  ಪ್ರತ್ಯಾಗಮ or ();		/* Same with this. */
}

ಸ್ಥಿರ bool
one_argument (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ argv[pos++][0] != '\0';
}

ಸ್ಥಿರ bool
two_arguments (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ;

  ಯದಿ (streq (argv[pos], "!"))
    {
      advance (false);
      ಮೌಲ್ಯ = ! one_argument ();
    }
  ಅನ್ಯಥಾ ಯದಿ (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      ಮೌಲ್ಯ = unary_operator ();
    }
  ಅನ್ಯಥಾ
    beyond ();
  ಪ್ರತ್ಯಾಗಮ (ಮೌಲ್ಯ);
}

ಸ್ಥಿರ bool
three_arguments (ಶೂನ್ಯ)
{
  bool ಮೌಲ್ಯ;
  ಪೂರ್ಣಾಙ್ಕ bop = binop (argv[pos + 1]);

  ಯದಿ (0 <= bop)
    ಮೌಲ್ಯ = ದ್ವಿಮಾನ_operator (false, bop);
  ಅನ್ಯಥಾ ಯದಿ (streq (argv[pos], "!"))
    {
      advance (true);
      ಮೌಲ್ಯ = !two_arguments ();
    }
  ಅನ್ಯಥಾ ಯದಿ (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      ಮೌಲ್ಯ = one_argument ();
      advance (false);
    }
  ಅನ್ಯಥಾ ಯದಿ (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    ಮೌಲ್ಯ = ಅಭಿವ್ಯಕ್ತಿ ();
  ಅನ್ಯಥಾ
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  ಪ್ರತ್ಯಾಗಮ (ಮೌಲ್ಯ);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
ಸ್ಥಿರ bool
posixtest (ಪೂರ್ಣಾಙ್ಕ nargs)
{
  bool ಮೌಲ್ಯ;

  ವಿಕಲ್ಪ (nargs)
    {
      ಪಕ್ಷ 1:
        ಮೌಲ್ಯ = one_argument ();
        ಭಙ್ಗ;

      ಪಕ್ಷ 2:
        ಮೌಲ್ಯ = two_arguments ();
        ಭಙ್ಗ;

      ಪಕ್ಷ 3:
        ಮೌಲ್ಯ = three_arguments ();
        ಭಙ್ಗ;

      ಪಕ್ಷ 4:
        ಯದಿ (streq (argv[pos], "!"))
          {
            advance (true);
            ಮೌಲ್ಯ = !three_arguments ();
            ಭಙ್ಗ;
          }
        ಯದಿ (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            ಮೌಲ್ಯ = two_arguments ();
            advance (false);
            ಭಙ್ಗ;
          }
        FALLTHROUGH;
      ಪಕ್ಷ 5:
      ಸ್ವಭಾವ:
        affirm (0 < nargs);
        ಮೌಲ್ಯ = ಅಭಿವ್ಯಕ್ತಿ ();
    }

  ಪ್ರತ್ಯಾಗಮ (ಮೌಲ್ಯ);
}

#ಯದಿ defined TEST_STANDALONE

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      fputs (_("\
Usage: test EXPRESSION\n\
  or:  test\n\
  or:  [ EXPRESSION ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), stdout);
      fputs (_("\
Exit with the status determined by EXPRESSION.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
An omitted EXPRESSION defaults to false.  Otherwise,\n\
EXPRESSION is true or false and sets exit status.  It is one of:\n\
"), stdout);
      fputs (_("\
\n\
  ( EXPRESSION )               EXPRESSION is true\n\
  ! EXPRESSION                 EXPRESSION is false\n\
  EXPRESSION1 -a EXPRESSION2   both EXPRESSION1 and EXPRESSION2 are true\n\
  EXPRESSION1 -o EXPRESSION2   either EXPRESSION1 or EXPRESSION2 is true\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -n STRING            the length of STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent to -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the length of STRING is zero\n\
"));
      fputs (_("\
  STRING1 = STRING2    the strings are equal\n\
  STRING1 != STRING2   the strings are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 in the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 in the current locale\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  INTEGER1 -eq INTEGER2   INTEGER1 is equal to INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal to INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -le INTEGER2   INTEGER1 is less than or equal to INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ne INTEGER2   INTEGER1 is not equal to INTEGER2\n\
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
  -b FILE     FILE exists and is block special\n\
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
  -g FILE     FILE exists and its set-group-ID bit is set\n\
"));
      oputs (_("\
  -G FILE     FILE exists and is owned by the effective group ID\n\
"));
      oputs (_("\
  -h FILE     FILE exists and is a symbolic link (same as -L)\n\
"));
      oputs (_("\
  -k FILE     FILE exists and has its sticky bit set\n\
"));
      oputs (_("\
  -L FILE     FILE exists and is a symbolic link (same as -h)\n\
"));
      oputs (_("\
  -N FILE     FILE exists and has been modified since it was last ಪಠ\n\
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
  -s FILE     FILE exists and has a size greater than zero\n\
"));
      oputs (_("\
  -S FILE     FILE exists and is a socket\n\
"));
      oputs (_("\
  -t FD       file descriptor FD is opened on a terminal\n\
"));
      oputs (_("\
  -u FILE     FILE exists and its set-user-ID bit is set\n\
"));
      oputs (_("\
  -w FILE     FILE exists and the user has ಲಿಖ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or ಹುಡುಕು) access\n\
"));
      fputs (_("\
\n\
Except ಪ್ರತಿ -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) ಪ್ರತಿ shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
ದ್ವಿಮಾನ_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
or 'test EXPR1 || test EXPR2' instead.\n\
"), stdout);
      fputs (_("\
\n\
'[' honors --help and --version, but 'test' treats them as STRINGs.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, _("test and/or ["));
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
#endif /* TEST_STANDALONE */

#ಯದಿ !defined TEST_STANDALONE
# define main test_command
#endif

#define AUTHORS \
  proper_name ("Kevin Braunsdorf"), \
  proper_name ("Matthew Bradburn")

/*
 * [:
 *	'[' expr ']'
 * test:
 *	test expr
 */
ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ margc, ವರ್ಣ **margv)
{
  bool ಮೌಲ್ಯ;

#ಯದಿ !defined TEST_STANDALONE
  ಪೂರ್ಣಾಙ್ಕ code;

  code = setjmp (test_exit_buf);

  ಯದಿ (code)
    ಪ್ರತ್ಯಾಗಮ (test_error_return);
#ಅನ್ಯಥಾ /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  ಯದಿ (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      ಯದಿ (margc == 2)
        {
          ಯದಿ (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          ಯದಿ (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (ವರ್ಣ *) ರಿಕ್ತ);
              test_main_return (EXIT_SUCCESS);
            }
        }
      ಯದಿ (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  ಯದಿ (pos >= argc)
    test_main_return (TEST_FALSE);

  ಮೌಲ್ಯ = posixtest (argc - 1);

  ಯದಿ (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (ಮೌಲ್ಯ ? TEST_TRUE : TEST_FALSE);
}
