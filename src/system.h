/* system-dependent definitions for coreutils
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Include this file _after_ system headers if possible.  */

#അന്തര്ഭാവ <attribute.h>

#അന്തര്ഭാവ <alloca.h>

#അന്തര്ഭാവ <ctype.h>

#അന്തര്ഭാവ <sys/stat.h>

/* Commonly used file permission combination.  */
#define MODE_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#എങ്കിൽ HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#അന്തര്ഭാവ <unistd.h>

#അന്തര്ഭാവ <limits.h>

#അന്തര്ഭാവ "pathmax.h"
#ifndef PATH_MAX
# define PATH_MAX 8192
#endif

#അന്തര്ഭാവ "configmake.h"

#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#എങ്കിൽ MAJOR_IN_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#endif
#എങ്കിൽ MAJOR_IN_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#endif
#ifdef major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#endif

#ifndef HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#endif
#undef HAVE_MAJOR

#എങ്കിൽ ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#endif

#അന്തര്ഭാവ <stdckdint.h>
#അന്തര്ഭാവ <stdcountof.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <uchar.h>
#അന്തര്ഭാവ <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#ifndef ENODATA
# define ENODATA (-1)
#endif

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
സ്ഥിതം വരിയിൽ bool
ignorable_traversal_errno (പൂർണ്ണസംഖ്യ errnum) { തിരികെ errnum == ENOENT; }

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
എണ്ണൽ
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#അന്തര്ഭാവ "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
സ്ഥിതം വരിയിൽ ശൂന്യം
initialize_exit_failure (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_FAILURE)
    exit_failure = status;
}

#അന്തര്ഭാവ <fcntl.h>
#ifdef O_PATH
എണ്ണൽ { O_PATHSEARCH = O_PATH };
#അല്ലെങ്കിൽ
എണ്ണൽ { O_PATHSEARCH = O_തിരയൽ };
#endif

#അന്തര്ഭാവ <dirent.h>
#ifndef _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) strlen ((dp)->d_name)
#endif

എണ്ണൽ
{
  NOT_AN_INODE_സംഖ്യ = 0
};

#ifdef D_INO_IN_DIRENT
# define D_INO(dp) (dp)->d_ino
#അല്ലെങ്കിൽ
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_സംഖ്യ
#endif

/* include here for SIZE_MAX.  */
#അന്തര്ഭാവ <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#ifndef initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# അല്ലെങ്കിൽ
#  define initialize_main(ac, av) \
     ചെയ്യുക { _wildcard (ac, av); _response (ac, av); } ആകുമ്പോൾ (0)
# endif
#endif

#അന്തര്ഭാവ "stat-macros.h"

#അന്തര്ഭാവ "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
സ്ഥിതം വരിയിൽ unsigned അക്ഷരം to_uchar (അക്ഷരം ch) { തിരികെ ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_PURE
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
c32isnbspace (char32_t wc)
{
  തിരികെ wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_PURE
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
c32isnewline (char32_t wc)
{
  തിരികെ wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_PURE
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  തിരികെ c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_PURE
സ്ഥിതം വരിയിൽ bool
c32issep (char32_t wc)
{
#എങ്കിൽ defined __GLIBC__
  തിരികെ !! c32isblank (wc);
#അല്ലെങ്കിൽ
  തിരികെ c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#endif
}

#അന്തര്ഭാവ <locale.h>

/* Take care of NLS matters.  */

#അന്തര്ഭാവ "gettext.h"
#എങ്കിൽ ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /* empty */
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /* empty */
#endif

#define _(msgid) gettext (msgid)
#define N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
സ്ഥിതം വരിയിൽ unsigned long പൂർണ്ണസംഖ്യ
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  എണ്ണൽ { PLURAL_REDUCER = 1000000 };
  തിരികെ (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#define STREQ_LEN(a, b, n) (strncmp (a, b, n) == 0) /* n==-1 means unbounded */
#define STRPREFIX(a, b) (strncmp (a, b, strlen (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#define STRNCMP_LIT(s, lit) strncmp (s, "" lit "", വലുപ്പം (lit) - 1)

#എങ്കിൽ !HAVE_DECL_GETLOGIN
അക്ഷരം *getlogin (ശൂന്യം);
#endif

#എങ്കിൽ !HAVE_DECL_TTYNAME
അക്ഷരം *ttyname (പൂർണ്ണസംഖ്യ);
#endif

#എങ്കിൽ !HAVE_DECL_GETEUID
uid_t geteuid (ശൂന്യം);
#endif

#എങ്കിൽ !HAVE_DECL_GETPWUID
ഘടന passwd *getpwuid (uid_t);
#endif

#എങ്കിൽ !HAVE_DECL_GETGRGID
ഘടന group *getgrgid (gid_t);
#endif

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#എങ്കിൽ ! HAVE_SETGROUPS
# എങ്കിൽ HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# എങ്കിൽ HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# എങ്കിൽ HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#endif

#എങ്കിൽ !HAVE_DECL_GETUID
uid_t getuid (ശൂന്യം);
#endif

#അന്തര്ഭാവ "idx.h"
#അന്തര്ഭാവ "xalloc.h"
#അന്തര്ഭാവ "verify.h"
#അന്തര്ഭാവ "unlocked-io.h"
#അന്തര്ഭാവ "same-inode.h"

#അന്തര്ഭാവ "dirname.h"
#അന്തര്ഭാവ "openat.h"

സ്ഥിതം വരിയിൽ bool
dot_or_dotdot (അക്ഷരം സ്ഥിരം *file_name)
{
  എങ്കിൽ (file_name[0] == '.')
    {
      അക്ഷരം sep = file_name[(file_name[1] == '.') + 1];
      തിരികെ (! sep || ISSLASH (sep));
    }
  അല്ലെങ്കിൽ
    തിരികെ false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
സ്ഥിതം വരിയിൽ ഘടന dirent സ്ഥിരം *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  ആകുമ്പോൾ (true)
    {
      ഘടന dirent സ്ഥിരം *dp = readdir (dirp);
      എങ്കിൽ (dp == ശൂന്യ || ! dot_or_dotdot (dp->d_name))
        തിരികെ dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
എണ്ണൽ {
    DS_UNKNOWN = -2,
    DS_EMPTY = -1,
    DS_NONEMPTY = 0,
};
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
directory_status (പൂർണ്ണസംഖ്യ fd_cwd, അക്ഷരം സ്ഥിരം *dir)
{
  DIR *dirp;
  bool no_direntries;
  പൂർണ്ണസംഖ്യ saved_errno;
  പൂർണ്ണസംഖ്യ fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  എങ്കിൽ (fd < 0)
    തിരികെ errno;

  dirp = fdopendir (fd);
  എങ്കിൽ (dirp == ശൂന്യ)
    {
      saved_errno = errno;
      close (fd);
      തിരികെ saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  തിരികെ no_direntries && saved_errno == 0 ? DS_EMPTY : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
എണ്ണൽ
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, ശൂന്യ, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, ശൂന്യ, GETOPT_VERSION_CHAR
#define GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_argument, ശൂന്യ, 'Z'

#define case_GETOPT_HELP_CHAR			\
  അവസ്ഥ GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    നിർത്തുക;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#define USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#define HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#define VERSION_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#അന്തര്ഭാവ "closein.h"
#അന്തര്ഭാവ "closeout.h"

#അന്തര്ഭാവ "version-etc.h"

#അന്തര്ഭാവ "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#define proper_name(x) proper_name_lite (x, x)

#അന്തര്ഭാവ "progname.h"

#define case_GETOPT_VERSION_CHAR(Program_name, Authors)			\
  അവസ്ഥ GETOPT_VERSION_CHAR:						\
    version_etc (stdout, Program_name, PACKAGE_NAME, Version, Authors,	\
                 (അക്ഷരം *) ശൂന്യ);					\
    exit (EXIT_SUCCESS);						\
    നിർത്തുക;

#അന്തര്ഭാവ "minmax.h"
#അന്തര്ഭാവ "intprops.h"

#ifndef SSIZE_MAX
# define SSIZE_MAX TYPE_MAXIMUM (ssize_t)
#endif

#ifndef OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#endif

#ifndef OFF_T_MAX
# define OFF_T_MAX TYPE_MAXIMUM (off_t)
#endif

#ifndef UID_T_MAX
# define UID_T_MAX TYPE_MAXIMUM (uid_t)
#endif

#ifndef GID_T_MAX
# define GID_T_MAX TYPE_MAXIMUM (gid_t)
#endif

#ifndef PID_T_MAX
# define PID_T_MAX TYPE_MAXIMUM (pid_t)
#endif

/* Use this to suppress gcc warnings.  */
#ifdef lint
# define IF_LINT(Code) Code
#അല്ലെങ്കിൽ
# define IF_LINT(Code) /* empty */
#endif

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#ifdef lint
# define main_exit(status) exit (status)
#അല്ലെങ്കിൽ
# define main_exit(status) തിരികെ status
#endif

#ifdef __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#അല്ലെങ്കിൽ
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#endif


#എങ്കിൽ defined strdupa
# define ASSIGN_STRDUPA(DEST, S)		\
  ചെയ്യുക { DEST = strdupa (S); } ആകുമ്പോൾ (0)
#അല്ലെങ്കിൽ
# define ASSIGN_STRDUPA(DEST, S)		\
  ചെയ്യുക						\
    {						\
      അക്ഷരം സ്ഥിരം *s_ = (S);			\
      size_t len_ = strlen (s_) + 1;		\
      അക്ഷരം *tmp_dest_ = alloca (len_);		\
      DEST = memcpy (tmp_dest_, s_, len_);	\
    }						\
  ആകുമ്പോൾ (0)
#endif

#എങ്കിൽ ! HAVE_SYNC
# define sync() /* empty */
#endif

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
സ്ഥിതം വരിയിൽ size_t
gcd (size_t u, size_t v)
{
  ചെയ്യുക
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  ആകുമ്പോൾ (v);

  തിരികെ u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
സ്ഥിതം വരിയിൽ size_t
lcm (size_t u, size_t v)
{
  തിരികെ u * (v / gcd (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

സ്ഥിതം വരിയിൽ ശൂന്യം *
ptr_align (ശൂന്യം സ്ഥിരം *ptr, size_t alignment)
{
  അക്ഷരം സ്ഥിരം *p0 = ptr;
  അക്ഷരം സ്ഥിരം *p1 = p0 + alignment - 1;
  തിരികെ (ശൂന്യം *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_PURE
സ്ഥിതം വരിയിൽ bool
is_nul (ശൂന്യം സ്ഥിരം *buf, size_t length)
{
  സ്ഥിരം unsigned അക്ഷരം *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#എങ്കിൽ 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  unsigned long word;
#അല്ലെങ്കിൽ
  unsigned അക്ഷരം word;
#endif

  എങ്കിൽ (! length)
    തിരികെ true;

  /* Check len bytes not aligned on a word.  */
  ആകുമ്പോൾ (UNLIKELY (length & (വലുപ്പം word - 1)))
    {
      എങ്കിൽ (*p)
        തിരികെ false;
      p++;
      length--;
      എങ്കിൽ (! length)
        തിരികെ true;
   }

  /* Check up to 16 bytes a word at a time.  */
  വേണ്ടി (;;)
    {
      memcpy (&word, p, വലുപ്പം word);
      എങ്കിൽ (word)
        തിരികെ false;
      p += വലുപ്പം word;
      length -= വലുപ്പം word;
      എങ്കിൽ (! length)
        തിരികെ true;
      എങ്കിൽ (UNLIKELY (length & 15) == 0)
        നിർത്തുക;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  തിരികെ memeq (buf, p, length);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#define DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_add (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#define oputs(option) oputs_ (PROGRAM_NAME, option)
സ്ഥിതം വരിയിൽ ശൂന്യം
oputs_ (MAYBE_UNUSED അക്ഷരം സ്ഥിരം *program, അക്ഷരം സ്ഥിരം *option)
{
  സ്ഥിതം പൂർണ്ണസംഖ്യ help_markup =
#എങ്കിൽ defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#അല്ലെങ്കിൽ
    0;   /* Disable.  */
#endif
  എങ്കിൽ (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      അക്ഷരം സ്ഥിരം *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != ശൂന്യ);
    }
  എങ്കിൽ (! help_markup)
    {
      fputs (option, stdout);
      തിരികെ;
    }

  bool double_space = true;
  അക്ഷരം സ്ഥിരം *first_word = option + strspn (option, " \t\n");
  അക്ഷരം സ്ഥിരം *option_text = strchr (option, '-');
  എങ്കിൽ (!option_text)                    /* for dd(1) option syntax.  */
    {
      option_text = first_word;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (option_text != first_word)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      അക്ഷരം സ്ഥിരം *s = first_word;
      size_t spaces = 0;
      ആകുമ്പോൾ (s < option_text && spaces < 2)
        spaces += !!isspace (to_uchar (*s++));
      എങ്കിൽ (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_text = first_word;
          double_space = false;
        }
    }

  size_t anchor_len = strcspn (option_text, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  അക്ഷരം സ്ഥിരം *desc_text = option_text + anchor_len;
  ആകുമ്പോൾ (*desc_text && *desc_text != '\n')
    {
      എങ്കിൽ (*desc_text == '-' && *(desc_text + 1) == '-')
        double_space = false;
      എങ്കിൽ (isspace (to_uchar (*desc_text)))
        {
          എങ്കിൽ (*desc_text == '\t' || isspace (to_uchar (*(desc_text + 1))))
            നിർത്തുക;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          എങ്കിൽ (!double_space && *(desc_text + 1) != '-')
            നിർത്തുക;
        }

      desc_text++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, first_word - option, stdout);

  /* write option text.  */
#ifdef MANUAL_URL
  അക്ഷരം സ്ഥിരം *url_program =   streq (program, "[") ? "test"
                            : streq (program, "dir") ? "ls"
                            : streq (program, "vdir") ? "ls"
                            : streq (program, "b2sum") ? "cksum"
                            : streq (program, "md5sum") ? "cksum"
                            : streq (program, "sha1sum") ? "cksum"
                            : streq (program, "sha224sum") ? "cksum"
                            : streq (program, "sha256sum") ? "cksum"
                            : streq (program, "sha384sum") ? "cksum"
                            : streq (program, "sha512sum") ? "cksum"
                            : program;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  എങ്കിൽ (STREQ_LEN (option_text, "--help", 6)
      || STREQ_LEN (option_text, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      printf ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_program, url_program, (പൂർണ്ണസംഖ്യ) anchor_len, option_text);
    }
  അല്ലെങ്കിൽ
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      printf ("\033]8;;%s#%s%.*s", MANUAL_URL, url_program,
              (പൂർണ്ണസംഖ്യ) anchor_len, option_text);
    }
  fputs ("\033\\", stdout);
#endif
#ifdef BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#endif
  /* first_word != option_text for test(1).  */
  fwrite (first_word, 1, desc_text - first_word, stdout);
#ifdef BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#endif
#ifdef MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#endif

  /* write description.  */
  fputs (desc_text, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#define oprintf(...) oprintf_ (PROGRAM_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((printf, 2, 3))
സ്ഥിതം വരിയിൽ ശൂന്യം
oprintf_ (അക്ഷരം സ്ഥിരം *program, അക്ഷരം സ്ഥിരം *message, ...)
{
  va_list args;
  അക്ഷരം *buf;
  പൂർണ്ണസംഖ്യ buflen = -1;

#എങ്കിൽ defined MANUAL_URL || defined BOLD_MAN_REFS
  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);
#endif

  എങ്കിൽ (buflen < 0)
    {
      va_start (args, message);
      vprintf (message, args);
      va_end (args);
      തിരികെ;
    }

  oputs_ (program, buf);
  സ്വതന്ത്രം (buf);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_stdin_note (ശൂന്യം)
{
  fputs (_("\n\
With no FILE, or when FILE is -, പഠ standard നിവേശം.\n\
"), stdout);
}
സ്ഥിതം വരിയിൽ ശൂന്യം
emit_mandatory_arg_note (ശൂന്യം)
{
  fputs (_("\n\
Mandatory arguments to long options are mandatory വേണ്ടി short options too.\n\
"), stdout);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_size_note (ശൂന്യം)
{
  fputs (_("\n\
The SIZE argument is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers of 1024) or KB,MB,... (powers of 1000).\n\
ദ്വിമാന_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_blocksize_note (അക്ഷരം സ്ഥിരം *program)
{
  printf (_("\n\
Display മൂല്യങ്ങൾ are in units of the first available SIZE from --block-size,\n\
and the %s_BLOCK_SIZE, BLOCK_SIZE and BLOCKSIZE environment variables.\n\
Otherwise, units സ്ഥിരസ്ഥിതി to 1024 bytes (or 512 എങ്കിൽ POSIXLY_CORRECT is set).\n\
"), program);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_update_parameters_note (ശൂന്യം)
{
  fputs (_("\
\n\
UPDATE controls which existing files in the destination are replaced.\n\
'all' is the സ്ഥിരസ്ഥിതി operation when an --update option is not specified,\n\
and results in all existing files in the destination being replaced.\n\
'none' is like the --no-clobber option, in that no files in the\n\
destination are replaced, and skipped files ചെയ്യുക not induce a failure.\n\
'none-fail' also ensures no files are replaced in the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the സ്ഥിരസ്ഥിതി operation when --update is specified, and results\n\
in files being replaced എങ്കിൽ they're older than the corresponding source file.\n\
"), stdout);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_backup_suffix_note (ശൂന്യം)
{
  fputs (_("\
\n\
The backup suffix is '~', unless set with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The version control method may be selected via the --backup option or through\n\
the VERSION_CONTROL environment variable.  Here are the മൂല്യങ്ങൾ:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even എങ്കിൽ --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered എങ്കിൽ numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#define emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (PROGRAM_NAME, default_opt)
സ്ഥിതം വരിയിൽ ശൂന്യം
emit_symlink_recurse_options_ (അക്ഷരം സ്ഥിരം *program, അക്ഷരം സ്ഥിരം *default_opt)
{
      printf (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If more than one is specified, only the final\n\
one takes effect. %s is the സ്ഥിരസ്ഥിതി.\n\
\n\
"), default_opt);
      oputs_ (program, _("\
  -H\n\
         എങ്കിൽ a command വരി argument is a symlink to a directory, traverse it\n\
"));
      oputs_ (program, _("\
  -L\n\
         traverse every symbolic link to a directory encountered\n\
"));
      oputs_ (program, _("\
  -P\n\
         ചെയ്യുക not traverse any symbolic links\n\
\n\
"));
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_exec_status (അക്ഷരം സ്ഥിരം *program)
{
      printf (_("\n\
Exit status:\n\
  125  എങ്കിൽ the %s command itself fails\n\
  126  എങ്കിൽ COMMAND is found but cannot be invoked\n\
  127  എങ്കിൽ COMMAND cannot be found\n\
  -    the exit status of COMMAND otherwise\n\
"), program);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
emit_ancillary_info (അക്ഷരം സ്ഥിരം *program)
{
  ഘടന infomap { അക്ഷരം സ്ഥിരം *program; അക്ഷരം സ്ഥിരം *കണ്ണി; } സ്ഥിരം infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { ശൂന്യ, ശൂന്യ }
  };

  അക്ഷരം സ്ഥിരം *കണ്ണി = program;
  ഘടന infomap സ്ഥിരം *map_prog = infomap;

  ആകുമ്പോൾ (map_prog->program && ! streq (program, map_prog->program))
    map_prog++;

  എങ്കിൽ (map_prog->കണ്ണി)
    കണ്ണി = map_prog->കണ്ണി;

  emit_bug_reporting_address ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  അക്ഷരം സ്ഥിരം *url_program = streq (program, "[") ? "test" : program;
  printf (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_program);
  printf (_("or available locally via: info '(coreutils) %s%s'\n"),
          കണ്ണി, കണ്ണി == program ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#define emit_try_help() \
  ചെയ്യുക \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               program_name); \
    } \
  ആകുമ്പോൾ (0)

#അന്തര്ഭാവ "inttostr.h"

സ്ഥിതം വരിയിൽ അക്ഷരം *
timetostr (time_t t, അക്ഷരം *buf)
{
  തിരികെ (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

സ്ഥിതം വരിയിൽ അക്ഷരം *
bad_cast (അക്ഷരം സ്ഥിരം *s)
{
  തിരികെ (അക്ഷരം *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
സ്ഥിതം വരിയിൽ bool
usable_st_size (ഘടന stat സ്ഥിരം *sb)
{
  തിരികെ (S_ISREG (sb->st_mode) || S_ISLNK (sb->st_mode)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn ശൂന്യം usage (പൂർണ്ണസംഖ്യ status);

#അന്തര്ഭാവ <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#define devmsg(...)			\
  ചെയ്യുക					\
    {					\
      എങ്കിൽ (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  ആകുമ്പോൾ (0)

#define emit_cycle_warning(file_name)	\
  ചെയ്യുക					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part of the cycle:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  ആകുമ്പോൾ (0)

/* exit with a _single_ "write error" diagnostic.  */

സ്ഥിതം വരിയിൽ ശൂന്യം
write_error (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ saved_errno = errno;
  fflush (stdout);    /* Last attempt to write any buffered data.  */
  fpurge (stdout);    /* Ensure nothing buffered that might induce an error. */
  clearerr (stdout);  /* Avoid extraneous diagnostic from close_stdout.  */
  error (EXIT_FAILURE, saved_errno, _("write error"));
}

/* Like stpncpy, but do ensure that the result is NUL-terminated,
   and do not NUL-pad out to LEN.  I.e., when strnlen (src, len) == len,
   this function writes a NUL byte into dest[len].  Thus, the length
   of the destination buffer must be at least LEN + 1.
   The DEST and SRC buffers must not overlap.  */
സ്ഥിതം വരിയിൽ അക്ഷരം *
stzncpy (അക്ഷരം *restrict dest, അക്ഷരം സ്ഥിരം *restrict src, size_t len)
{
  വേണ്ടി (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  തിരികെ dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
സ്ഥിതം വരിയിൽ bool
is_ENOTSUP (പൂർണ്ണസംഖ്യ err)
{
  തിരികെ err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#അന്തര്ഭാവ "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#define quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#define quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#define quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#define quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#define quoteN(x) quotearg_style (get_quoting_style (ശൂന്യ), x)
#define quoteN_mem(x, size)                                     \
  quotearg_style_mem (get_quoting_style (ശൂന്യ), x, size)

#ifdef ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
getenv_quoting_style (ശൂന്യം)
{
  അക്ഷരം സ്ഥിരം *q_style = getenv ("QUOTING_STYLE");
  എങ്കിൽ (!q_style)
    തിരികെ -1;

  പൂർണ്ണസംഖ്യ i = ARGMATCH (q_style, quoting_style_args, quoting_style_vals);
  എങ്കിൽ (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      തിരികെ -1;
    }
  തിരികെ quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
സ്ഥിതം വരിയിൽ ptrdiff_t
x_timestyle_match (അക്ഷരം സ്ഥിരം * style, bool allow_posix,
                   അക്ഷരം സ്ഥിരം *സ്ഥിരം * timestyle_args,
                   അക്ഷരം സ്ഥിരം * timestyle_types,
                   size_t timestyle_types_size,
                   പൂർണ്ണസംഖ്യ fail_status)
{
  ptrdiff_t res = argmatch (style, timestyle_args,
                            timestyle_types,
                            timestyle_types_size);
  എങ്കിൽ (res < 0)
    {
      /* This whole block used to be a simple use of XARGMATCH.
         but that didn't print the "posix-"-prefixed variants or
         the "+"-prefixed format string option upon failure.  */
      argmatch_invalid ("time style", style, res);

      /* The following is a manual expansion of argmatch_valid,
         but with the added "+ ..." description and the [posix-]
         prefixes prepended.  Note that this simplification works
         only because all four existing time_style_types values
         are distinct.  */
      fputs (_("Valid arguments are:\n"), stderr);
      അക്ഷരം സ്ഥിരം *സ്ഥിരം *p = timestyle_args;
      അക്ഷരം സ്ഥിരം *posix_prefix = allow_posix ? "[posix-]" : "";
      ആകുമ്പോൾ (*p)
        fprintf (stderr, "  - %s%s\n", posix_prefix, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_status);
    }

  തിരികെ res;
}
#endif
