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

#यदि HAVE_SYS_PARAM_H
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

#यदि defined TEST_STANDALONE
# define test_exit(val) निकास (val)
# define test_main_return(val) फर्काऊ val
#अन्यथा
   स्थिर लाँघ_बफ test_exit_buf;
   स्थिर पूर्णाङ्क test_error_return = 0;
# define test_exit(val) test_error_return = val, दीर्घ_लाँघ (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

स्थिर पूर्णाङ्क pos;		/* The offset of the current argument in ARGV. */
स्थिर पूर्णाङ्क argc;	/* The number of arguments present in ARGV. */
स्थिर वर्ण **argv;	/* The argument list. */

स्थिर bool unary_operator (शून्य);
स्थिर bool द्विआधारी_operator (bool, गणना binop);
स्थिर bool two_arguments (शून्य);
स्थिर bool three_arguments (शून्य);
स्थिर bool posixtest (पूर्णाङ्क);

स्थिर bool अभिव्यक्ति (शून्य);
स्थिर bool term (शून्य);
स्थिर bool and (शून्य);
स्थिर bool or (शून्य);

स्थिर शून्य beyond (शून्य);

ATTRIBUTE_FORMAT ((म_लिखो, 1, 2))
स्थिर _Noreturn शून्य
test_syntax_error (वर्ण अचर *format, ...)
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

  यदि (f && pos >= argc)
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
स्थिर वर्ण अचर *
find_int (वर्ण अचर *string)
{
  वर्ण अचर *p;
  वर्ण अचर *सङ्ख्या_start;

  लागि (p = string; है_खाली (to_uchar (*p)); p++)
    जारी;

  यदि (*p == '+')
    {
      p++;
      सङ्ख्या_start = p;
    }
  अन्यथा
    {
      सङ्ख्या_start = p;
      p += (*p == '-');
    }

  यदि (c_isdigit (*p++))
    {
      जबसम्म (c_isdigit (*p))
        p++;
      जबसम्म (है_खाली (to_uchar (*p)))
        p++;
      यदि (!*p)
        फर्काऊ सङ्ख्या_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
स्थिर संरचना timespec
get_mtime (वर्ण अचर *filename)
{
  संरचना stat finfo;
  फर्काऊ (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (समय_प्रकार), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
स्थिर पूर्णाङ्क
binop (वर्ण अचर *s)
{
  फर्काऊ (  streq (s, "="  ) ? EQ_STRING_BINOP
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
  bool मान;
  bool negated = false;
  पूर्णाङ्क bop;

  /* Deal with leading 'not's.  */
  जबसम्म (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  यदि (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  यदि (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      पूर्णाङ्क nargs;

      advance (true);

      लागि (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        यदि (nargs == 4)
          {
            nargs = argc - pos;
            रोक;
          }

      मान = posixtest (nargs);
      यदि (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      अन्यथा
        यदि (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  अन्यथा यदि (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    मान = द्विआधारी_operator (true, bop);
  अन्यथा यदि (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    मान = द्विआधारी_operator (false, bop);

  /* It might be a switch type argument.  */
  अन्यथा यदि (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    मान = unary_operator ();
  अन्यथा
    {
      मान = (argv[pos][0] != '\0');
      advance (false);
    }

  फर्काऊ negated ^ मान;
}

स्थिर bool
द्विआधारी_operator (bool l_is_l, गणना binop bop)
{
  पूर्णाङ्क op;

  यदि (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  यदि (r_is_l)
    advance (false);

  pos += 3;

  विकल्प (bop)
    {
    पक्ष EQ_BINOP: पक्ष GE_BINOP: पक्ष GT_BINOP:
    पक्ष LE_BINOP: पक्ष LT_BINOP: पक्ष NE_BINOP:
      {
        वर्ण lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        वर्ण rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        वर्ण अचर *l = (l_is_l
                         ? umaxtostr (म_माप (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        वर्ण अचर *r = (r_is_l
                         ? umaxtostr (म_माप (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        पूर्णाङ्क cmp = strintcmp (l, r);
        विकल्प (+bop)
          {
          पक्ष EQ_BINOP: फर्काऊ cmp == 0;
          पक्ष GE_BINOP: फर्काऊ cmp >= 0;
          पक्ष GT_BINOP: फर्काऊ cmp >  0;
          पक्ष LE_BINOP: फर्काऊ cmp <= 0;
          पक्ष LT_BINOP: फर्काऊ cmp <  0;
          पक्ष NE_BINOP: फर्काऊ cmp != 0;
          }
        unreachable ();
      }

    पक्ष NT_BINOP: पक्ष OT_BINOP:
      {
        यदि (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        पूर्णाङ्क cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        फर्काऊ bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    पक्ष EF_BINOP:
      यदि (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      अन्यथा
        {
          संरचना stat st[2];
          फर्काऊ (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    पक्ष EQ_STRING_BINOP:
    पक्ष NE_STRING_BINOP:
      फर्काऊ streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    पक्ष GT_STRING_BINOP:
    पक्ष LT_STRING_BINOP:
      {
        पूर्णाङ्क cmp = strcoll (argv[op - 1], argv[op + 1]);
        फर्काऊ bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
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
      फर्काऊ stat (argv[pos - 1], &stat_buf) == 0;

    पक्ष 'r':			/* file is readable? */
      unary_advance ();
      फर्काऊ euidaccess (argv[pos - 1], R_OK) == 0;

    पक्ष 'w':			/* File is writable? */
      unary_advance ();
      फर्काऊ euidaccess (argv[pos - 1], W_OK) == 0;

    पक्ष 'x':			/* File is executable? */
      unary_advance ();
      फर्काऊ euidaccess (argv[pos - 1], X_OK) == 0;

    पक्ष 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        यदि (stat (argv[pos - 1], &stat_buf) != 0)
          फर्काऊ false;
        संरचना timespec atime = get_stat_atime (&stat_buf);
        संरचना timespec mtime = get_stat_mtime (&stat_buf);
        फर्काऊ (timespec_cmp (mtime, atime) > 0);
      }

    पक्ष 'O':			/* File is owned by you? */
      {
        unary_advance ();
        यदि (stat (argv[pos - 1], &stat_buf) != 0)
          फर्काऊ false;
        त्रुटि_सं = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        फर्काऊ ! (euid == NO_UID && त्रुटि_सं) && euid == stat_buf.st_uid;
      }

    पक्ष 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        यदि (stat (argv[pos - 1], &stat_buf) != 0)
          फर्काऊ false;
        त्रुटि_सं = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        फर्काऊ ! (egid == NO_GID && त्रुटि_सं) && egid == stat_buf.st_gid;
      }

    पक्ष 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    पक्ष 'd':			/* File is a directory? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    पक्ष 's':			/* File has something in it? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    पक्ष 'S':			/* File is a socket? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    पक्ष 'c':			/* File is character special? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    पक्ष 'b':			/* File is block special? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    पक्ष 'p':			/* File is a named pipe? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    पक्ष 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    पक्ष 'h':			/* File is a symbolic link? */
      unary_advance ();
      फर्काऊ issymlink (argv[pos - 1]) == 1;

    पक्ष 'u':			/* File is setuid? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    पक्ष 'g':			/* File is setgid? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    पक्ष 'k':			/* File has sticky bit set? */
      unary_advance ();
      फर्काऊ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    पक्ष 't':			/* File (fd) is a terminal? */
      {
        दीर्घ पूर्णाङ्क fd;
        वर्ण अचर *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        त्रुटि_सं = 0;
        fd = म_से_दीर्घ (arg, रिक्त, 10);
        फर्काऊ (त्रुटि_सं != दुस्फल && 0 <= fd && fd <= पूर्णांक_उच्च && isatty (fd));
      }

    पक्ष 'n':			/* True if arg has some length. */
      unary_advance ();
      फर्काऊ argv[pos - 1][0] != 0;

    पक्ष 'z':			/* True if arg has no length. */
      unary_advance ();
      फर्काऊ argv[pos - 1][0] == '\0';
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
  bool मान = true;

  जबसम्म (true)
    {
      मान &= term ();
      यदि (! (pos < argc && streq (argv[pos], "-a")))
        फर्काऊ मान;
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
  bool मान = false;

  जबसम्म (true)
    {
      मान |= and ();
      यदि (! (pos < argc && streq (argv[pos], "-o")))
        फर्काऊ मान;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
स्थिर bool
अभिव्यक्ति (शून्य)
{
  यदि (pos >= argc)
    beyond ();

  फर्काऊ or ();		/* Same with this. */
}

स्थिर bool
one_argument (शून्य)
{
  फर्काऊ argv[pos++][0] != '\0';
}

स्थिर bool
two_arguments (शून्य)
{
  bool मान;

  यदि (streq (argv[pos], "!"))
    {
      advance (false);
      मान = ! one_argument ();
    }
  अन्यथा यदि (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      मान = unary_operator ();
    }
  अन्यथा
    beyond ();
  फर्काऊ (मान);
}

स्थिर bool
three_arguments (शून्य)
{
  bool मान;
  पूर्णाङ्क bop = binop (argv[pos + 1]);

  यदि (0 <= bop)
    मान = द्विआधारी_operator (false, bop);
  अन्यथा यदि (streq (argv[pos], "!"))
    {
      advance (true);
      मान = !two_arguments ();
    }
  अन्यथा यदि (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      मान = one_argument ();
      advance (false);
    }
  अन्यथा यदि (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    मान = अभिव्यक्ति ();
  अन्यथा
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  फर्काऊ (मान);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
स्थिर bool
posixtest (पूर्णाङ्क nargs)
{
  bool मान;

  विकल्प (nargs)
    {
      पक्ष 1:
        मान = one_argument ();
        रोक;

      पक्ष 2:
        मान = two_arguments ();
        रोक;

      पक्ष 3:
        मान = three_arguments ();
        रोक;

      पक्ष 4:
        यदि (streq (argv[pos], "!"))
          {
            advance (true);
            मान = !three_arguments ();
            रोक;
          }
        यदि (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            मान = two_arguments ();
            advance (false);
            रोक;
          }
        FALLTHROUGH;
      पक्ष 5:
      स्वभाव:
        affirm (0 < nargs);
        मान = अभिव्यक्ति ();
    }

  फर्काऊ (मान);
}

#यदि defined TEST_STANDALONE

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
  -x खाता     खाता exists and the user has execute (or खोज) access\n\
"));
      ख_माला_दो (_("\
\n\
Except लागि -h and -L, all खाता-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) लागि shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
द्विआधारी_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#यदि !defined TEST_STANDALONE
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
  bool मान;

#यदि !defined TEST_STANDALONE
  पूर्णाङ्क code;

  code = बनाओ_लाँघ (test_exit_buf);

  यदि (code)
    फर्काऊ (test_error_return);
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

  यदि (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      यदि (margc == 2)
        {
          यदि (streq (margv[1], "--help"))
            usage (निकास_सफल);

          यदि (streq (margv[1], "--version"))
            {
              version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (वर्ण *) रिक्त);
              test_main_return (निकास_सफल);
            }
        }
      यदि (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  यदि (pos >= argc)
    test_main_return (TEST_FALSE);

  मान = posixtest (argc - 1);

  यदि (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (मान ? TEST_TRUE : TEST_FALSE);
}
