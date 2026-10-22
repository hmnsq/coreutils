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

#ಅನ್ತರ್ಭಾವ <attribute.h>

#ಅನ್ತರ್ಭಾವ <alloca.h>

#ಅನ್ತರ್ಭಾವ <ctype.h>

#ಅನ್ತರ್ಭಾವ <sys/stat.h>

/* Commonly used file permission combination.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಬಹುಲಕ_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#ಆದರೆ HAVE_SYS_PARAM_H
# include <sys/param.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <unistd.h>

#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "pathmax.h"
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PATH_MAX
# define PATH_MAX 8192
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "configmake.h"

#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#ಆದರೆ MAJOR_ಒಳ_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ MAJOR_ಒಳ_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು HAVE_MAJOR

#ಆದರೆ ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <stdckdint.h>
#ಅನ್ತರ್ಭಾವ <stdcountof.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <uchar.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ENODATA
# define ENODATA (-1)
#ಆದರೆ_ಅಂತ್ಯ

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ignorable_traversal_errno (ಪೂರ್ಣಾಂಕ errnum) { ಹಿಂತಿರುಗಿ errnum == ENOENT; }

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
ಎಣಿಕೆ
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#ಅನ್ತರ್ಭಾವ "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
initialize_exit_failure (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_FAILURE)
    exit_failure = ಸ್ಥಿತಿ_2;
}

#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ O_ಮಾರ್ಗ
ಎಣಿಕೆ { O_PATHSEARCH = O_ಮಾರ್ಗ };
#ಇಲ್ಲದಿದ್ದರೆ
ಎಣಿಕೆ { O_PATHSEARCH = O_ಹುಡುಕು };
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <dirent.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ((dp)->d_name)
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ
{
  NOT_AN_INODE_ಸಂಖ್ಯೆ = 0
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ D_INO_ಒಳ_DIRENT
# define D_INO(dp) (dp)->d_ino
#ಇಲ್ಲದಿದ್ದರೆ
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_ಸಂಖ್ಯೆ
#ಆದರೆ_ಅಂತ್ಯ

/* include here for SIZE_MAX.  */
#ಅನ್ತರ್ಭಾವ <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# ಇಲ್ಲದಿದ್ದರೆ
#  define initialize_main(ac, av) \
     ಮಾಡು { _wildcard (ac, av); _response (ac, av); } ಆಗ (0)
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "stat-macros.h"

#ಅನ್ತರ್ಭಾವ "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಗಮ್ಯ_uchar (ಅಕ್ಷರ ch) { ಹಿಂತಿರುಗಿ ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
c32isnbspace (char32_t wc)
{
  ಹಿಂತಿರುಗಿ wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
c32isnewline (char32_t wc)
{
  ಹಿಂತಿರುಗಿ wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  ಹಿಂತಿರುಗಿ c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
c32issep (char32_t wc)
{
#ಆದರೆ defined __GLIBC__
  ಹಿಂತಿರುಗಿ !! c32isblank (wc);
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#ಆದರೆ_ಅಂತ್ಯ
}

#ಅನ್ತರ್ಭಾವ <locale.h>

/* Take care of NLS matters.  */

#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಆದರೆ ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /*ಖಾಲಿಗಳು*/
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /*ಖಾಲಿಗಳು*/
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು _(msgid) gettext (msgid)
#ವ್ಯಾಖ್ಯಾನಿಸು N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  ಎಣಿಕೆ { PLURAL_REDUCER = 1000000 };
  ಹಿಂತಿರುಗಿ (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#ವ್ಯಾಖ್ಯಾನಿಸು STREQ_LEN(a, b, n) (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (a, b, n) == 0) /* n==-1 means unbounded */
#ವ್ಯಾಖ್ಯಾನಿಸು STRPREFIX(a, b) (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (a, b, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#ವ್ಯಾಖ್ಯಾನಿಸು STRNCMP_LIT(s, lit) ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (s, "" lit "", ಗಾತ್ರ (lit) - 1)

#ಆದರೆ !HAVE_DECL_GETLOGIN
ಅಕ್ಷರ *getlogin (ಶೂನ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HAVE_DECL_TTYNAME
ಅಕ್ಷರ *ttyname (ಪೂರ್ಣಾಂಕ);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HAVE_DECL_GETEUID
uid_t geteuid (ಶೂನ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HAVE_DECL_GETPWUID
ರಚನೆ passwd *getpwuid (uid_t);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HAVE_DECL_GETGRGID
ರಚನೆ ಗುಂಪು *getgrgid (gid_t);
#ಆದರೆ_ಅಂತ್ಯ

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#ಆದರೆ ! HAVE_SETGROUPS
# ಆದರೆ HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# ಆದರೆ HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# ಆದರೆ HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !HAVE_DECL_GETUID
uid_t getuid (ಶೂನ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "idx.h"
#ಅನ್ತರ್ಭಾವ "xalloc.h"
#ಅನ್ತರ್ಭಾವ "verify.h"
#ಅನ್ತರ್ಭಾವ "unlocked-io.h"
#ಅನ್ತರ್ಭಾವ "same-inode.h"

#ಅನ್ತರ್ಭಾವ "dirname.h"
#ಅನ್ತರ್ಭಾವ "openat.h"

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
dot_or_dotdot (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_name)
{
  ಆದರೆ (file_name[0] == '.')
    {
      ಅಕ್ಷರ sep = file_name[(file_name[1] == '.') + 1];
      ಹಿಂತಿರುಗಿ (! sep || ISSLASH (sep));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ dirent ಸ್ಥಿರಾಂಕ *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  ಆಗ (true)
    {
      ರಚನೆ dirent ಸ್ಥಿರಾಂಕ *dp = readdir (dirp);
      ಆದರೆ (dp == ಖಾಲಿ || ! dot_or_dotdot (dp->d_name))
        ಹಿಂತಿರುಗಿ dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
ಎಣಿಕೆ {
    DS_UNKNOWN = -2,
    DS_ಖಾಲಿಗಳು = -1,
    DS_NONEMPTY = 0,
};
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
directory_ಸ್ಥಿತಿ (ಪೂರ್ಣಾಂಕ fd_cwd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir)
{
  DIR *dirp;
  bool no_direntries;
  ಪೂರ್ಣಾಂಕ saved_errno;
  ಪೂರ್ಣಾಂಕ fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ errno;

  dirp = fdopendir (fd);
  ಆದರೆ (dirp == ಖಾಲಿ)
    {
      saved_errno = errno;
      close (fd);
      ಹಿಂತಿರುಗಿ saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  ಹಿಂತಿರುಗಿ no_direntries && saved_errno == 0 ? DS_ಖಾಲಿಗಳು : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
ಎಣಿಕೆ
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_ಆವೃತ್ತಿ_CHAR = (CHAR_MIN - 3)
};

#ವ್ಯಾಖ್ಯಾನಿಸು GETOPT_HELP_OPTION_DECL \
  "help", no_ವಾದ, ಖಾಲಿ, GETOPT_HELP_CHAR
#ವ್ಯಾಖ್ಯಾನಿಸು GETOPT_ಆವೃತ್ತಿ_OPTION_DECL \
  "version", no_ವಾದ, ಖಾಲಿ, GETOPT_ಆವೃತ್ತಿ_CHAR
#ವ್ಯಾಖ್ಯಾನಿಸು GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_ವಾದ, ಖಾಲಿ, 'Z'

#ವ್ಯಾಖ್ಯಾನಿಸು case_GETOPT_HELP_CHAR			\
  ಸ್ಥಿತಿ GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    ನಿಲ್ಲಿಸು;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#ವ್ಯಾಖ್ಯಾನಿಸು USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#ವ್ಯಾಖ್ಯಾನಿಸು HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#ವ್ಯಾಖ್ಯಾನಿಸು ಆವೃತ್ತಿ_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#ಅನ್ತರ್ಭಾವ "closein.h"
#ಅನ್ತರ್ಭಾವ "closeout.h"

#ಅನ್ತರ್ಭಾವ "version-etc.h"

#ಅನ್ತರ್ಭಾವ "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#ವ್ಯಾಖ್ಯಾನಿಸು proper_name(x) proper_name_lite (x, x)

#ಅನ್ತರ್ಭಾವ "progname.h"

#ವ್ಯಾಖ್ಯಾನಿಸು case_GETOPT_ಆವೃತ್ತಿ_CHAR(ಕಾರ್ಯಕ್ರಮ_name_2, Authors)			\
  ಸ್ಥಿತಿ GETOPT_ಆವೃತ್ತಿ_CHAR:						\
    ಆವೃತ್ತಿ_etc (stdout, ಕಾರ್ಯಕ್ರಮ_name_2, PACKAGE_NAME, ಆವೃತ್ತಿ, Authors,	\
                 (ಅಕ್ಷರ *) ಖಾಲಿ);					\
    ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);						\
    ನಿಲ್ಲಿಸು;

#ಅನ್ತರ್ಭಾವ "minmax.h"
#ಅನ್ತರ್ಭಾವ "intprops.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SSIZE_MAX
# define SSIZE_MAX TYPE_ಗರಿಷ್ಠ (ssize_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OFF_T_MAX
# define OFF_T_MAX TYPE_ಗರಿಷ್ಠ (off_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UID_T_MAX
# define UID_T_MAX TYPE_ಗರಿಷ್ಠ (uid_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GID_T_MAX
# define GID_T_MAX TYPE_ಗರಿಷ್ಠ (gid_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PID_T_MAX
# define PID_T_MAX TYPE_ಗರಿಷ್ಠ (pid_t)
#ಆದರೆ_ಅಂತ್ಯ

/* Use this to suppress gcc warnings.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ lint
# define IF_LINT(ಸಂಕೇತ_3) ಸಂಕೇತ_3
#ಇಲ್ಲದಿದ್ದರೆ
# define IF_LINT(ಸಂಕೇತ_3) /*ಖಾಲಿಗಳು*/
#ಆದರೆ_ಅಂತ್ಯ

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ lint
# define main_exit(ಸ್ಥಿತಿ_2) ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2)
#ಇಲ್ಲದಿದ್ದರೆ
# define main_exit(ಸ್ಥಿತಿ_2) ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#ಇಲ್ಲದಿದ್ದರೆ
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ defined strdupa
# define ನಿಯೋಜಿಸು_STRDUPA(DEST, S)		\
  ಮಾಡು { DEST = strdupa (S); } ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
# define ನಿಯೋಜಿಸು_STRDUPA(DEST, S)		\
  ಮಾಡು						\
    {						\
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s_ = (S);			\
      size_t len_ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s_) + 1;		\
      ಅಕ್ಷರ *tmp_dest_ = alloca (len_);		\
      DEST = ಸ್ಮರಣೆ_ನಕಲು (tmp_dest_, s_, len_);	\
    }						\
  ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ! HAVE_SYNC
# define sync() /*ಖಾಲಿಗಳು*/
#ಆದರೆ_ಅಂತ್ಯ

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t
ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ (size_t u, size_t v)
{
  ಮಾಡು
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  ಆಗ (v);

  ಹಿಂತಿರುಗಿ u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t
lcm (size_t u, size_t v)
{
  ಹಿಂತಿರುಗಿ u * (v / ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ *
ptr_align (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ptr, size_t alignment)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p0 = ptr;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p1 = p0 + alignment - 1;
  ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_nul (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *buf, size_t ಉದ್ದಳತೆ)
{
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#ಆದರೆ 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಶಬ್ದ_2;
#ಇಲ್ಲದಿದ್ದರೆ
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಶಬ್ದ_2;
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (! ಉದ್ದಳತೆ)
    ಹಿಂತಿರುಗಿ true;

  /* Check len bytes not aligned on a word.  */
  ಆಗ (UNLIKELY (ಉದ್ದಳತೆ & (ಗಾತ್ರ ಶಬ್ದ_2 - 1)))
    {
      ಆದರೆ (*p)
        ಹಿಂತಿರುಗಿ false;
      p++;
      ಉದ್ದಳತೆ--;
      ಆದರೆ (! ಉದ್ದಳತೆ)
        ಹಿಂತಿರುಗಿ true;
   }

  /* Check up to 16 bytes a word at a time.  */
  ಗಾಗಿ (;;)
    {
      ಸ್ಮರಣೆ_ನಕಲು (&ಶಬ್ದ_2, p, ಗಾತ್ರ ಶಬ್ದ_2);
      ಆದರೆ (ಶಬ್ದ_2)
        ಹಿಂತಿರುಗಿ false;
      p += ಗಾತ್ರ ಶಬ್ದ_2;
      ಉದ್ದಳತೆ -= ಗಾತ್ರ ಶಬ್ದ_2;
      ಆದರೆ (! ಉದ್ದಳತೆ)
        ಹಿಂತಿರುಗಿ true;
      ಆದರೆ (UNLIKELY (ಉದ್ದಳತೆ & 15) == 0)
        ನಿಲ್ಲಿಸು;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  ಹಿಂತಿರುಗಿ memeq (buf, p, ಉದ್ದಳತೆ);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#ವ್ಯಾಖ್ಯಾನಿಸು DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_ಕೂಡಿಸು (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#ವ್ಯಾಖ್ಯಾನಿಸು oputs(option) oputs_ (ಕಾರ್ಯಕ್ರಮ_NAME, option)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
oputs_ (MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *option)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ help_markup =
#ಆದರೆ defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#ಇಲ್ಲದಿದ್ದರೆ
    0;   /* Disable.  */
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != ಖಾಲಿ);
    }
  ಆದರೆ (! help_markup)
    {
      fputs (option, stdout);
      ಹಿಂತಿರುಗಿ;
    }

  bool double_space = true;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೊದಲ_ಶಬ್ದ = option + strspn (option, " \t\n");
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *option_ಪಠ್ಯ = strchr (option, '-');
  ಆದರೆ (!option_ಪಠ್ಯ)                    /* for dd(1) option syntax.  */
    {
      option_ಪಠ್ಯ = ಮೊದಲ_ಶಬ್ದ;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (option_ಪಠ್ಯ != ಮೊದಲ_ಶಬ್ದ)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = ಮೊದಲ_ಶಬ್ದ;
      size_t spaces = 0;
      ಆಗ (s < option_ಪಠ್ಯ && spaces < 2)
        spaces += !!ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*s++));
      ಆದರೆ (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_ಪಠ್ಯ = ಮೊದಲ_ಶಬ್ದ;
          double_space = false;
        }
    }

  size_t anchor_len = strcspn (option_ಪಠ್ಯ, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *desc_ಪಠ್ಯ = option_ಪಠ್ಯ + anchor_len;
  ಆಗ (*desc_ಪಠ್ಯ && *desc_ಪಠ್ಯ != '\n')
    {
      ಆದರೆ (*desc_ಪಠ್ಯ == '-' && *(desc_ಪಠ್ಯ + 1) == '-')
        double_space = false;
      ಆದರೆ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*desc_ಪಠ್ಯ)))
        {
          ಆದರೆ (*desc_ಪಠ್ಯ == '\t' || ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*(desc_ಪಠ್ಯ + 1))))
            ನಿಲ್ಲಿಸು;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          ಆದರೆ (!double_space && *(desc_ಪಠ್ಯ + 1) != '-')
            ನಿಲ್ಲಿಸು;
        }

      desc_ಪಠ್ಯ++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, ಮೊದಲ_ಶಬ್ದ - option, stdout);

  /* write option text.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MANUAL_URL
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *url_ಕಾರ್ಯಕ್ರಮ =   streq (ಕಾರ್ಯಕ್ರಮ, "[") ? "test"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "dir") ? "ls"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "vdir") ? "ls"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "b2sum") ? "cksum"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "md5sum") ? "cksum"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "sha1sum") ? "cksum"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "sha224sum") ? "cksum"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "sha256sum") ? "cksum"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "sha384sum") ? "cksum"
                            : streq (ಕಾರ್ಯಕ್ರಮ, "sha512sum") ? "cksum"
                            : ಕಾರ್ಯಕ್ರಮ;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  ಆದರೆ (STREQ_LEN (option_ಪಠ್ಯ, "--help", 6)
      || STREQ_LEN (option_ಪಠ್ಯ, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      ಬರೆ ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_ಕಾರ್ಯಕ್ರಮ, url_ಕಾರ್ಯಕ್ರಮ, (ಪೂರ್ಣಾಂಕ) anchor_len, option_ಪಠ್ಯ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      ಬರೆ ("\033]8;;%s#%s%.*s", MANUAL_URL, url_ಕಾರ್ಯಕ್ರಮ,
              (ಪೂರ್ಣಾಂಕ) anchor_len, option_ಪಠ್ಯ);
    }
  fputs ("\033\\", stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#ಆದರೆ_ಅಂತ್ಯ
  /* first_word != option_text for test(1).  */
  fwrite (ಮೊದಲ_ಶಬ್ದ, 1, desc_ಪಠ್ಯ - ಮೊದಲ_ಶಬ್ದ, stdout);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#ಆದರೆ_ಅಂತ್ಯ

  /* write description.  */
  fputs (desc_ಪಠ್ಯ, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#ವ್ಯಾಖ್ಯಾನಿಸು oprintf(...) oprintf_ (ಕಾರ್ಯಕ್ರಮ_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((ಬರೆ, 2, 3))
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
oprintf_ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2, ...)
{
  va_list ವಾದಗಳು;
  ಅಕ್ಷರ *buf;
  ಪೂರ್ಣಾಂಕ buflen = -1;

#ಆದರೆ defined MANUAL_URL || defined BOLD_MAN_REFS
  va_ಆರಂಭ (ವಾದಗಳು, ಸಂದೇಶ_2);
  buflen = vasprintf (&buf, ಸಂದೇಶ_2, ವಾದಗಳು);
  va_ಕೊನೆ (ವಾದಗಳು);
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (buflen < 0)
    {
      va_ಆರಂಭ (ವಾದಗಳು, ಸಂದೇಶ_2);
      vprintf (ಸಂದೇಶ_2, ವಾದಗಳು);
      va_ಕೊನೆ (ವಾದಗಳು);
      ಹಿಂತಿರುಗಿ;
    }

  oputs_ (ಕಾರ್ಯಕ್ರಮ, buf);
  ಬಿಡುಗಡೆ (buf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_stdin_note (ಶೂನ್ಯ)
{
  fputs (_("\n\
With no FILE, or when FILE is -, ಪಠ standard ಒಳಹರಿವು.\n\
"), stdout);
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_mandatory_arg_note (ಶೂನ್ಯ)
{
  fputs (_("\n\
Mandatory arguments ಗಮ್ಯ ದೀರ್ಘ options are mandatory ಗಾಗಿ ಗಿಡ್ಡ options too.\n\
"), stdout);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_ಗಾತ್ರ_note (ಶೂನ್ಯ)
{
  fputs (_("\n\
The ಗಾತ್ರ_3 ವಾದ is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers ದ 1024) or KB,MB,... (powers ದ 1000).\n\
ದ್ವಿಮಾನ_2 prefixes can be ಬಳಸಿದ, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_blocksize_note (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ)
{
  ಬರೆ (_("\n\
Display ಮೌಲ್ಯಗಳು are ಒಳ units ದ the ಮೊದಲ ಲಭ್ಯ ಗಾತ್ರ_3 from --ಖಂಡ-ಗಾತ್ರ_2,\n\
and the %s_ಖಂಡ_ಗಾತ್ರ, ಖಂಡ_ಗಾತ್ರ_2 and BLOCKSIZE environment variables.\n\
Otherwise, units ಪೂರ್ವನಿಯೋಜಿತ ಗಮ್ಯ 1024 ಬೈಟುಗಳು (or 512 ಆದರೆ POSIXLY_CORRECT is ಗಣ_2).\n\
"), ಕಾರ್ಯಕ್ರಮ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_update_parameters_note (ಶೂನ್ಯ)
{
  fputs (_("\
\n\
UPDATE controls which existing files ಒಳ the destination are replaced.\n\
'all' is the ಪೂರ್ವನಿಯೋಜಿತ operation when an --update option is not specified,\n\
and results ಒಳ all existing files ಒಳ the destination being replaced.\n\
'none' is like the --no-clobber option, ಒಳ that no files ಒಳ the\n\
destination are replaced, and skipped files ಮಾಡು not induce a failure.\n\
'none-fail' also ensures no files are replaced ಒಳ the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the ಪೂರ್ವನಿಯೋಜಿತ operation when --update is specified, and results\n\
ಒಳ files being replaced ಆದರೆ they're older than the corresponding ಮೂಲ_2 file.\n\
"), stdout);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_backup_suffix_note (ಶೂನ್ಯ)
{
  fputs (_("\
\n\
The backup suffix is '~', unless ಗಣ_2 with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The ಆವೃತ್ತಿ_3 control ವಿಧಾನ_2 may be selected ಮೂಲಕ the --backup option or through\n\
the ಆವೃತ್ತಿ_CONTROL environment variable.  ಇಲ್ಲಿ_2 are the ಮೌಲ್ಯಗಳು:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even ಆದರೆ --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered ಆದರೆ numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#ವ್ಯಾಖ್ಯಾನಿಸು emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (ಕಾರ್ಯಕ್ರಮ_NAME, default_opt)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_symlink_recurse_options_ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *default_opt)
{
      ಬರೆ (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If ಇನ್ನಷ್ಟು than one is specified, only the final\n\
one takes effect. %s is the ಪೂರ್ವನಿಯೋಜಿತ.\n\
\n\
"), default_opt);
      oputs_ (ಕಾರ್ಯಕ್ರಮ, _("\
  -H\n\
         ಆದರೆ a command ಸಾಲು ವಾದ is a symlink ಗಮ್ಯ a directory, traverse it\n\
"));
      oputs_ (ಕಾರ್ಯಕ್ರಮ, _("\
  -L\n\
         traverse every symbolic link ಗಮ್ಯ a directory encountered\n\
"));
      oputs_ (ಕಾರ್ಯಕ್ರಮ, _("\
  -P\n\
         ಮಾಡು not traverse any symbolic links\n\
\n\
"));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_exec_ಸ್ಥಿತಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ)
{
      ಬರೆ (_("\n\
Exit ಸ್ಥಿತಿ_2:\n\
  125  ಆದರೆ the %s command itself fails\n\
  126  ಆದರೆ COMMAND is ಸಿಕ್ಕಿತು but cannot be invoked\n\
  127  ಆದರೆ COMMAND cannot be ಸಿಕ್ಕಿತು\n\
  -    the ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 ದ COMMAND otherwise\n\
"), ಕಾರ್ಯಕ್ರಮ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
emit_ancillary_info (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ)
{
  ರಚನೆ infomap { ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕಾರ್ಯಕ್ರಮ; ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಂಟು; } ಸ್ಥಿರಾಂಕ infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { ಖಾಲಿ, ಖಾಲಿ }
  };

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಂಟು = ಕಾರ್ಯಕ್ರಮ;
  ರಚನೆ infomap ಸ್ಥಿರಾಂಕ *map_prog = infomap;

  ಆಗ (map_prog->ಕಾರ್ಯಕ್ರಮ && ! streq (ಕಾರ್ಯಕ್ರಮ, map_prog->ಕಾರ್ಯಕ್ರಮ))
    map_prog++;

  ಆದರೆ (map_prog->ಗಂಟು)
    ಗಂಟು = map_prog->ಗಂಟು;

  emit_bug_reporting_ವಿಳಾಸ ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *url_ಕಾರ್ಯಕ್ರಮ = streq (ಕಾರ್ಯಕ್ರಮ, "[") ? "test" : ಕಾರ್ಯಕ್ರಮ;
  ಬರೆ (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_ಕಾರ್ಯಕ್ರಮ);
  ಬರೆ (_("or available locally via: info '(coreutils) %s%s'\n"),
          ಗಂಟು, ಗಂಟು == ಕಾರ್ಯಕ್ರಮ ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#ವ್ಯಾಖ್ಯಾನಿಸು emit_try_help() \
  ಮಾಡು \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               ಕಾರ್ಯಕ್ರಮ_name); \
    } \
  ಆಗ (0)

#ಅನ್ತರ್ಭಾವ "inttostr.h"

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
timetostr (time_t t, ಅಕ್ಷರ *buf)
{
  ಹಿಂತಿರುಗಿ (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
bad_cast (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
usable_st_ಗಾತ್ರ (ರಚನೆ stat ಸ್ಥಿರಾಂಕ *sb)
{
  ಹಿಂತಿರುಗಿ (S_ISREG (sb->st_ಬಹುಲಕ) || S_ISLNK (sb->st_ಬಹುಲಕ)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn ಶೂನ್ಯ usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2);

#ಅನ್ತರ್ಭಾವ <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#ವ್ಯಾಖ್ಯಾನಿಸು devmsg(...)			\
  ಮಾಡು					\
    {					\
      ಆದರೆ (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು emit_ಚಕ್ರ_warning(file_name)	\
  ಮಾಡು					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part ದ the ಚಕ್ರ:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  ಆಗ (0)

/* exit with a _single_ "write error" diagnostic.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
write_error (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ saved_errno = errno;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ *
stzncpy (ಅಕ್ಷರ *ಸೀಮಿತ dest, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ src, size_t len)
{
  ಗಾಗಿ (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  ಹಿಂತಿರುಗಿ dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_ENOTSUP (ಪೂರ್ಣಾಂಕ err)
{
  ಹಿಂತಿರುಗಿ err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#ಅನ್ತರ್ಭಾವ "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#ವ್ಯಾಖ್ಯಾನಿಸು quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#ವ್ಯಾಖ್ಯಾನಿಸು quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#ವ್ಯಾಖ್ಯಾನಿಸು quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#ವ್ಯಾಖ್ಯಾನಿಸು quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#ವ್ಯಾಖ್ಯಾನಿಸು quoteN(x) quotearg_style (get_quoting_style (ಖಾಲಿ), x)
#ವ್ಯಾಖ್ಯಾನಿಸು quoteN_mem(x, ಗಾತ್ರ_2)                                     \
  quotearg_style_mem (get_quoting_style (ಖಾಲಿ), x, ಗಾತ್ರ_2)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
getenv_quoting_style (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *q_style = getenv ("QUOTING_STYLE");
  ಆದರೆ (!q_style)
    ಹಿಂತಿರುಗಿ -1;

  ಪೂರ್ಣಾಂಕ i = ARGMATCH (q_style, quoting_style_ವಾದಗಳು, quoting_style_vals);
  ಆದರೆ (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      ಹಿಂತಿರುಗಿ -1;
    }
  ಹಿಂತಿರುಗಿ quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ptrdiff_t
x_timestyle_ಹೊಂದಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * style, bool allow_posix,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ * timestyle_ವಾದಗಳು,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * timestyle_types,
                   size_t timestyle_types_ಗಾತ್ರ,
                   ಪೂರ್ಣಾಂಕ fail_ಸ್ಥಿತಿ)
{
  ptrdiff_t res = argmatch (style, timestyle_ವಾದಗಳು,
                            timestyle_types,
                            timestyle_types_ಗಾತ್ರ);
  ಆದರೆ (res < 0)
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
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ *p = timestyle_ವಾದಗಳು;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *posix_ಪೂರ್ವಪ್ರತ್ಯಯ = allow_posix ? "[posix-]" : "";
      ಆಗ (*p)
        fprintf (stderr, "  - %s%s\n", posix_ಪೂರ್ವಪ್ರತ್ಯಯ, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_ಸ್ಥಿತಿ);
    }

  ಹಿಂತಿರುಗಿ res;
}
#ಆದರೆ_ಅಂತ್ಯ
