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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#تعریف_کرو TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME (test_منوال == TEST_TEST ? "test" : "[")

#شامل "system.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "issymlink.h"
#شامل "quote.h"
#شامل "stat-time.h"
#شامل "strnumcmp.h"
#شامل "test.h"

#شامل <stdarg.h>

#اگر HAVE_SYS_PARAM_H
# include <sys/param.h>
#اگر_ختم

/* Exit status for syntax errors, etc.  */
شمار { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
شمار binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#اگر defined TEST_STANDALONE
# define test_exit(val) باہر_نکلو (val)
# define test_main_return(val) واپس val
#ورنہ
   ساکن jmp_buf test_exit_buf;
   ساکن صحیح test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#اگر_ختم /* !TEST_STANDALONE */

ساکن صحیح pos;		/* The offset of the current argument in ARGV. */
ساکن صحیح argc;	/* The number of arguments present in ARGV. */
ساکن حرف **argv;	/* The argument list. */

ساکن bool unary_operator (خالی);
ساکن bool ثنائی_operator (bool, شمار binop);
ساکن bool two_arguments (خالی);
ساکن bool three_arguments (خالی);
ساکن bool posixtest (صحیح);

ساکن bool اظہاریہ (خالی);
ساکن bool رکن (خالی);
ساکن bool and (خالی);
ساکن bool or (خالی);

ساکن خالی beyond (خالی);

ATTRIBUTE_FORMAT ((لکھو, 1, 2))
ساکن _Noreturn خالی
test_syntax_error (حرف ثابت *format, ...)
{
  va_list ap;
  va_آغاز (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

ساکن خالی
advance (bool f)
{
  ++pos;

  اگر (f && pos >= argc)
    beyond ();
}

ساکن خالی
unary_advance (خالی)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
ساکن _Noreturn خالی
beyond (خالی)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
ساکن حرف ثابت *
ڈھونڈو_int (حرف ثابت *string)
{
  حرف ثابت *p;
  حرف ثابت *نمبر_آغاز;

  برائے (p = string; خلا_ہے (مقصد_uchar (*p)); p++)
    جاری;

  اگر (*p == '+')
    {
      p++;
      نمبر_آغاز = p;
    }
  ورنہ
    {
      نمبر_آغاز = p;
      p += (*p == '-');
    }

  اگر (c_isdigit (*p++))
    {
      جب (c_isdigit (*p))
        p++;
      جب (خلا_ہے (مقصد_uchar (*p)))
        p++;
      اگر (!*p)
        واپس نمبر_آغاز;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
ساکن ساخت timespec
get_mtime (حرف ثابت *filename)
{
  ساخت stat finfo;
  واپس (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
ساکن صحیح
binop (حرف ثابت *s)
{
  واپس (  streq (s, "="  ) ? EQ_STRING_BINOP
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
ساکن bool
رکن (خالی)
{
  bool قدر;
  bool negated = false;
  صحیح bop;

  /* Deal with leading 'not's.  */
  جب (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  اگر (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  اگر (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      صحیح nargs;

      advance (true);

      برائے (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        اگر (nargs == 4)
          {
            nargs = argc - pos;
            توڑو;
          }

      قدر = posixtest (nargs);
      اگر (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      ورنہ
        اگر (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  ورنہ اگر (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    قدر = ثنائی_operator (true, bop);
  ورنہ اگر (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    قدر = ثنائی_operator (false, bop);

  /* It might be a switch type argument.  */
  ورنہ اگر (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    قدر = unary_operator ();
  ورنہ
    {
      قدر = (argv[pos][0] != '\0');
      advance (false);
    }

  واپس negated ^ قدر;
}

ساکن bool
ثنائی_operator (bool l_is_l, شمار binop bop)
{
  صحیح op;

  اگر (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  اگر (r_is_l)
    advance (false);

  pos += 3;

  انتخاب (bop)
    {
    حالت EQ_BINOP: حالت GE_BINOP: حالت GT_BINOP:
    حالت LE_BINOP: حالت LT_BINOP: حالت NE_BINOP:
      {
        حرف lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        حرف rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        حرف ثابت *l = (l_is_l
                         ? umaxtostr (لڑی_لمبائی (argv[op - 1]), lbuf)
                         : ڈھونڈو_int (argv[op - 1]));
        حرف ثابت *r = (r_is_l
                         ? umaxtostr (لڑی_لمبائی (argv[op + 2]), rbuf)
                         : ڈھونڈو_int (argv[op + 1]));
        صحیح cmp = strintcmp (l, r);
        انتخاب (+bop)
          {
          حالت EQ_BINOP: واپس cmp == 0;
          حالت GE_BINOP: واپس cmp >= 0;
          حالت GT_BINOP: واپس cmp >  0;
          حالت LE_BINOP: واپس cmp <= 0;
          حالت LT_BINOP: واپس cmp <  0;
          حالت NE_BINOP: واپس cmp != 0;
          }
        unreachable ();
      }

    حالت NT_BINOP: حالت OT_BINOP:
      {
        اگر (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        صحیح cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        واپس bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    حالت EF_BINOP:
      اگر (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      ورنہ
        {
          ساخت stat st[2];
          واپس (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    حالت EQ_STRING_BINOP:
    حالت NE_STRING_BINOP:
      واپس streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    حالت GT_STRING_BINOP:
    حالت LT_STRING_BINOP:
      {
        صحیح cmp = strcoll (argv[op - 1], argv[op + 1]);
        واپس bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

ساکن bool
unary_operator (خالی)
{
  ساخت stat stat_buf;

  انتخاب (argv[pos][1])
    {
    طےشدہ:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    حالت 'e':			/* file exists in the file system? */
      unary_advance ();
      واپس stat (argv[pos - 1], &stat_buf) == 0;

    حالت 'r':			/* file is readable? */
      unary_advance ();
      واپس euidaccess (argv[pos - 1], R_OK) == 0;

    حالت 'w':			/* File is writable? */
      unary_advance ();
      واپس euidaccess (argv[pos - 1], W_OK) == 0;

    حالت 'x':			/* File is executable? */
      unary_advance ();
      واپس euidaccess (argv[pos - 1], X_OK) == 0;

    حالت 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        اگر (stat (argv[pos - 1], &stat_buf) != 0)
          واپس false;
        ساخت timespec atime = get_stat_atime (&stat_buf);
        ساخت timespec mtime = get_stat_mtime (&stat_buf);
        واپس (timespec_cmp (mtime, atime) > 0);
      }

    حالت 'O':			/* File is owned by you? */
      {
        unary_advance ();
        اگر (stat (argv[pos - 1], &stat_buf) != 0)
          واپس false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        واپس ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    حالت 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        اگر (stat (argv[pos - 1], &stat_buf) != 0)
          واپس false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        واپس ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    حالت 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_منوال));

    حالت 'd':			/* File is a directory? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_منوال));

    حالت 's':			/* File has something in it? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_سائز);

    حالت 'S':			/* File is a socket? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_منوال));

    حالت 'c':			/* File is character special? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_منوال));

    حالت 'b':			/* File is block special? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_منوال));

    حالت 'p':			/* File is a named pipe? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_منوال));

    حالت 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    حالت 'h':			/* File is a symbolic link? */
      unary_advance ();
      واپس issymlink (argv[pos - 1]) == 1;

    حالت 'u':			/* File is setuid? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_منوال & S_ISUID));

    حالت 'g':			/* File is setgid? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_منوال & S_ISGID));

    حالت 'k':			/* File has sticky bit set? */
      unary_advance ();
      واپس (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_منوال & S_ISVTX));

    حالت 't':			/* File (fd) is a terminal? */
      {
        طویل صحیح fd;
        حرف ثابت *arg;
        unary_advance ();
        arg = ڈھونڈو_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, تہی, 10);
        واپس (errno != ERANGE && 0 <= fd && fd <= زیادہ_سے_زیادہ_عدد && isatty (fd));
      }

    حالت 'n':			/* True if arg has some length. */
      unary_advance ();
      واپس argv[pos - 1][0] != 0;

    حالت 'z':			/* True if arg has no length. */
      unary_advance ();
      واپس argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
ساکن bool
and (خالی)
{
  bool قدر = true;

  جب (true)
    {
      قدر &= رکن ();
      اگر (! (pos < argc && streq (argv[pos], "-a")))
        واپس قدر;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
ساکن bool
or (خالی)
{
  bool قدر = false;

  جب (true)
    {
      قدر |= and ();
      اگر (! (pos < argc && streq (argv[pos], "-o")))
        واپس قدر;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
ساکن bool
اظہاریہ (خالی)
{
  اگر (pos >= argc)
    beyond ();

  واپس or ();		/* Same with this. */
}

ساکن bool
one_دلیل (خالی)
{
  واپس argv[pos++][0] != '\0';
}

ساکن bool
two_arguments (خالی)
{
  bool قدر;

  اگر (streq (argv[pos], "!"))
    {
      advance (false);
      قدر = ! one_دلیل ();
    }
  ورنہ اگر (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      قدر = unary_operator ();
    }
  ورنہ
    beyond ();
  واپس (قدر);
}

ساکن bool
three_arguments (خالی)
{
  bool قدر;
  صحیح bop = binop (argv[pos + 1]);

  اگر (0 <= bop)
    قدر = ثنائی_operator (false, bop);
  ورنہ اگر (streq (argv[pos], "!"))
    {
      advance (true);
      قدر = !two_arguments ();
    }
  ورنہ اگر (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      قدر = one_دلیل ();
      advance (false);
    }
  ورنہ اگر (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    قدر = اظہاریہ ();
  ورنہ
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  واپس (قدر);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
ساکن bool
posixtest (صحیح nargs)
{
  bool قدر;

  انتخاب (nargs)
    {
      حالت 1:
        قدر = one_دلیل ();
        توڑو;

      حالت 2:
        قدر = two_arguments ();
        توڑو;

      حالت 3:
        قدر = three_arguments ();
        توڑو;

      حالت 4:
        اگر (streq (argv[pos], "!"))
          {
            advance (true);
            قدر = !three_arguments ();
            توڑو;
          }
        اگر (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            قدر = two_arguments ();
            advance (false);
            توڑو;
          }
        FALLTHROUGH;
      حالت 5:
      طےشدہ:
        affirm (0 < nargs);
        قدر = اظہاریہ ();
    }

  واپس (قدر);
}

#اگر defined TEST_STANDALONE

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      fputs (_("\
Usage: test عبارت\n\
  or:  test\n\
  or:  [ عبارت ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), stdout);
      fputs (_("\
Exit with the حالت_2 determined by عبارت.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
An omitted عبارت defaults مقصد false.  Otherwise,\n\
عبارت is true or false and sets باہر_نکلو حالت_2.  It is one کا:\n\
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
  -n STRING            the لمبائی کا STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent مقصد -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the لمبائی کا STRING is zero\n\
"));
      fputs (_("\
  STRING1 = STRING2    the لڑیاں are equal\n\
  STRING1 != STRING2   the لڑیاں are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 اندر the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 اندر the current locale\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  INTEGER1 -eq INTEGER2   INTEGER1 is equal مقصد INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal مقصد INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -le INTEGER2   INTEGER1 is less than or equal مقصد INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ne INTEGER2   INTEGER1 is not equal مقصد INTEGER2\n\
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
  -s FILE     FILE exists and has a سائز greater than zero\n\
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
Except برائے -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need مقصد be escaped (e.g., by backslashes) برائے shells.\n\
INTEGER may also be -l STRING, which evaluates مقصد the لمبائی کا STRING.\n\
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
  باہر_نکلو (حالت_2);
}
#اگر_ختم /* TEST_STANDALONE */

#اگر !defined TEST_STANDALONE
# define مین test_command
#اگر_ختم

#تعریف_کرو AUTHORS \
  proper_name ("Kevin Braunsdorf"), \
  proper_name ("Matthew Bradburn")

/*
 * [:
 *	'[' expr ']'
 * test:
 *	test expr
 */
صحیح
مین (صحیح margc, حرف **margv)
{
  bool قدر;

#اگر !defined TEST_STANDALONE
  صحیح رمز;

  رمز = setjmp (test_exit_buf);

  اگر (رمز)
    واپس (test_error_return);
#ورنہ /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  سیٹ_پروگرام_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#اگر_ختم /* TEST_STANDALONE */

  argv = margv;

  اگر (test_منوال == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      اگر (margc == 2)
        {
          اگر (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          اگر (streq (margv[1], "--version"))
            {
              نسخہ_etc (stdout, پروگرام_NAME, PACKAGE_NAME, نسخہ, AUTHORS,
                           (حرف *) تہی);
              test_main_return (EXIT_SUCCESS);
            }
        }
      اگر (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  اگر (pos >= argc)
    test_main_return (TEST_FALSE);

  قدر = posixtest (argc - 1);

  اگر (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (قدر ? TEST_TRUE : TEST_FALSE);
}
