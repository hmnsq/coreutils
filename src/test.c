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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "issymlink.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "strnumcmp.h"
#अन्तर्भाव "test.h"

#अन्तर्भाव <stdarg.h>

#सचे HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
गणना { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
गणना binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#सचे defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) पटिनिवत्त val
#अञ्ञथा
   स्थिर jmp_buf test_exit_buf;
   स्थिर पूर्णाङ्क test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

स्थिर पूर्णाङ्क pos;		/* The offset of the current argument in ARGV. */
स्थिर पूर्णाङ्क argc;	/* The number of arguments present in ARGV. */
स्थिर वर्ण **argv;	/* The argument list. */

स्थिर bool unary_operator (शून्य);
स्थिर bool द्विमान_operator (bool, गणना binop);
स्थिर bool two_arguments (शून्य);
स्थिर bool three_arguments (शून्य);
स्थिर bool posixtest (पूर्णाङ्क);

स्थिर bool व्यञ्जकम् (शून्य);
स्थिर bool term (शून्य);
स्थिर bool and (शून्य);
स्थिर bool or (शून्य);

स्थिर शून्य beyond (शून्य);

ATTRIBUTE_FORMAT ((printf, 1, 2))
स्थिर _Noreturn शून्य
test_syntax_error (वर्ण ध्रुव *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

स्थिर शून्य
advance (bool f)
{
  ++pos;

  सचे (f && pos >= argc)
    beyond ();
}

स्थिर शून्य
unary_advance (शून्य)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
स्थिर _Noreturn शून्य
beyond (शून्य)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
स्थिर वर्ण ध्रुव *
find_int (वर्ण ध्रुव *string)
{
  वर्ण ध्रुव *p;
  वर्ण ध्रुव *सङ्ख्या_start;

  अत्थाय (p = string; isspace (to_uchar (*p)); p++)
    अनुवर्त;

  सचे (*p == '+')
    {
      p++;
      सङ्ख्या_start = p;
    }
  अञ्ञथा
    {
      सङ्ख्या_start = p;
      p += (*p == '-');
    }

  सचे (c_isdigit (*p++))
    {
      याव (c_isdigit (*p))
        p++;
      याव (isspace (to_uchar (*p)))
        p++;
      सचे (!*p)
        पटिनिवत्त सङ्ख्या_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
स्थिर संरचना timespec
get_mtime (वर्ण ध्रुव *filename)
{
  संरचना stat finfo;
  पटिनिवत्त (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
स्थिर पूर्णाङ्क
binop (वर्ण ध्रुव *s)
{
  पटिनिवत्त (  streq (s, "="  ) ? EQ_STRING_BINOP
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
स्थिर bool
term (शून्य)
{
  bool मानम्;
  bool negated = false;
  पूर्णाङ्क bop;

  /* Deal with leading 'not's.  */
  याव (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  सचे (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  सचे (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      पूर्णाङ्क nargs;

      advance (true);

      अत्थाय (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        सचे (nargs == 4)
          {
            nargs = argc - pos;
            भङ्ग;
          }

      मानम् = posixtest (nargs);
      सचे (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      अञ्ञथा
        सचे (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  अञ्ञथा सचे (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    मानम् = द्विमान_operator (true, bop);
  अञ्ञथा सचे (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    मानम् = द्विमान_operator (false, bop);

  /* It might be a switch type argument.  */
  अञ्ञथा सचे (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    मानम् = unary_operator ();
  अञ्ञथा
    {
      मानम् = (argv[pos][0] != '\0');
      advance (false);
    }

  पटिनिवत्त negated ^ मानम्;
}

स्थिर bool
द्विमान_operator (bool l_is_l, गणना binop bop)
{
  पूर्णाङ्क op;

  सचे (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  सचे (r_is_l)
    advance (false);

  pos += 3;

  विकल्प (bop)
    {
    पक्ष EQ_BINOP: पक्ष GE_BINOP: पक्ष GT_BINOP:
    पक्ष LE_BINOP: पक्ष LT_BINOP: पक्ष NE_BINOP:
      {
        वर्ण lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        वर्ण rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        वर्ण ध्रुव *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        वर्ण ध्रुव *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        पूर्णाङ्क cmp = strintcmp (l, r);
        विकल्प (+bop)
          {
          पक्ष EQ_BINOP: पटिनिवत्त cmp == 0;
          पक्ष GE_BINOP: पटिनिवत्त cmp >= 0;
          पक्ष GT_BINOP: पटिनिवत्त cmp >  0;
          पक्ष LE_BINOP: पटिनिवत्त cmp <= 0;
          पक्ष LT_BINOP: पटिनिवत्त cmp <  0;
          पक्ष NE_BINOP: पटिनिवत्त cmp != 0;
          }
        unreachable ();
      }

    पक्ष NT_BINOP: पक्ष OT_BINOP:
      {
        सचे (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        पूर्णाङ्क cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        पटिनिवत्त bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    पक्ष EF_BINOP:
      सचे (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      अञ्ञथा
        {
          संरचना stat st[2];
          पटिनिवत्त (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    पक्ष EQ_STRING_BINOP:
    पक्ष NE_STRING_BINOP:
      पटिनिवत्त streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    पक्ष GT_STRING_BINOP:
    पक्ष LT_STRING_BINOP:
      {
        पूर्णाङ्क cmp = strcoll (argv[op - 1], argv[op + 1]);
        पटिनिवत्त bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

स्थिर bool
unary_operator (शून्य)
{
  संरचना stat stat_buf;

  विकल्प (argv[pos][1])
    {
    स्वभाव:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    पक्ष 'e':			/* file exists in the file system? */
      unary_advance ();
      पटिनिवत्त stat (argv[pos - 1], &stat_buf) == 0;

    पक्ष 'r':			/* file is readable? */
      unary_advance ();
      पटिनिवत्त euidaccess (argv[pos - 1], R_OK) == 0;

    पक्ष 'w':			/* File is writable? */
      unary_advance ();
      पटिनिवत्त euidaccess (argv[pos - 1], W_OK) == 0;

    पक्ष 'x':			/* File is executable? */
      unary_advance ();
      पटिनिवत्त euidaccess (argv[pos - 1], X_OK) == 0;

    पक्ष 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        सचे (stat (argv[pos - 1], &stat_buf) != 0)
          पटिनिवत्त false;
        संरचना timespec atime = get_stat_atime (&stat_buf);
        संरचना timespec mtime = get_stat_mtime (&stat_buf);
        पटिनिवत्त (timespec_cmp (mtime, atime) > 0);
      }

    पक्ष 'O':			/* File is owned by you? */
      {
        unary_advance ();
        सचे (stat (argv[pos - 1], &stat_buf) != 0)
          पटिनिवत्त false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        पटिनिवत्त ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    पक्ष 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        सचे (stat (argv[pos - 1], &stat_buf) != 0)
          पटिनिवत्त false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        पटिनिवत्त ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    पक्ष 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    पक्ष 'd':			/* File is a directory? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    पक्ष 's':			/* File has something in it? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    पक्ष 'S':			/* File is a socket? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    पक्ष 'c':			/* File is character special? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    पक्ष 'b':			/* File is block special? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    पक्ष 'p':			/* File is a named pipe? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    पक्ष 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    पक्ष 'h':			/* File is a symbolic link? */
      unary_advance ();
      पटिनिवत्त issymlink (argv[pos - 1]) == 1;

    पक्ष 'u':			/* File is setuid? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    पक्ष 'g':			/* File is setgid? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    पक्ष 'k':			/* File has sticky bit set? */
      unary_advance ();
      पटिनिवत्त (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    पक्ष 't':			/* File (fd) is a terminal? */
      {
        long पूर्णाङ्क fd;
        वर्ण ध्रुव *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, रिक्त, 10);
        पटिनिवत्त (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    पक्ष 'n':			/* True if arg has some length. */
      unary_advance ();
      पटिनिवत्त argv[pos - 1][0] != 0;

    पक्ष 'z':			/* True if arg has no length. */
      unary_advance ();
      पटिनिवत्त argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
स्थिर bool
and (शून्य)
{
  bool मानम् = true;

  याव (true)
    {
      मानम् &= term ();
      सचे (! (pos < argc && streq (argv[pos], "-a")))
        पटिनिवत्त मानम्;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
स्थिर bool
or (शून्य)
{
  bool मानम् = false;

  याव (true)
    {
      मानम् |= and ();
      सचे (! (pos < argc && streq (argv[pos], "-o")))
        पटिनिवत्त मानम्;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
स्थिर bool
व्यञ्जकम् (शून्य)
{
  सचे (pos >= argc)
    beyond ();

  पटिनिवत्त or ();		/* Same with this. */
}

स्थिर bool
one_argument (शून्य)
{
  पटिनिवत्त argv[pos++][0] != '\0';
}

स्थिर bool
two_arguments (शून्य)
{
  bool मानम्;

  सचे (streq (argv[pos], "!"))
    {
      advance (false);
      मानम् = ! one_argument ();
    }
  अञ्ञथा सचे (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      मानम् = unary_operator ();
    }
  अञ्ञथा
    beyond ();
  पटिनिवत्त (मानम्);
}

स्थिर bool
three_arguments (शून्य)
{
  bool मानम्;
  पूर्णाङ्क bop = binop (argv[pos + 1]);

  सचे (0 <= bop)
    मानम् = द्विमान_operator (false, bop);
  अञ्ञथा सचे (streq (argv[pos], "!"))
    {
      advance (true);
      मानम् = !two_arguments ();
    }
  अञ्ञथा सचे (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      मानम् = one_argument ();
      advance (false);
    }
  अञ्ञथा सचे (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    मानम् = व्यञ्जकम् ();
  अञ्ञथा
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  पटिनिवत्त (मानम्);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
स्थिर bool
posixtest (पूर्णाङ्क nargs)
{
  bool मानम्;

  विकल्प (nargs)
    {
      पक्ष 1:
        मानम् = one_argument ();
        भङ्ग;

      पक्ष 2:
        मानम् = two_arguments ();
        भङ्ग;

      पक्ष 3:
        मानम् = three_arguments ();
        भङ्ग;

      पक्ष 4:
        सचे (streq (argv[pos], "!"))
          {
            advance (true);
            मानम् = !three_arguments ();
            भङ्ग;
          }
        सचे (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            मानम् = two_arguments ();
            advance (false);
            भङ्ग;
          }
        FALLTHROUGH;
      पक्ष 5:
      स्वभाव:
        affirm (0 < nargs);
        मानम् = व्यञ्जकम् ();
    }

  पटिनिवत्त (मानम्);
}

#सचे defined TEST_STANDALONE

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
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
  -N FILE     FILE exists and has been modified since it was last पठ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has पठ access\n\
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
  -w FILE     FILE exists and the user has लिख access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or अन्वेषणम्) access\n\
"));
      fputs (_("\
\n\
Except अत्थाय -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) अत्थाय shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
द्विमान_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#सचे !defined TEST_STANDALONE
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
पूर्णाङ्क
main (पूर्णाङ्क margc, वर्ण **margv)
{
  bool मानम्;

#सचे !defined TEST_STANDALONE
  पूर्णाङ्क code;

  code = setjmp (test_exit_buf);

  सचे (code)
    पटिनिवत्त (test_error_return);
#अञ्ञथा /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  सचे (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      सचे (margc == 2)
        {
          सचे (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          सचे (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (वर्ण *) रिक्त);
              test_main_return (EXIT_SUCCESS);
            }
        }
      सचे (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  सचे (pos >= argc)
    test_main_return (TEST_FALSE);

  मानम् = posixtest (argc - 1);

  सचे (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (मानम् ? TEST_TRUE : TEST_FALSE);
}
