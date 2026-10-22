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

#include <attribute.h>

#include <alloca.h>

#include <ctype.h>

#include <sys/stat.h>

/* Commonly used file permission combination.  */
#define MODE_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#ᮚᮦᮔ᮪ HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#include <unistd.h>

#include <limits.h>

#include "pathmax.h"
#ifndef PATH_MAX
# define PATH_MAX 8192
#endif

#include "configmake.h"

#include <sys/time.h>
#include <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#ᮚᮦᮔ᮪ MAJOR_IN_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#endif
#ᮚᮦᮔ᮪ MAJOR_IN_SYSMACROS
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

#ᮚᮦᮔ᮪ ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#endif

#include <stdckdint.h>
#include <stdcountof.h>
#include <stddef.h>
#include <string.h>
#include <uchar.h>
#include <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#ifndef ENODATA
# define ENODATA (-1)
#endif

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
static anta_hstha bool
ignorable_traversal_errno (puur_naa_nka errnum) { ᮝᮜᮤ errnum == ENOENT; }

#include <stdlib.h>
#include "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
ga_nanaa
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#include "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
static anta_hstha void
initialize_exit_failure (puur_naa_nka status)
{
  ᮚᮦᮔ᮪ (status != EXIT_FAILURE)
    exit_failure = status;
}

#include <fcntl.h>
#ifdef O_PATH
ga_nanaa { O_PATHSEARCH = O_PATH };
#ᮜᮤᮚ
ga_nanaa { O_PATHSEARCH = O_SEARCH };
#endif

#include <dirent.h>
#ifndef _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) strlen ((dp)->d_name)
#endif

ga_nanaa
{
  NOT_AN_INODE_NUMBER = 0
};

#ifdef D_INO_IN_DIRENT
# define D_INO(dp) (dp)->d_ino
#ᮜᮤᮚ
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_NUMBER
#endif

/* include here for SIZE_MAX.  */
#include <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#ifndef initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# ᮜᮤᮚ
#  define initialize_main(ac, av) \
     do { _wildcard (ac, av); _response (ac, av); } ᮞᮨᮓᮨᮜ (0)
# endif
#endif

#include "stat-macros.h"

#include "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
static anta_hstha unsigned var_na to_uchar (var_na ch) { ᮝᮜᮤ ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_PURE
static anta_hstha puur_naa_nka
c32isnbspace (char32_t wc)
{
  ᮝᮜᮤ wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_PURE
static anta_hstha puur_naa_nka
c32isnewline (char32_t wc)
{
  ᮝᮜᮤ wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_PURE
static anta_hstha puur_naa_nka
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  ᮝᮜᮤ c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_PURE
static anta_hstha bool
c32issep (char32_t wc)
{
#ᮚᮦᮔ᮪ defined __GLIBC__
  ᮝᮜᮤ !! c32isblank (wc);
#ᮜᮤᮚ
  ᮝᮜᮤ c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#endif
}

#include <locale.h>

/* Take care of NLS matters.  */

#include "gettext.h"
#ᮚᮦᮔ᮪ ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /* empty */
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /* empty */
#endif

#define _(msgid) gettext (msgid)
#define N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
static anta_hstha unsigned long puur_naa_nka
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  ga_nanaa { PLURAL_REDUCER = 1000000 };
  ᮝᮜᮤ (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#define STREQ_LEN(a, b, n) (strncmp (a, b, n) == 0) /* n==-1 means unbounded */
#define STRPREFIX(a, b) (strncmp (a, b, strlen (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#define STRNCMP_LIT(s, lit) strncmp (s, "" lit "", parimaa_na (lit) - 1)

#ᮚᮦᮔ᮪ !HAVE_DECL_GETLOGIN
var_na *getlogin (void);
#endif

#ᮚᮦᮔ᮪ !HAVE_DECL_TTYNAME
var_na *ttyname (puur_naa_nka);
#endif

#ᮚᮦᮔ᮪ !HAVE_DECL_GETEUID
uid_t geteuid (void);
#endif

#ᮚᮦᮔ᮪ !HAVE_DECL_GETPWUID
sa_nracanaa passwd *getpwuid (uid_t);
#endif

#ᮚᮦᮔ᮪ !HAVE_DECL_GETGRGID
sa_nracanaa group *getgrgid (gid_t);
#endif

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#ᮚᮦᮔ᮪ ! HAVE_SETGROUPS
# ᮚᮦᮔ᮪ HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# ᮚᮦᮔ᮪ HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# ᮚᮦᮔ᮪ HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#endif

#ᮚᮦᮔ᮪ !HAVE_DECL_GETUID
uid_t getuid (void);
#endif

#include "idx.h"
#include "xalloc.h"
#include "verify.h"
#include "unlocked-io.h"
#include "same-inode.h"

#include "dirname.h"
#include "openat.h"

static anta_hstha bool
dot_or_dotdot (var_na const *file_name)
{
  ᮚᮦᮔ᮪ (file_name[0] == '.')
    {
      var_na sep = file_name[(file_name[1] == '.') + 1];
      ᮝᮜᮤ (! sep || ISSLASH (sep));
    }
  ᮜᮤᮚ
    ᮝᮜᮤ false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
static anta_hstha sa_nracanaa dirent const *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  ᮞᮨᮓᮨᮜ (true)
    {
      sa_nracanaa dirent const *dp = readdir (dirp);
      ᮚᮦᮔ᮪ (dp == NULL || ! dot_or_dotdot (dp->d_name))
        ᮝᮜᮤ dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
ga_nanaa {
    DS_UNKNOWN = -2,
    DS_EMPTY = -1,
    DS_NONEMPTY = 0,
};
static anta_hstha puur_naa_nka
directory_status (puur_naa_nka fd_cwd, var_na const *dir)
{
  DIR *dirp;
  bool no_direntries;
  puur_naa_nka saved_errno;
  puur_naa_nka fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  ᮚᮦᮔ᮪ (fd < 0)
    ᮝᮜᮤ errno;

  dirp = fdopendir (fd);
  ᮚᮦᮔ᮪ (dirp == NULL)
    {
      saved_errno = errno;
      close (fd);
      ᮝᮜᮤ saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  ᮝᮜᮤ no_direntries && saved_errno == 0 ? DS_EMPTY : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
ga_nanaa
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, NULL, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, NULL, GETOPT_VERSION_CHAR
#define GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_argument, NULL, 'Z'

#define case_GETOPT_HELP_CHAR			\
  pak_sa GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    bha_nga;

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

#include "closein.h"
#include "closeout.h"

#include "version-etc.h"

#include "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#define proper_name(x) proper_name_lite (x, x)

#include "progname.h"

#define case_GETOPT_VERSION_CHAR(Program_name, Authors)			\
  pak_sa GETOPT_VERSION_CHAR:						\
    version_etc (stdout, Program_name, PACKAGE_NAME, Version, Authors,	\
                 (var_na *) NULL);					\
    exit (EXIT_SUCCESS);						\
    bha_nga;

#include "minmax.h"
#include "intprops.h"

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
#ᮜᮤᮚ
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
#ᮜᮤᮚ
# define main_exit(status) ᮝᮜᮤ status
#endif

#ifdef __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#ᮜᮤᮚ
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#endif


#ᮚᮦᮔ᮪ defined strdupa
# define ASSIGN_STRDUPA(DEST, S)		\
  do { DEST = strdupa (S); } ᮞᮨᮓᮨᮜ (0)
#ᮜᮤᮚ
# define ASSIGN_STRDUPA(DEST, S)		\
  do						\
    {						\
      var_na const *s_ = (S);			\
      size_t len_ = strlen (s_) + 1;		\
      var_na *tmp_dest_ = alloca (len_);		\
      DEST = memcpy (tmp_dest_, s_, len_);	\
    }						\
  ᮞᮨᮓᮨᮜ (0)
#endif

#ᮚᮦᮔ᮪ ! HAVE_SYNC
# define sync() /* empty */
#endif

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
static anta_hstha size_t
gcd (size_t u, size_t v)
{
  do
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  ᮞᮨᮓᮨᮜ (v);

  ᮝᮜᮤ u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
static anta_hstha size_t
lcm (size_t u, size_t v)
{
  ᮝᮜᮤ u * (v / gcd (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

static anta_hstha void *
ptr_align (void const *ptr, size_t alignment)
{
  var_na const *p0 = ptr;
  var_na const *p1 = p0 + alignment - 1;
  ᮝᮜᮤ (void *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_PURE
static anta_hstha bool
is_nul (void const *buf, size_t length)
{
  const unsigned var_na *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#ᮚᮦᮔ᮪ 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  unsigned long word;
#ᮜᮤᮚ
  unsigned var_na word;
#endif

  ᮚᮦᮔ᮪ (! length)
    ᮝᮜᮤ true;

  /* Check len bytes not aligned on a word.  */
  ᮞᮨᮓᮨᮜ (UNLIKELY (length & (parimaa_na word - 1)))
    {
      ᮚᮦᮔ᮪ (*p)
        ᮝᮜᮤ false;
      p++;
      length--;
      ᮚᮦᮔ᮪ (! length)
        ᮝᮜᮤ true;
   }

  /* Check up to 16 bytes a word at a time.  */
  for (;;)
    {
      memcpy (&word, p, parimaa_na word);
      ᮚᮦᮔ᮪ (word)
        ᮝᮜᮤ false;
      p += parimaa_na word;
      length -= parimaa_na word;
      ᮚᮦᮔ᮪ (! length)
        ᮝᮜᮤ true;
      ᮚᮦᮔ᮪ (UNLIKELY (length & 15) == 0)
        bha_nga;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  ᮝᮜᮤ memeq (buf, p, length);
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
static anta_hstha void
oputs_ (MAYBE_UNUSED var_na const *program, var_na const *option)
{
  static puur_naa_nka help_markup =
#ᮚᮦᮔ᮪ defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#ᮜᮤᮚ
    0;   /* Disable.  */
#endif
  ᮚᮦᮔ᮪ (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      var_na const *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != NULL);
    }
  ᮚᮦᮔ᮪ (! help_markup)
    {
      fputs (option, stdout);
      ᮝᮜᮤ;
    }

  bool double_space = true;
  var_na const *first_word = option + strspn (option, " \t\n");
  var_na const *option_text = strchr (option, '-');
  ᮚᮦᮔ᮪ (!option_text)                    /* for dd(1) option syntax.  */
    {
      option_text = first_word;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  ᮜᮤᮚ ᮚᮦᮔ᮪ (option_text != first_word)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      var_na const *s = first_word;
      size_t spaces = 0;
      ᮞᮨᮓᮨᮜ (s < option_text && spaces < 2)
        spaces += !!isspace (to_uchar (*s++));
      ᮚᮦᮔ᮪ (spaces == 2)
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
  var_na const *desc_text = option_text + anchor_len;
  ᮞᮨᮓᮨᮜ (*desc_text && *desc_text != '\n')
    {
      ᮚᮦᮔ᮪ (*desc_text == '-' && *(desc_text + 1) == '-')
        double_space = false;
      ᮚᮦᮔ᮪ (isspace (to_uchar (*desc_text)))
        {
          ᮚᮦᮔ᮪ (*desc_text == '\t' || isspace (to_uchar (*(desc_text + 1))))
            bha_nga;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          ᮚᮦᮔ᮪ (!double_space && *(desc_text + 1) != '-')
            bha_nga;
        }

      desc_text++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, first_word - option, stdout);

  /* write option text.  */
#ifdef MANUAL_URL
  var_na const *url_program =   streq (program, "[") ? "test"
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
  ᮚᮦᮔ᮪ (STREQ_LEN (option_text, "--help", 6)
      || STREQ_LEN (option_text, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      printf ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_program, url_program, (puur_naa_nka) anchor_len, option_text);
    }
  ᮜᮤᮚ
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      printf ("\033]8;;%s#%s%.*s", MANUAL_URL, url_program,
              (puur_naa_nka) anchor_len, option_text);
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
static anta_hstha void
oprintf_ (var_na const *program, var_na const *message, ...)
{
  va_list args;
  var_na *buf;
  puur_naa_nka buflen = -1;

#ᮚᮦᮔ᮪ defined MANUAL_URL || defined BOLD_MAN_REFS
  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);
#endif

  ᮚᮦᮔ᮪ (buflen < 0)
    {
      va_start (args, message);
      vprintf (message, args);
      va_end (args);
      ᮝᮜᮤ;
    }

  oputs_ (program, buf);
  free (buf);
}

static anta_hstha void
emit_stdin_note (void)
{
  fputs (_("\n\
With no FILE, or when FILE is -, read standard input.\n\
"), stdout);
}
static anta_hstha void
emit_mandatory_arg_note (void)
{
  fputs (_("\n\
Mandatory arguments to long options are mandatory for short options too.\n\
"), stdout);
}

static anta_hstha void
emit_size_note (void)
{
  fputs (_("\n\
The SIZE argument is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers of 1024) or KB,MB,... (powers of 1000).\n\
Binary prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

static anta_hstha void
emit_blocksize_note (var_na const *program)
{
  printf (_("\n\
Display values are in units of the first available SIZE from --block-size,\n\
and the %s_BLOCK_SIZE, BLOCK_SIZE and BLOCKSIZE environment variables.\n\
Otherwise, units default to 1024 bytes (or 512 ᮚᮦᮔ᮪ POSIXLY_CORRECT is set).\n\
"), program);
}

static anta_hstha void
emit_update_parameters_note (void)
{
  fputs (_("\
\n\
UPDATE controls which existing files in the destination are replaced.\n\
'all' is the default operation when an --update option is not specified,\n\
and results in all existing files in the destination being replaced.\n\
'none' is like the --no-clobber option, in that no files in the\n\
destination are replaced, and skipped files do not induce a failure.\n\
'none-fail' also ensures no files are replaced in the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the default operation when --update is specified, and results\n\
in files being replaced ᮚᮦᮔ᮪ they're older than the corresponding source file.\n\
"), stdout);
}

static anta_hstha void
emit_backup_suffix_note (void)
{
  fputs (_("\
\n\
The backup suffix is '~', unless set with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The version control method may be selected via the --backup option or through\n\
the VERSION_CONTROL environment variable.  Here are the values:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even ᮚᮦᮔ᮪ --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered ᮚᮦᮔ᮪ numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#define emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (PROGRAM_NAME, default_opt)
static anta_hstha void
emit_symlink_recurse_options_ (var_na const *program, var_na const *default_opt)
{
      printf (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If more than one is specified, only the final\n\
one takes effect. %s is the default.\n\
\n\
"), default_opt);
      oputs_ (program, _("\
  -H\n\
         ᮚᮦᮔ᮪ a command line argument is a symlink to a directory, traverse it\n\
"));
      oputs_ (program, _("\
  -L\n\
         traverse every symbolic link to a directory encountered\n\
"));
      oputs_ (program, _("\
  -P\n\
         do not traverse any symbolic links\n\
\n\
"));
}

static anta_hstha void
emit_exec_status (var_na const *program)
{
      printf (_("\n\
Exit status:\n\
  125  ᮚᮦᮔ᮪ the %s command itself fails\n\
  126  ᮚᮦᮔ᮪ COMMAND is found but cannot be invoked\n\
  127  ᮚᮦᮔ᮪ COMMAND cannot be found\n\
  -    the exit status of COMMAND otherwise\n\
"), program);
}

static anta_hstha void
emit_ancillary_info (var_na const *program)
{
  sa_nracanaa infomap { var_na const *program; var_na const *node; } const infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { NULL, NULL }
  };

  var_na const *node = program;
  sa_nracanaa infomap const *map_prog = infomap;

  ᮞᮨᮓᮨᮜ (map_prog->program && ! streq (program, map_prog->program))
    map_prog++;

  ᮚᮦᮔ᮪ (map_prog->node)
    node = map_prog->node;

  emit_bug_reporting_address ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  var_na const *url_program = streq (program, "[") ? "test" : program;
  printf (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_program);
  printf (_("or available locally via: info '(coreutils) %s%s'\n"),
          node, node == program ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#define emit_try_help() \
  do \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               program_name); \
    } \
  ᮞᮨᮓᮨᮜ (0)

#include "inttostr.h"

static anta_hstha var_na *
timetostr (time_t t, var_na *buf)
{
  ᮝᮜᮤ (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

static anta_hstha var_na *
bad_cast (var_na const *s)
{
  ᮝᮜᮤ (var_na *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
static anta_hstha bool
usable_st_size (sa_nracanaa stat const *sb)
{
  ᮝᮜᮤ (S_ISREG (sb->st_mode) || S_ISLNK (sb->st_mode)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn void usage (puur_naa_nka status);

#include <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#define devmsg(...)			\
  do					\
    {					\
      ᮚᮦᮔ᮪ (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  ᮞᮨᮓᮨᮜ (0)

#define emit_cycle_warning(file_name)	\
  do					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part of the cycle:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  ᮞᮨᮓᮨᮜ (0)

/* exit with a _single_ "write error" diagnostic.  */

static anta_hstha void
write_error (void)
{
  puur_naa_nka saved_errno = errno;
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
static anta_hstha var_na *
stzncpy (var_na *restrict dest, var_na const *restrict src, size_t len)
{
  for (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  ᮝᮜᮤ dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
static anta_hstha bool
is_ENOTSUP (puur_naa_nka err)
{
  ᮝᮜᮤ err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#include "quotearg.h"

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
#define quoteN(x) quotearg_style (get_quoting_style (NULL), x)
#define quoteN_mem(x, size)                                     \
  quotearg_style_mem (get_quoting_style (NULL), x, size)

#ifdef ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
static anta_hstha puur_naa_nka
getenv_quoting_style (void)
{
  var_na const *q_style = getenv ("QUOTING_STYLE");
  ᮚᮦᮔ᮪ (!q_style)
    ᮝᮜᮤ -1;

  puur_naa_nka i = ARGMATCH (q_style, quoting_style_args, quoting_style_vals);
  ᮚᮦᮔ᮪ (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      ᮝᮜᮤ -1;
    }
  ᮝᮜᮤ quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
static anta_hstha ptrdiff_t
x_timestyle_match (var_na const * style, bool allow_posix,
                   var_na const *const * timestyle_args,
                   var_na const * timestyle_types,
                   size_t timestyle_types_size,
                   puur_naa_nka fail_status)
{
  ptrdiff_t res = argmatch (style, timestyle_args,
                            timestyle_types,
                            timestyle_types_size);
  ᮚᮦᮔ᮪ (res < 0)
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
      var_na const *const *p = timestyle_args;
      var_na const *posix_prefix = allow_posix ? "[posix-]" : "";
      ᮞᮨᮓᮨᮜ (*p)
        fprintf (stderr, "  - %s%s\n", posix_prefix, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_status);
    }

  ᮝᮜᮤ res;
}
#endif
