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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "issymlink.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stat-time.h"
#অন্তর্ভাব "strnumcmp.h"
#অন্তর্ভাব "test.h"

#অন্তর্ভাব <stdarg.h>

#যদি HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
গণনা { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
গণনা binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#যদি defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) প্রত্যাগম val
#অন্যথা
   স্থির jmp_buf test_exit_buf;
   স্থির পূর্ণাঙ্ক test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

স্থির পূর্ণাঙ্ক pos;		/* The offset of the current argument in ARGV. */
স্থির পূর্ণাঙ্ক argc;	/* The number of arguments present in ARGV. */
স্থির বর্ণ **argv;	/* The argument list. */

স্থির bool unary_operator (শূন্য);
স্থির bool দ্বিমিক_operator (bool, গণনা binop);
স্থির bool two_arguments (শূন্য);
স্থির bool three_arguments (শূন্য);
স্থির bool posixtest (পূর্ণাঙ্ক);

স্থির bool রাশিমালা (শূন্য);
স্থির bool term (শূন্য);
স্থির bool and (শূন্য);
স্থির bool or (শূন্য);

স্থির শূন্য beyond (শূন্য);

ATTRIBUTE_FORMAT ((printf, 1, 2))
স্থির _Noreturn শূন্য
test_syntax_error (বর্ণ ধ্রুব *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

স্থির শূন্য
advance (bool f)
{
  ++pos;

  যদি (f && pos >= argc)
    beyond ();
}

স্থির শূন্য
unary_advance (শূন্য)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
স্থির _Noreturn শূন্য
beyond (শূন্য)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
স্থির বর্ণ ধ্রুব *
find_int (বর্ণ ধ্রুব *string)
{
  বর্ণ ধ্রুব *p;
  বর্ণ ধ্রুব *সংখ্যা_start;

  প্রতি (p = string; isspace (to_uchar (*p)); p++)
    অনুবর্ত;

  যদি (*p == '+')
    {
      p++;
      সংখ্যা_start = p;
    }
  অন্যথা
    {
      সংখ্যা_start = p;
      p += (*p == '-');
    }

  যদি (c_isdigit (*p++))
    {
      যাবত্ (c_isdigit (*p))
        p++;
      যাবত্ (isspace (to_uchar (*p)))
        p++;
      যদি (!*p)
        প্রত্যাগম সংখ্যা_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
স্থির সংরচনা timespec
get_mtime (বর্ণ ধ্রুব *filename)
{
  সংরচনা stat finfo;
  প্রত্যাগম (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
স্থির পূর্ণাঙ্ক
binop (বর্ণ ধ্রুব *s)
{
  প্রত্যাগম (  streq (s, "="  ) ? EQ_STRING_BINOP
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
স্থির bool
term (শূন্য)
{
  bool মান;
  bool negated = false;
  পূর্ণাঙ্ক bop;

  /* Deal with leading 'not's.  */
  যাবত্ (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  যদি (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  যদি (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      পূর্ণাঙ্ক nargs;

      advance (true);

      প্রতি (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        যদি (nargs == 4)
          {
            nargs = argc - pos;
            ভঙ্গ;
          }

      মান = posixtest (nargs);
      যদি (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      অন্যথা
        যদি (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  অন্যথা যদি (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    মান = দ্বিমিক_operator (true, bop);
  অন্যথা যদি (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    মান = দ্বিমিক_operator (false, bop);

  /* It might be a switch type argument.  */
  অন্যথা যদি (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    মান = unary_operator ();
  অন্যথা
    {
      মান = (argv[pos][0] != '\0');
      advance (false);
    }

  প্রত্যাগম negated ^ মান;
}

স্থির bool
দ্বিমিক_operator (bool l_is_l, গণনা binop bop)
{
  পূর্ণাঙ্ক op;

  যদি (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  যদি (r_is_l)
    advance (false);

  pos += 3;

  বিকল্প (bop)
    {
    পক্ষ EQ_BINOP: পক্ষ GE_BINOP: পক্ষ GT_BINOP:
    পক্ষ LE_BINOP: পক্ষ LT_BINOP: পক্ষ NE_BINOP:
      {
        বর্ণ lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        বর্ণ rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        বর্ণ ধ্রুব *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        বর্ণ ধ্রুব *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        পূর্ণাঙ্ক cmp = strintcmp (l, r);
        বিকল্প (+bop)
          {
          পক্ষ EQ_BINOP: প্রত্যাগম cmp == 0;
          পক্ষ GE_BINOP: প্রত্যাগম cmp >= 0;
          পক্ষ GT_BINOP: প্রত্যাগম cmp >  0;
          পক্ষ LE_BINOP: প্রত্যাগম cmp <= 0;
          পক্ষ LT_BINOP: প্রত্যাগম cmp <  0;
          পক্ষ NE_BINOP: প্রত্যাগম cmp != 0;
          }
        unreachable ();
      }

    পক্ষ NT_BINOP: পক্ষ OT_BINOP:
      {
        যদি (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        পূর্ণাঙ্ক cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        প্রত্যাগম bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    পক্ষ EF_BINOP:
      যদি (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      অন্যথা
        {
          সংরচনা stat st[2];
          প্রত্যাগম (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    পক্ষ EQ_STRING_BINOP:
    পক্ষ NE_STRING_BINOP:
      প্রত্যাগম streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    পক্ষ GT_STRING_BINOP:
    পক্ষ LT_STRING_BINOP:
      {
        পূর্ণাঙ্ক cmp = strcoll (argv[op - 1], argv[op + 1]);
        প্রত্যাগম bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

স্থির bool
unary_operator (শূন্য)
{
  সংরচনা stat stat_buf;

  বিকল্প (argv[pos][1])
    {
    স্বভাব:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    পক্ষ 'e':			/* file exists in the file system? */
      unary_advance ();
      প্রত্যাগম stat (argv[pos - 1], &stat_buf) == 0;

    পক্ষ 'r':			/* file is readable? */
      unary_advance ();
      প্রত্যাগম euidaccess (argv[pos - 1], R_OK) == 0;

    পক্ষ 'w':			/* File is writable? */
      unary_advance ();
      প্রত্যাগম euidaccess (argv[pos - 1], W_OK) == 0;

    পক্ষ 'x':			/* File is executable? */
      unary_advance ();
      প্রত্যাগম euidaccess (argv[pos - 1], X_OK) == 0;

    পক্ষ 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        যদি (stat (argv[pos - 1], &stat_buf) != 0)
          প্রত্যাগম false;
        সংরচনা timespec atime = get_stat_atime (&stat_buf);
        সংরচনা timespec mtime = get_stat_mtime (&stat_buf);
        প্রত্যাগম (timespec_cmp (mtime, atime) > 0);
      }

    পক্ষ 'O':			/* File is owned by you? */
      {
        unary_advance ();
        যদি (stat (argv[pos - 1], &stat_buf) != 0)
          প্রত্যাগম false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        প্রত্যাগম ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    পক্ষ 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        যদি (stat (argv[pos - 1], &stat_buf) != 0)
          প্রত্যাগম false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        প্রত্যাগম ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    পক্ষ 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    পক্ষ 'd':			/* File is a directory? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    পক্ষ 's':			/* File has something in it? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    পক্ষ 'S':			/* File is a socket? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    পক্ষ 'c':			/* File is character special? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    পক্ষ 'b':			/* File is block special? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    পক্ষ 'p':			/* File is a named pipe? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    পক্ষ 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    পক্ষ 'h':			/* File is a symbolic link? */
      unary_advance ();
      প্রত্যাগম issymlink (argv[pos - 1]) == 1;

    পক্ষ 'u':			/* File is setuid? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    পক্ষ 'g':			/* File is setgid? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    পক্ষ 'k':			/* File has sticky bit set? */
      unary_advance ();
      প্রত্যাগম (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    পক্ষ 't':			/* File (fd) is a terminal? */
      {
        long পূর্ণাঙ্ক fd;
        বর্ণ ধ্রুব *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, রিক্ত, 10);
        প্রত্যাগম (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    পক্ষ 'n':			/* True if arg has some length. */
      unary_advance ();
      প্রত্যাগম argv[pos - 1][0] != 0;

    পক্ষ 'z':			/* True if arg has no length. */
      unary_advance ();
      প্রত্যাগম argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
স্থির bool
and (শূন্য)
{
  bool মান = true;

  যাবত্ (true)
    {
      মান &= term ();
      যদি (! (pos < argc && streq (argv[pos], "-a")))
        প্রত্যাগম মান;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
স্থির bool
or (শূন্য)
{
  bool মান = false;

  যাবত্ (true)
    {
      মান |= and ();
      যদি (! (pos < argc && streq (argv[pos], "-o")))
        প্রত্যাগম মান;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
স্থির bool
রাশিমালা (শূন্য)
{
  যদি (pos >= argc)
    beyond ();

  প্রত্যাগম or ();		/* Same with this. */
}

স্থির bool
one_argument (শূন্য)
{
  প্রত্যাগম argv[pos++][0] != '\0';
}

স্থির bool
two_arguments (শূন্য)
{
  bool মান;

  যদি (streq (argv[pos], "!"))
    {
      advance (false);
      মান = ! one_argument ();
    }
  অন্যথা যদি (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      মান = unary_operator ();
    }
  অন্যথা
    beyond ();
  প্রত্যাগম (মান);
}

স্থির bool
three_arguments (শূন্য)
{
  bool মান;
  পূর্ণাঙ্ক bop = binop (argv[pos + 1]);

  যদি (0 <= bop)
    মান = দ্বিমিক_operator (false, bop);
  অন্যথা যদি (streq (argv[pos], "!"))
    {
      advance (true);
      মান = !two_arguments ();
    }
  অন্যথা যদি (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      মান = one_argument ();
      advance (false);
    }
  অন্যথা যদি (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    মান = রাশিমালা ();
  অন্যথা
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  প্রত্যাগম (মান);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
স্থির bool
posixtest (পূর্ণাঙ্ক nargs)
{
  bool মান;

  বিকল্প (nargs)
    {
      পক্ষ 1:
        মান = one_argument ();
        ভঙ্গ;

      পক্ষ 2:
        মান = two_arguments ();
        ভঙ্গ;

      পক্ষ 3:
        মান = three_arguments ();
        ভঙ্গ;

      পক্ষ 4:
        যদি (streq (argv[pos], "!"))
          {
            advance (true);
            মান = !three_arguments ();
            ভঙ্গ;
          }
        যদি (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            মান = two_arguments ();
            advance (false);
            ভঙ্গ;
          }
        FALLTHROUGH;
      পক্ষ 5:
      স্বভাব:
        affirm (0 < nargs);
        মান = রাশিমালা ();
    }

  প্রত্যাগম (মান);
}

#যদি defined TEST_STANDALONE

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
  -N FILE     FILE exists and has been modified since it was last পঠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has পঠ access\n\
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
  -w FILE     FILE exists and the user has লিখ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or অনুসন্ধান) access\n\
"));
      fputs (_("\
\n\
Except প্রতি -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) প্রতি shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
দ্বিমিক_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#যদি !defined TEST_STANDALONE
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
পূর্ণাঙ্ক
main (পূর্ণাঙ্ক margc, বর্ণ **margv)
{
  bool মান;

#যদি !defined TEST_STANDALONE
  পূর্ণাঙ্ক code;

  code = setjmp (test_exit_buf);

  যদি (code)
    প্রত্যাগম (test_error_return);
#অন্যথা /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  যদি (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      যদি (margc == 2)
        {
          যদি (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          যদি (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (বর্ণ *) রিক্ত);
              test_main_return (EXIT_SUCCESS);
            }
        }
      যদি (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  যদি (pos >= argc)
    test_main_return (TEST_FALSE);

  মান = posixtest (argc - 1);

  যদি (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (মান ? TEST_TRUE : TEST_FALSE);
}
