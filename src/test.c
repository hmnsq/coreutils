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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#تعریف_کرو TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME (test_منوال == TEST_TEST ? "test" : "[")

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "issymlink.h"
#ضمن "quote.h"
#ضمن "stat-time.h"
#ضمن "strnumcmp.h"
#ضمن "test.h"

#ضمن <stdarg.h>

#إذا HAVE_SYS_PARAM_H
# include <sys/param.h>
#جے_انت

/* Exit status for syntax errors, etc.  */
تعداد { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
تعداد binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#إذا defined TEST_STANDALONE
# define test_exit(val) باہر_نکلو (val)
# define test_main_return(val) أرجع val
#وإلا
   ساكن jmp_buf test_exit_buf;
   ساكن صحيح test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#جے_انت /* !TEST_STANDALONE */

ساكن صحيح pos;		/* The offset of the current argument in ARGV. */
ساكن صحيح argc;	/* The number of arguments present in ARGV. */
ساكن حرف **argv;	/* The argument list. */

ساكن bool unary_operator (فراغ);
ساكن bool ثنائی_operator (bool, تعداد binop);
ساكن bool two_arguments (فراغ);
ساكن bool three_arguments (فراغ);
ساكن bool posixtest (صحيح);

ساكن bool اظہاریہ (فراغ);
ساكن bool رکن (فراغ);
ساكن bool and (فراغ);
ساكن bool or (فراغ);

ساكن فراغ beyond (فراغ);

ATTRIBUTE_FORMAT ((لکھو, 1, 2))
ساكن _Noreturn فراغ
test_syntax_error (حرف ثابت *format, ...)
{
  va_list ap;
  va_شروع (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

ساكن فراغ
advance (bool f)
{
  ++pos;

  إذا (f && pos >= argc)
    beyond ();
}

ساكن فراغ
unary_advance (فراغ)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
ساكن _Noreturn فراغ
beyond (فراغ)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
ساكن حرف ثابت *
لبھو_int (حرف ثابت *string)
{
  حرف ثابت *p;
  حرف ثابت *نمبر_شروع;

  لكل (p = string; خلا_اے (منزل_uchar (*p)); p++)
    استمر;

  إذا (*p == '+')
    {
      p++;
      نمبر_شروع = p;
    }
  وإلا
    {
      نمبر_شروع = p;
      p += (*p == '-');
    }

  إذا (c_isdigit (*p++))
    {
      طالما (c_isdigit (*p))
        p++;
      طالما (خلا_اے (منزل_uchar (*p)))
        p++;
      إذا (!*p)
        أرجع نمبر_شروع;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
ساكن بنية timespec
get_mtime (حرف ثابت *filename)
{
  بنية stat finfo;
  أرجع (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
ساكن صحيح
binop (حرف ثابت *s)
{
  أرجع (  streq (s, "="  ) ? EQ_STRING_BINOP
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
ساكن bool
رکن (فراغ)
{
  bool قدر;
  bool negated = false;
  صحيح bop;

  /* Deal with leading 'not's.  */
  طالما (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  إذا (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  إذا (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      صحيح nargs;

      advance (true);

      لكل (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        إذا (nargs == 4)
          {
            nargs = argc - pos;
            اكسر;
          }

      قدر = posixtest (nargs);
      إذا (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      وإلا
        إذا (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  وإلا إذا (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    قدر = ثنائی_operator (true, bop);
  وإلا إذا (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    قدر = ثنائی_operator (false, bop);

  /* It might be a switch type argument.  */
  وإلا إذا (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    قدر = unary_operator ();
  وإلا
    {
      قدر = (argv[pos][0] != '\0');
      advance (false);
    }

  أرجع negated ^ قدر;
}

ساكن bool
ثنائی_operator (bool l_is_l, تعداد binop bop)
{
  صحيح op;

  إذا (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  إذا (r_is_l)
    advance (false);

  pos += 3;

  بدل (bop)
    {
    حالة EQ_BINOP: حالة GE_BINOP: حالة GT_BINOP:
    حالة LE_BINOP: حالة LT_BINOP: حالة NE_BINOP:
      {
        حرف lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        حرف rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        حرف ثابت *l = (l_is_l
                         ? umaxtostr (لڑی_لمبائی (argv[op - 1]), lbuf)
                         : لبھو_int (argv[op - 1]));
        حرف ثابت *r = (r_is_l
                         ? umaxtostr (لڑی_لمبائی (argv[op + 2]), rbuf)
                         : لبھو_int (argv[op + 1]));
        صحيح cmp = strintcmp (l, r);
        بدل (+bop)
          {
          حالة EQ_BINOP: أرجع cmp == 0;
          حالة GE_BINOP: أرجع cmp >= 0;
          حالة GT_BINOP: أرجع cmp >  0;
          حالة LE_BINOP: أرجع cmp <= 0;
          حالة LT_BINOP: أرجع cmp <  0;
          حالة NE_BINOP: أرجع cmp != 0;
          }
        unreachable ();
      }

    حالة NT_BINOP: حالة OT_BINOP:
      {
        إذا (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        صحيح cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        أرجع bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    حالة EF_BINOP:
      إذا (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      وإلا
        {
          بنية stat st[2];
          أرجع (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    حالة EQ_STRING_BINOP:
    حالة NE_STRING_BINOP:
      أرجع streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    حالة GT_STRING_BINOP:
    حالة LT_STRING_BINOP:
      {
        صحيح cmp = strcoll (argv[op - 1], argv[op + 1]);
        أرجع bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

ساكن bool
unary_operator (فراغ)
{
  بنية stat stat_buf;

  بدل (argv[pos][1])
    {
    افتراضي:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    حالة 'e':			/* file exists in the file system? */
      unary_advance ();
      أرجع stat (argv[pos - 1], &stat_buf) == 0;

    حالة 'r':			/* file is readable? */
      unary_advance ();
      أرجع euidaccess (argv[pos - 1], R_OK) == 0;

    حالة 'w':			/* File is writable? */
      unary_advance ();
      أرجع euidaccess (argv[pos - 1], W_OK) == 0;

    حالة 'x':			/* File is executable? */
      unary_advance ();
      أرجع euidaccess (argv[pos - 1], X_OK) == 0;

    حالة 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        إذا (stat (argv[pos - 1], &stat_buf) != 0)
          أرجع false;
        بنية timespec atime = get_stat_atime (&stat_buf);
        بنية timespec mtime = get_stat_mtime (&stat_buf);
        أرجع (timespec_cmp (mtime, atime) > 0);
      }

    حالة 'O':			/* File is owned by you? */
      {
        unary_advance ();
        إذا (stat (argv[pos - 1], &stat_buf) != 0)
          أرجع false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        أرجع ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    حالة 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        إذا (stat (argv[pos - 1], &stat_buf) != 0)
          أرجع false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        أرجع ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    حالة 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_منوال));

    حالة 'd':			/* File is a directory? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_منوال));

    حالة 's':			/* File has something in it? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_ناپ);

    حالة 'S':			/* File is a socket? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_منوال));

    حالة 'c':			/* File is character special? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_منوال));

    حالة 'b':			/* File is block special? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_منوال));

    حالة 'p':			/* File is a named pipe? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_منوال));

    حالة 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    حالة 'h':			/* File is a symbolic link? */
      unary_advance ();
      أرجع issymlink (argv[pos - 1]) == 1;

    حالة 'u':			/* File is setuid? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_منوال & S_ISUID));

    حالة 'g':			/* File is setgid? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_منوال & S_ISGID));

    حالة 'k':			/* File has sticky bit set? */
      unary_advance ();
      أرجع (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_منوال & S_ISVTX));

    حالة 't':			/* File (fd) is a terminal? */
      {
        لما صحيح fd;
        حرف ثابت *arg;
        unary_advance ();
        arg = لبھو_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, عدم, 10);
        أرجع (errno != ERANGE && 0 <= fd && fd <= ودھ_توں_ودھ_عدد && isatty (fd));
      }

    حالة 'n':			/* True if arg has some length. */
      unary_advance ();
      أرجع argv[pos - 1][0] != 0;

    حالة 'z':			/* True if arg has no length. */
      unary_advance ();
      أرجع argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
ساكن bool
and (فراغ)
{
  bool قدر = true;

  طالما (true)
    {
      قدر &= رکن ();
      إذا (! (pos < argc && streq (argv[pos], "-a")))
        أرجع قدر;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
ساكن bool
or (فراغ)
{
  bool قدر = false;

  طالما (true)
    {
      قدر |= and ();
      إذا (! (pos < argc && streq (argv[pos], "-o")))
        أرجع قدر;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
ساكن bool
اظہاریہ (فراغ)
{
  إذا (pos >= argc)
    beyond ();

  أرجع or ();		/* Same with this. */
}

ساكن bool
one_دلیل (فراغ)
{
  أرجع argv[pos++][0] != '\0';
}

ساكن bool
two_arguments (فراغ)
{
  bool قدر;

  إذا (streq (argv[pos], "!"))
    {
      advance (false);
      قدر = ! one_دلیل ();
    }
  وإلا إذا (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      قدر = unary_operator ();
    }
  وإلا
    beyond ();
  أرجع (قدر);
}

ساكن bool
three_arguments (فراغ)
{
  bool قدر;
  صحيح bop = binop (argv[pos + 1]);

  إذا (0 <= bop)
    قدر = ثنائی_operator (false, bop);
  وإلا إذا (streq (argv[pos], "!"))
    {
      advance (true);
      قدر = !two_arguments ();
    }
  وإلا إذا (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      قدر = one_دلیل ();
      advance (false);
    }
  وإلا إذا (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    قدر = اظہاریہ ();
  وإلا
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  أرجع (قدر);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
ساكن bool
posixtest (صحيح nargs)
{
  bool قدر;

  بدل (nargs)
    {
      حالة 1:
        قدر = one_دلیل ();
        اكسر;

      حالة 2:
        قدر = two_arguments ();
        اكسر;

      حالة 3:
        قدر = three_arguments ();
        اكسر;

      حالة 4:
        إذا (streq (argv[pos], "!"))
          {
            advance (true);
            قدر = !three_arguments ();
            اكسر;
          }
        إذا (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            قدر = two_arguments ();
            advance (false);
            اكسر;
          }
        FALLTHROUGH;
      حالة 5:
      افتراضي:
        affirm (0 < nargs);
        قدر = اظہاریہ ();
    }

  أرجع (قدر);
}

#إذا defined TEST_STANDALONE

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      fputs (_("\
Usage: test عبارت\n\
  or:  test\n\
  or:  [ عبارت ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), stdout);
      fputs (_("\
Exit with the حالت determined by عبارت.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
An omitted عبارت defaults منزل false.  Otherwise,\n\
عبارت is true or false and sets باہر_نکلو حالت.  It is one دا:\n\
"), stdout);
      fputs (_("\
\n\
  ( عبارت )               عبارت is true\n\
  ! عبارت                 عبارت is false\n\
  EXPRESSION1 -a EXPRESSION2   both EXPRESSION1 and EXPRESSION2 are true\n\
  EXPRESSION1 -o EXPRESSION2   either EXPRESSION1 or EXPRESSION2 is true\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -n STRING            the لمائی دا STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent منزل -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the لمائی دا STRING is zero\n\
"));
      fputs (_("\
  STRING1 = STRING2    the لڑیاں are equal\n\
  STRING1 != STRING2   the لڑیاں are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 وچ the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 وچ the current locale\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  INTEGER1 -eq INTEGER2   INTEGER1 is equal منزل INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal منزل INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -le INTEGER2   INTEGER1 is less than or equal منزل INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ne INTEGER2   INTEGER1 is not equal منزل INTEGER2\n\
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
  -b FILE     FILE exists and is بلاک special\n\
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
  -g FILE     FILE exists and its سیٹ_2-گروہ-ID ثنائی_ہندسہ is سیٹ_2\n\
"));
      oputs (_("\
  -G FILE     FILE exists and is owned by the effective گروہ ID\n\
"));
      oputs (_("\
  -h FILE     FILE exists and is a symbolic link (same as -L)\n\
"));
      oputs (_("\
  -k FILE     FILE exists and has its sticky ثنائی_ہندسہ سیٹ_2\n\
"));
      oputs (_("\
  -L FILE     FILE exists and is a symbolic link (same as -h)\n\
"));
      oputs (_("\
  -N FILE     FILE exists and has been modified since it was آخری اقرأ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has اقرأ access\n\
"));
      oputs (_("\
  -s FILE     FILE exists and has a ناپ greater than zero\n\
"));
      oputs (_("\
  -S FILE     FILE exists and is a socket\n\
"));
      oputs (_("\
  -t FD       file descriptor FD is opened on a terminal\n\
"));
      oputs (_("\
  -u FILE     FILE exists and its سیٹ_2-user-ID ثنائی_ہندسہ is سیٹ_2\n\
"));
      oputs (_("\
  -w FILE     FILE exists and the user has اكتب access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or تلاش) access\n\
"));
      fputs (_("\
\n\
Except لكل -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need منزل be escaped (e.g., by backslashes) لكل shells.\n\
INTEGER may also be -l STRING, which evaluates منزل the لمائی دا STRING.\n\
"), stdout);
      fputs (_("\
\n\
ثنائی_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
or 'test EXPR1 || test EXPR2' instead.\n\
"), stdout);
      fputs (_("\
\n\
'[' honors --help and --نسخہ_3, but 'test' treats them as لڑیاں_2.\n\
"), stdout);
      لکھو (USAGE_BUILTIN_WARNING, _("test and/or ["));
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}
#جے_انت /* TEST_STANDALONE */

#إذا !defined TEST_STANDALONE
# define مکھ test_command
#جے_انت

#تعریف_کرو AUTHORS \
  proper_name ("Kevin Braunsdorf"), \
  proper_name ("Matthew Bradburn")

/*
 * [:
 *	'[' expr ']'
 * test:
 *	test expr
 */
صحيح
مکھ (صحيح margc, حرف **margv)
{
  bool قدر;

#إذا !defined TEST_STANDALONE
  صحيح کوڈ;

  کوڈ = setjmp (test_exit_buf);

  إذا (کوڈ)
    أرجع (test_error_return);
#وإلا /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  سیٹ_پروگرام_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#جے_انت /* TEST_STANDALONE */

  argv = margv;

  إذا (test_منوال == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      إذا (margc == 2)
        {
          إذا (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          إذا (streq (margv[1], "--version"))
            {
              نسخہ_etc (stdout, پروگرام_NAME, PACKAGE_NAME, نسخہ, AUTHORS,
                           (حرف *) عدم);
              test_main_return (EXIT_SUCCESS);
            }
        }
      إذا (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  إذا (pos >= argc)
    test_main_return (TEST_FALSE);

  قدر = posixtest (argc - 1);

  إذا (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (قدر ? TEST_TRUE : TEST_FALSE);
}
