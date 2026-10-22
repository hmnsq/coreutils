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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "issymlink.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "strnumcmp.h"
#അന്തര്ഭാവ "test.h"

#അന്തര്ഭാവ <stdarg.h>

#എങ്കിൽ HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
എണ്ണൽ { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
എണ്ണൽ binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#എങ്കിൽ defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) തിരികെ val
#അല്ലെങ്കിൽ
   സ്ഥിതം jmp_buf test_exit_buf;
   സ്ഥിതം പൂർണ്ണസംഖ്യ test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

സ്ഥിതം പൂർണ്ണസംഖ്യ pos;		/* The offset of the current argument in ARGV. */
സ്ഥിതം പൂർണ്ണസംഖ്യ argc;	/* The number of arguments present in ARGV. */
സ്ഥിതം അക്ഷരം **argv;	/* The argument list. */

സ്ഥിതം bool unary_operator (ശൂന്യം);
സ്ഥിതം bool ദ്വിമാന_operator (bool, എണ്ണൽ binop);
സ്ഥിതം bool two_arguments (ശൂന്യം);
സ്ഥിതം bool three_arguments (ശൂന്യം);
സ്ഥിതം bool posixtest (പൂർണ്ണസംഖ്യ);

സ്ഥിതം bool വ്യഞ്ജകം (ശൂന്യം);
സ്ഥിതം bool term (ശൂന്യം);
സ്ഥിതം bool and (ശൂന്യം);
സ്ഥിതം bool or (ശൂന്യം);

സ്ഥിതം ശൂന്യം beyond (ശൂന്യം);

ATTRIBUTE_FORMAT ((printf, 1, 2))
സ്ഥിതം _Noreturn ശൂന്യം
test_syntax_error (അക്ഷരം സ്ഥിരം *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

സ്ഥിതം ശൂന്യം
advance (bool f)
{
  ++pos;

  എങ്കിൽ (f && pos >= argc)
    beyond ();
}

സ്ഥിതം ശൂന്യം
unary_advance (ശൂന്യം)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
സ്ഥിതം _Noreturn ശൂന്യം
beyond (ശൂന്യം)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *
find_int (അക്ഷരം സ്ഥിരം *string)
{
  അക്ഷരം സ്ഥിരം *p;
  അക്ഷരം സ്ഥിരം *സംഖ്യ_start;

  വേണ്ടി (p = string; isspace (to_uchar (*p)); p++)
    തുടരുക;

  എങ്കിൽ (*p == '+')
    {
      p++;
      സംഖ്യ_start = p;
    }
  അല്ലെങ്കിൽ
    {
      സംഖ്യ_start = p;
      p += (*p == '-');
    }

  എങ്കിൽ (c_isdigit (*p++))
    {
      ആകുമ്പോൾ (c_isdigit (*p))
        p++;
      ആകുമ്പോൾ (isspace (to_uchar (*p)))
        p++;
      എങ്കിൽ (!*p)
        തിരികെ സംഖ്യ_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
സ്ഥിതം ഘടന timespec
get_mtime (അക്ഷരം സ്ഥിരം *filename)
{
  ഘടന stat finfo;
  തിരികെ (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
binop (അക്ഷരം സ്ഥിരം *s)
{
  തിരികെ (  streq (s, "="  ) ? EQ_STRING_BINOP
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
സ്ഥിതം bool
term (ശൂന്യം)
{
  bool മൂല്യം;
  bool negated = false;
  പൂർണ്ണസംഖ്യ bop;

  /* Deal with leading 'not's.  */
  ആകുമ്പോൾ (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  എങ്കിൽ (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  എങ്കിൽ (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      പൂർണ്ണസംഖ്യ nargs;

      advance (true);

      വേണ്ടി (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        എങ്കിൽ (nargs == 4)
          {
            nargs = argc - pos;
            നിർത്തുക;
          }

      മൂല്യം = posixtest (nargs);
      എങ്കിൽ (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      അല്ലെങ്കിൽ
        എങ്കിൽ (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  അല്ലെങ്കിൽ എങ്കിൽ (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    മൂല്യം = ദ്വിമാന_operator (true, bop);
  അല്ലെങ്കിൽ എങ്കിൽ (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    മൂല്യം = ദ്വിമാന_operator (false, bop);

  /* It might be a switch type argument.  */
  അല്ലെങ്കിൽ എങ്കിൽ (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    മൂല്യം = unary_operator ();
  അല്ലെങ്കിൽ
    {
      മൂല്യം = (argv[pos][0] != '\0');
      advance (false);
    }

  തിരികെ negated ^ മൂല്യം;
}

സ്ഥിതം bool
ദ്വിമാന_operator (bool l_is_l, എണ്ണൽ binop bop)
{
  പൂർണ്ണസംഖ്യ op;

  എങ്കിൽ (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  എങ്കിൽ (r_is_l)
    advance (false);

  pos += 3;

  തിരഞ്ഞെടുപ്പ് (bop)
    {
    അവസ്ഥ EQ_BINOP: അവസ്ഥ GE_BINOP: അവസ്ഥ GT_BINOP:
    അവസ്ഥ LE_BINOP: അവസ്ഥ LT_BINOP: അവസ്ഥ NE_BINOP:
      {
        അക്ഷരം lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        അക്ഷരം rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        അക്ഷരം സ്ഥിരം *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        അക്ഷരം സ്ഥിരം *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        പൂർണ്ണസംഖ്യ cmp = strintcmp (l, r);
        തിരഞ്ഞെടുപ്പ് (+bop)
          {
          അവസ്ഥ EQ_BINOP: തിരികെ cmp == 0;
          അവസ്ഥ GE_BINOP: തിരികെ cmp >= 0;
          അവസ്ഥ GT_BINOP: തിരികെ cmp >  0;
          അവസ്ഥ LE_BINOP: തിരികെ cmp <= 0;
          അവസ്ഥ LT_BINOP: തിരികെ cmp <  0;
          അവസ്ഥ NE_BINOP: തിരികെ cmp != 0;
          }
        unreachable ();
      }

    അവസ്ഥ NT_BINOP: അവസ്ഥ OT_BINOP:
      {
        എങ്കിൽ (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        പൂർണ്ണസംഖ്യ cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        തിരികെ bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    അവസ്ഥ EF_BINOP:
      എങ്കിൽ (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      അല്ലെങ്കിൽ
        {
          ഘടന stat st[2];
          തിരികെ (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    അവസ്ഥ EQ_STRING_BINOP:
    അവസ്ഥ NE_STRING_BINOP:
      തിരികെ streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    അവസ്ഥ GT_STRING_BINOP:
    അവസ്ഥ LT_STRING_BINOP:
      {
        പൂർണ്ണസംഖ്യ cmp = strcoll (argv[op - 1], argv[op + 1]);
        തിരികെ bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

സ്ഥിതം bool
unary_operator (ശൂന്യം)
{
  ഘടന stat stat_buf;

  തിരഞ്ഞെടുപ്പ് (argv[pos][1])
    {
    സ്ഥിരസ്ഥിതി:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    അവസ്ഥ 'e':			/* file exists in the file system? */
      unary_advance ();
      തിരികെ stat (argv[pos - 1], &stat_buf) == 0;

    അവസ്ഥ 'r':			/* file is readable? */
      unary_advance ();
      തിരികെ euidaccess (argv[pos - 1], R_OK) == 0;

    അവസ്ഥ 'w':			/* File is writable? */
      unary_advance ();
      തിരികെ euidaccess (argv[pos - 1], W_OK) == 0;

    അവസ്ഥ 'x':			/* File is executable? */
      unary_advance ();
      തിരികെ euidaccess (argv[pos - 1], X_OK) == 0;

    അവസ്ഥ 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        എങ്കിൽ (stat (argv[pos - 1], &stat_buf) != 0)
          തിരികെ false;
        ഘടന timespec atime = get_stat_atime (&stat_buf);
        ഘടന timespec mtime = get_stat_mtime (&stat_buf);
        തിരികെ (timespec_cmp (mtime, atime) > 0);
      }

    അവസ്ഥ 'O':			/* File is owned by you? */
      {
        unary_advance ();
        എങ്കിൽ (stat (argv[pos - 1], &stat_buf) != 0)
          തിരികെ false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        തിരികെ ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    അവസ്ഥ 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        എങ്കിൽ (stat (argv[pos - 1], &stat_buf) != 0)
          തിരികെ false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        തിരികെ ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    അവസ്ഥ 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    അവസ്ഥ 'd':			/* File is a directory? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    അവസ്ഥ 's':			/* File has something in it? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    അവസ്ഥ 'S':			/* File is a socket? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    അവസ്ഥ 'c':			/* File is character special? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    അവസ്ഥ 'b':			/* File is block special? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    അവസ്ഥ 'p':			/* File is a named pipe? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    അവസ്ഥ 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    അവസ്ഥ 'h':			/* File is a symbolic link? */
      unary_advance ();
      തിരികെ issymlink (argv[pos - 1]) == 1;

    അവസ്ഥ 'u':			/* File is setuid? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    അവസ്ഥ 'g':			/* File is setgid? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    അവസ്ഥ 'k':			/* File has sticky bit set? */
      unary_advance ();
      തിരികെ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    അവസ്ഥ 't':			/* File (fd) is a terminal? */
      {
        long പൂർണ്ണസംഖ്യ fd;
        അക്ഷരം സ്ഥിരം *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ശൂന്യ, 10);
        തിരികെ (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    അവസ്ഥ 'n':			/* True if arg has some length. */
      unary_advance ();
      തിരികെ argv[pos - 1][0] != 0;

    അവസ്ഥ 'z':			/* True if arg has no length. */
      unary_advance ();
      തിരികെ argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
സ്ഥിതം bool
and (ശൂന്യം)
{
  bool മൂല്യം = true;

  ആകുമ്പോൾ (true)
    {
      മൂല്യം &= term ();
      എങ്കിൽ (! (pos < argc && streq (argv[pos], "-a")))
        തിരികെ മൂല്യം;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
സ്ഥിതം bool
or (ശൂന്യം)
{
  bool മൂല്യം = false;

  ആകുമ്പോൾ (true)
    {
      മൂല്യം |= and ();
      എങ്കിൽ (! (pos < argc && streq (argv[pos], "-o")))
        തിരികെ മൂല്യം;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
സ്ഥിതം bool
വ്യഞ്ജകം (ശൂന്യം)
{
  എങ്കിൽ (pos >= argc)
    beyond ();

  തിരികെ or ();		/* Same with this. */
}

സ്ഥിതം bool
one_argument (ശൂന്യം)
{
  തിരികെ argv[pos++][0] != '\0';
}

സ്ഥിതം bool
two_arguments (ശൂന്യം)
{
  bool മൂല്യം;

  എങ്കിൽ (streq (argv[pos], "!"))
    {
      advance (false);
      മൂല്യം = ! one_argument ();
    }
  അല്ലെങ്കിൽ എങ്കിൽ (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      മൂല്യം = unary_operator ();
    }
  അല്ലെങ്കിൽ
    beyond ();
  തിരികെ (മൂല്യം);
}

സ്ഥിതം bool
three_arguments (ശൂന്യം)
{
  bool മൂല്യം;
  പൂർണ്ണസംഖ്യ bop = binop (argv[pos + 1]);

  എങ്കിൽ (0 <= bop)
    മൂല്യം = ദ്വിമാന_operator (false, bop);
  അല്ലെങ്കിൽ എങ്കിൽ (streq (argv[pos], "!"))
    {
      advance (true);
      മൂല്യം = !two_arguments ();
    }
  അല്ലെങ്കിൽ എങ്കിൽ (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      മൂല്യം = one_argument ();
      advance (false);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    മൂല്യം = വ്യഞ്ജകം ();
  അല്ലെങ്കിൽ
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  തിരികെ (മൂല്യം);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
സ്ഥിതം bool
posixtest (പൂർണ്ണസംഖ്യ nargs)
{
  bool മൂല്യം;

  തിരഞ്ഞെടുപ്പ് (nargs)
    {
      അവസ്ഥ 1:
        മൂല്യം = one_argument ();
        നിർത്തുക;

      അവസ്ഥ 2:
        മൂല്യം = two_arguments ();
        നിർത്തുക;

      അവസ്ഥ 3:
        മൂല്യം = three_arguments ();
        നിർത്തുക;

      അവസ്ഥ 4:
        എങ്കിൽ (streq (argv[pos], "!"))
          {
            advance (true);
            മൂല്യം = !three_arguments ();
            നിർത്തുക;
          }
        എങ്കിൽ (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            മൂല്യം = two_arguments ();
            advance (false);
            നിർത്തുക;
          }
        FALLTHROUGH;
      അവസ്ഥ 5:
      സ്ഥിരസ്ഥിതി:
        affirm (0 < nargs);
        മൂല്യം = വ്യഞ്ജകം ();
    }

  തിരികെ (മൂല്യം);
}

#എങ്കിൽ defined TEST_STANDALONE

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  -N FILE     FILE exists and has been modified since it was last പഠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has പഠ access\n\
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
  -w FILE     FILE exists and the user has ലിഖ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or തിരയൽ) access\n\
"));
      fputs (_("\
\n\
Except വേണ്ടി -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) വേണ്ടി shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
ദ്വിമാന_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#എങ്കിൽ !defined TEST_STANDALONE
# define പ്രധാന test_command
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
പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ margc, അക്ഷരം **margv)
{
  bool മൂല്യം;

#എങ്കിൽ !defined TEST_STANDALONE
  പൂർണ്ണസംഖ്യ code;

  code = setjmp (test_exit_buf);

  എങ്കിൽ (code)
    തിരികെ (test_error_return);
#അല്ലെങ്കിൽ /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  എങ്കിൽ (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      എങ്കിൽ (margc == 2)
        {
          എങ്കിൽ (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          എങ്കിൽ (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (അക്ഷരം *) ശൂന്യ);
              test_main_return (EXIT_SUCCESS);
            }
        }
      എങ്കിൽ (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  എങ്കിൽ (pos >= argc)
    test_main_return (TEST_FALSE);

  മൂല്യം = posixtest (argc - 1);

  എങ്കിൽ (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (മൂല്യം ? TEST_TRUE : TEST_FALSE);
}
