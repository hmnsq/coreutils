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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "issymlink.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stat-time.h"
#અન્તર્ભાવ "strnumcmp.h"
#અન્તર્ભાવ "test.h"

#અન્તર્ભાવ <stdarg.h>

#જો HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
ગણના { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
ગણના binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#જો defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) પાછા val
#નહીંતર
   સ્થિર jmp_buf test_exit_buf;
   સ્થિર પૂર્ણાંક test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

સ્થિર પૂર્ણાંક pos;		/* The offset of the current argument in ARGV. */
સ્થિર પૂર્ણાંક argc;	/* The number of arguments present in ARGV. */
સ્થિર અક્ષર **argv;	/* The argument list. */

સ્થિર bool unary_operator (શૂન્ય);
સ્થિર bool દ્વિઅંકી_operator (bool, ગણના binop);
સ્થિર bool two_arguments (શૂન્ય);
સ્થિર bool three_arguments (શૂન્ય);
સ્થિર bool posixtest (પૂર્ણાંક);

સ્થિર bool પદાવલિ (શૂન્ય);
સ્થિર bool term (શૂન્ય);
સ્થિર bool and (શૂન્ય);
સ્થિર bool or (શૂન્ય);

સ્થિર શૂન્ય beyond (શૂન્ય);

ATTRIBUTE_FORMAT ((printf, 1, 2))
સ્થિર _Noreturn શૂન્ય
test_syntax_error (અક્ષર અચલ *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

સ્થિર શૂન્ય
advance (bool f)
{
  ++pos;

  જો (f && pos >= argc)
    beyond ();
}

સ્થિર શૂન્ય
unary_advance (શૂન્ય)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
સ્થિર _Noreturn શૂન્ય
beyond (શૂન્ય)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
સ્થિર અક્ષર અચલ *
find_int (અક્ષર અચલ *string)
{
  અક્ષર અચલ *p;
  અક્ષર અચલ *સંખ્યા_start;

  માટે (p = string; isspace (to_uchar (*p)); p++)
    ચાલુ;

  જો (*p == '+')
    {
      p++;
      સંખ્યા_start = p;
    }
  નહીંતર
    {
      સંખ્યા_start = p;
      p += (*p == '-');
    }

  જો (c_isdigit (*p++))
    {
      જ્યારે (c_isdigit (*p))
        p++;
      જ્યારે (isspace (to_uchar (*p)))
        p++;
      જો (!*p)
        પાછા સંખ્યા_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
સ્થિર સંરચના timespec
get_mtime (અક્ષર અચલ *filename)
{
  સંરચના stat finfo;
  પાછા (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
સ્થિર પૂર્ણાંક
binop (અક્ષર અચલ *s)
{
  પાછા (  streq (s, "="  ) ? EQ_STRING_BINOP
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
સ્થિર bool
term (શૂન્ય)
{
  bool કિંમત;
  bool negated = false;
  પૂર્ણાંક bop;

  /* Deal with leading 'not's.  */
  જ્યારે (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  જો (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  જો (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      પૂર્ણાંક nargs;

      advance (true);

      માટે (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        જો (nargs == 4)
          {
            nargs = argc - pos;
            તોડો;
          }

      કિંમત = posixtest (nargs);
      જો (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      નહીંતર
        જો (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  નહીંતર જો (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    કિંમત = દ્વિઅંકી_operator (true, bop);
  નહીંતર જો (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    કિંમત = દ્વિઅંકી_operator (false, bop);

  /* It might be a switch type argument.  */
  નહીંતર જો (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    કિંમત = unary_operator ();
  નહીંતર
    {
      કિંમત = (argv[pos][0] != '\0');
      advance (false);
    }

  પાછા negated ^ કિંમત;
}

સ્થિર bool
દ્વિઅંકી_operator (bool l_is_l, ગણના binop bop)
{
  પૂર્ણાંક op;

  જો (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  જો (r_is_l)
    advance (false);

  pos += 3;

  પસંદગી (bop)
    {
    સ્થિતિ EQ_BINOP: સ્થિતિ GE_BINOP: સ્થિતિ GT_BINOP:
    સ્થિતિ LE_BINOP: સ્થિતિ LT_BINOP: સ્થિતિ NE_BINOP:
      {
        અક્ષર lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        અક્ષર rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        અક્ષર અચલ *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        અક્ષર અચલ *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        પૂર્ણાંક cmp = strintcmp (l, r);
        પસંદગી (+bop)
          {
          સ્થિતિ EQ_BINOP: પાછા cmp == 0;
          સ્થિતિ GE_BINOP: પાછા cmp >= 0;
          સ્થિતિ GT_BINOP: પાછા cmp >  0;
          સ્થિતિ LE_BINOP: પાછા cmp <= 0;
          સ્થિતિ LT_BINOP: પાછા cmp <  0;
          સ્થિતિ NE_BINOP: પાછા cmp != 0;
          }
        unreachable ();
      }

    સ્થિતિ NT_BINOP: સ્થિતિ OT_BINOP:
      {
        જો (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        પૂર્ણાંક cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        પાછા bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    સ્થિતિ EF_BINOP:
      જો (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      નહીંતર
        {
          સંરચના stat st[2];
          પાછા (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    સ્થિતિ EQ_STRING_BINOP:
    સ્થિતિ NE_STRING_BINOP:
      પાછા streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    સ્થિતિ GT_STRING_BINOP:
    સ્થિતિ LT_STRING_BINOP:
      {
        પૂર્ણાંક cmp = strcoll (argv[op - 1], argv[op + 1]);
        પાછા bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

સ્થિર bool
unary_operator (શૂન્ય)
{
  સંરચના stat stat_buf;

  પસંદગી (argv[pos][1])
    {
    મૂળભૂત:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    સ્થિતિ 'e':			/* file exists in the file system? */
      unary_advance ();
      પાછા stat (argv[pos - 1], &stat_buf) == 0;

    સ્થિતિ 'r':			/* file is readable? */
      unary_advance ();
      પાછા euidaccess (argv[pos - 1], R_OK) == 0;

    સ્થિતિ 'w':			/* File is writable? */
      unary_advance ();
      પાછા euidaccess (argv[pos - 1], W_OK) == 0;

    સ્થિતિ 'x':			/* File is executable? */
      unary_advance ();
      પાછા euidaccess (argv[pos - 1], X_OK) == 0;

    સ્થિતિ 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        જો (stat (argv[pos - 1], &stat_buf) != 0)
          પાછા false;
        સંરચના timespec atime = get_stat_atime (&stat_buf);
        સંરચના timespec mtime = get_stat_mtime (&stat_buf);
        પાછા (timespec_cmp (mtime, atime) > 0);
      }

    સ્થિતિ 'O':			/* File is owned by you? */
      {
        unary_advance ();
        જો (stat (argv[pos - 1], &stat_buf) != 0)
          પાછા false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        પાછા ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    સ્થિતિ 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        જો (stat (argv[pos - 1], &stat_buf) != 0)
          પાછા false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        પાછા ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    સ્થિતિ 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    સ્થિતિ 'd':			/* File is a directory? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    સ્થિતિ 's':			/* File has something in it? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    સ્થિતિ 'S':			/* File is a socket? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    સ્થિતિ 'c':			/* File is character special? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    સ્થિતિ 'b':			/* File is block special? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    સ્થિતિ 'p':			/* File is a named pipe? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    સ્થિતિ 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    સ્થિતિ 'h':			/* File is a symbolic link? */
      unary_advance ();
      પાછા issymlink (argv[pos - 1]) == 1;

    સ્થિતિ 'u':			/* File is setuid? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    સ્થિતિ 'g':			/* File is setgid? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    સ્થિતિ 'k':			/* File has sticky bit set? */
      unary_advance ();
      પાછા (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    સ્થિતિ 't':			/* File (fd) is a terminal? */
      {
        long પૂર્ણાંક fd;
        અક્ષર અચલ *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ખાલી, 10);
        પાછા (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    સ્થિતિ 'n':			/* True if arg has some length. */
      unary_advance ();
      પાછા argv[pos - 1][0] != 0;

    સ્થિતિ 'z':			/* True if arg has no length. */
      unary_advance ();
      પાછા argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
સ્થિર bool
and (શૂન્ય)
{
  bool કિંમત = true;

  જ્યારે (true)
    {
      કિંમત &= term ();
      જો (! (pos < argc && streq (argv[pos], "-a")))
        પાછા કિંમત;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
સ્થિર bool
or (શૂન્ય)
{
  bool કિંમત = false;

  જ્યારે (true)
    {
      કિંમત |= and ();
      જો (! (pos < argc && streq (argv[pos], "-o")))
        પાછા કિંમત;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
સ્થિર bool
પદાવલિ (શૂન્ય)
{
  જો (pos >= argc)
    beyond ();

  પાછા or ();		/* Same with this. */
}

સ્થિર bool
one_argument (શૂન્ય)
{
  પાછા argv[pos++][0] != '\0';
}

સ્થિર bool
two_arguments (શૂન્ય)
{
  bool કિંમત;

  જો (streq (argv[pos], "!"))
    {
      advance (false);
      કિંમત = ! one_argument ();
    }
  નહીંતર જો (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      કિંમત = unary_operator ();
    }
  નહીંતર
    beyond ();
  પાછા (કિંમત);
}

સ્થિર bool
three_arguments (શૂન્ય)
{
  bool કિંમત;
  પૂર્ણાંક bop = binop (argv[pos + 1]);

  જો (0 <= bop)
    કિંમત = દ્વિઅંકી_operator (false, bop);
  નહીંતર જો (streq (argv[pos], "!"))
    {
      advance (true);
      કિંમત = !two_arguments ();
    }
  નહીંતર જો (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      કિંમત = one_argument ();
      advance (false);
    }
  નહીંતર જો (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    કિંમત = પદાવલિ ();
  નહીંતર
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  પાછા (કિંમત);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
સ્થિર bool
posixtest (પૂર્ણાંક nargs)
{
  bool કિંમત;

  પસંદગી (nargs)
    {
      સ્થિતિ 1:
        કિંમત = one_argument ();
        તોડો;

      સ્થિતિ 2:
        કિંમત = two_arguments ();
        તોડો;

      સ્થિતિ 3:
        કિંમત = three_arguments ();
        તોડો;

      સ્થિતિ 4:
        જો (streq (argv[pos], "!"))
          {
            advance (true);
            કિંમત = !three_arguments ();
            તોડો;
          }
        જો (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            કિંમત = two_arguments ();
            advance (false);
            તોડો;
          }
        FALLTHROUGH;
      સ્થિતિ 5:
      મૂળભૂત:
        affirm (0 < nargs);
        કિંમત = પદાવલિ ();
    }

  પાછા (કિંમત);
}

#જો defined TEST_STANDALONE

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  -N FILE     FILE exists and has been modified since it was last પઠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has પઠ access\n\
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
  -w FILE     FILE exists and the user has લિખ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or શોધ) access\n\
"));
      fputs (_("\
\n\
Except માટે -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) માટે shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
દ્વિઅંકી_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#જો !defined TEST_STANDALONE
# define મુખ્ય test_command
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
પૂર્ણાંક
મુખ્ય (પૂર્ણાંક margc, અક્ષર **margv)
{
  bool કિંમત;

#જો !defined TEST_STANDALONE
  પૂર્ણાંક code;

  code = setjmp (test_exit_buf);

  જો (code)
    પાછા (test_error_return);
#નહીંતર /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  જો (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      જો (margc == 2)
        {
          જો (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          જો (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (અક્ષર *) ખાલી);
              test_main_return (EXIT_SUCCESS);
            }
        }
      જો (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  જો (pos >= argc)
    test_main_return (TEST_FALSE);

  કિંમત = posixtest (argc - 1);

  જો (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (કિંમત ? TEST_TRUE : TEST_FALSE);
}
