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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME (test_ਬਹੁਲਕ == TEST_TEST ? "test" : "[")

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "issymlink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strnumcmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>

#ਜੇ HAVE_SYS_PARAM_H
# include <sys/param.h>
#ਜੇ_ਅੰਤ

/* Exit status for syntax errors, etc.  */
ਗਿਣਤੀ { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
ਗਿਣਤੀ binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#ਜੇ defined TEST_STANDALONE
# define test_exit(val) ਬਾਹਰ_ਨਿਕਲੋ (val)
# define test_main_return(val) ਵਾਪਸ val
#ਨਹੀਂਤਾਂ
   ਸਥਿਰ jmp_buf test_exit_buf;
   ਸਥਿਰ ਪੂਰਨ test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#ਜੇ_ਅੰਤ /* !TEST_STANDALONE */

ਸਥਿਰ ਪੂਰਨ pos;		/* The offset of the current argument in ARGV. */
ਸਥਿਰ ਪੂਰਨ argc;	/* The number of arguments present in ARGV. */
ਸਥਿਰ ਅੱਖਰ **argv;	/* The argument list. */

ਸਥਿਰ bool unary_operator (ਖਾਲੀ);
ਸਥਿਰ bool ਦੁਆਧਾਰੀ_operator (bool, ਗਿਣਤੀ binop);
ਸਥਿਰ bool two_arguments (ਖਾਲੀ);
ਸਥਿਰ bool three_arguments (ਖਾਲੀ);
ਸਥਿਰ bool posixtest (ਪੂਰਨ);

ਸਥਿਰ bool ਵਿਅੰਜਕ_3 (ਖਾਲੀ);
ਸਥਿਰ bool ਪਦ (ਖਾਲੀ);
ਸਥਿਰ bool and (ਖਾਲੀ);
ਸਥਿਰ bool or (ਖਾਲੀ);

ਸਥਿਰ ਖਾਲੀ beyond (ਖਾਲੀ);

ATTRIBUTE_FORMAT ((ਲਿਖੋ, 1, 2))
ਸਥਿਰ _Noreturn ਖਾਲੀ
test_syntax_error (ਅੱਖਰ ਅਚਲ *format, ...)
{
  va_list ap;
  va_ਸ਼ੁਰੂ (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

ਸਥਿਰ ਖਾਲੀ
advance (bool f)
{
  ++pos;

  ਜੇ (f && pos >= argc)
    beyond ();
}

ਸਥਿਰ ਖਾਲੀ
unary_advance (ਖਾਲੀ)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
ਸਥਿਰ _Noreturn ਖਾਲੀ
beyond (ਖਾਲੀ)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ਲੱਭੋ_int (ਅੱਖਰ ਅਚਲ *string)
{
  ਅੱਖਰ ਅਚਲ *p;
  ਅੱਖਰ ਅਚਲ *ਸੰਖਿਆ_ਸ਼ੁਰੂ;

  ਲਈ (p = string; ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*p)); p++)
    ਜਾਰੀ;

  ਜੇ (*p == '+')
    {
      p++;
      ਸੰਖਿਆ_ਸ਼ੁਰੂ = p;
    }
  ਨਹੀਂਤਾਂ
    {
      ਸੰਖਿਆ_ਸ਼ੁਰੂ = p;
      p += (*p == '-');
    }

  ਜੇ (c_isdigit (*p++))
    {
      ਜਦੋਂ (c_isdigit (*p))
        p++;
      ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*p)))
        p++;
      ਜੇ (!*p)
        ਵਾਪਸ ਸੰਖਿਆ_ਸ਼ੁਰੂ;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
ਸਥਿਰ ਬਣਤਰ timespec
get_mtime (ਅੱਖਰ ਅਚਲ *filename)
{
  ਬਣਤਰ stat finfo;
  ਵਾਪਸ (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
ਸਥਿਰ ਪੂਰਨ
binop (ਅੱਖਰ ਅਚਲ *s)
{
  ਵਾਪਸ (  streq (s, "="  ) ? EQ_STRING_BINOP
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
ਸਥਿਰ bool
ਪਦ (ਖਾਲੀ)
{
  bool ਮੁੱਲ;
  bool negated = false;
  ਪੂਰਨ bop;

  /* Deal with leading 'not's.  */
  ਜਦੋਂ (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  ਜੇ (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  ਜੇ (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      ਪੂਰਨ nargs;

      advance (true);

      ਲਈ (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        ਜੇ (nargs == 4)
          {
            nargs = argc - pos;
            ਰੋਕੋ;
          }

      ਮੁੱਲ = posixtest (nargs);
      ਜੇ (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      ਨਹੀਂਤਾਂ
        ਜੇ (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  ਨਹੀਂਤਾਂ ਜੇ (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    ਮੁੱਲ = ਦੁਆਧਾਰੀ_operator (true, bop);
  ਨਹੀਂਤਾਂ ਜੇ (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    ਮੁੱਲ = ਦੁਆਧਾਰੀ_operator (false, bop);

  /* It might be a switch type argument.  */
  ਨਹੀਂਤਾਂ ਜੇ (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    ਮੁੱਲ = unary_operator ();
  ਨਹੀਂਤਾਂ
    {
      ਮੁੱਲ = (argv[pos][0] != '\0');
      advance (false);
    }

  ਵਾਪਸ negated ^ ਮੁੱਲ;
}

ਸਥਿਰ bool
ਦੁਆਧਾਰੀ_operator (bool l_is_l, ਗਿਣਤੀ binop bop)
{
  ਪੂਰਨ op;

  ਜੇ (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  ਜੇ (r_is_l)
    advance (false);

  pos += 3;

  ਚੋਣ (bop)
    {
    ਹਾਲਤ EQ_BINOP: ਹਾਲਤ GE_BINOP: ਹਾਲਤ GT_BINOP:
    ਹਾਲਤ LE_BINOP: ਹਾਲਤ LT_BINOP: ਹਾਲਤ NE_BINOP:
      {
        ਅੱਖਰ lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ਅੱਖਰ rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ਅੱਖਰ ਅਚਲ *l = (l_is_l
                         ? umaxtostr (ਲੜੀ_ਲੰਬਾਈ (argv[op - 1]), lbuf)
                         : ਲੱਭੋ_int (argv[op - 1]));
        ਅੱਖਰ ਅਚਲ *r = (r_is_l
                         ? umaxtostr (ਲੜੀ_ਲੰਬਾਈ (argv[op + 2]), rbuf)
                         : ਲੱਭੋ_int (argv[op + 1]));
        ਪੂਰਨ cmp = strintcmp (l, r);
        ਚੋਣ (+bop)
          {
          ਹਾਲਤ EQ_BINOP: ਵਾਪਸ cmp == 0;
          ਹਾਲਤ GE_BINOP: ਵਾਪਸ cmp >= 0;
          ਹਾਲਤ GT_BINOP: ਵਾਪਸ cmp >  0;
          ਹਾਲਤ LE_BINOP: ਵਾਪਸ cmp <= 0;
          ਹਾਲਤ LT_BINOP: ਵਾਪਸ cmp <  0;
          ਹਾਲਤ NE_BINOP: ਵਾਪਸ cmp != 0;
          }
        unreachable ();
      }

    ਹਾਲਤ NT_BINOP: ਹਾਲਤ OT_BINOP:
      {
        ਜੇ (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        ਪੂਰਨ cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        ਵਾਪਸ bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    ਹਾਲਤ EF_BINOP:
      ਜੇ (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      ਨਹੀਂਤਾਂ
        {
          ਬਣਤਰ stat st[2];
          ਵਾਪਸ (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    ਹਾਲਤ EQ_STRING_BINOP:
    ਹਾਲਤ NE_STRING_BINOP:
      ਵਾਪਸ streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    ਹਾਲਤ GT_STRING_BINOP:
    ਹਾਲਤ LT_STRING_BINOP:
      {
        ਪੂਰਨ cmp = strcoll (argv[op - 1], argv[op + 1]);
        ਵਾਪਸ bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

ਸਥਿਰ bool
unary_operator (ਖਾਲੀ)
{
  ਬਣਤਰ stat stat_buf;

  ਚੋਣ (argv[pos][1])
    {
    ਮੂਲ:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    ਹਾਲਤ 'e':			/* file exists in the file system? */
      unary_advance ();
      ਵਾਪਸ stat (argv[pos - 1], &stat_buf) == 0;

    ਹਾਲਤ 'r':			/* file is readable? */
      unary_advance ();
      ਵਾਪਸ euidaccess (argv[pos - 1], R_OK) == 0;

    ਹਾਲਤ 'w':			/* File is writable? */
      unary_advance ();
      ਵਾਪਸ euidaccess (argv[pos - 1], W_OK) == 0;

    ਹਾਲਤ 'x':			/* File is executable? */
      unary_advance ();
      ਵਾਪਸ euidaccess (argv[pos - 1], X_OK) == 0;

    ਹਾਲਤ 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        ਜੇ (stat (argv[pos - 1], &stat_buf) != 0)
          ਵਾਪਸ false;
        ਬਣਤਰ timespec atime = get_stat_atime (&stat_buf);
        ਬਣਤਰ timespec mtime = get_stat_mtime (&stat_buf);
        ਵਾਪਸ (timespec_cmp (mtime, atime) > 0);
      }

    ਹਾਲਤ 'O':			/* File is owned by you? */
      {
        unary_advance ();
        ਜੇ (stat (argv[pos - 1], &stat_buf) != 0)
          ਵਾਪਸ false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        ਵਾਪਸ ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    ਹਾਲਤ 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        ਜੇ (stat (argv[pos - 1], &stat_buf) != 0)
          ਵਾਪਸ false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        ਵਾਪਸ ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    ਹਾਲਤ 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_ਬਹੁਲਕ));

    ਹਾਲਤ 'd':			/* File is a directory? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_ਬਹੁਲਕ));

    ਹਾਲਤ 's':			/* File has something in it? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_ਮਾਪ);

    ਹਾਲਤ 'S':			/* File is a socket? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_ਬਹੁਲਕ));

    ਹਾਲਤ 'c':			/* File is character special? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_ਬਹੁਲਕ));

    ਹਾਲਤ 'b':			/* File is block special? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_ਬਹੁਲਕ));

    ਹਾਲਤ 'p':			/* File is a named pipe? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_ਬਹੁਲਕ));

    ਹਾਲਤ 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    ਹਾਲਤ 'h':			/* File is a symbolic link? */
      unary_advance ();
      ਵਾਪਸ issymlink (argv[pos - 1]) == 1;

    ਹਾਲਤ 'u':			/* File is setuid? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_ਬਹੁਲਕ & S_ISUID));

    ਹਾਲਤ 'g':			/* File is setgid? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_ਬਹੁਲਕ & S_ISGID));

    ਹਾਲਤ 'k':			/* File has sticky bit set? */
      unary_advance ();
      ਵਾਪਸ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_ਬਹੁਲਕ & S_ISVTX));

    ਹਾਲਤ 't':			/* File (fd) is a terminal? */
      {
        ਲੰਮਾ ਪੂਰਨ fd;
        ਅੱਖਰ ਅਚਲ *arg;
        unary_advance ();
        arg = ਲੱਭੋ_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ਨਿਰਾ, 10);
        ਵਾਪਸ (errno != ERANGE && 0 <= fd && fd <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ && isatty (fd));
      }

    ਹਾਲਤ 'n':			/* True if arg has some length. */
      unary_advance ();
      ਵਾਪਸ argv[pos - 1][0] != 0;

    ਹਾਲਤ 'z':			/* True if arg has no length. */
      unary_advance ();
      ਵਾਪਸ argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
ਸਥਿਰ bool
and (ਖਾਲੀ)
{
  bool ਮੁੱਲ = true;

  ਜਦੋਂ (true)
    {
      ਮੁੱਲ &= ਪਦ ();
      ਜੇ (! (pos < argc && streq (argv[pos], "-a")))
        ਵਾਪਸ ਮੁੱਲ;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
ਸਥਿਰ bool
or (ਖਾਲੀ)
{
  bool ਮੁੱਲ = false;

  ਜਦੋਂ (true)
    {
      ਮੁੱਲ |= and ();
      ਜੇ (! (pos < argc && streq (argv[pos], "-o")))
        ਵਾਪਸ ਮੁੱਲ;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
ਸਥਿਰ bool
ਵਿਅੰਜਕ_3 (ਖਾਲੀ)
{
  ਜੇ (pos >= argc)
    beyond ();

  ਵਾਪਸ or ();		/* Same with this. */
}

ਸਥਿਰ bool
one_ਦਲੀਲ (ਖਾਲੀ)
{
  ਵਾਪਸ argv[pos++][0] != '\0';
}

ਸਥਿਰ bool
two_arguments (ਖਾਲੀ)
{
  bool ਮੁੱਲ;

  ਜੇ (streq (argv[pos], "!"))
    {
      advance (false);
      ਮੁੱਲ = ! one_ਦਲੀਲ ();
    }
  ਨਹੀਂਤਾਂ ਜੇ (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      ਮੁੱਲ = unary_operator ();
    }
  ਨਹੀਂਤਾਂ
    beyond ();
  ਵਾਪਸ (ਮੁੱਲ);
}

ਸਥਿਰ bool
three_arguments (ਖਾਲੀ)
{
  bool ਮੁੱਲ;
  ਪੂਰਨ bop = binop (argv[pos + 1]);

  ਜੇ (0 <= bop)
    ਮੁੱਲ = ਦੁਆਧਾਰੀ_operator (false, bop);
  ਨਹੀਂਤਾਂ ਜੇ (streq (argv[pos], "!"))
    {
      advance (true);
      ਮੁੱਲ = !two_arguments ();
    }
  ਨਹੀਂਤਾਂ ਜੇ (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      ਮੁੱਲ = one_ਦਲੀਲ ();
      advance (false);
    }
  ਨਹੀਂਤਾਂ ਜੇ (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    ਮੁੱਲ = ਵਿਅੰਜਕ_3 ();
  ਨਹੀਂਤਾਂ
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  ਵਾਪਸ (ਮੁੱਲ);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
ਸਥਿਰ bool
posixtest (ਪੂਰਨ nargs)
{
  bool ਮੁੱਲ;

  ਚੋਣ (nargs)
    {
      ਹਾਲਤ 1:
        ਮੁੱਲ = one_ਦਲੀਲ ();
        ਰੋਕੋ;

      ਹਾਲਤ 2:
        ਮੁੱਲ = two_arguments ();
        ਰੋਕੋ;

      ਹਾਲਤ 3:
        ਮੁੱਲ = three_arguments ();
        ਰੋਕੋ;

      ਹਾਲਤ 4:
        ਜੇ (streq (argv[pos], "!"))
          {
            advance (true);
            ਮੁੱਲ = !three_arguments ();
            ਰੋਕੋ;
          }
        ਜੇ (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            ਮੁੱਲ = two_arguments ();
            advance (false);
            ਰੋਕੋ;
          }
        FALLTHROUGH;
      ਹਾਲਤ 5:
      ਮੂਲ:
        affirm (0 < nargs);
        ਮੁੱਲ = ਵਿਅੰਜਕ_3 ();
    }

  ਵਾਪਸ (ਮੁੱਲ);
}

#ਜੇ defined TEST_STANDALONE

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      fputs (_("\
Usage: test ਵਿਅੰਜਕ\n\
  or:  test\n\
  or:  [ ਵਿਅੰਜਕ ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), stdout);
      fputs (_("\
Exit with the ਸਥਿਤੀ determined by ਵਿਅੰਜਕ.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
An omitted ਵਿਅੰਜਕ defaults ਮੰਜ਼ਿਲ false.  Otherwise,\n\
ਵਿਅੰਜਕ is true or false and sets ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ.  It is one ਦਾ:\n\
"), stdout);
      fputs (_("\
\n\
  ( ਵਿਅੰਜਕ )               ਵਿਅੰਜਕ is true\n\
  ! ਵਿਅੰਜਕ                 ਵਿਅੰਜਕ is false\n\
  EXPRESSION1 -a EXPRESSION2   both EXPRESSION1 and EXPRESSION2 are true\n\
  EXPRESSION1 -o EXPRESSION2   either EXPRESSION1 or EXPRESSION2 is true\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -n STRING            the ਲੰਮਾਈ ਦਾ STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent ਮੰਜ਼ਿਲ -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the ਲੰਮਾਈ ਦਾ STRING is zero\n\
"));
      fputs (_("\
  STRING1 = STRING2    the ਲੜੀਆਂ are equal\n\
  STRING1 != STRING2   the ਲੜੀਆਂ are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 ਵਿੱਚ the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 ਵਿੱਚ the current locale\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  INTEGER1 -eq INTEGER2   INTEGER1 is equal ਮੰਜ਼ਿਲ INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal ਮੰਜ਼ਿਲ INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -le INTEGER2   INTEGER1 is less than or equal ਮੰਜ਼ਿਲ INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ne INTEGER2   INTEGER1 is not equal ਮੰਜ਼ਿਲ INTEGER2\n\
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
  -b FILE     FILE exists and is ਖੰਡ special\n\
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
  -g FILE     FILE exists and its ਸੈੱਟ_2-ਸਮੂਹ-ID ਦੋਆਧਾਰੀ_ਅੰਕ is ਸੈੱਟ_2\n\
"));
      oputs (_("\
  -G FILE     FILE exists and is owned by the effective ਸਮੂਹ ID\n\
"));
      oputs (_("\
  -h FILE     FILE exists and is a symbolic link (same as -L)\n\
"));
      oputs (_("\
  -k FILE     FILE exists and has its sticky ਦੋਆਧਾਰੀ_ਅੰਕ ਸੈੱਟ_2\n\
"));
      oputs (_("\
  -L FILE     FILE exists and is a symbolic link (same as -h)\n\
"));
      oputs (_("\
  -N FILE     FILE exists and has been modified since it was ਆਖਰੀ ਪਠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has ਪਠ access\n\
"));
      oputs (_("\
  -s FILE     FILE exists and has a ਮਾਪ greater than zero\n\
"));
      oputs (_("\
  -S FILE     FILE exists and is a socket\n\
"));
      oputs (_("\
  -t FD       file descriptor FD is opened on a terminal\n\
"));
      oputs (_("\
  -u FILE     FILE exists and its ਸੈੱਟ_2-user-ID ਦੋਆਧਾਰੀ_ਅੰਕ is ਸੈੱਟ_2\n\
"));
      oputs (_("\
  -w FILE     FILE exists and the user has ਲਿਖ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or ਖੋਜ) access\n\
"));
      fputs (_("\
\n\
Except ਲਈ -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need ਮੰਜ਼ਿਲ be escaped (e.g., by backslashes) ਲਈ shells.\n\
INTEGER may also be -l STRING, which evaluates ਮੰਜ਼ਿਲ the ਲੰਮਾਈ ਦਾ STRING.\n\
"), stdout);
      fputs (_("\
\n\
ਦੁਆਧਾਰੀ_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
or 'test EXPR1 || test EXPR2' instead.\n\
"), stdout);
      fputs (_("\
\n\
'[' honors --help and --ਸੰਸਕਰਣ_3, but 'test' treats them as ਲੜੀਆਂ_2.\n\
"), stdout);
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, _("test and/or ["));
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}
#ਜੇ_ਅੰਤ /* TEST_STANDALONE */

#ਜੇ !defined TEST_STANDALONE
# define ਮੁੱਖ test_command
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Kevin Braunsdorf"), \
  proper_name ("Matthew Bradburn")

/*
 * [:
 *	'[' expr ']'
 * test:
 *	test expr
 */
ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ margc, ਅੱਖਰ **margv)
{
  bool ਮੁੱਲ;

#ਜੇ !defined TEST_STANDALONE
  ਪੂਰਨ ਕੋਡ;

  ਕੋਡ = setjmp (test_exit_buf);

  ਜੇ (ਕੋਡ)
    ਵਾਪਸ (test_error_return);
#ਨਹੀਂਤਾਂ /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#ਜੇ_ਅੰਤ /* TEST_STANDALONE */

  argv = margv;

  ਜੇ (test_ਬਹੁਲਕ == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      ਜੇ (margc == 2)
        {
          ਜੇ (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          ਜੇ (streq (margv[1], "--version"))
            {
              ਸੰਸਕਰਣ_etc (stdout, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME, ਸੰਸਕਰਣ, AUTHORS,
                           (ਅੱਖਰ *) ਨਿਰਾ);
              test_main_return (EXIT_SUCCESS);
            }
        }
      ਜੇ (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  ਜੇ (pos >= argc)
    test_main_return (TEST_FALSE);

  ਮੁੱਲ = posixtest (argc - 1);

  ਜੇ (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (ਮੁੱਲ ? TEST_TRUE : TEST_FALSE);
}
