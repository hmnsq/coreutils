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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#include "system.h"
#include "assure.h"
#include "c-ctype.h"
#include "issymlink.h"
#include "quote.h"
#include "stat-time.h"
#include "strnumcmp.h"
#include "test.h"

#include <stdarg.h>

#གལ་ཏེ HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
ga_nanaa { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
ga_nanaa binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#གལ་ཏེ defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) ཕྱིར val
#དེ་མིན
   static jmp_buf test_exit_buf;
   static ཧྲིལ test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

static ཧྲིལ pos;		/* The offset of the current argument in ARGV. */
static ཧྲིལ argc;	/* The number of arguments present in ARGV. */
static var_na **argv;	/* The argument list. */

static bool unary_operator (void);
static bool binary_operator (bool, ga_nanaa binop);
static bool two_arguments (void);
static bool three_arguments (void);
static bool posixtest (ཧྲིལ);

static bool expr (void);
static bool term (void);
static bool and (void);
static bool or (void);

static void beyond (void);

ATTRIBUTE_FORMAT ((printf, 1, 2))
static _Noreturn void
test_syntax_error (var_na const *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

static void
advance (bool f)
{
  ++pos;

  གལ་ཏེ (f && pos >= argc)
    beyond ();
}

static void
unary_advance (void)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
static _Noreturn void
beyond (void)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
static var_na const *
find_int (var_na const *string)
{
  var_na const *p;
  var_na const *number_start;

  ཆེད་དུ (p = string; isspace (to_uchar (*p)); p++)
    continue;

  གལ་ཏེ (*p == '+')
    {
      p++;
      number_start = p;
    }
  དེ་མིན
    {
      number_start = p;
      p += (*p == '-');
    }

  གལ་ཏེ (c_isdigit (*p++))
    {
      སྐབས (c_isdigit (*p))
        p++;
      སྐབས (isspace (to_uchar (*p)))
        p++;
      གལ་ཏེ (!*p)
        ཕྱིར number_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
static sa_nracanaa timespec
get_mtime (var_na const *filename)
{
  sa_nracanaa stat finfo;
  ཕྱིར (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
static ཧྲིལ
binop (var_na const *s)
{
  ཕྱིར (  streq (s, "="  ) ? EQ_STRING_BINOP
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
static bool
term (void)
{
  bool value;
  bool negated = false;
  ཧྲིལ bop;

  /* Deal with leading 'not's.  */
  སྐབས (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  གལ་ཏེ (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  གལ་ཏེ (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      ཧྲིལ nargs;

      advance (true);

      ཆེད་དུ (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        གལ་ཏེ (nargs == 4)
          {
            nargs = argc - pos;
            bha_nga;
          }

      value = posixtest (nargs);
      གལ་ཏེ (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      དེ་མིན
        གལ་ཏེ (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  དེ་མིན གལ་ཏེ (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    value = binary_operator (true, bop);
  དེ་མིན གལ་ཏེ (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    value = binary_operator (false, bop);

  /* It might be a switch type argument.  */
  དེ་མིན གལ་ཏེ (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    value = unary_operator ();
  དེ་མིན
    {
      value = (argv[pos][0] != '\0');
      advance (false);
    }

  ཕྱིར negated ^ value;
}

static bool
binary_operator (bool l_is_l, ga_nanaa binop bop)
{
  ཧྲིལ op;

  གལ་ཏེ (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  གལ་ཏེ (r_is_l)
    advance (false);

  pos += 3;

  switch (bop)
    {
    pak_sa EQ_BINOP: pak_sa GE_BINOP: pak_sa GT_BINOP:
    pak_sa LE_BINOP: pak_sa LT_BINOP: pak_sa NE_BINOP:
      {
        var_na lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        var_na rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        var_na const *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        var_na const *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        ཧྲིལ cmp = strintcmp (l, r);
        switch (+bop)
          {
          pak_sa EQ_BINOP: ཕྱིར cmp == 0;
          pak_sa GE_BINOP: ཕྱིར cmp >= 0;
          pak_sa GT_BINOP: ཕྱིར cmp >  0;
          pak_sa LE_BINOP: ཕྱིར cmp <= 0;
          pak_sa LT_BINOP: ཕྱིར cmp <  0;
          pak_sa NE_BINOP: ཕྱིར cmp != 0;
          }
        unreachable ();
      }

    pak_sa NT_BINOP: pak_sa OT_BINOP:
      {
        གལ་ཏེ (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        ཧྲིལ cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        ཕྱིར bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    pak_sa EF_BINOP:
      གལ་ཏེ (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      དེ་མིན
        {
          sa_nracanaa stat st[2];
          ཕྱིར (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    pak_sa EQ_STRING_BINOP:
    pak_sa NE_STRING_BINOP:
      ཕྱིར streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    pak_sa GT_STRING_BINOP:
    pak_sa LT_STRING_BINOP:
      {
        ཧྲིལ cmp = strcoll (argv[op - 1], argv[op + 1]);
        ཕྱིར bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

static bool
unary_operator (void)
{
  sa_nracanaa stat stat_buf;

  switch (argv[pos][1])
    {
    default:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    pak_sa 'e':			/* file exists in the file system? */
      unary_advance ();
      ཕྱིར stat (argv[pos - 1], &stat_buf) == 0;

    pak_sa 'r':			/* file is readable? */
      unary_advance ();
      ཕྱིར euidaccess (argv[pos - 1], R_OK) == 0;

    pak_sa 'w':			/* File is writable? */
      unary_advance ();
      ཕྱིར euidaccess (argv[pos - 1], W_OK) == 0;

    pak_sa 'x':			/* File is executable? */
      unary_advance ();
      ཕྱིར euidaccess (argv[pos - 1], X_OK) == 0;

    pak_sa 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        གལ་ཏེ (stat (argv[pos - 1], &stat_buf) != 0)
          ཕྱིར false;
        sa_nracanaa timespec atime = get_stat_atime (&stat_buf);
        sa_nracanaa timespec mtime = get_stat_mtime (&stat_buf);
        ཕྱིར (timespec_cmp (mtime, atime) > 0);
      }

    pak_sa 'O':			/* File is owned by you? */
      {
        unary_advance ();
        གལ་ཏེ (stat (argv[pos - 1], &stat_buf) != 0)
          ཕྱིར false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        ཕྱིར ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    pak_sa 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        གལ་ཏེ (stat (argv[pos - 1], &stat_buf) != 0)
          ཕྱིར false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        ཕྱིར ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    pak_sa 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    pak_sa 'd':			/* File is a directory? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    pak_sa 's':			/* File has something in it? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    pak_sa 'S':			/* File is a socket? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    pak_sa 'c':			/* File is character special? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    pak_sa 'b':			/* File is block special? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    pak_sa 'p':			/* File is a named pipe? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    pak_sa 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    pak_sa 'h':			/* File is a symbolic link? */
      unary_advance ();
      ཕྱིར issymlink (argv[pos - 1]) == 1;

    pak_sa 'u':			/* File is setuid? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    pak_sa 'g':			/* File is setgid? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    pak_sa 'k':			/* File has sticky bit set? */
      unary_advance ();
      ཕྱིར (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    pak_sa 't':			/* File (fd) is a terminal? */
      {
        long ཧྲིལ fd;
        var_na const *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, NULL, 10);
        ཕྱིར (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    pak_sa 'n':			/* True if arg has some length. */
      unary_advance ();
      ཕྱིར argv[pos - 1][0] != 0;

    pak_sa 'z':			/* True if arg has no length. */
      unary_advance ();
      ཕྱིར argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
static bool
and (void)
{
  bool value = true;

  སྐབས (true)
    {
      value &= term ();
      གལ་ཏེ (! (pos < argc && streq (argv[pos], "-a")))
        ཕྱིར value;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
static bool
or (void)
{
  bool value = false;

  སྐབས (true)
    {
      value |= and ();
      གལ་ཏེ (! (pos < argc && streq (argv[pos], "-o")))
        ཕྱིར value;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
static bool
expr (void)
{
  གལ་ཏེ (pos >= argc)
    beyond ();

  ཕྱིར or ();		/* Same with this. */
}

static bool
one_argument (void)
{
  ཕྱིར argv[pos++][0] != '\0';
}

static bool
two_arguments (void)
{
  bool value;

  གལ་ཏེ (streq (argv[pos], "!"))
    {
      advance (false);
      value = ! one_argument ();
    }
  དེ་མིན གལ་ཏེ (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      value = unary_operator ();
    }
  དེ་མིན
    beyond ();
  ཕྱིར (value);
}

static bool
three_arguments (void)
{
  bool value;
  ཧྲིལ bop = binop (argv[pos + 1]);

  གལ་ཏེ (0 <= bop)
    value = binary_operator (false, bop);
  དེ་མིན གལ་ཏེ (streq (argv[pos], "!"))
    {
      advance (true);
      value = !two_arguments ();
    }
  དེ་མིན གལ་ཏེ (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      value = one_argument ();
      advance (false);
    }
  དེ་མིན གལ་ཏེ (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    value = expr ();
  དེ་མིན
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  ཕྱིར (value);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
static bool
posixtest (ཧྲིལ nargs)
{
  bool value;

  switch (nargs)
    {
      pak_sa 1:
        value = one_argument ();
        bha_nga;

      pak_sa 2:
        value = two_arguments ();
        bha_nga;

      pak_sa 3:
        value = three_arguments ();
        bha_nga;

      pak_sa 4:
        གལ་ཏེ (streq (argv[pos], "!"))
          {
            advance (true);
            value = !three_arguments ();
            bha_nga;
          }
        གལ་ཏེ (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            value = two_arguments ();
            advance (false);
            bha_nga;
          }
        FALLTHROUGH;
      pak_sa 5:
      default:
        affirm (0 < nargs);
        value = expr ();
    }

  ཕྱིར (value);
}

#གལ་ཏེ defined TEST_STANDALONE

void
usage (ཧྲིལ status)
{
  གལ་ཏེ (status != EXIT_SUCCESS)
    emit_try_help ();
  དེ་མིན
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
  -N FILE     FILE exists and has been modified since it was last read\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has read access\n\
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
  -w FILE     FILE exists and the user has write access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or search) access\n\
"));
      fputs (_("\
\n\
Except ཆེད་དུ -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) ཆེད་དུ shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
Binary -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#གལ་ཏེ !defined TEST_STANDALONE
# define གཙོ test_command
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
ཧྲིལ
གཙོ (ཧྲིལ margc, var_na **margv)
{
  bool value;

#གལ་ཏེ !defined TEST_STANDALONE
  ཧྲིལ code;

  code = setjmp (test_exit_buf);

  གལ་ཏེ (code)
    ཕྱིར (test_error_return);
#དེ་མིན /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  གལ་ཏེ (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      གལ་ཏེ (margc == 2)
        {
          གལ་ཏེ (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          གལ་ཏེ (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (var_na *) NULL);
              test_main_return (EXIT_SUCCESS);
            }
        }
      གལ་ཏེ (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  གལ་ཏེ (pos >= argc)
    test_main_return (TEST_FALSE);

  value = posixtest (argc - 1);

  གལ་ཏེ (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (value ? TEST_TRUE : TEST_FALSE);
}
