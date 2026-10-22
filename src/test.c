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

#जर HAVE_SYS_PARAM_H
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

#जर defined TEST_STANDALONE
# define test_exit(val) निकास (val)
# define test_main_return(val) परत val
#अन्यथा
   स्थिर लाँघ_बफ test_exit_buf;
   स्थिर पूर्णांक test_error_return = 0;
# define test_exit(val) test_error_return = val, दीर्घ_लाँघ (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

स्थिर पूर्णांक pos;		/* The offset of the current argument in ARGV. */
स्थिर पूर्णांक argc;	/* The number of arguments present in ARGV. */
स्थिर अक्षर **argv;	/* The argument list. */

स्थिर bool unary_operator (शून्य);
स्थिर bool द्विमान_operator (bool, गणना binop);
स्थिर bool two_arguments (शून्य);
स्थिर bool three_arguments (शून्य);
स्थिर bool posixtest (पूर्णांक);

स्थिर bool अभिव्यक्ती (शून्य);
स्थिर bool term (शून्य);
स्थिर bool and (शून्य);
स्थिर bool or (शून्य);

स्थिर शून्य beyond (शून्य);

ATTRIBUTE_FORMAT ((म_लिखो, 1, 2))
स्थिर _Noreturn शून्य
test_syntax_error (अक्षर स्थिरांक *format, ...)
{
  बहु_सूची ap;
  बहु_शुरू (ap, format);
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

  जर (f && pos >= argc)
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
स्थिर अक्षर स्थिरांक *
find_int (अक्षर स्थिरांक *string)
{
  अक्षर स्थिरांक *p;
  अक्षर स्थिरांक *संख्या_start;

  साठी (p = string; है_खाली (to_uchar (*p)); p++)
    सुरूठेवा;

  जर (*p == '+')
    {
      p++;
      संख्या_start = p;
    }
  अन्यथा
    {
      संख्या_start = p;
      p += (*p == '-');
    }

  जर (c_isdigit (*p++))
    {
      पर्यंत (c_isdigit (*p))
        p++;
      पर्यंत (है_खाली (to_uchar (*p)))
        p++;
      जर (!*p)
        परत संख्या_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
स्थिर संरचना timespec
get_mtime (अक्षर स्थिरांक *filename)
{
  संरचना stat finfo;
  परत (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (समय_प्रकार), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
स्थिर पूर्णांक
binop (अक्षर स्थिरांक *s)
{
  परत (  streq (s, "="  ) ? EQ_STRING_BINOP
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
  bool मूल्य;
  bool negated = false;
  पूर्णांक bop;

  /* Deal with leading 'not's.  */
  पर्यंत (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  जर (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  जर (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      पूर्णांक nargs;

      advance (true);

      साठी (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        जर (nargs == 4)
          {
            nargs = argc - pos;
            थांबा;
          }

      मूल्य = posixtest (nargs);
      जर (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      अन्यथा
        जर (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  अन्यथा जर (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    मूल्य = द्विमान_operator (true, bop);
  अन्यथा जर (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    मूल्य = द्विमान_operator (false, bop);

  /* It might be a switch type argument.  */
  अन्यथा जर (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    मूल्य = unary_operator ();
  अन्यथा
    {
      मूल्य = (argv[pos][0] != '\0');
      advance (false);
    }

  परत negated ^ मूल्य;
}

स्थिर bool
द्विमान_operator (bool l_is_l, गणना binop bop)
{
  पूर्णांक op;

  जर (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  जर (r_is_l)
    advance (false);

  pos += 3;

  निवड (bop)
    {
    स्थिती EQ_BINOP: स्थिती GE_BINOP: स्थिती GT_BINOP:
    स्थिती LE_BINOP: स्थिती LT_BINOP: स्थिती NE_BINOP:
      {
        अक्षर lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        अक्षर rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        अक्षर स्थिरांक *l = (l_is_l
                         ? umaxtostr (म_माप (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        अक्षर स्थिरांक *r = (r_is_l
                         ? umaxtostr (म_माप (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        पूर्णांक cmp = strintcmp (l, r);
        निवड (+bop)
          {
          स्थिती EQ_BINOP: परत cmp == 0;
          स्थिती GE_BINOP: परत cmp >= 0;
          स्थिती GT_BINOP: परत cmp >  0;
          स्थिती LE_BINOP: परत cmp <= 0;
          स्थिती LT_BINOP: परत cmp <  0;
          स्थिती NE_BINOP: परत cmp != 0;
          }
        unreachable ();
      }

    स्थिती NT_BINOP: स्थिती OT_BINOP:
      {
        जर (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        पूर्णांक cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        परत bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    स्थिती EF_BINOP:
      जर (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      अन्यथा
        {
          संरचना stat st[2];
          परत (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    स्थिती EQ_STRING_BINOP:
    स्थिती NE_STRING_BINOP:
      परत streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    स्थिती GT_STRING_BINOP:
    स्थिती LT_STRING_BINOP:
      {
        पूर्णांक cmp = strcoll (argv[op - 1], argv[op + 1]);
        परत bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

स्थिर bool
unary_operator (शून्य)
{
  संरचना stat stat_buf;

  निवड (argv[pos][1])
    {
    पूर्वनिर्धारित:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    स्थिती 'e':			/* file exists in the file system? */
      unary_advance ();
      परत stat (argv[pos - 1], &stat_buf) == 0;

    स्थिती 'r':			/* file is readable? */
      unary_advance ();
      परत euidaccess (argv[pos - 1], R_OK) == 0;

    स्थिती 'w':			/* File is writable? */
      unary_advance ();
      परत euidaccess (argv[pos - 1], W_OK) == 0;

    स्थिती 'x':			/* File is executable? */
      unary_advance ();
      परत euidaccess (argv[pos - 1], X_OK) == 0;

    स्थिती 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        जर (stat (argv[pos - 1], &stat_buf) != 0)
          परत false;
        संरचना timespec atime = get_stat_atime (&stat_buf);
        संरचना timespec mtime = get_stat_mtime (&stat_buf);
        परत (timespec_cmp (mtime, atime) > 0);
      }

    स्थिती 'O':			/* File is owned by you? */
      {
        unary_advance ();
        जर (stat (argv[pos - 1], &stat_buf) != 0)
          परत false;
        त्रुटि_सं = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        परत ! (euid == NO_UID && त्रुटि_सं) && euid == stat_buf.st_uid;
      }

    स्थिती 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        जर (stat (argv[pos - 1], &stat_buf) != 0)
          परत false;
        त्रुटि_सं = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        परत ! (egid == NO_GID && त्रुटि_सं) && egid == stat_buf.st_gid;
      }

    स्थिती 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    स्थिती 'd':			/* File is a directory? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    स्थिती 's':			/* File has something in it? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    स्थिती 'S':			/* File is a socket? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    स्थिती 'c':			/* File is character special? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    स्थिती 'b':			/* File is block special? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    स्थिती 'p':			/* File is a named pipe? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    स्थिती 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    स्थिती 'h':			/* File is a symbolic link? */
      unary_advance ();
      परत issymlink (argv[pos - 1]) == 1;

    स्थिती 'u':			/* File is setuid? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    स्थिती 'g':			/* File is setgid? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    स्थिती 'k':			/* File has sticky bit set? */
      unary_advance ();
      परत (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    स्थिती 't':			/* File (fd) is a terminal? */
      {
        दीर्घ पूर्णांक fd;
        अक्षर स्थिरांक *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        त्रुटि_सं = 0;
        fd = म_से_दीर्घ (arg, रिक्त, 10);
        परत (त्रुटि_सं != दुस्फल && 0 <= fd && fd <= पूर्णांक_उच्च && isatty (fd));
      }

    स्थिती 'n':			/* True if arg has some length. */
      unary_advance ();
      परत argv[pos - 1][0] != 0;

    स्थिती 'z':			/* True if arg has no length. */
      unary_advance ();
      परत argv[pos - 1][0] == '\0';
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
  bool मूल्य = true;

  पर्यंत (true)
    {
      मूल्य &= term ();
      जर (! (pos < argc && streq (argv[pos], "-a")))
        परत मूल्य;
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
  bool मूल्य = false;

  पर्यंत (true)
    {
      मूल्य |= and ();
      जर (! (pos < argc && streq (argv[pos], "-o")))
        परत मूल्य;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
स्थिर bool
अभिव्यक्ती (शून्य)
{
  जर (pos >= argc)
    beyond ();

  परत or ();		/* Same with this. */
}

स्थिर bool
one_argument (शून्य)
{
  परत argv[pos++][0] != '\0';
}

स्थिर bool
two_arguments (शून्य)
{
  bool मूल्य;

  जर (streq (argv[pos], "!"))
    {
      advance (false);
      मूल्य = ! one_argument ();
    }
  अन्यथा जर (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      मूल्य = unary_operator ();
    }
  अन्यथा
    beyond ();
  परत (मूल्य);
}

स्थिर bool
three_arguments (शून्य)
{
  bool मूल्य;
  पूर्णांक bop = binop (argv[pos + 1]);

  जर (0 <= bop)
    मूल्य = द्विमान_operator (false, bop);
  अन्यथा जर (streq (argv[pos], "!"))
    {
      advance (true);
      मूल्य = !two_arguments ();
    }
  अन्यथा जर (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      मूल्य = one_argument ();
      advance (false);
    }
  अन्यथा जर (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    मूल्य = अभिव्यक्ती ();
  अन्यथा
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  परत (मूल्य);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
स्थिर bool
posixtest (पूर्णांक nargs)
{
  bool मूल्य;

  निवड (nargs)
    {
      स्थिती 1:
        मूल्य = one_argument ();
        थांबा;

      स्थिती 2:
        मूल्य = two_arguments ();
        थांबा;

      स्थिती 3:
        मूल्य = three_arguments ();
        थांबा;

      स्थिती 4:
        जर (streq (argv[pos], "!"))
          {
            advance (true);
            मूल्य = !three_arguments ();
            थांबा;
          }
        जर (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            मूल्य = two_arguments ();
            advance (false);
            थांबा;
          }
        FALLTHROUGH;
      स्थिती 5:
      पूर्वनिर्धारित:
        affirm (0 < nargs);
        मूल्य = अभिव्यक्ती ();
    }

  परत (मूल्य);
}

#जर defined TEST_STANDALONE

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      ख_माला_दो (_("\
Usage: test EXPRESSION\n\
  or:  test\n\
  or:  [ EXPRESSION ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), मानक_निकास);
      ख_माला_दो (_("\
Exit with the status determined by EXPRESSION.\n\
\n\
"), मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
An omitted EXPRESSION defaults to false.  Otherwise,\n\
EXPRESSION is true or false and sets निकास status.  It is one of:\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
  ( EXPRESSION )               EXPRESSION is true\n\
  ! EXPRESSION                 EXPRESSION is false\n\
  EXPRESSION1 -a EXPRESSION2   both EXPRESSION1 and EXPRESSION2 are true\n\
  EXPRESSION1 -o EXPRESSION2   either EXPRESSION1 or EXPRESSION2 is true\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (_("\
  -n STRING            the length of STRING is nonzero\n\
"));
      ख_माला_दो (_("\
  STRING               equivalent to -n STRING\n\
"), मानक_निकास);
      oputs (_("\
  -z STRING            the length of STRING is zero\n\
"));
      ख_माला_दो (_("\
  STRING1 = STRING2    the strings are equal\n\
  STRING1 != STRING2   the strings are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 in the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 in the current locale\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
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
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (_("\
  FILE1 -ef FILE2   FILE1 and FILE2 have the same device and inode numbers\n\
"));
      oputs (_("\
  FILE1 -nt FILE2   FILE1 is newer (modification date) than FILE2\n\
"));
      oputs (_("\
  FILE1 -ot FILE2   FILE1 is older than FILE2\n\
"));
      ख_माला_दो (_("\
\n\
"), मानक_निकास);
      oputs (_("\
  -b खाता     खाता exists and is block special\n\
"));
      oputs (_("\
  -c खाता     खाता exists and is character special\n\
"));
      oputs (_("\
  -d खाता     खाता exists and is a directory\n\
"));
      oputs (_("\
  -e खाता     खाता exists\n\
"));
      oputs (_("\
  -f खाता     खाता exists and is a regular file\n\
"));
      oputs (_("\
  -g खाता     खाता exists and its set-group-ID bit is set\n\
"));
      oputs (_("\
  -G खाता     खाता exists and is owned by the effective group ID\n\
"));
      oputs (_("\
  -h खाता     खाता exists and is a symbolic link (same as -L)\n\
"));
      oputs (_("\
  -k खाता     खाता exists and has its sticky bit set\n\
"));
      oputs (_("\
  -L खाता     खाता exists and is a symbolic link (same as -h)\n\
"));
      oputs (_("\
  -N खाता     खाता exists and has been modified since it was last पठ\n\
"));
      oputs (_("\
  -O खाता     खाता exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p खाता     खाता exists and is a named pipe\n\
"));
      oputs (_("\
  -r खाता     खाता exists and the user has पठ access\n\
"));
      oputs (_("\
  -s खाता     खाता exists and has a size greater than zero\n\
"));
      oputs (_("\
  -S खाता     खाता exists and is a socket\n\
"));
      oputs (_("\
  -t FD       file descriptor FD is opened on a terminal\n\
"));
      oputs (_("\
  -u खाता     खाता exists and its set-user-ID bit is set\n\
"));
      oputs (_("\
  -w खाता     खाता exists and the user has लिख access\n\
"));
      oputs (_("\
  -x खाता     खाता exists and the user has execute (or शोध) access\n\
"));
      ख_माला_दो (_("\
\n\
Except साठी -h and -L, all खाता-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) साठी shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
द्विमान_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
or 'test EXPR1 || test EXPR2' instead.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
'[' honors --help and --version, but 'test' treats them as STRINGs.\n\
"), मानक_निकास);
      म_लिखो (USAGE_BUILTIN_WARNING, _("test and/or ["));
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}
#endif /* TEST_STANDALONE */

#जर !defined TEST_STANDALONE
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
पूर्णांक
main (पूर्णांक margc, अक्षर **margv)
{
  bool मूल्य;

#जर !defined TEST_STANDALONE
  पूर्णांक code;

  code = बनाओ_लाँघ (test_exit_buf);

  जर (code)
    परत (test_error_return);
#अन्यथा /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  निकास_पर (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  जर (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      जर (margc == 2)
        {
          जर (streq (margv[1], "--help"))
            usage (निकास_सफल);

          जर (streq (margv[1], "--version"))
            {
              version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (अक्षर *) रिक्त);
              test_main_return (निकास_सफल);
            }
        }
      जर (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  जर (pos >= argc)
    test_main_return (TEST_FALSE);

  मूल्य = posixtest (argc - 1);

  जर (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (मूल्य ? TEST_TRUE : TEST_FALSE);
}
