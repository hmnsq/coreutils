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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#define TEST_STANDALONE 1

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (test_mode == TEST_TEST ? "test" : "[")

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "issymlink.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"
#ଅନ୍ତର୍ଭାଵ "strnumcmp.h"
#ଅନ୍ତର୍ଭାଵ "test.h"

#ଅନ୍ତର୍ଭାଵ <stdarg.h>

#ଯଦି HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

/* Exit status for syntax errors, etc.  */
ଗଣନା { TEST_TRUE, TEST_FALSE, TEST_FAILURE };

/* Binary operators.  */
ଗଣନା binop
  {
    /* String comparisons, e.g., EQ_STRING_BINOP for = or ==.  */
    EQ_STRING_BINOP, GT_STRING_BINOP, LT_STRING_BINOP, NE_STRING_BINOP,

    /* Two-letter binary operators, e.g, EF_BINOP for -ef.  */
    EQ_BINOP, GE_BINOP, GT_BINOP, LE_BINOP, LT_BINOP, NE_BINOP,
    EF_BINOP, NT_BINOP, OT_BINOP
  };

#ଯଦି defined TEST_STANDALONE
# define test_exit(val) exit (val)
# define test_main_return(val) ଫେରନ୍ତୁ val
#ଅନ୍ୟଥା
   ସ୍ଥିର jmp_buf test_exit_buf;
   ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_error_return = 0;
# define test_exit(val) test_error_return = val, longjmp (test_exit_buf, 1)
# define test_main_return(val) test_exit (val)
#endif /* !TEST_STANDALONE */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos;		/* The offset of the current argument in ARGV. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;	/* The number of arguments present in ARGV. */
ସ୍ଥିର ଅକ୍ଷର **argv;	/* The argument list. */

ସ୍ଥିର bool unary_operator (ଶୂନ୍ୟ);
ସ୍ଥିର bool ଦ୍ୱିମିକ_operator (bool, ଗଣନା binop);
ସ୍ଥିର bool two_arguments (ଶୂନ୍ୟ);
ସ୍ଥିର bool three_arguments (ଶୂନ୍ୟ);
ସ୍ଥିର bool posixtest (ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

ସ୍ଥିର bool ପଦସମଷ୍ଟି (ଶୂନ୍ୟ);
ସ୍ଥିର bool term (ଶୂନ୍ୟ);
ସ୍ଥିର bool and (ଶୂନ୍ୟ);
ସ୍ଥିର bool or (ଶୂନ୍ୟ);

ସ୍ଥିର ଶୂନ୍ୟ beyond (ଶୂନ୍ୟ);

ATTRIBUTE_FORMAT ((printf, 1, 2))
ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
test_syntax_error (ଅକ୍ଷର ଅଚଳ *format, ...)
{
  va_list ap;
  va_start (ap, format);
  verror (0, 0, format, ap);
  test_exit (TEST_FAILURE);
}

/* Increment our position in the argument list.  Check that we're not
   past the end of the argument list.  This check is suppressed if the
   argument is false.  */

ସ୍ଥିର ଶୂନ୍ୟ
advance (bool f)
{
  ++pos;

  ଯଦି (f && pos >= argc)
    beyond ();
}

ସ୍ଥିର ଶୂନ୍ୟ
unary_advance (ଶୂନ୍ୟ)
{
  advance (true);
  ++pos;
}

/*
 * beyond - call when we're beyond the end of the argument list (an
 *	error condition)
 */
ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
beyond (ଶୂନ୍ୟ)
{
  test_syntax_error (_("missing argument after %s"), quote (argv[argc - 1]));
}

/* If the characters pointed to by STRING constitute a valid number,
   return a pointer to the start of the number, skipping any blanks or
   leading '+'.  Otherwise, report an error and exit.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
find_int (ଅକ୍ଷର ଅଚଳ *string)
{
  ଅକ୍ଷର ଅଚଳ *p;
  ଅକ୍ଷର ଅଚଳ *ସଂଖ୍ୟା_start;

  ପାଇଁ (p = string; isspace (to_uchar (*p)); p++)
    ଜାରି;

  ଯଦି (*p == '+')
    {
      p++;
      ସଂଖ୍ୟା_start = p;
    }
  ଅନ୍ୟଥା
    {
      ସଂଖ୍ୟା_start = p;
      p += (*p == '-');
    }

  ଯଦି (c_isdigit (*p++))
    {
      ଯେତେବେଳେ (c_isdigit (*p))
        p++;
      ଯେତେବେଳେ (isspace (to_uchar (*p)))
        p++;
      ଯଦି (!*p)
        ଫେରନ୍ତୁ ସଂଖ୍ୟା_start;
    }

  test_syntax_error (_("invalid integer %s"), quote (string));
}

/* Return the modification time of FILENAME.
   If unsuccessful, return an invalid timestamp that is less
   than all valid timestamps.  */
ସ୍ଥିର ସଂରଚନା timespec
get_mtime (ଅକ୍ଷର ଅଚଳ *filename)
{
  ସଂରଚନା stat finfo;
  ଫେରନ୍ତୁ (stat (filename, &finfo) < 0
          ? make_timespec (TYPE_MINIMUM (time_t), -1)
          : get_stat_mtime (&finfo));
}

/* Return the type of S, one of the test command's binary operators.
   If S is not a binary operator, return -1.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
binop (ଅକ୍ଷର ଅଚଳ *s)
{
  ଫେରନ୍ତୁ (  streq (s, "="  ) ? EQ_STRING_BINOP
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
ସ୍ଥିର bool
term (ଶୂନ୍ୟ)
{
  bool ମୂଲ୍ୟ;
  bool negated = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bop;

  /* Deal with leading 'not's.  */
  ଯେତେବେଳେ (pos < argc && argv[pos][0] == '!' && argv[pos][1] == '\0')
    {
      advance (true);
      negated = !negated;
    }

  ଯଦି (pos >= argc)
    beyond ();

  /* A paren-bracketed argument. */
  ଯଦି (argv[pos][0] == '(' && argv[pos][1] == '\0')
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା nargs;

      advance (true);

      ପାଇଁ (nargs = 1;
           pos + nargs < argc && ! streq (argv[pos + nargs], ")");
           nargs++)
        ଯଦି (nargs == 4)
          {
            nargs = argc - pos;
            ଭାଙ୍ଗ;
          }

      ମୂଲ୍ୟ = posixtest (nargs);
      ଯଦି (!argv[pos])
        test_syntax_error (_("%s expected"), quote (")"));
      ଅନ୍ୟଥା
        ଯଦି (argv[pos][0] != ')' || argv[pos][1])
          test_syntax_error (_("%s expected, found %s"),
                             quote_n (0, ")"), quote_n (1, argv[pos]));
      advance (false);
    }

  /* Are there enough arguments left that this could be dyadic?  */
  ଅନ୍ୟଥା ଯଦି (4 <= argc - pos && streq (argv[pos], "-l")
           && 0 <= (bop = binop (argv[pos + 2])))
    ମୂଲ୍ୟ = ଦ୍ୱିମିକ_operator (true, bop);
  ଅନ୍ୟଥା ଯଦି (3 <= argc - pos
           && 0 <= (bop = binop (argv[pos + 1])))
    ମୂଲ୍ୟ = ଦ୍ୱିମିକ_operator (false, bop);

  /* It might be a switch type argument.  */
  ଅନ୍ୟଥା ଯଦି (argv[pos][0] == '-' && argv[pos][1] && argv[pos][1] != 'a'
           && argv[pos][1] != 'o' && argv[pos][2] == '\0')
    ମୂଲ୍ୟ = unary_operator ();
  ଅନ୍ୟଥା
    {
      ମୂଲ୍ୟ = (argv[pos][0] != '\0');
      advance (false);
    }

  ଫେରନ୍ତୁ negated ^ ମୂଲ୍ୟ;
}

ସ୍ଥିର bool
ଦ୍ୱିମିକ_operator (bool l_is_l, ଗଣନା binop bop)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା op;

  ଯଦି (l_is_l)
    advance (false);
  op = pos + 1;

  /* Is the right integer expression of the form '-l string'? */
  bool r_is_l = op < argc - 2 && streq (argv[op + 1], "-l");
  ଯଦି (r_is_l)
    advance (false);

  pos += 3;

  ବାଛ (bop)
    {
    ସ୍ଥିତି EQ_BINOP: ସ୍ଥିତି GE_BINOP: ସ୍ଥିତି GT_BINOP:
    ସ୍ଥିତି LE_BINOP: ସ୍ଥିତି LT_BINOP: ସ୍ଥିତି NE_BINOP:
      {
        ଅକ୍ଷର lbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ଅକ୍ଷର rbuf[INT_BUFSIZE_BOUND (uintmax_t)];
        ଅକ୍ଷର ଅଚଳ *l = (l_is_l
                         ? umaxtostr (strlen (argv[op - 1]), lbuf)
                         : find_int (argv[op - 1]));
        ଅକ୍ଷର ଅଚଳ *r = (r_is_l
                         ? umaxtostr (strlen (argv[op + 2]), rbuf)
                         : find_int (argv[op + 1]));
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = strintcmp (l, r);
        ବାଛ (+bop)
          {
          ସ୍ଥିତି EQ_BINOP: ଫେରନ୍ତୁ cmp == 0;
          ସ୍ଥିତି GE_BINOP: ଫେରନ୍ତୁ cmp >= 0;
          ସ୍ଥିତି GT_BINOP: ଫେରନ୍ତୁ cmp >  0;
          ସ୍ଥିତି LE_BINOP: ଫେରନ୍ତୁ cmp <= 0;
          ସ୍ଥିତି LT_BINOP: ଫେରନ୍ତୁ cmp <  0;
          ସ୍ଥିତି NE_BINOP: ଫେରନ୍ତୁ cmp != 0;
          }
        unreachable ();
      }

    ସ୍ଥିତି NT_BINOP: ସ୍ଥିତି OT_BINOP:
      {
        ଯଦି (l_is_l | r_is_l)
          test_syntax_error (_("%s does not accept -l"),
                             argv[op]);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = timespec_cmp (get_mtime (argv[op - 1]),
                                get_mtime (argv[op + 1]));
        ଫେରନ୍ତୁ bop == OT_BINOP ? cmp < 0 : cmp > 0;
      }

    ସ୍ଥିତି EF_BINOP:
      ଯଦି (l_is_l | r_is_l)
        test_syntax_error (_("-ef does not accept -l"));
      ଅନ୍ୟଥା
        {
          ସଂରଚନା stat st[2];
          ଫେରନ୍ତୁ (stat (argv[op - 1], &st[0]) == 0
                  && stat (argv[op + 1], &st[1]) == 0
                  && psame_inode (&st[0], &st[1]));
        }

    ସ୍ଥିତି EQ_STRING_BINOP:
    ସ୍ଥିତି NE_STRING_BINOP:
      ଫେରନ୍ତୁ streq (argv[op - 1], argv[op + 1]) == (bop == EQ_STRING_BINOP);

    ସ୍ଥିତି GT_STRING_BINOP:
    ସ୍ଥିତି LT_STRING_BINOP:
      {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = strcoll (argv[op - 1], argv[op + 1]);
        ଫେରନ୍ତୁ bop == LT_STRING_BINOP ? cmp < 0 : cmp > 0;
      }
    }

  /* Not reached.  */
  affirm (false);
}

ସ୍ଥିର bool
unary_operator (ଶୂନ୍ୟ)
{
  ସଂରଚନା stat stat_buf;

  ବାଛ (argv[pos][1])
    {
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      test_syntax_error (_("%s: unary operator expected"), quote (argv[pos]));

      /* All of the following unary operators use unary_advance (), which
         checks to make sure that there is an argument, and then advances
         pos right past it.  This means that pos - 1 is the location of the
         argument. */

    ସ୍ଥିତି 'e':			/* file exists in the file system? */
      unary_advance ();
      ଫେରନ୍ତୁ stat (argv[pos - 1], &stat_buf) == 0;

    ସ୍ଥିତି 'r':			/* file is readable? */
      unary_advance ();
      ଫେରନ୍ତୁ euidaccess (argv[pos - 1], R_OK) == 0;

    ସ୍ଥିତି 'w':			/* File is writable? */
      unary_advance ();
      ଫେରନ୍ତୁ euidaccess (argv[pos - 1], W_OK) == 0;

    ସ୍ଥିତି 'x':			/* File is executable? */
      unary_advance ();
      ଫେରନ୍ତୁ euidaccess (argv[pos - 1], X_OK) == 0;

    ସ୍ଥିତି 'N':  /* File exists and has been modified since it was last read? */
      {
        unary_advance ();
        ଯଦି (stat (argv[pos - 1], &stat_buf) != 0)
          ଫେରନ୍ତୁ false;
        ସଂରଚନା timespec atime = get_stat_atime (&stat_buf);
        ସଂରଚନା timespec mtime = get_stat_mtime (&stat_buf);
        ଫେରନ୍ତୁ (timespec_cmp (mtime, atime) > 0);
      }

    ସ୍ଥିତି 'O':			/* File is owned by you? */
      {
        unary_advance ();
        ଯଦି (stat (argv[pos - 1], &stat_buf) != 0)
          ଫେରନ୍ତୁ false;
        errno = 0;
        uid_t euid = geteuid ();
        uid_t NO_UID = -1;
        ଫେରନ୍ତୁ ! (euid == NO_UID && errno) && euid == stat_buf.st_uid;
      }

    ସ୍ଥିତି 'G':			/* File is owned by your group? */
      {
        unary_advance ();
        ଯଦି (stat (argv[pos - 1], &stat_buf) != 0)
          ଫେରନ୍ତୁ false;
        errno = 0;
        gid_t egid = getegid ();
        gid_t NO_GID = -1;
        ଫେରନ୍ତୁ ! (egid == NO_GID && errno) && egid == stat_buf.st_gid;
      }

    ସ୍ଥିତି 'f':			/* File is a file? */
      unary_advance ();
      /* Under POSIX, -f is true if the given file exists
         and is a regular file. */
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISREG (stat_buf.st_mode));

    ସ୍ଥିତି 'd':			/* File is a directory? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISDIR (stat_buf.st_mode));

    ସ୍ଥିତି 's':			/* File has something in it? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && 0 < stat_buf.st_size);

    ସ୍ଥିତି 'S':			/* File is a socket? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISSOCK (stat_buf.st_mode));

    ସ୍ଥିତି 'c':			/* File is character special? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISCHR (stat_buf.st_mode));

    ସ୍ଥିତି 'b':			/* File is block special? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISBLK (stat_buf.st_mode));

    ସ୍ଥିତି 'p':			/* File is a named pipe? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && S_ISFIFO (stat_buf.st_mode));

    ସ୍ଥିତି 'L':			/* Same as -h  */
      /*FALLTHROUGH*/

    ସ୍ଥିତି 'h':			/* File is a symbolic link? */
      unary_advance ();
      ଫେରନ୍ତୁ issymlink (argv[pos - 1]) == 1;

    ସ୍ଥିତି 'u':			/* File is setuid? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISUID));

    ସ୍ଥିତି 'g':			/* File is setgid? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISGID));

    ସ୍ଥିତି 'k':			/* File has sticky bit set? */
      unary_advance ();
      ଫେରନ୍ତୁ (stat (argv[pos - 1], &stat_buf) == 0
              && (stat_buf.st_mode & S_ISVTX));

    ସ୍ଥିତି 't':			/* File (fd) is a terminal? */
      {
        long ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
        ଅକ୍ଷର ଅଚଳ *arg;
        unary_advance ();
        arg = find_int (argv[pos - 1]);
        errno = 0;
        fd = strtol (arg, ଖାଲି, 10);
        ଫେରନ୍ତୁ (errno != ERANGE && 0 <= fd && fd <= INT_MAX && isatty (fd));
      }

    ସ୍ଥିତି 'n':			/* True if arg has some length. */
      unary_advance ();
      ଫେରନ୍ତୁ argv[pos - 1][0] != 0;

    ସ୍ଥିତି 'z':			/* True if arg has no length. */
      unary_advance ();
      ଫେରନ୍ତୁ argv[pos - 1][0] == '\0';
    }
}

/*
 * and:
 *	term
 *	term '-a' and
 */
ସ୍ଥିର bool
and (ଶୂନ୍ୟ)
{
  bool ମୂଲ୍ୟ = true;

  ଯେତେବେଳେ (true)
    {
      ମୂଲ୍ୟ &= term ();
      ଯଦି (! (pos < argc && streq (argv[pos], "-a")))
        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
      advance (false);
    }
}

/*
 * or:
 *	and
 *	and '-o' or
 */
ସ୍ଥିର bool
or (ଶୂନ୍ୟ)
{
  bool ମୂଲ୍ୟ = false;

  ଯେତେବେଳେ (true)
    {
      ମୂଲ୍ୟ |= and ();
      ଯଦି (! (pos < argc && streq (argv[pos], "-o")))
        ଫେରନ୍ତୁ ମୂଲ୍ୟ;
      advance (false);
    }
}

/*
 * expr:
 *	or
 */
ସ୍ଥିର bool
ପଦସମଷ୍ଟି (ଶୂନ୍ୟ)
{
  ଯଦି (pos >= argc)
    beyond ();

  ଫେରନ୍ତୁ or ();		/* Same with this. */
}

ସ୍ଥିର bool
one_argument (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ argv[pos++][0] != '\0';
}

ସ୍ଥିର bool
two_arguments (ଶୂନ୍ୟ)
{
  bool ମୂଲ୍ୟ;

  ଯଦି (streq (argv[pos], "!"))
    {
      advance (false);
      ମୂଲ୍ୟ = ! one_argument ();
    }
  ଅନ୍ୟଥା ଯଦି (argv[pos][0] == '-'
           && argv[pos][1] != '\0'
           && argv[pos][2] == '\0')
    {
      ମୂଲ୍ୟ = unary_operator ();
    }
  ଅନ୍ୟଥା
    beyond ();
  ଫେରନ୍ତୁ (ମୂଲ୍ୟ);
}

ସ୍ଥିର bool
three_arguments (ଶୂନ୍ୟ)
{
  bool ମୂଲ୍ୟ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bop = binop (argv[pos + 1]);

  ଯଦି (0 <= bop)
    ମୂଲ୍ୟ = ଦ୍ୱିମିକ_operator (false, bop);
  ଅନ୍ୟଥା ଯଦି (streq (argv[pos], "!"))
    {
      advance (true);
      ମୂଲ୍ୟ = !two_arguments ();
    }
  ଅନ୍ୟଥା ଯଦି (streq (argv[pos], "(") && streq (argv[pos + 2], ")"))
    {
      advance (false);
      ମୂଲ୍ୟ = one_argument ();
      advance (false);
    }
  ଅନ୍ୟଥା ଯଦି (streq (argv[pos + 1], "-a") || streq (argv[pos + 1], "-o")
           || streq (argv[pos + 1], ">") || streq (argv[pos + 1], "<"))
    ମୂଲ୍ୟ = ପଦସମଷ୍ଟି ();
  ଅନ୍ୟଥା
    test_syntax_error (_("%s: binary operator expected"),
                       quote (argv[pos + 1]));
  ଫେରନ୍ତୁ (ମୂଲ୍ୟ);
}

/* This is an implementation of a Posix.2 proposal by David Korn. */
ସ୍ଥିର bool
posixtest (ପୂର୍ଣ୍ଣସଂଖ୍ୟା nargs)
{
  bool ମୂଲ୍ୟ;

  ବାଛ (nargs)
    {
      ସ୍ଥିତି 1:
        ମୂଲ୍ୟ = one_argument ();
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 2:
        ମୂଲ୍ୟ = two_arguments ();
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 3:
        ମୂଲ୍ୟ = three_arguments ();
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 4:
        ଯଦି (streq (argv[pos], "!"))
          {
            advance (true);
            ମୂଲ୍ୟ = !three_arguments ();
            ଭାଙ୍ଗ;
          }
        ଯଦି (streq (argv[pos], "(") && streq (argv[pos + 3], ")"))
          {
            advance (false);
            ମୂଲ୍ୟ = two_arguments ();
            advance (false);
            ଭାଙ୍ଗ;
          }
        FALLTHROUGH;
      ସ୍ଥିତି 5:
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        affirm (0 < nargs);
        ମୂଲ୍ୟ = ପଦସମଷ୍ଟି ();
    }

  ଫେରନ୍ତୁ (ମୂଲ୍ୟ);
}

#ଯଦି defined TEST_STANDALONE

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
  -N FILE     FILE exists and has been modified since it was last ପଠ\n\
"));
      oputs (_("\
  -O FILE     FILE exists and is owned by the effective user ID\n\
"));
      oputs (_("\
  -p FILE     FILE exists and is a named pipe\n\
"));
      oputs (_("\
  -r FILE     FILE exists and the user has ପଠ access\n\
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
  -w FILE     FILE exists and the user has ଲିଖ access\n\
"));
      oputs (_("\
  -x FILE     FILE exists and the user has execute (or ସନ୍ଧାନ) access\n\
"));
      fputs (_("\
\n\
Except ପାଇଁ -h and -L, all FILE-related tests dereference symbolic links.\n\
Beware that parentheses need to be escaped (e.g., by backslashes) ପାଇଁ shells.\n\
INTEGER may also be -l STRING, which evaluates to the length of STRING.\n\
"), stdout);
      fputs (_("\
\n\
ଦ୍ୱିମିକ_2 -a and -o are ambiguous.  Use 'test EXPR1 && test EXPR2'\n\
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

#ଯଦି !defined TEST_STANDALONE
# define ମୁଖ୍ୟ test_command
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା margc, ଅକ୍ଷର **margv)
{
  bool ମୂଲ୍ୟ;

#ଯଦି !defined TEST_STANDALONE
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

  code = setjmp (test_exit_buf);

  ଯଦି (code)
    ଫେରନ୍ତୁ (test_error_return);
#ଅନ୍ୟଥା /* TEST_STANDALONE */
  initialize_main (&margc, &margv);
  set_program_name (margv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TEST_FAILURE);
  atexit (close_stdout);
#endif /* TEST_STANDALONE */

  argv = margv;

  ଯଦି (test_mode == TEST_LBRACKET)
    {
      /* Recognize --help or --version, but only when invoked in the
         "[" form, when the last argument is not "]".  Use direct
         parsing, rather than parse_long_options, to avoid accepting
         abbreviations.  POSIX allows "[ --help" and "[ --version" to
         have the usual GNU behavior, but it requires "test --help"
         and "test --version" to exit silently with status 0.  */
      ଯଦି (margc == 2)
        {
          ଯଦି (streq (margv[1], "--help"))
            usage (EXIT_SUCCESS);

          ଯଦି (streq (margv[1], "--version"))
            {
              version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                           (ଅକ୍ଷର *) ଖାଲି);
              test_main_return (EXIT_SUCCESS);
            }
        }
      ଯଦି (margc < 2 || !streq (margv[margc - 1], "]"))
        test_syntax_error (_("missing %s"), quote ("]"));

      --margc;
    }

  argc = margc;
  pos = 1;

  ଯଦି (pos >= argc)
    test_main_return (TEST_FALSE);

  ମୂଲ୍ୟ = posixtest (argc - 1);

  ଯଦି (pos != argc)
    test_syntax_error (_("extra argument %s"), quote (argv[pos]));

  test_main_return (ମୂଲ୍ୟ ? TEST_TRUE : TEST_FALSE);
}
