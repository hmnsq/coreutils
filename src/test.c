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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#నిర్వచించు TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME (test_బహుళకం == TEST_TEST ? "test" : "[")

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "issymlink.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "strnumcmp.h"
#అన్తర్భావ "test.h"

#అన్తర్భావ <stdarg.h>

#ఒకవేళ HAVE_SYS_PARAM_H
# include <sys/param.h>
#అయితే_ముగింపు

/* Exit status for syntax errors, etc.  */
గణన { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
గణన binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#ఒకవేళ defined TEST_STANDALONE
# define test_exit(val) నిష్క్రమించు (val)
# define test_main_return(val) తిరిగి val
#లేకపోతే
   స్థిర jmp_buf test_exit_buf;
   స్థిర పూర్ణాంకం test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#అయితే_ముగింపు /* !TEST_STANDALONE */

స్థిర పూర్ణాంకం pos;		/* The offset of the current argument in ARGV. */
స్థిర పూర్ణాంకం argc;	/* The number of arguments present in ARGV. */
స్థిర అక్షరం **argv;	/* The argument list. */

స్థిర bool unary_operator (శూన్య);
స్థిర bool ద్విమాన_operator (bool, గణన binop);
స్థిర bool two_arguments (శూన్య);
స్థిర bool three_arguments (శూన్య);
స్థిర bool posixtest (పూర్ణాంకం);

స్థిర bool సమాసం (శూన్య);
స్థిర bool పదం (శూన్య);
స్థిర bool and (శూన్య);
స్థిర bool or (శూన్య);

స్థిర శూన్య beyond (శూన్య);

ATTRIBUTE_FORMAT ((రాయి, 1, 2))
స్థిర _Noreturn శూన్య
test_syntax_error (అక్షరం స్థిరరాశి *format, ...)
{
  va_list ap;
  va_ప్రారంభం (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

స్థిర శూన్య
advance (bool f)
{
  ++pos;

  ఒకవేళ (f && pos >= argc)
    beyond ();
}

స్థిర శూన్య
unary_advance (శూన్య)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
స్థిర _Noreturn శూన్య
beyond (శూన్య)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
స్థిర అక్షరం స్థిరరాశి *
కనుగొను_int (అక్షరం స్థిరరాశి *string)
{
  అక్షరం స్థిరరాశి *p;
  అక్షరం స్థిరరాశి *సంఖ్య_ప్రారంభం;

  కొరకు (p = string; ఖాళీనా (గమ్యం_uchar (*p)); p++)
    కొనసాగు;

  ఒకవేళ (*p == '+')
    {
      p++;
      సంఖ్య_ప్రారంభం = p;
    }
  లేకపోతే
    {
      సంఖ్య_ప్రారంభం = p;
      p += (*p == '-');
    }

  ఒకవేళ (c_isdigit (*p++))
    {
      అయితే (c_isdigit (*p))
        p++;
      అయితే (ఖాళీనా (గమ్యం_uchar (*p)))
        p++;
      ఒకవేళ (!*p)
        తిరిగి సంఖ్య_ప్రారంభం;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
స్థిర నిర్మాణం timespec
get_mtime (అక్షరం స్థిరరాశి *filename)
{
  నిర్మాణం stat finfo;
  తిరిగి (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
స్థిర పూర్ణాంకం
binop (అక్షరం స్థిరరాశి *s)
{
  తిరిగి (  streq (s, "="  ) ? EQ_STRING_BINOP
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
స్థిర bool
పదం (శూన్య)
{
  bool విలువ;
  bool negated = false;
  పూర్ణాంకం bop;

  /* Deal with leading 'not's.  */
  అయితే (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  ఒకవేళ (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  ఒకవేళ (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      పూర్ణాంకం nargs;

      advance (true);

      కొరకు (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        ఒకవేళ (nargs == 4)
          {
            nargs = argc - pos;
            ఆపు;
          }

      విలువ = posixtest (nargs);
      ఒకవేళ (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      లేకపోతే
        ఒకవేళ (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  లేకపోతే ఒకవేళ (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    విలువ = ద్విమాన_operator (true, bop);
  లేకపోతే ఒకవేళ (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    విలువ = ద్విమాన_operator (false, bop);

  /* It might be a switch type argument.  */
  లేకపోతే ఒకవేళ (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    విలువ = unary_operator ();
  లేకపోతే
    {
      విలువ = (argv[pos][0] != '\0');
      advance (false);
    }

  తిరిగి negated ^ విలువ;
}

స్థిర bool
ద్విమాన_operator (bool l_is_l, గణన binop bop)
{
  పూర్ణాంకం op;

  ఒకవేళ (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  ఒకవేళ (r_is_l)
    advance (false);

  pos += 3;

  ఎంపిక (bop)
    {
    స్థితి EQ_BINOP: స్థితి GE_BINOP: స్థితి GT_BINOP:
    స్థితి LE_BINOP: స్థితి LT_BINOP: స్థితి NE_BINOP:
      {
        అక్షరం lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        అక్షరం rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        అక్షరం స్థిరరాశి *l = (l_is_l
                         ? umaxtostr (అక్షరశ్రేణి_పొడవు (argv[op - 1]), lbuf)
                         : కనుగొను_int (argv[op - 1]));
        అక్షరం స్థిరరాశి *r = (r_is_l
                         ? umaxtostr (అక్షరశ్రేణి_పొడవు (argv[op + 2]), rbuf)
                         : కనుగొను_int (argv[op + 1]));
        పూర్ణాంకం cmp = strintcmp (l, r);
        ఎంపిక (+bop)
          {
          స్థితి EQ_BINOP: తిరిగి cmp == 0;
          స్థితి GE_BINOP: తిరిగి cmp >= 0;
          స్థితి GT_BINOP: తిరిగి cmp >  0;
          స్థితి LE_BINOP: తిరిగి cmp <= 0;
          స్థితి LT_BINOP: తిరిగి cmp <  0;
          స్థితి NE_BINOP: తిరిగి cmp != 0;
          }
        unreachable ();
      }

    స్థితి NT_BINOP: స్థితి OT_BINOP:
      {
        ఒకవేళ (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        పూర్ణాంకం cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        తిరిగి bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    స్థితి EF_BINOP:
      ఒకవేళ (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      లేకపోతే
        {
          నిర్మాణం stat st[2];
          తిరిగి (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    స్థితి EQ_STRING_BINOP:
    స్థితి NE_STRING_BINOP:
      తిరిగి streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    స్థితి GT_STRING_BINOP:
    స్థితి LT_STRING_BINOP:
      {
        పూర్ణాంకం cmp = strcoll (argv[op - 1], argv[op + 1]);
        తిరిగి bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

స్థిర bool
unary_operator (శూన్య)
{
  నిర్మాణం stat stat_buf;

  ఎంపిక (argv[pos][1])
    {
    అప్రమేయ:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    స్థితి 'e':			/* file exists in the file system? */
      unary_advance ();
      తిరిగి stat (argv[pos - 1], &stat_buf) == 0;

    స్థితి 'r':			/* file is readable? */
      unary_advance ();
      తిరిగి euidaccess (argv[pos - 1], R_OK) == 0;

    స్థితి 'w':			/* File is writable? */
      unary_advance ();
      తిరిగి euidaccess (argv[pos - 1], W_OK) == 0;

    స్థితి 'x':			/* File is executable? */
      unary_advance ();
      తిరిగి euidaccess (argv[pos - 1], X_OK) == 0;

    స్థితి 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        ఒకవేళ (stat (argv[pos - 1], &stat_buf) != 0)
          తిరిగి false;
        నిర్మాణం timespec atime = get_stat_atime (&stat_buf);
        నిర్మాణం timespec mtime = get_stat_mtime (&stat_buf);
        తిరిగి (timespec_cmp (mtime, atime) > 0);
      }

    స్థితి 'O':			/* File is owned by you? */
      {
        unary_advance ();
        ఒకవేళ (stat (argv[pos - 1], &stat_buf) != 0)
          తిరిగి false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        తిరిగి ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    స్థితి 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        ఒకవేళ (stat (argv[pos - 1], &stat_buf) != 0)
          తిరిగి false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        తిరిగి ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    స్థితి 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_బహుళకం));

    స్థితి 'd':			/* File is a directory? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_బహుళకం));

    స్థితి 's':			/* File has something in it? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_కొలత);

    స్థితి 'S':			/* File is a socket? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_బహుళకం));

    స్థితి 'c':			/* File is character special? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_బహుళకం));

    స్థితి 'b':			/* File is block special? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_బహుళకం));

    స్థితి 'p':			/* File is a named pipe? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_బహుళకం));

    స్థితి 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    స్థితి 'h':			/* File is a symbolic link? */
      unary_advance ();
      తిరిగి issymlink (argv[pos - 1]) == 1;

    స్థితి 'u':			/* File is setuid? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_బహుళకం & S_ISUID));

    స్థితి 'g':			/* File is setgid? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_బహుళకం & S_ISGID));

    స్థితి 'k':			/* File has sticky bit set? */
      unary_advance ();
      తిరిగి (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_బహుళకం & S_ISVTX));

    స్థితి 't':			/* File (fd) is a terminal? */
      {
        దీర్ఘ పూర్ణాంకం fd;
        అక్షరం స్థిరరాశి *arg;
        unary_advance ();
        arg = కనుగొను_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ఖాళీ, 10);
        తిరిగి (errno != ERANGE && 0 <= fd && fd <= గరిష్ఠ_పూర్ణాంకం && isatty (fd));
      }

    స్థితి 'n':			/* True if arg has some length. */
      unary_advance ();
      తిరిగి argv[pos - 1][0] != 0;

    స్థితి 'z':			/* True if arg has no length. */
      unary_advance ();
      తిరిగి argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
స్థిర bool
and (శూన్య)
{
  bool విలువ = true;

  అయితే (true)
    {
      విలువ &= పదం ();
      ఒకవేళ (! (pos < argc && streq (argv[pos], "-a")))
        తిరిగి విలువ;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
స్థిర bool
or (శూన్య)
{
  bool విలువ = false;

  అయితే (true)
    {
      విలువ |= and ();
      ఒకవేళ (! (pos < argc && streq (argv[pos], "-o")))
        తిరిగి విలువ;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
స్థిర bool
సమాసం (శూన్య)
{
  ఒకవేళ (pos >= argc)
    beyond ();

  తిరిగి or ();		/* Same with this. */
}

స్థిర bool
one_వాదం (శూన్య)
{
  తిరిగి argv[pos++][0] != '\0';
}

స్థిర bool
two_arguments (శూన్య)
{
  bool విలువ;

  ఒకవేళ (streq (argv[pos], "!"))
    {
      advance (false);
      విలువ = ! one_వాదం ();
    }
  లేకపోతే ఒకవేళ (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      విలువ = unary_operator ();
    }
  లేకపోతే
    beyond ();
  తిరిగి (విలువ);
}

స్థిర bool
three_arguments (శూన్య)
{
  bool విలువ;
  పూర్ణాంకం bop = binop (argv[pos + 1]);

  ఒకవేళ (0 <= bop)
    విలువ = ద్విమాన_operator (false, bop);
  లేకపోతే ఒకవేళ (streq (argv[pos], "!"))
    {
      advance (true);
      విలువ = !two_arguments ();
    }
  లేకపోతే ఒకవేళ (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      విలువ = one_వాదం ();
      advance (false);
    }
  లేకపోతే ఒకవేళ (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    విలువ = సమాసం ();
  లేకపోతే
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  తిరిగి (విలువ);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
స్థిర bool
posixtest (పూర్ణాంకం nargs)
{
  bool విలువ;

  ఎంపిక (nargs)
    {
      స్థితి 1:
        విలువ = one_వాదం ();
        ఆపు;

      స్థితి 2:
        విలువ = two_arguments ();
        ఆపు;

      స్థితి 3:
        విలువ = three_arguments ();
        ఆపు;

      స్థితి 4:
        ఒకవేళ (streq (argv[pos], "!"))
          {
            advance (true);
            విలువ = !three_arguments ();
            ఆపు;
          }
        ఒకవేళ (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            విలువ = two_arguments ();
            advance (false);
            ఆపు;
          }
        FALLTHROUGH;
      స్థితి 5:
      అప్రమేయ:
        affirm (0 < nargs);
        విలువ = సమాసం ();
    }

  తిరిగి (విలువ);
}

#ఒకవేళ defined TEST_STANDALONE

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      fputs (_("\
Usage: test వ్యక్తీకరణ\n\
  or:  test\n\
  or:  [ వ్యక్తీకరణ ]\n\
  or:  [ ]\n\
  or:  [ OPTION\n\
"), stdout);
      fputs (_("\
Exit with the స్థితి_2 determined by వ్యక్తీకరణ.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
An omitted వ్యక్తీకరణ defaults గమ్యం false.  Otherwise,\n\
వ్యక్తీకరణ is true or false and sets నిష్క్రమించు స్థితి_2.  It is one యొక్క:\n\
"), stdout);
      fputs (_("\
\n\
  ( వ్యక్తీకరణ )               వ్యక్తీకరణ is true\n\
  ! వ్యక్తీకరణ                 వ్యక్తీకరణ is false\n\
  EXPRESSION1 -a EXPRESSION2   both EXPRESSION1 and EXPRESSION2 are true\n\
  EXPRESSION1 -o EXPRESSION2   either EXPRESSION1 or EXPRESSION2 is true\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  -n STRING            the నిడివి యొక్క STRING is nonzero\n\
"));
      fputs (_("\
  STRING               equivalent గమ్యం -n STRING\n\
"), stdout);
      oputs (_("\
  -z STRING            the నిడివి యొక్క STRING is zero\n\
"));
      fputs (_("\
  STRING1 = STRING2    the అక్షరశ్రేణులు are equal\n\
  STRING1 != STRING2   the అక్షరశ్రేణులు are not equal\n\
  STRING1 > STRING2    STRING1 is greater than STRING2 లోపల the current locale\n\
  STRING1 < STRING2    STRING1 is less than STRING2 లోపల the current locale\n\
"), stdout);
      fputs (_("\
\n\
"), stdout);
      oputs (_("\
  INTEGER1 -eq INTEGER2   INTEGER1 is equal గమ్యం INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ge INTEGER2   INTEGER1 is greater than or equal గమ్యం INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -gt INTEGER2   INTEGER1 is greater than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -le INTEGER2   INTEGER1 is less than or equal గమ్యం INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -lt INTEGER2   INTEGER1 is less than INTEGER2\n\
"));
      oputs (_("\
  INTEGER1 -ne INTEGER2   INTEGER1 is not equal గమ్యం INTEGER2\n\
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
  -b FILE     FILE exists and is ఖండం special\n\
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
  -g FILE     FILE exists and its సమితి_2-సమూహం-ID బిట్టు is సమితి_2\n\
"));
      oputs (_("\
  -G FILE     FILE exists and is owned by the effective సమూహం ID\n\
"));
      oputs (_("\
  -h FILE     FILE exists and is a symbolic link (same as -L)\n\
"));
      oputs (_("\
  -k FILE     FILE exists and has its sticky బిట్టు సమితి_2\n\
"));
      oputs (_("\
  -L FILE     FILE exists and is a symbolic link (same as -h)\n\
"));
      oputs (_("\
  -N FILE     FILE exists and has been modified since it was చివరి పఠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has పఠ access\n\
"));
      oputs (_("\
  -s FILE     FILE exists and has a కొలత greater than zero\n\
"));
      oputs (_("\
  -S FILE     FILE exists and is a socket\n\
"));
      oputs (_("\
  -t FD       file descriptor FD is opened on a terminal\n\
"));
      oputs (_("\
  -u FILE     FILE exists and its సమితి_2-user-ID బిట్టు is సమితి_2\n\
"));
      oputs (_("\
  -w FILE     FILE exists and the user has లిఖ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or శోధన) access\n\
"));
      fputs (_("\
\n\
Except కొరకు -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need గమ్యం be escaped (e.g., by backslashes) కొరకు shells.\n\
INTEGER may also be -l STRING, which evaluates గమ్యం the నిడివి యొక్క STRING.\n\
"), stdout);
      fputs (_("\
\n\
ద్విమాన_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
or 'test EXPR1 || test EXPR2' instead.\n\
"), stdout);
      fputs (_("\
\n\
'[' honors --help and --సంస్కరణ_3, but 'test' treats them as అక్షరశ్రేణులు_2.\n\
"), stdout);
      రాయి (USAGE_BUILTIN_WARNING, _("test and/or ["));
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}
#అయితే_ముగింపు /* TEST_STANDALONE */

#ఒకవేళ !defined TEST_STANDALONE
# define ప్రధాన test_command
#అయితే_ముగింపు

#నిర్వచించు AUTHORS \
  proper_name ("Kevin Braunsdorf"), \
  proper_name ("Matthew Bradburn")

/*
 * [:
 *	'[' expr ']'
 * test:
 *	test expr
 */
పూర్ణాంకం
ప్రధాన (పూర్ణాంకం margc, అక్షరం **margv)
{
  bool విలువ;

#ఒకవేళ !defined TEST_STANDALONE
  పూర్ణాంకం సంకేతం_2;

  సంకేతం_2 = setjmp (test_exit_buf);

  ఒకవేళ (సంకేతం_2)
    తిరిగి (test_error_return);
#లేకపోతే /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  సమితి_కార్యక్రమం_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#అయితే_ముగింపు /* TEST_STANDALONE */

  argv = margv;

  ఒకవేళ (test_బహుళకం == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      ఒకవేళ (margc == 2)
        {
          ఒకవేళ (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          ఒకవేళ (streq (margv[1], "--version"))
            {
              సంస్కరణ_etc (stdout, కార్యక్రమం_NAME, PACKAGE_NAME, సంస్కరణ, AUTHORS,
                           (అక్షరం *) ఖాళీ);
              test_main_return (EXIT_SUCCESS);
            }
        }
      ఒకవేళ (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  ఒకవేళ (pos >= argc)
    test_main_return (TEST_FALSE);

  విలువ = posixtest (argc - 1);

  ఒకవేళ (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (విలువ ? TEST_TRUE : TEST_FALSE);
}
