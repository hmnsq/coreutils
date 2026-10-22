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

#অন্তর্ভাব <attribute.h>

#অন্তর্ভাব <alloca.h>

#অন্তর্ভাব <ctype.h>

#অন্তর্ভাব <sys/stat.h>

/* Commonly used file permission combination.  */
#define MODE_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#যদি HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#অন্তর্ভাব <unistd.h>

#অন্তর্ভাব <limits.h>

#অন্তর্ভাব "pathmax.h"
#ifndef PATH_MAX
# define PATH_MAX 8192
#endif

#অন্তর্ভাব "configmake.h"

#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#যদি MAJOR_IN_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#endif
#যদি MAJOR_IN_SYSMACROS
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

#যদি ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#endif

#অন্তর্ভাব <stdckdint.h>
#অন্তর্ভাব <stdcountof.h>
#অন্তর্ভাব <stddef.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <uchar.h>
#অন্তর্ভাব <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#ifndef ENODATA
# define ENODATA (-1)
#endif

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
স্থির অন্তঃরেখ bool
ignorable_traversal_errno (পূর্ণসংখ্যা errnum) { ফেরত errnum == ENOENT; }

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
গণনা
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#অন্তর্ভাব "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
স্থির অন্তঃরেখ শূন্য
initialize_exit_failure (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_FAILURE)
    exit_failure = status;
}

#অন্তর্ভাব <fcntl.h>
#ifdef O_PATH
গণনা { O_PATHSEARCH = O_PATH };
#অন্যথা
গণনা { O_PATHSEARCH = O_অনুসন্ধান };
#endif

#অন্তর্ভাব <dirent.h>
#ifndef _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) strlen ((dp)->d_name)
#endif

গণনা
{
  NOT_AN_INODE_সংখ্যা = 0
};

#ifdef D_INO_IN_DIRENT
# define D_INO(dp) (dp)->d_ino
#অন্যথা
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_সংখ্যা
#endif

/* include here for SIZE_MAX.  */
#অন্তর্ভাব <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#ifndef initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# অন্যথা
#  define initialize_main(ac, av) \
     করো { _wildcard (ac, av); _response (ac, av); } যখন (0)
# endif
#endif

#অন্তর্ভাব "stat-macros.h"

#অন্তর্ভাব "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
স্থির অন্তঃরেখ unsigned অক্ষর to_uchar (অক্ষর ch) { ফেরত ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_PURE
স্থির অন্তঃরেখ পূর্ণসংখ্যা
c32isnbspace (char32_t wc)
{
  ফেরত wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_PURE
স্থির অন্তঃরেখ পূর্ণসংখ্যা
c32isnewline (char32_t wc)
{
  ফেরত wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_PURE
স্থির অন্তঃরেখ পূর্ণসংখ্যা
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  ফেরত c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_PURE
স্থির অন্তঃরেখ bool
c32issep (char32_t wc)
{
#যদি defined __GLIBC__
  ফেরত !! c32isblank (wc);
#অন্যথা
  ফেরত c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#endif
}

#অন্তর্ভাব <locale.h>

/* Take care of NLS matters.  */

#অন্তর্ভাব "gettext.h"
#যদি ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /* empty */
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /* empty */
#endif

#define _(msgid) gettext (msgid)
#define N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
স্থির অন্তঃরেখ unsigned long পূর্ণসংখ্যা
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  গণনা { PLURAL_REDUCER = 1000000 };
  ফেরত (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#define STREQ_LEN(a, b, n) (strncmp (a, b, n) == 0) /* n==-1 means unbounded */
#define STRPREFIX(a, b) (strncmp (a, b, strlen (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#define STRNCMP_LIT(s, lit) strncmp (s, "" lit "", আকার (lit) - 1)

#যদি !HAVE_DECL_GETLOGIN
অক্ষর *getlogin (শূন্য);
#endif

#যদি !HAVE_DECL_TTYNAME
অক্ষর *ttyname (পূর্ণসংখ্যা);
#endif

#যদি !HAVE_DECL_GETEUID
uid_t geteuid (শূন্য);
#endif

#যদি !HAVE_DECL_GETPWUID
কাঠামো passwd *getpwuid (uid_t);
#endif

#যদি !HAVE_DECL_GETGRGID
কাঠামো group *getgrgid (gid_t);
#endif

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#যদি ! HAVE_SETGROUPS
# যদি HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# যদি HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# যদি HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#endif

#যদি !HAVE_DECL_GETUID
uid_t getuid (শূন্য);
#endif

#অন্তর্ভাব "idx.h"
#অন্তর্ভাব "xalloc.h"
#অন্তর্ভাব "verify.h"
#অন্তর্ভাব "unlocked-io.h"
#অন্তর্ভাব "same-inode.h"

#অন্তর্ভাব "dirname.h"
#অন্তর্ভাব "openat.h"

স্থির অন্তঃরেখ bool
dot_or_dotdot (অক্ষর ধ্রুবক *file_name)
{
  যদি (file_name[0] == '.')
    {
      অক্ষর sep = file_name[(file_name[1] == '.') + 1];
      ফেরত (! sep || ISSLASH (sep));
    }
  অন্যথা
    ফেরত false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
স্থির অন্তঃরেখ কাঠামো dirent ধ্রুবক *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  যখন (true)
    {
      কাঠামো dirent ধ্রুবক *dp = readdir (dirp);
      যদি (dp == ফাঁকা || ! dot_or_dotdot (dp->d_name))
        ফেরত dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
গণনা {
    DS_UNKNOWN = -2,
    DS_EMPTY = -1,
    DS_NONEMPTY = 0,
};
স্থির অন্তঃরেখ পূর্ণসংখ্যা
directory_status (পূর্ণসংখ্যা fd_cwd, অক্ষর ধ্রুবক *dir)
{
  DIR *dirp;
  bool no_direntries;
  পূর্ণসংখ্যা saved_errno;
  পূর্ণসংখ্যা fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  যদি (fd < 0)
    ফেরত errno;

  dirp = fdopendir (fd);
  যদি (dirp == ফাঁকা)
    {
      saved_errno = errno;
      close (fd);
      ফেরত saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  ফেরত no_direntries && saved_errno == 0 ? DS_EMPTY : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
গণনা
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

#define GETOPT_HELP_OPTION_DECL \
  "help", no_argument, ফাঁকা, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, ফাঁকা, GETOPT_VERSION_CHAR
#define GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_argument, ফাঁকা, 'Z'

#define case_GETOPT_HELP_CHAR			\
  ক্ষেত্র GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    বিরতি;

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

#অন্তর্ভাব "closein.h"
#অন্তর্ভাব "closeout.h"

#অন্তর্ভাব "version-etc.h"

#অন্তর্ভাব "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#define proper_name(x) proper_name_lite (x, x)

#অন্তর্ভাব "progname.h"

#define case_GETOPT_VERSION_CHAR(Program_name, Authors)			\
  ক্ষেত্র GETOPT_VERSION_CHAR:						\
    version_etc (stdout, Program_name, PACKAGE_NAME, Version, Authors,	\
                 (অক্ষর *) ফাঁকা);					\
    exit (EXIT_SUCCESS);						\
    বিরতি;

#অন্তর্ভাব "minmax.h"
#অন্তর্ভাব "intprops.h"

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
#অন্যথা
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
#অন্যথা
# define main_exit(status) ফেরত status
#endif

#ifdef __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#অন্যথা
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#endif


#যদি defined strdupa
# define ASSIGN_STRDUPA(DEST, S)		\
  করো { DEST = strdupa (S); } যখন (0)
#অন্যথা
# define ASSIGN_STRDUPA(DEST, S)		\
  করো						\
    {						\
      অক্ষর ধ্রুবক *s_ = (S);			\
      size_t len_ = strlen (s_) + 1;		\
      অক্ষর *tmp_dest_ = alloca (len_);		\
      DEST = memcpy (tmp_dest_, s_, len_);	\
    }						\
  যখন (0)
#endif

#যদি ! HAVE_SYNC
# define sync() /* empty */
#endif

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
স্থির অন্তঃরেখ size_t
gcd (size_t u, size_t v)
{
  করো
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  যখন (v);

  ফেরত u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
স্থির অন্তঃরেখ size_t
lcm (size_t u, size_t v)
{
  ফেরত u * (v / gcd (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

স্থির অন্তঃরেখ শূন্য *
ptr_align (শূন্য ধ্রুবক *ptr, size_t alignment)
{
  অক্ষর ধ্রুবক *p0 = ptr;
  অক্ষর ধ্রুবক *p1 = p0 + alignment - 1;
  ফেরত (শূন্য *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_PURE
স্থির অন্তঃরেখ bool
is_nul (শূন্য ধ্রুবক *buf, size_t দৈর্ঘ্য)
{
  ধ্রুবক unsigned অক্ষর *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#যদি 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  unsigned long word;
#অন্যথা
  unsigned অক্ষর word;
#endif

  যদি (! দৈর্ঘ্য)
    ফেরত true;

  /* Check len bytes not aligned on a word.  */
  যখন (UNLIKELY (দৈর্ঘ্য & (আকার word - 1)))
    {
      যদি (*p)
        ফেরত false;
      p++;
      দৈর্ঘ্য--;
      যদি (! দৈর্ঘ্য)
        ফেরত true;
   }

  /* Check up to 16 bytes a word at a time.  */
  জন্য (;;)
    {
      memcpy (&word, p, আকার word);
      যদি (word)
        ফেরত false;
      p += আকার word;
      দৈর্ঘ্য -= আকার word;
      যদি (! দৈর্ঘ্য)
        ফেরত true;
      যদি (UNLIKELY (দৈর্ঘ্য & 15) == 0)
        বিরতি;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  ফেরত memeq (buf, p, দৈর্ঘ্য);
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
স্থির অন্তঃরেখ শূন্য
oputs_ (MAYBE_UNUSED অক্ষর ধ্রুবক *program, অক্ষর ধ্রুবক *option)
{
  স্থির পূর্ণসংখ্যা help_markup =
#যদি defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#অন্যথা
    0;   /* Disable.  */
#endif
  যদি (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      অক্ষর ধ্রুবক *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != ফাঁকা);
    }
  যদি (! help_markup)
    {
      fputs (option, stdout);
      ফেরত;
    }

  bool double_space = true;
  অক্ষর ধ্রুবক *first_word = option + strspn (option, " \t\n");
  অক্ষর ধ্রুবক *option_text = strchr (option, '-');
  যদি (!option_text)                    /* for dd(1) option syntax.  */
    {
      option_text = first_word;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  অন্যথা যদি (option_text != first_word)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      অক্ষর ধ্রুবক *s = first_word;
      size_t spaces = 0;
      যখন (s < option_text && spaces < 2)
        spaces += !!isspace (to_uchar (*s++));
      যদি (spaces == 2)
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
  অক্ষর ধ্রুবক *desc_text = option_text + anchor_len;
  যখন (*desc_text && *desc_text != '\n')
    {
      যদি (*desc_text == '-' && *(desc_text + 1) == '-')
        double_space = false;
      যদি (isspace (to_uchar (*desc_text)))
        {
          যদি (*desc_text == '\t' || isspace (to_uchar (*(desc_text + 1))))
            বিরতি;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          যদি (!double_space && *(desc_text + 1) != '-')
            বিরতি;
        }

      desc_text++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, first_word - option, stdout);

  /* write option text.  */
#ifdef MANUAL_URL
  অক্ষর ধ্রুবক *url_program =   streq (program, "[") ? "test"
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
  যদি (STREQ_LEN (option_text, "--help", 6)
      || STREQ_LEN (option_text, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      printf ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_program, url_program, (পূর্ণসংখ্যা) anchor_len, option_text);
    }
  অন্যথা
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      printf ("\033]8;;%s#%s%.*s", MANUAL_URL, url_program,
              (পূর্ণসংখ্যা) anchor_len, option_text);
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
স্থির অন্তঃরেখ শূন্য
oprintf_ (অক্ষর ধ্রুবক *program, অক্ষর ধ্রুবক *message, ...)
{
  va_list args;
  অক্ষর *buf;
  পূর্ণসংখ্যা buflen = -1;

#যদি defined MANUAL_URL || defined BOLD_MAN_REFS
  va_start (args, message);
  buflen = vasprintf (&buf, message, args);
  va_end (args);
#endif

  যদি (buflen < 0)
    {
      va_start (args, message);
      vprintf (message, args);
      va_end (args);
      ফেরত;
    }

  oputs_ (program, buf);
  মুক্ত (buf);
}

স্থির অন্তঃরেখ শূন্য
emit_stdin_note (শূন্য)
{
  fputs (_("\n\
With no FILE, or when FILE is -, পঠ standard প্রবেশ.\n\
"), stdout);
}
স্থির অন্তঃরেখ শূন্য
emit_mandatory_arg_note (শূন্য)
{
  fputs (_("\n\
Mandatory arguments to long options are mandatory জন্য short options too.\n\
"), stdout);
}

স্থির অন্তঃরেখ শূন্য
emit_আকার_note (শূন্য)
{
  fputs (_("\n\
The আকার_3 argument is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers of 1024) or KB,MB,... (powers of 1000).\n\
দ্বিমিক_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

স্থির অন্তঃরেখ শূন্য
emit_blocksize_note (অক্ষর ধ্রুবক *program)
{
  printf (_("\n\
Display মানসমূহ are in units of the first available আকার_3 from --block-আকার_2,\n\
and the %s_BLOCK_আকার, BLOCK_আকার and BLOCKSIZE environment variables.\n\
Otherwise, units পূর্বনির্ধারিত to 1024 bytes (or 512 যদি POSIXLY_CORRECT is set).\n\
"), program);
}

স্থির অন্তঃরেখ শূন্য
emit_update_parameters_note (শূন্য)
{
  fputs (_("\
\n\
UPDATE controls which existing files in the destination are replaced.\n\
'all' is the পূর্বনির্ধারিত operation when an --update option is not specified,\n\
and results in all existing files in the destination being replaced.\n\
'none' is like the --no-clobber option, in that no files in the\n\
destination are replaced, and skipped files করো not induce a failure.\n\
'none-fail' also ensures no files are replaced in the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the পূর্বনির্ধারিত operation when --update is specified, and results\n\
in files being replaced যদি they're older than the corresponding source file.\n\
"), stdout);
}

স্থির অন্তঃরেখ শূন্য
emit_backup_suffix_note (শূন্য)
{
  fputs (_("\
\n\
The backup suffix is '~', unless set with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The version control method may be selected via the --backup option or through\n\
the VERSION_CONTROL environment variable.  Here are the মানসমূহ:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even যদি --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered যদি numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#define emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (PROGRAM_NAME, default_opt)
স্থির অন্তঃরেখ শূন্য
emit_symlink_recurse_options_ (অক্ষর ধ্রুবক *program, অক্ষর ধ্রুবক *default_opt)
{
      printf (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If more than one is specified, only the final\n\
one takes effect. %s is the পূর্বনির্ধারিত.\n\
\n\
"), default_opt);
      oputs_ (program, _("\
  -H\n\
         যদি a command সারি argument is a symlink to a directory, traverse it\n\
"));
      oputs_ (program, _("\
  -L\n\
         traverse every symbolic link to a directory encountered\n\
"));
      oputs_ (program, _("\
  -P\n\
         করো not traverse any symbolic links\n\
\n\
"));
}

স্থির অন্তঃরেখ শূন্য
emit_exec_status (অক্ষর ধ্রুবক *program)
{
      printf (_("\n\
Exit status:\n\
  125  যদি the %s command itself fails\n\
  126  যদি COMMAND is found but cannot be invoked\n\
  127  যদি COMMAND cannot be found\n\
  -    the exit status of COMMAND otherwise\n\
"), program);
}

স্থির অন্তঃরেখ শূন্য
emit_ancillary_info (অক্ষর ধ্রুবক *program)
{
  কাঠামো infomap { অক্ষর ধ্রুবক *program; অক্ষর ধ্রুবক *গ্রন্থি; } ধ্রুবক infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { ফাঁকা, ফাঁকা }
  };

  অক্ষর ধ্রুবক *গ্রন্থি = program;
  কাঠামো infomap ধ্রুবক *map_prog = infomap;

  যখন (map_prog->program && ! streq (program, map_prog->program))
    map_prog++;

  যদি (map_prog->গ্রন্থি)
    গ্রন্থি = map_prog->গ্রন্থি;

  emit_bug_reporting_address ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  অক্ষর ধ্রুবক *url_program = streq (program, "[") ? "test" : program;
  printf (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_program);
  printf (_("or available locally via: info '(coreutils) %s%s'\n"),
          গ্রন্থি, গ্রন্থি == program ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#define emit_try_help() \
  করো \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               program_name); \
    } \
  যখন (0)

#অন্তর্ভাব "inttostr.h"

স্থির অন্তঃরেখ অক্ষর *
timetostr (time_t t, অক্ষর *buf)
{
  ফেরত (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

স্থির অন্তঃরেখ অক্ষর *
bad_cast (অক্ষর ধ্রুবক *s)
{
  ফেরত (অক্ষর *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
স্থির অন্তঃরেখ bool
usable_st_আকার (কাঠামো stat ধ্রুবক *sb)
{
  ফেরত (S_ISREG (sb->st_mode) || S_ISLNK (sb->st_mode)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn শূন্য usage (পূর্ণসংখ্যা status);

#অন্তর্ভাব <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#define devmsg(...)			\
  করো					\
    {					\
      যদি (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  যখন (0)

#define emit_cycle_warning(file_name)	\
  করো					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part of the cycle:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  যখন (0)

/* exit with a _single_ "write error" diagnostic.  */

স্থির অন্তঃরেখ শূন্য
write_error (শূন্য)
{
  পূর্ণসংখ্যা saved_errno = errno;
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
স্থির অন্তঃরেখ অক্ষর *
stzncpy (অক্ষর *restrict dest, অক্ষর ধ্রুবক *restrict src, size_t len)
{
  জন্য (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  ফেরত dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
স্থির অন্তঃরেখ bool
is_ENOTSUP (পূর্ণসংখ্যা err)
{
  ফেরত err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#অন্তর্ভাব "quotearg.h"

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
#define quoteN(x) quotearg_style (get_quoting_style (ফাঁকা), x)
#define quoteN_mem(x, আকার_2)                                     \
  quotearg_style_mem (get_quoting_style (ফাঁকা), x, আকার_2)

#ifdef ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
getenv_quoting_style (শূন্য)
{
  অক্ষর ধ্রুবক *q_style = getenv ("QUOTING_STYLE");
  যদি (!q_style)
    ফেরত -1;

  পূর্ণসংখ্যা i = ARGMATCH (q_style, quoting_style_args, quoting_style_vals);
  যদি (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      ফেরত -1;
    }
  ফেরত quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
স্থির অন্তঃরেখ ptrdiff_t
x_timestyle_match (অক্ষর ধ্রুবক * style, bool allow_posix,
                   অক্ষর ধ্রুবক *ধ্রুবক * timestyle_args,
                   অক্ষর ধ্রুবক * timestyle_types,
                   size_t timestyle_types_আকার,
                   পূর্ণসংখ্যা fail_status)
{
  ptrdiff_t res = argmatch (style, timestyle_args,
                            timestyle_types,
                            timestyle_types_আকার);
  যদি (res < 0)
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
      অক্ষর ধ্রুবক *ধ্রুবক *p = timestyle_args;
      অক্ষর ধ্রুবক *posix_prefix = allow_posix ? "[posix-]" : "";
      যখন (*p)
        fprintf (stderr, "  - %s%s\n", posix_prefix, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_status);
    }

  ফেরত res;
}
#endif
