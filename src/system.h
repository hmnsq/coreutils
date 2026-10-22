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

#ਅਨ੍ਤਰ੍ਭਾਵ <attribute.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <alloca.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>

/* Commonly used file permission combination.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਹੁਲਕ_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#ਜੇ HAVE_SYS_PARAM_H
# include <sys/param.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "pathmax.h"
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PATH_MAX
# define PATH_MAX 8192
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "configmake.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#ਜੇ MAJOR_ਵਿੱਚ_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#ਜੇ_ਅੰਤ
#ਜੇ MAJOR_ਵਿੱਚ_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HAVE_MAJOR

#ਜੇ ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <stdckdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdcountof.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <uchar.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ENODATA
# define ENODATA (-1)
#ਜੇ_ਅੰਤ

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ignorable_traversal_errno (ਪੂਰਨ errnum) { ਵਾਪਸ errnum == ENOENT; }

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
ਗਿਣਤੀ
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#ਅਨ੍ਤਰ੍ਭਾਵ "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
initialize_exit_failure (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_FAILURE)
    exit_failure = ਸਥਿਤੀ;
}

#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ O_ਰਸਤਾ
ਗਿਣਤੀ { O_PATHSEARCH = O_ਰਸਤਾ };
#ਨਹੀਂਤਾਂ
ਗਿਣਤੀ { O_PATHSEARCH = O_ਖੋਜ };
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <dirent.h>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) ਲੜੀ_ਲੰਬਾਈ ((dp)->d_name)
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ
{
  NOT_AN_INODE_ਸੰਖਿਆ = 0
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ D_INO_ਵਿੱਚ_DIRENT
# define D_INO(dp) (dp)->d_ino
#ਨਹੀਂਤਾਂ
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_ਸੰਖਿਆ
#ਜੇ_ਅੰਤ

/* include here for SIZE_MAX.  */
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# ਨਹੀਂਤਾਂ
#  define initialize_main(ac, av) \
     ਕਰੋ { _wildcard (ac, av); _response (ac, av); } ਜਦੋਂ (0)
# endif
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "stat-macros.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਮੰਜ਼ਿਲ_uchar (ਅੱਖਰ ch) { ਵਾਪਸ ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
c32isnbspace (char32_t wc)
{
  ਵਾਪਸ wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
c32isnewline (char32_t wc)
{
  ਵਾਪਸ wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  ਵਾਪਸ c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
c32issep (char32_t wc)
{
#ਜੇ defined __GLIBC__
  ਵਾਪਸ !! c32isblank (wc);
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#ਜੇ_ਅੰਤ
}

#ਅਨ੍ਤਰ੍ਭਾਵ <locale.h>

/* Take care of NLS matters.  */

#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਜੇ ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /*ਖਾਲੀ_ਥਾਵਾਂ*/
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /*ਖਾਲੀ_ਥਾਵਾਂ*/
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _(msgid) gettext (msgid)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  ਗਿਣਤੀ { PLURAL_REDUCER = 1000000 };
  ਵਾਪਸ (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STREQ_LEN(a, b, n) (ਲੜੀ_ਤੁਲਨਾ (a, b, n) == 0) /* n==-1 means unbounded */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRPREFIX(a, b) (ਲੜੀ_ਤੁਲਨਾ (a, b, ਲੜੀ_ਲੰਬਾਈ (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRNCMP_LIT(s, lit) ਲੜੀ_ਤੁਲਨਾ (s, "" lit "", ਆਕਾਰ (lit) - 1)

#ਜੇ !HAVE_DECL_GETLOGIN
ਅੱਖਰ *getlogin (ਖਾਲੀ);
#ਜੇ_ਅੰਤ

#ਜੇ !HAVE_DECL_TTYNAME
ਅੱਖਰ *ttyname (ਪੂਰਨ);
#ਜੇ_ਅੰਤ

#ਜੇ !HAVE_DECL_GETEUID
uid_t geteuid (ਖਾਲੀ);
#ਜੇ_ਅੰਤ

#ਜੇ !HAVE_DECL_GETPWUID
ਬਣਤਰ passwd *getpwuid (uid_t);
#ਜੇ_ਅੰਤ

#ਜੇ !HAVE_DECL_GETGRGID
ਬਣਤਰ ਸਮੂਹ *getgrgid (gid_t);
#ਜੇ_ਅੰਤ

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#ਜੇ ! HAVE_SETGROUPS
# ਜੇ HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# ਜੇ HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# ਜੇ HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#ਜੇ_ਅੰਤ

#ਜੇ !HAVE_DECL_GETUID
uid_t getuid (ਖਾਲੀ);
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "idx.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "verify.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unlocked-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "same-inode.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "dirname.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "openat.h"

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
dot_or_dotdot (ਅੱਖਰ ਅਚਲ *file_name)
{
  ਜੇ (file_name[0] == '.')
    {
      ਅੱਖਰ sep = file_name[(file_name[1] == '.') + 1];
      ਵਾਪਸ (! sep || ISSLASH (sep));
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ dirent ਅਚਲ *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  ਜਦੋਂ (true)
    {
      ਬਣਤਰ dirent ਅਚਲ *dp = readdir (dirp);
      ਜੇ (dp == ਨਿਰਾ || ! dot_or_dotdot (dp->d_name))
        ਵਾਪਸ dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
ਗਿਣਤੀ {
    DS_UNKNOWN = -2,
    DS_ਖਾਲੀ_ਥਾਵਾਂ = -1,
    DS_NONEMPTY = 0,
};
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
directory_ਸਥਿਤੀ (ਪੂਰਨ fd_cwd, ਅੱਖਰ ਅਚਲ *dir)
{
  DIR *dirp;
  bool no_direntries;
  ਪੂਰਨ saved_errno;
  ਪੂਰਨ fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  ਜੇ (fd < 0)
    ਵਾਪਸ errno;

  dirp = fdopendir (fd);
  ਜੇ (dirp == ਨਿਰਾ)
    {
      saved_errno = errno;
      close (fd);
      ਵਾਪਸ saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  ਵਾਪਸ no_direntries && saved_errno == 0 ? DS_ਖਾਲੀ_ਥਾਵਾਂ : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
ਗਿਣਤੀ
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_ਸੰਸਕਰਣ_CHAR = (CHAR_MIN - 3)
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GETOPT_HELP_OPTION_DECL \
  "help", no_ਦਲੀਲ, ਨਿਰਾ, GETOPT_HELP_CHAR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GETOPT_ਸੰਸਕਰਣ_OPTION_DECL \
  "version", no_ਦਲੀਲ, ਨਿਰਾ, GETOPT_ਸੰਸਕਰਣ_CHAR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_ਦਲੀਲ, ਨਿਰਾ, 'Z'

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ case_GETOPT_HELP_CHAR			\
  ਹਾਲਤ GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    ਰੋਕੋ;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੰਸਕਰਣ_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#ਅਨ੍ਤਰ੍ਭਾਵ "closein.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "closeout.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "version-etc.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ proper_name(x) proper_name_lite (x, x)

#ਅਨ੍ਤਰ੍ਭਾਵ "progname.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ case_GETOPT_ਸੰਸਕਰਣ_CHAR(ਪ੍ਰੋਗਰਾਮ_name_2, Authors)			\
  ਹਾਲਤ GETOPT_ਸੰਸਕਰਣ_CHAR:						\
    ਸੰਸਕਰਣ_etc (stdout, ਪ੍ਰੋਗਰਾਮ_name_2, PACKAGE_NAME, ਸੰਸਕਰਣ, Authors,	\
                 (ਅੱਖਰ *) ਨਿਰਾ);					\
    ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);						\
    ਰੋਕੋ;

#ਅਨ੍ਤਰ੍ਭਾਵ "minmax.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "intprops.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SSIZE_MAX
# define SSIZE_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (ssize_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OFF_T_MAX
# define OFF_T_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (off_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UID_T_MAX
# define UID_T_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (uid_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GID_T_MAX
# define GID_T_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (gid_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PID_T_MAX
# define PID_T_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (pid_t)
#ਜੇ_ਅੰਤ

/* Use this to suppress gcc warnings.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ lint
# define IF_LINT(ਕੋਡ_2) ਕੋਡ_2
#ਨਹੀਂਤਾਂ
# define IF_LINT(ਕੋਡ_2) /*ਖਾਲੀ_ਥਾਵਾਂ*/
#ਜੇ_ਅੰਤ

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ lint
# define main_exit(ਸਥਿਤੀ) ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ)
#ਨਹੀਂਤਾਂ
# define main_exit(ਸਥਿਤੀ) ਵਾਪਸ ਸਥਿਤੀ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#ਨਹੀਂਤਾਂ
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#ਜੇ_ਅੰਤ


#ਜੇ defined strdupa
# define ਸੌਂਪੋ_STRDUPA(DEST, S)		\
  ਕਰੋ { DEST = strdupa (S); } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
# define ਸੌਂਪੋ_STRDUPA(DEST, S)		\
  ਕਰੋ						\
    {						\
      ਅੱਖਰ ਅਚਲ *s_ = (S);			\
      size_t len_ = ਲੜੀ_ਲੰਬਾਈ (s_) + 1;		\
      ਅੱਖਰ *tmp_dest_ = alloca (len_);		\
      DEST = ਸਮ੍ਰਿਤੀ_ਨਕਲ (tmp_dest_, s_, len_);	\
    }						\
  ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ ! HAVE_SYNC
# define sync() /*ਖਾਲੀ_ਥਾਵਾਂ*/
#ਜੇ_ਅੰਤ

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ (size_t u, size_t v)
{
  ਕਰੋ
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  ਜਦੋਂ (v);

  ਵਾਪਸ u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t
lcm (size_t u, size_t v)
{
  ਵਾਪਸ u * (v / ਮਹੱਤਮ_ਸਾਂਝਾ_ਭਾਜਕ (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ *
ptr_align (ਖਾਲੀ ਅਚਲ *ptr, size_t alignment)
{
  ਅੱਖਰ ਅਚਲ *p0 = ptr;
  ਅੱਖਰ ਅਚਲ *p1 = p0 + alignment - 1;
  ਵਾਪਸ (ਖਾਲੀ *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_nul (ਖਾਲੀ ਅਚਲ *buf, size_t ਲੰਮਾਈ)
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#ਜੇ 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸ਼ਬਦ_2;
#ਨਹੀਂਤਾਂ
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸ਼ਬਦ_2;
#ਜੇ_ਅੰਤ

  ਜੇ (! ਲੰਮਾਈ)
    ਵਾਪਸ true;

  /* Check len bytes not aligned on a word.  */
  ਜਦੋਂ (UNLIKELY (ਲੰਮਾਈ & (ਆਕਾਰ ਸ਼ਬਦ_2 - 1)))
    {
      ਜੇ (*p)
        ਵਾਪਸ false;
      p++;
      ਲੰਮਾਈ--;
      ਜੇ (! ਲੰਮਾਈ)
        ਵਾਪਸ true;
   }

  /* Check up to 16 bytes a word at a time.  */
  ਲਈ (;;)
    {
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (&ਸ਼ਬਦ_2, p, ਆਕਾਰ ਸ਼ਬਦ_2);
      ਜੇ (ਸ਼ਬਦ_2)
        ਵਾਪਸ false;
      p += ਆਕਾਰ ਸ਼ਬਦ_2;
      ਲੰਮਾਈ -= ਆਕਾਰ ਸ਼ਬਦ_2;
      ਜੇ (! ਲੰਮਾਈ)
        ਵਾਪਸ true;
      ਜੇ (UNLIKELY (ਲੰਮਾਈ & 15) == 0)
        ਰੋਕੋ;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  ਵਾਪਸ memeq (buf, p, ਲੰਮਾਈ);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_ਜੋੜੋ (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ oputs(option) oputs_ (ਪ੍ਰੋਗਰਾਮ_NAME, option)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
oputs_ (MAYBE_ਅਣਵਰਤਿਆ ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ, ਅੱਖਰ ਅਚਲ *option)
{
  ਸਥਿਰ ਪੂਰਨ help_markup =
#ਜੇ defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#ਨਹੀਂਤਾਂ
    0;   /* Disable.  */
#ਜੇ_ਅੰਤ
  ਜੇ (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      ਅੱਖਰ ਅਚਲ *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != ਨਿਰਾ);
    }
  ਜੇ (! help_markup)
    {
      fputs (option, stdout);
      ਵਾਪਸ;
    }

  bool double_space = true;
  ਅੱਖਰ ਅਚਲ *ਪਹਿਲਾ_ਸ਼ਬਦ = option + strspn (option, " \t\n");
  ਅੱਖਰ ਅਚਲ *option_ਪਾਠ = strchr (option, '-');
  ਜੇ (!option_ਪਾਠ)                    /* for dd(1) option syntax.  */
    {
      option_ਪਾਠ = ਪਹਿਲਾ_ਸ਼ਬਦ;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  ਨਹੀਂਤਾਂ ਜੇ (option_ਪਾਠ != ਪਹਿਲਾ_ਸ਼ਬਦ)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      ਅੱਖਰ ਅਚਲ *s = ਪਹਿਲਾ_ਸ਼ਬਦ;
      size_t spaces = 0;
      ਜਦੋਂ (s < option_ਪਾਠ && spaces < 2)
        spaces += !!ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*s++));
      ਜੇ (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_ਪਾਠ = ਪਹਿਲਾ_ਸ਼ਬਦ;
          double_space = false;
        }
    }

  size_t anchor_len = strcspn (option_ਪਾਠ, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  ਅੱਖਰ ਅਚਲ *desc_ਪਾਠ = option_ਪਾਠ + anchor_len;
  ਜਦੋਂ (*desc_ਪਾਠ && *desc_ਪਾਠ != '\n')
    {
      ਜੇ (*desc_ਪਾਠ == '-' && *(desc_ਪਾਠ + 1) == '-')
        double_space = false;
      ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*desc_ਪਾਠ)))
        {
          ਜੇ (*desc_ਪਾਠ == '\t' || ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*(desc_ਪਾਠ + 1))))
            ਰੋਕੋ;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          ਜੇ (!double_space && *(desc_ਪਾਠ + 1) != '-')
            ਰੋਕੋ;
        }

      desc_ਪਾਠ++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, ਪਹਿਲਾ_ਸ਼ਬਦ - option, stdout);

  /* write option text.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MANUAL_URL
  ਅੱਖਰ ਅਚਲ *url_ਪ੍ਰੋਗਰਾਮ =   streq (ਪ੍ਰੋਗਰਾਮ, "[") ? "test"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "dir") ? "ls"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "vdir") ? "ls"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "b2sum") ? "cksum"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "md5sum") ? "cksum"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "sha1sum") ? "cksum"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "sha224sum") ? "cksum"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "sha256sum") ? "cksum"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "sha384sum") ? "cksum"
                            : streq (ਪ੍ਰੋਗਰਾਮ, "sha512sum") ? "cksum"
                            : ਪ੍ਰੋਗਰਾਮ;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  ਜੇ (STREQ_LEN (option_ਪਾਠ, "--help", 6)
      || STREQ_LEN (option_ਪਾਠ, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      ਲਿਖੋ ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_ਪ੍ਰੋਗਰਾਮ, url_ਪ੍ਰੋਗਰਾਮ, (ਪੂਰਨ) anchor_len, option_ਪਾਠ);
    }
  ਨਹੀਂਤਾਂ
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      ਲਿਖੋ ("\033]8;;%s#%s%.*s", MANUAL_URL, url_ਪ੍ਰੋਗਰਾਮ,
              (ਪੂਰਨ) anchor_len, option_ਪਾਠ);
    }
  fputs ("\033\\", stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#ਜੇ_ਅੰਤ
  /* first_word != option_text for test(1).  */
  fwrite (ਪਹਿਲਾ_ਸ਼ਬਦ, 1, desc_ਪਾਠ - ਪਹਿਲਾ_ਸ਼ਬਦ, stdout);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#ਜੇ_ਅੰਤ

  /* write description.  */
  fputs (desc_ਪਾਠ, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ oprintf(...) oprintf_ (ਪ੍ਰੋਗਰਾਮ_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((ਲਿਖੋ, 2, 3))
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
oprintf_ (ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ, ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_2, ...)
{
  va_list ਦਲੀਲਾਂ;
  ਅੱਖਰ *buf;
  ਪੂਰਨ buflen = -1;

#ਜੇ defined MANUAL_URL || defined BOLD_MAN_REFS
  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ, ਸੁਨੇਹਾ_2);
  buflen = vasprintf (&buf, ਸੁਨੇਹਾ_2, ਦਲੀਲਾਂ);
  va_ਸਿਰਾ (ਦਲੀਲਾਂ);
#ਜੇ_ਅੰਤ

  ਜੇ (buflen < 0)
    {
      va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ, ਸੁਨੇਹਾ_2);
      vprintf (ਸੁਨੇਹਾ_2, ਦਲੀਲਾਂ);
      va_ਸਿਰਾ (ਦਲੀਲਾਂ);
      ਵਾਪਸ;
    }

  oputs_ (ਪ੍ਰੋਗਰਾਮ, buf);
  ਮੁਕਤ (buf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_stdin_note (ਖਾਲੀ)
{
  fputs (_("\n\
With no FILE, or when FILE is -, ਪਠ standard ਨਿਵੇਸ਼.\n\
"), stdout);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_mandatory_arg_note (ਖਾਲੀ)
{
  fputs (_("\n\
Mandatory arguments ਮੰਜ਼ਿਲ ਲੰਮਾ options are mandatory ਲਈ ਛੋਟਾ options too.\n\
"), stdout);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_ਮਾਪ_note (ਖਾਲੀ)
{
  fputs (_("\n\
The ਮਾਪ_2 ਦਲੀਲ is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers ਦਾ 1024) or KB,MB,... (powers ਦਾ 1000).\n\
ਦੁਆਧਾਰੀ_2 prefixes can be ਵਰਤਿਆ, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_blocksize_note (ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ)
{
  ਲਿਖੋ (_("\n\
Display ਮੁੱਲਾਂ are ਵਿੱਚ units ਦਾ the ਪਹਿਲਾ ਉਪਲਬਧ ਮਾਪ_2 from --ਖੰਡ-ਮਾਪ,\n\
and the %s_ਖੰਡ_ਮਾਪ, ਖੰਡ_ਮਾਪ_2 and BLOCKSIZE environment variables.\n\
Otherwise, units ਮੂਲ ਮੰਜ਼ਿਲ 1024 ਬਾਈਟਾਂ (or 512 ਜੇ POSIXLY_CORRECT is ਸੈੱਟ_2).\n\
"), ਪ੍ਰੋਗਰਾਮ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_update_parameters_note (ਖਾਲੀ)
{
  fputs (_("\
\n\
UPDATE controls which existing files ਵਿੱਚ the destination are replaced.\n\
'all' is the ਮੂਲ operation when an --update option is not specified,\n\
and results ਵਿੱਚ all existing files ਵਿੱਚ the destination being replaced.\n\
'none' is like the --no-clobber option, ਵਿੱਚ that no files ਵਿੱਚ the\n\
destination are replaced, and skipped files ਕਰੋ not induce a failure.\n\
'none-fail' also ensures no files are replaced ਵਿੱਚ the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the ਮੂਲ operation when --update is specified, and results\n\
ਵਿੱਚ files being replaced ਜੇ they're older than the corresponding ਸਰੋਤ_2 file.\n\
"), stdout);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_backup_suffix_note (ਖਾਲੀ)
{
  fputs (_("\
\n\
The backup suffix is '~', unless ਸੈੱਟ_2 with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The ਸੰਸਕਰਣ_3 control ਵਿਧੀ_2 may be selected ਰਾਹੀਂ the --backup option or through\n\
the ਸੰਸਕਰਣ_CONTROL environment variable.  ਇੱਥੇ_2 are the ਮੁੱਲਾਂ:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even ਜੇ --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered ਜੇ numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (ਪ੍ਰੋਗਰਾਮ_NAME, default_opt)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_symlink_recurse_options_ (ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ, ਅੱਖਰ ਅਚਲ *default_opt)
{
      ਲਿਖੋ (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If ਹੋਰ than one is specified, only the final\n\
one takes effect. %s is the ਮੂਲ.\n\
\n\
"), default_opt);
      oputs_ (ਪ੍ਰੋਗਰਾਮ, _("\
  -H\n\
         ਜੇ a command ਸਤਰ ਦਲੀਲ is a symlink ਮੰਜ਼ਿਲ a directory, traverse it\n\
"));
      oputs_ (ਪ੍ਰੋਗਰਾਮ, _("\
  -L\n\
         traverse every symbolic link ਮੰਜ਼ਿਲ a directory encountered\n\
"));
      oputs_ (ਪ੍ਰੋਗਰਾਮ, _("\
  -P\n\
         ਕਰੋ not traverse any symbolic links\n\
\n\
"));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_exec_ਸਥਿਤੀ (ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ)
{
      ਲਿਖੋ (_("\n\
Exit ਸਥਿਤੀ:\n\
  125  ਜੇ the %s command itself fails\n\
  126  ਜੇ COMMAND is ਮਿਲਿਆ but cannot be invoked\n\
  127  ਜੇ COMMAND cannot be ਮਿਲਿਆ\n\
  -    the ਬਾਹਰ_ਨਿਕਲੋ ਸਥਿਤੀ ਦਾ COMMAND otherwise\n\
"), ਪ੍ਰੋਗਰਾਮ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
emit_ancillary_info (ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ)
{
  ਬਣਤਰ infomap { ਅੱਖਰ ਅਚਲ *ਪ੍ਰੋਗਰਾਮ; ਅੱਖਰ ਅਚਲ *ਗੰਢ; } ਅਚਲ infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { ਨਿਰਾ, ਨਿਰਾ }
  };

  ਅੱਖਰ ਅਚਲ *ਗੰਢ = ਪ੍ਰੋਗਰਾਮ;
  ਬਣਤਰ infomap ਅਚਲ *map_prog = infomap;

  ਜਦੋਂ (map_prog->ਪ੍ਰੋਗਰਾਮ && ! streq (ਪ੍ਰੋਗਰਾਮ, map_prog->ਪ੍ਰੋਗਰਾਮ))
    map_prog++;

  ਜੇ (map_prog->ਗੰਢ)
    ਗੰਢ = map_prog->ਗੰਢ;

  emit_bug_reporting_ਪਤਾ ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  ਅੱਖਰ ਅਚਲ *url_ਪ੍ਰੋਗਰਾਮ = streq (ਪ੍ਰੋਗਰਾਮ, "[") ? "test" : ਪ੍ਰੋਗਰਾਮ;
  ਲਿਖੋ (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_ਪ੍ਰੋਗਰਾਮ);
  ਲਿਖੋ (_("or available locally via: info '(coreutils) %s%s'\n"),
          ਗੰਢ, ਗੰਢ == ਪ੍ਰੋਗਰਾਮ ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ emit_try_help() \
  ਕਰੋ \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               ਪ੍ਰੋਗਰਾਮ_name); \
    } \
  ਜਦੋਂ (0)

#ਅਨ੍ਤਰ੍ਭਾਵ "inttostr.h"

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
timetostr (time_t t, ਅੱਖਰ *buf)
{
  ਵਾਪਸ (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
bad_cast (ਅੱਖਰ ਅਚਲ *s)
{
  ਵਾਪਸ (ਅੱਖਰ *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
usable_st_ਮਾਪ (ਬਣਤਰ stat ਅਚਲ *sb)
{
  ਵਾਪਸ (S_ISREG (sb->st_ਬਹੁਲਕ) || S_ISLNK (sb->st_ਬਹੁਲਕ)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn ਖਾਲੀ usage (ਪੂਰਨ ਸਥਿਤੀ);

#ਅਨ੍ਤਰ੍ਭਾਵ <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ devmsg(...)			\
  ਕਰੋ					\
    {					\
      ਜੇ (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ emit_ਚੱਕਰ_warning(file_name)	\
  ਕਰੋ					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part ਦਾ the ਚੱਕਰ:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  ਜਦੋਂ (0)

/* exit with a _single_ "write error" diagnostic.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
write_error (ਖਾਲੀ)
{
  ਪੂਰਨ saved_errno = errno;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ *
stzncpy (ਅੱਖਰ *ਸੀਮਿਤ dest, ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ src, size_t len)
{
  ਲਈ (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  ਵਾਪਸ dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_ENOTSUP (ਪੂਰਨ err)
{
  ਵਾਪਸ err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#ਅਨ੍ਤਰ੍ਭਾਵ "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ quoteN(x) quotearg_style (get_quoting_style (ਨਿਰਾ), x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ quoteN_mem(x, ਮਾਪ)                                     \
  quotearg_style_mem (get_quoting_style (ਨਿਰਾ), x, ਮਾਪ)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
getenv_quoting_style (ਖਾਲੀ)
{
  ਅੱਖਰ ਅਚਲ *q_style = getenv ("QUOTING_STYLE");
  ਜੇ (!q_style)
    ਵਾਪਸ -1;

  ਪੂਰਨ i = ARGMATCH (q_style, quoting_style_ਦਲੀਲਾਂ, quoting_style_vals);
  ਜੇ (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      ਵਾਪਸ -1;
    }
  ਵਾਪਸ quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ptrdiff_t
x_timestyle_ਮੇਲ (ਅੱਖਰ ਅਚਲ * style, bool allow_posix,
                   ਅੱਖਰ ਅਚਲ *ਅਚਲ * timestyle_ਦਲੀਲਾਂ,
                   ਅੱਖਰ ਅਚਲ * timestyle_types,
                   size_t timestyle_types_ਮਾਪ,
                   ਪੂਰਨ fail_ਸਥਿਤੀ)
{
  ptrdiff_t res = argmatch (style, timestyle_ਦਲੀਲਾਂ,
                            timestyle_types,
                            timestyle_types_ਮਾਪ);
  ਜੇ (res < 0)
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
      ਅੱਖਰ ਅਚਲ *ਅਚਲ *p = timestyle_ਦਲੀਲਾਂ;
      ਅੱਖਰ ਅਚਲ *posix_ਅਗੇਤਰ = allow_posix ? "[posix-]" : "";
      ਜਦੋਂ (*p)
        fprintf (stderr, "  - %s%s\n", posix_ਅਗੇਤਰ, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_ਸਥਿਤੀ);
    }

  ਵਾਪਸ res;
}
#ਜੇ_ਅੰਤ
