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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "issymlink.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "strnumcmp.h"
#அந்தர்பாவ "test.h"

#அந்தர்பாவ <stdarg.h>

#எனில் HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
எண்ணிக்கை { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
எண்ணிக்கை binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#எனில் defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) திரும்பு val
#இல்லையெனில்
   நிலையான jmp_buf test_exit_buf;
   நிலையான முழு test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

நிலையான முழு pos;		/* The offset of the current argument in ARGV. */
நிலையான முழு argc;	/* The number of arguments present in ARGV. */
நிலையான எழுத்து **argv;	/* The argument list. */

நிலையான bool unary_operator (வெறுமை);
நிலையான bool இருமம்_operator (bool, எண்ணிக்கை binop);
நிலையான bool two_arguments (வெறுமை);
நிலையான bool three_arguments (வெறுமை);
நிலையான bool posixtest (முழு);

நிலையான bool கோவை (வெறுமை);
நிலையான bool term (வெறுமை);
நிலையான bool and (வெறுமை);
நிலையான bool or (வெறுமை);

நிலையான வெறுமை beyond (வெறுமை);

ATTRIBUTE_FORMAT ((printf, 1, 2))
நிலையான _Noreturn வெறுமை
test_syntax_error (எழுத்து மாறிலி *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

நிலையான வெறுமை
advance (bool f)
{
  ++pos;

  எனில் (f && pos >= argc)
    beyond ();
}

நிலையான வெறுமை
unary_advance (வெறுமை)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
நிலையான _Noreturn வெறுமை
beyond (வெறுமை)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
நிலையான எழுத்து மாறிலி *
find_int (எழுத்து மாறிலி *string)
{
  எழுத்து மாறிலி *p;
  எழுத்து மாறிலி *எண்_start;

  க்கு (p = string; isspace (to_uchar (*p)); p++)
    தொடர்;

  எனில் (*p == '+')
    {
      p++;
      எண்_start = p;
    }
  இல்லையெனில்
    {
      எண்_start = p;
      p += (*p == '-');
    }

  எனில் (c_isdigit (*p++))
    {
      போது (c_isdigit (*p))
        p++;
      போது (isspace (to_uchar (*p)))
        p++;
      எனில் (!*p)
        திரும்பு எண்_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
நிலையான கட்டமைப்பு timespec
get_mtime (எழுத்து மாறிலி *filename)
{
  கட்டமைப்பு stat finfo;
  திரும்பு (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
நிலையான முழு
binop (எழுத்து மாறிலி *s)
{
  திரும்பு (  streq (s, "="  ) ? EQ_STRING_BINOP
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
நிலையான bool
term (வெறுமை)
{
  bool மதிப்பு;
  bool negated = false;
  முழு bop;

  /* Deal with leading 'not's.  */
  போது (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  எனில் (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  எனில் (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      முழு nargs;

      advance (true);

      க்கு (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        எனில் (nargs == 4)
          {
            nargs = argc - pos;
            நிறுத்து;
          }

      மதிப்பு = posixtest (nargs);
      எனில் (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      இல்லையெனில்
        எனில் (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  இல்லையெனில் எனில் (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    மதிப்பு = இருமம்_operator (true, bop);
  இல்லையெனில் எனில் (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    மதிப்பு = இருமம்_operator (false, bop);

  /* It might be a switch type argument.  */
  இல்லையெனில் எனில் (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    மதிப்பு = unary_operator ();
  இல்லையெனில்
    {
      மதிப்பு = (argv[pos][0] != '\0');
      advance (false);
    }

  திரும்பு negated ^ மதிப்பு;
}

நிலையான bool
இருமம்_operator (bool l_is_l, எண்ணிக்கை binop bop)
{
  முழு op;

  எனில் (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  எனில் (r_is_l)
    advance (false);

  pos += 3;

  தேர்வு (bop)
    {
    நிலை EQ_BINOP: நிலை GE_BINOP: நிலை GT_BINOP:
    நிலை LE_BINOP: நிலை LT_BINOP: நிலை NE_BINOP:
      {
        எழுத்து lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        எழுத்து rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        எழுத்து மாறிலி *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        எழுத்து மாறிலி *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        முழு cmp = strintcmp (l, r);
        தேர்வு (+bop)
          {
          நிலை EQ_BINOP: திரும்பு cmp == 0;
          நிலை GE_BINOP: திரும்பு cmp >= 0;
          நிலை GT_BINOP: திரும்பு cmp >  0;
          நிலை LE_BINOP: திரும்பு cmp <= 0;
          நிலை LT_BINOP: திரும்பு cmp <  0;
          நிலை NE_BINOP: திரும்பு cmp != 0;
          }
        unreachable ();
      }

    நிலை NT_BINOP: நிலை OT_BINOP:
      {
        எனில் (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        முழு cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        திரும்பு bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    நிலை EF_BINOP:
      எனில் (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      இல்லையெனில்
        {
          கட்டமைப்பு stat st[2];
          திரும்பு (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    நிலை EQ_STRING_BINOP:
    நிலை NE_STRING_BINOP:
      திரும்பு streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    நிலை GT_STRING_BINOP:
    நிலை LT_STRING_BINOP:
      {
        முழு cmp = strcoll (argv[op - 1], argv[op + 1]);
        திரும்பு bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

நிலையான bool
unary_operator (வெறுமை)
{
  கட்டமைப்பு stat stat_buf;

  தேர்வு (argv[pos][1])
    {
    இயல்பு:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    நிலை 'e':			/* file exists in the file system? */
      unary_advance ();
      திரும்பு stat (argv[pos - 1], &stat_buf) == 0;

    நிலை 'r':			/* file is readable? */
      unary_advance ();
      திரும்பு euidaccess (argv[pos - 1], R_OK) == 0;

    நிலை 'w':			/* File is writable? */
      unary_advance ();
      திரும்பு euidaccess (argv[pos - 1], W_OK) == 0;

    நிலை 'x':			/* File is executable? */
      unary_advance ();
      திரும்பு euidaccess (argv[pos - 1], X_OK) == 0;

    நிலை 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        எனில் (stat (argv[pos - 1], &stat_buf) != 0)
          திரும்பு false;
        கட்டமைப்பு timespec atime = get_stat_atime (&stat_buf);
        கட்டமைப்பு timespec mtime = get_stat_mtime (&stat_buf);
        திரும்பு (timespec_cmp (mtime, atime) > 0);
      }

    நிலை 'O':			/* File is owned by you? */
      {
        unary_advance ();
        எனில் (stat (argv[pos - 1], &stat_buf) != 0)
          திரும்பு false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        திரும்பு ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    நிலை 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        எனில் (stat (argv[pos - 1], &stat_buf) != 0)
          திரும்பு false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        திரும்பு ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    நிலை 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    நிலை 'd':			/* File is a directory? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    நிலை 's':			/* File has something in it? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    நிலை 'S':			/* File is a socket? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    நிலை 'c':			/* File is character special? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    நிலை 'b':			/* File is block special? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    நிலை 'p':			/* File is a named pipe? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    நிலை 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    நிலை 'h':			/* File is a symbolic link? */
      unary_advance ();
      திரும்பு issymlink (argv[pos - 1]) == 1;

    நிலை 'u':			/* File is setuid? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    நிலை 'g':			/* File is setgid? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    நிலை 'k':			/* File has sticky bit set? */
      unary_advance ();
      திரும்பு (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    நிலை 't':			/* File (fd) is a terminal? */
      {
        long முழு fd;
        எழுத்து மாறிலி *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, வெற்று, 10);
        திரும்பு (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    நிலை 'n':			/* True if arg has some length. */
      unary_advance ();
      திரும்பு argv[pos - 1][0] != 0;

    நிலை 'z':			/* True if arg has no length. */
      unary_advance ();
      திரும்பு argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
நிலையான bool
and (வெறுமை)
{
  bool மதிப்பு = true;

  போது (true)
    {
      மதிப்பு &= term ();
      எனில் (! (pos < argc && streq (argv[pos], "-a")))
        திரும்பு மதிப்பு;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
நிலையான bool
or (வெறுமை)
{
  bool மதிப்பு = false;

  போது (true)
    {
      மதிப்பு |= and ();
      எனில் (! (pos < argc && streq (argv[pos], "-o")))
        திரும்பு மதிப்பு;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
நிலையான bool
கோவை (வெறுமை)
{
  எனில் (pos >= argc)
    beyond ();

  திரும்பு or ();		/* Same with this. */
}

நிலையான bool
one_argument (வெறுமை)
{
  திரும்பு argv[pos++][0] != '\0';
}

நிலையான bool
two_arguments (வெறுமை)
{
  bool மதிப்பு;

  எனில் (streq (argv[pos], "!"))
    {
      advance (false);
      மதிப்பு = ! one_argument ();
    }
  இல்லையெனில் எனில் (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      மதிப்பு = unary_operator ();
    }
  இல்லையெனில்
    beyond ();
  திரும்பு (மதிப்பு);
}

நிலையான bool
three_arguments (வெறுமை)
{
  bool மதிப்பு;
  முழு bop = binop (argv[pos + 1]);

  எனில் (0 <= bop)
    மதிப்பு = இருமம்_operator (false, bop);
  இல்லையெனில் எனில் (streq (argv[pos], "!"))
    {
      advance (true);
      மதிப்பு = !two_arguments ();
    }
  இல்லையெனில் எனில் (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      மதிப்பு = one_argument ();
      advance (false);
    }
  இல்லையெனில் எனில் (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    மதிப்பு = கோவை ();
  இல்லையெனில்
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  திரும்பு (மதிப்பு);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
நிலையான bool
posixtest (முழு nargs)
{
  bool மதிப்பு;

  தேர்வு (nargs)
    {
      நிலை 1:
        மதிப்பு = one_argument ();
        நிறுத்து;

      நிலை 2:
        மதிப்பு = two_arguments ();
        நிறுத்து;

      நிலை 3:
        மதிப்பு = three_arguments ();
        நிறுத்து;

      நிலை 4:
        எனில் (streq (argv[pos], "!"))
          {
            advance (true);
            மதிப்பு = !three_arguments ();
            நிறுத்து;
          }
        எனில் (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            மதிப்பு = two_arguments ();
            advance (false);
            நிறுத்து;
          }
        FALLTHROUGH;
      நிலை 5:
      இயல்பு:
        affirm (0 < nargs);
        மதிப்பு = கோவை ();
    }

  திரும்பு (மதிப்பு);
}

#எனில் defined TEST_STANDALONE

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
  -N FILE     FILE exists and has been modified since it was last பட\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has பட access\n\
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
  -w FILE     FILE exists and the user has லிக access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or தேடல்) access\n\
"));
      fputs (_("\
\n\
Except க்கு -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) க்கு shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
இருமம்_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#எனில் !defined TEST_STANDALONE
# define முதன்மை test_command
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
முழு
முதன்மை (முழு margc, எழுத்து **margv)
{
  bool மதிப்பு;

#எனில் !defined TEST_STANDALONE
  முழு code;

  code = setjmp (test_exit_buf);

  எனில் (code)
    திரும்பு (test_error_return);
#இல்லையெனில் /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  எனில் (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      எனில் (margc == 2)
        {
          எனில் (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          எனில் (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (எழுத்து *) வெற்று);
              test_main_return (EXIT_SUCCESS);
            }
        }
      எனில் (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  எனில் (pos >= argc)
    test_main_return (TEST_FALSE);

  மதிப்பு = posixtest (argc - 1);

  எனில் (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (மதிப்பு ? TEST_TRUE : TEST_FALSE);
}
