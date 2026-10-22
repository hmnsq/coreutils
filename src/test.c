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
# define test_main_return(val) ফেরত val
#অন্যথা
   স্থির jmp_buf test_exit_buf;
   স্থির পূর্ণসংখ্যা test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

স্থির পূর্ণসংখ্যা pos;		/* The offset of the current argument in ARGV. */
স্থির পূর্ণসংখ্যা argc;	/* The number of arguments present in ARGV. */
স্থির অক্ষর **argv;	/* The argument list. */

স্থির bool unary_operator (শূন্য);
স্থির bool দ্বিমিক_operator (bool, গণনা binop);
স্থির bool two_arguments (শূন্য);
স্থির bool three_arguments (শূন্য);
স্থির bool posixtest (পূর্ণসংখ্যা);

স্থির bool রাশিমালা (শূন্য);
স্থির bool term (শূন্য);
স্থির bool and (শূন্য);
স্থির bool or (শূন্য);

স্থির শূন্য beyond (শূন্য);

ATTRIBUTE_FORMAT ((printf, 1, 2))
স্থির _Noreturn শূন্য
test_syntax_error (অক্ষর ধ্রুবক *format, ...)
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
স্থির অক্ষর ধ্রুবক *
find_int (অক্ষর ধ্রুবক *string)
{
  অক্ষর ধ্রুবক *p;
  অক্ষর ধ্রুবক *সংখ্যা_start;

  জন্য (p = string; isspace (to_uchar (*p)); p++)
    চালু;

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
      যখন (c_isdigit (*p))
        p++;
      যখন (isspace (to_uchar (*p)))
        p++;
      যদি (!*p)
        ফেরত সংখ্যা_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
স্থির কাঠামো timespec
get_mtime (অক্ষর ধ্রুবক *filename)
{
  কাঠামো stat finfo;
  ফেরত (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
স্থির পূর্ণসংখ্যা
binop (অক্ষর ধ্রুবক *s)
{
  ফেরত (  streq (s, "="  ) ? EQ_STRING_BINOP
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
  পূর্ণসংখ্যা bop;

  /* Deal with leading 'not's.  */
  যখন (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  যদি (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  যদি (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      পূর্ণসংখ্যা nargs;

      advance (true);

      জন্য (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        যদি (nargs == 4)
          {
            nargs = argc - pos;
            বিরতি;
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

  ফেরত negated ^ মান;
}

স্থির bool
দ্বিমিক_operator (bool l_is_l, গণনা binop bop)
{
  পূর্ণসংখ্যা op;

  যদি (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  যদি (r_is_l)
    advance (false);

  pos += 3;

  নির্বাচন (bop)
    {
    ক্ষেত্র EQ_BINOP: ক্ষেত্র GE_BINOP: ক্ষেত্র GT_BINOP:
    ক্ষেত্র LE_BINOP: ক্ষেত্র LT_BINOP: ক্ষেত্র NE_BINOP:
      {
        অক্ষর lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        অক্ষর rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        অক্ষর ধ্রুবক *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        অক্ষর ধ্রুবক *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        পূর্ণসংখ্যা cmp = strintcmp (l, r);
        নির্বাচন (+bop)
          {
          ক্ষেত্র EQ_BINOP: ফেরত cmp == 0;
          ক্ষেত্র GE_BINOP: ফেরত cmp >= 0;
          ক্ষেত্র GT_BINOP: ফেরত cmp >  0;
          ক্ষেত্র LE_BINOP: ফেরত cmp <= 0;
          ক্ষেত্র LT_BINOP: ফেরত cmp <  0;
          ক্ষেত্র NE_BINOP: ফেরত cmp != 0;
          }
        unreachable ();
      }

    ক্ষেত্র NT_BINOP: ক্ষেত্র OT_BINOP:
      {
        যদি (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        পূর্ণসংখ্যা cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        ফেরত bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    ক্ষেত্র EF_BINOP:
      যদি (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      অন্যথা
        {
          কাঠামো stat st[2];
          ফেরত (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    ক্ষেত্র EQ_STRING_BINOP:
    ক্ষেত্র NE_STRING_BINOP:
      ফেরত streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    ক্ষেত্র GT_STRING_BINOP:
    ক্ষেত্র LT_STRING_BINOP:
      {
        পূর্ণসংখ্যা cmp = strcoll (argv[op - 1], argv[op + 1]);
        ফেরত bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

স্থির bool
unary_operator (শূন্য)
{
  কাঠামো stat stat_buf;

  নির্বাচন (argv[pos][1])
    {
    পূর্বনির্ধারিত:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    ক্ষেত্র 'e':			/* file exists in the file system? */
      unary_advance ();
      ফেরত stat (argv[pos - 1], &stat_buf) == 0;

    ক্ষেত্র 'r':			/* file is readable? */
      unary_advance ();
      ফেরত euidaccess (argv[pos - 1], R_OK) == 0;

    ক্ষেত্র 'w':			/* File is writable? */
      unary_advance ();
      ফেরত euidaccess (argv[pos - 1], W_OK) == 0;

    ক্ষেত্র 'x':			/* File is executable? */
      unary_advance ();
      ফেরত euidaccess (argv[pos - 1], X_OK) == 0;

    ক্ষেত্র 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        যদি (stat (argv[pos - 1], &stat_buf) != 0)
          ফেরত false;
        কাঠামো timespec atime = get_stat_atime (&stat_buf);
        কাঠামো timespec mtime = get_stat_mtime (&stat_buf);
        ফেরত (timespec_cmp (mtime, atime) > 0);
      }

    ক্ষেত্র 'O':			/* File is owned by you? */
      {
        unary_advance ();
        যদি (stat (argv[pos - 1], &stat_buf) != 0)
          ফেরত false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        ফেরত ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    ক্ষেত্র 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        যদি (stat (argv[pos - 1], &stat_buf) != 0)
          ফেরত false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        ফেরত ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    ক্ষেত্র 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    ক্ষেত্র 'd':			/* File is a directory? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    ক্ষেত্র 's':			/* File has something in it? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_আকার);

    ক্ষেত্র 'S':			/* File is a socket? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    ক্ষেত্র 'c':			/* File is character special? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    ক্ষেত্র 'b':			/* File is block special? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    ক্ষেত্র 'p':			/* File is a named pipe? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    ক্ষেত্র 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    ক্ষেত্র 'h':			/* File is a symbolic link? */
      unary_advance ();
      ফেরত issymlink (argv[pos - 1]) == 1;

    ক্ষেত্র 'u':			/* File is setuid? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    ক্ষেত্র 'g':			/* File is setgid? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    ক্ষেত্র 'k':			/* File has sticky bit set? */
      unary_advance ();
      ফেরত (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    ক্ষেত্র 't':			/* File (fd) is a terminal? */
      {
        long পূর্ণসংখ্যা fd;
        অক্ষর ধ্রুবক *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ফাঁকা, 10);
        ফেরত (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    ক্ষেত্র 'n':			/* True if arg has some length. */
      unary_advance ();
      ফেরত argv[pos - 1][0] != 0;

    ক্ষেত্র 'z':			/* True if arg has no length. */
      unary_advance ();
      ফেরত argv[pos - 1][0] == '\0';
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

  যখন (true)
    {
      মান &= term ();
      যদি (! (pos < argc && streq (argv[pos], "-a")))
        ফেরত মান;
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

  যখন (true)
    {
      মান |= and ();
      যদি (! (pos < argc && streq (argv[pos], "-o")))
        ফেরত মান;
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

  ফেরত or ();		/* Same with this. */
}

স্থির bool
one_argument (শূন্য)
{
  ফেরত argv[pos++][0] != '\0';
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
  ফেরত (মান);
}

স্থির bool
three_arguments (শূন্য)
{
  bool মান;
  পূর্ণসংখ্যা bop = binop (argv[pos + 1]);

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
  ফেরত (মান);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
স্থির bool
posixtest (পূর্ণসংখ্যা nargs)
{
  bool মান;

  নির্বাচন (nargs)
    {
      ক্ষেত্র 1:
        মান = one_argument ();
        বিরতি;

      ক্ষেত্র 2:
        মান = two_arguments ();
        বিরতি;

      ক্ষেত্র 3:
        মান = three_arguments ();
        বিরতি;

      ক্ষেত্র 4:
        যদি (streq (argv[pos], "!"))
          {
            advance (true);
            মান = !three_arguments ();
            বিরতি;
          }
        যদি (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            মান = two_arguments ();
            advance (false);
            বিরতি;
          }
        FALLTHROUGH;
      ক্ষেত্র 5:
      পূর্বনির্ধারিত:
        affirm (0 < nargs);
        মান = রাশিমালা ();
    }

  ফেরত (মান);
}

#যদি defined TEST_STANDALONE

শূন্য
usage (পূর্ণসংখ্যা status)
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
  -n STRING            the দৈর্ঘ্য of STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent to -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the দৈর্ঘ্য of STRING is zero\n\
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
  -s FILE     FILE exists and has a আকার_2 greater than zero\n\
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
Except জন্য -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) জন্য shells.\n\
INTEGER may also be -l STRING, which evaluates to the দৈর্ঘ্য of STRING.\n\
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
# define প্রধান test_command
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
পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা margc, অক্ষর **margv)
{
  bool মান;

#যদি !defined TEST_STANDALONE
  পূর্ণসংখ্যা code;

  code = setjmp (test_exit_buf);

  যদি (code)
    ফেরত (test_error_return);
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
                           (অক্ষর *) ফাঁকা);
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
