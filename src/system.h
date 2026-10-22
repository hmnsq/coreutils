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

#अन्तर्भाव <attribute.h>

#अन्तर्भाव <alloca.h>

#अन्तर्भाव <ctype.h>

#अन्तर्भाव <sys/stat.h>

/* Commonly used file permission combination.  */
#परिभाषय बहुलकम्_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#यदि HAVE_SYS_PARAM_H
# include <sys/param.h>
#यदि_अन्तः

#अन्तर्भाव <unistd.h>

#अन्तर्भाव <limits.h>

#अन्तर्भाव "pathmax.h"
#यदि_अपरिभाषितम् PATH_MAX
# define PATH_MAX 8192
#यदि_अन्तः

#अन्तर्भाव "configmake.h"

#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#यदि MAJOR_अन्तर्_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#यदि_अन्तः
#यदि MAJOR_अन्तर्_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#यदि_अन्तः
#यदि_परिभाषितम् major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#यदि_अन्तः

#यदि_अपरिभाषितम् HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#यदि_अन्तः
#अपरिभाषय HAVE_MAJOR

#यदि ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#यदि_अन्तः

#अन्तर्भाव <stdckdint.h>
#अन्तर्भाव <stdcountof.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <uchar.h>
#अन्तर्भाव <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#यदि_अपरिभाषितम् ENODATA
# define ENODATA (-1)
#यदि_अन्तः

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
स्थिर अन्तःस्थ bool
ignorable_traversal_errno (पूर्णाङ्क errnum) { प्रत्यागम errnum == ENOENT; }

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
गणना
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#अन्तर्भाव "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
स्थिर अन्तःस्थ शून्य
initialize_exit_failure (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_FAILURE)
    exit_failure = स्थितिः;
}

#अन्तर्भाव <fcntl.h>
#यदि_परिभाषितम् O_मार्गः
गणना { O_PATHSEARCH = O_मार्गः };
#अन्यथा
गणना { O_PATHSEARCH = O_अन्वेषणम् };
#यदि_अन्तः

#अन्तर्भाव <dirent.h>
#यदि_अपरिभाषितम् _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) वाक्यदैर्घ्यम् ((dp)->d_name)
#यदि_अन्तः

गणना
{
  NOT_AN_INODE_सङ्ख्या = 0
};

#यदि_परिभाषितम् D_INO_अन्तर्_DIRENT
# define D_INO(dp) (dp)->d_ino
#अन्यथा
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_सङ्ख्या
#यदि_अन्तः

/* include here for SIZE_MAX.  */
#अन्तर्भाव <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#यदि_अपरिभाषितम् initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# अन्यथा
#  define initialize_main(ac, av) \
     कुरु { _wildcard (ac, av); _response (ac, av); } यावत् (0)
# endif
#यदि_अन्तः

#अन्तर्भाव "stat-macros.h"

#अन्तर्भाव "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
स्थिर अन्तःस्थ अचिह्नित वर्ण गन्तव्यम्_uchar (वर्ण ch) { प्रत्यागम ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_विशुद्धम्
स्थिर अन्तःस्थ पूर्णाङ्क
c32isnbspace (char32_t wc)
{
  प्रत्यागम wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_विशुद्धम्
स्थिर अन्तःस्थ पूर्णाङ्क
c32isnewline (char32_t wc)
{
  प्रत्यागम wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_विशुद्धम्
स्थिर अन्तःस्थ पूर्णाङ्क
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  प्रत्यागम c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_विशुद्धम्
स्थिर अन्तःस्थ bool
c32issep (char32_t wc)
{
#यदि defined __GLIBC__
  प्रत्यागम !! c32isblank (wc);
#अन्यथा
  प्रत्यागम c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#यदि_अन्तः
}

#अन्तर्भाव <locale.h>

/* Take care of NLS matters.  */

#अन्तर्भाव "gettext.h"
#यदि ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /*रिक्तानि*/
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /*रिक्तानि*/
#यदि_अन्तः

#परिभाषय _(msgid) gettext (msgid)
#परिभाषय N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
स्थिर अन्तःस्थ अचिह्नित दीर्घ पूर्णाङ्क
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  गणना { PLURAL_REDUCER = 1000000 };
  प्रत्यागम (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#परिभाषय STREQ_LEN(a, b, n) (वाक्ये_तुलय (a, b, n) == 0) /* n==-1 means unbounded */
#परिभाषय STRPREFIX(a, b) (वाक्ये_तुलय (a, b, वाक्यदैर्घ्यम् (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#परिभाषय STRNCMP_LIT(s, lit) वाक्ये_तुलय (s, "" lit "", परिमाण (lit) - 1)

#यदि !HAVE_DECL_GETLOGIN
वर्ण *getlogin (शून्य);
#यदि_अन्तः

#यदि !HAVE_DECL_TTYNAME
वर्ण *ttyname (पूर्णाङ्क);
#यदि_अन्तः

#यदि !HAVE_DECL_GETEUID
uid_t geteuid (शून्य);
#यदि_अन्तः

#यदि !HAVE_DECL_GETPWUID
संरचना passwd *getpwuid (uid_t);
#यदि_अन्तः

#यदि !HAVE_DECL_GETGRGID
संरचना गणः *getgrgid (gid_t);
#यदि_अन्तः

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#यदि ! HAVE_SETGROUPS
# यदि HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# यदि HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# यदि HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#यदि_अन्तः

#यदि !HAVE_DECL_GETUID
uid_t getuid (शून्य);
#यदि_अन्तः

#अन्तर्भाव "idx.h"
#अन्तर्भाव "xalloc.h"
#अन्तर्भाव "verify.h"
#अन्तर्भाव "unlocked-io.h"
#अन्तर्भाव "same-inode.h"

#अन्तर्भाव "dirname.h"
#अन्तर्भाव "openat.h"

स्थिर अन्तःस्थ bool
dot_or_dotdot (वर्ण ध्रुव *file_name)
{
  यदि (file_name[0] == '.')
    {
      वर्ण sep = file_name[(file_name[1] == '.') + 1];
      प्रत्यागम (! sep || ISSLASH (sep));
    }
  अन्यथा
    प्रत्यागम false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
स्थिर अन्तःस्थ संरचना dirent ध्रुव *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  यावत् (true)
    {
      संरचना dirent ध्रुव *dp = readdir (dirp);
      यदि (dp == रिक्त || ! dot_or_dotdot (dp->d_name))
        प्रत्यागम dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
गणना {
    DS_UNKNOWN = -2,
    DS_रिक्तानि = -1,
    DS_NONEMPTY = 0,
};
स्थिर अन्तःस्थ पूर्णाङ्क
directory_स्थितिः (पूर्णाङ्क fd_cwd, वर्ण ध्रुव *dir)
{
  DIR *dirp;
  bool no_direntries;
  पूर्णाङ्क saved_errno;
  पूर्णाङ्क fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  यदि (fd < 0)
    प्रत्यागम errno;

  dirp = fdopendir (fd);
  यदि (dirp == रिक्त)
    {
      saved_errno = errno;
      close (fd);
      प्रत्यागम saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  प्रत्यागम no_direntries && saved_errno == 0 ? DS_रिक्तानि : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
गणना
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_संस्करणम्_CHAR = (CHAR_MIN - 3)
};

#परिभाषय GETOPT_HELP_OPTION_DECL \
  "help", no_तर्कः, रिक्त, GETOPT_HELP_CHAR
#परिभाषय GETOPT_संस्करणम्_OPTION_DECL \
  "version", no_तर्कः, रिक्त, GETOPT_संस्करणम्_CHAR
#परिभाषय GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_तर्कः, रिक्त, 'Z'

#परिभाषय case_GETOPT_HELP_CHAR			\
  पक्ष GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    भङ्ग;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#परिभाषय USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#परिभाषय HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#परिभाषय संस्करणम्_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#अन्तर्भाव "closein.h"
#अन्तर्भाव "closeout.h"

#अन्तर्भाव "version-etc.h"

#अन्तर्भाव "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#परिभाषय proper_name(x) proper_name_lite (x, x)

#अन्तर्भाव "progname.h"

#परिभाषय case_GETOPT_संस्करणम्_CHAR(क्रमादेशः_name_2, Authors)			\
  पक्ष GETOPT_संस्करणम्_CHAR:						\
    संस्करणम्_etc (stdout, क्रमादेशः_name_2, PACKAGE_NAME, संस्करणम्, Authors,	\
                 (वर्ण *) रिक्त);					\
    निर्गच्छ (EXIT_SUCCESS);						\
    भङ्ग;

#अन्तर्भाव "minmax.h"
#अन्तर्भाव "intprops.h"

#यदि_अपरिभाषितम् SSIZE_MAX
# define SSIZE_MAX TYPE_अधिकतमम् (ssize_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् OFF_T_MAX
# define OFF_T_MAX TYPE_अधिकतमम् (off_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् UID_T_MAX
# define UID_T_MAX TYPE_अधिकतमम् (uid_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् GID_T_MAX
# define GID_T_MAX TYPE_अधिकतमम् (gid_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् PID_T_MAX
# define PID_T_MAX TYPE_अधिकतमम् (pid_t)
#यदि_अन्तः

/* Use this to suppress gcc warnings.  */
#यदि_परिभाषितम् lint
# define IF_LINT(संहिता_2) संहिता_2
#अन्यथा
# define IF_LINT(संहिता_2) /*रिक्तानि*/
#यदि_अन्तः

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#यदि_परिभाषितम् lint
# define main_exit(स्थितिः) निर्गच्छ (स्थितिः)
#अन्यथा
# define main_exit(स्थितिः) प्रत्यागम स्थितिः
#यदि_अन्तः

#यदि_परिभाषितम् __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#अन्यथा
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#यदि_अन्तः


#यदि defined strdupa
# define नियोजय_STRDUPA(DEST, S)		\
  कुरु { DEST = strdupa (S); } यावत् (0)
#अन्यथा
# define नियोजय_STRDUPA(DEST, S)		\
  कुरु						\
    {						\
      वर्ण ध्रुव *s_ = (S);			\
      size_t len_ = वाक्यदैर्घ्यम् (s_) + 1;		\
      वर्ण *tmp_dest_ = alloca (len_);		\
      DEST = स्मृतिं_प्रतिलिख (tmp_dest_, s_, len_);	\
    }						\
  यावत् (0)
#यदि_अन्तः

#यदि ! HAVE_SYNC
# define sync() /*रिक्तानि*/
#यदि_अन्तः

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
स्थिर अन्तःस्थ size_t
gcd (size_t u, size_t v)
{
  कुरु
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  यावत् (v);

  प्रत्यागम u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
स्थिर अन्तःस्थ size_t
lcm (size_t u, size_t v)
{
  प्रत्यागम u * (v / gcd (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

स्थिर अन्तःस्थ शून्य *
ptr_align (शून्य ध्रुव *ptr, size_t alignment)
{
  वर्ण ध्रुव *p0 = ptr;
  वर्ण ध्रुव *p1 = p0 + alignment - 1;
  प्रत्यागम (शून्य *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_विशुद्धम्
स्थिर अन्तःस्थ bool
is_nul (शून्य ध्रुव *buf, size_t दीर्घता)
{
  ध्रुव अचिह्नित वर्ण *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#यदि 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  अचिह्नित दीर्घ word;
#अन्यथा
  अचिह्नित वर्ण word;
#यदि_अन्तः

  यदि (! दीर्घता)
    प्रत्यागम true;

  /* Check len bytes not aligned on a word.  */
  यावत् (UNLIKELY (दीर्घता & (परिमाण word - 1)))
    {
      यदि (*p)
        प्रत्यागम false;
      p++;
      दीर्घता--;
      यदि (! दीर्घता)
        प्रत्यागम true;
   }

  /* Check up to 16 bytes a word at a time.  */
  प्रति (;;)
    {
      स्मृतिं_प्रतिलिख (&word, p, परिमाण word);
      यदि (word)
        प्रत्यागम false;
      p += परिमाण word;
      दीर्घता -= परिमाण word;
      यदि (! दीर्घता)
        प्रत्यागम true;
      यदि (UNLIKELY (दीर्घता & 15) == 0)
        भङ्ग;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  प्रत्यागम memeq (buf, p, दीर्घता);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#परिभाषय DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_सङ्कलय (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#परिभाषय oputs(option) oputs_ (क्रमादेशः_NAME, option)
स्थिर अन्तःस्थ शून्य
oputs_ (MAYBE_अप्रयुक्तम् वर्ण ध्रुव *क्रमादेशः, वर्ण ध्रुव *option)
{
  स्थिर पूर्णाङ्क help_markup =
#यदि defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#अन्यथा
    0;   /* Disable.  */
#यदि_अन्तः
  यदि (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      वर्ण ध्रुव *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != रिक्त);
    }
  यदि (! help_markup)
    {
      fputs (option, stdout);
      प्रत्यागम;
    }

  bool double_space = true;
  वर्ण ध्रुव *first_word = option + strspn (option, " \t\n");
  वर्ण ध्रुव *option_text = strchr (option, '-');
  यदि (!option_text)                    /* for dd(1) option syntax.  */
    {
      option_text = first_word;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  अन्यथा यदि (option_text != first_word)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      वर्ण ध्रुव *s = first_word;
      size_t spaces = 0;
      यावत् (s < option_text && spaces < 2)
        spaces += !!रिक्तम्_वा (गन्तव्यम्_uchar (*s++));
      यदि (spaces == 2)
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
  वर्ण ध्रुव *desc_text = option_text + anchor_len;
  यावत् (*desc_text && *desc_text != '\n')
    {
      यदि (*desc_text == '-' && *(desc_text + 1) == '-')
        double_space = false;
      यदि (रिक्तम्_वा (गन्तव्यम्_uchar (*desc_text)))
        {
          यदि (*desc_text == '\t' || रिक्तम्_वा (गन्तव्यम्_uchar (*(desc_text + 1))))
            भङ्ग;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          यदि (!double_space && *(desc_text + 1) != '-')
            भङ्ग;
        }

      desc_text++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, first_word - option, stdout);

  /* write option text.  */
#यदि_परिभाषितम् MANUAL_URL
  वर्ण ध्रुव *url_क्रमादेशः =   streq (क्रमादेशः, "[") ? "test"
                            : streq (क्रमादेशः, "dir") ? "ls"
                            : streq (क्रमादेशः, "vdir") ? "ls"
                            : streq (क्रमादेशः, "b2sum") ? "cksum"
                            : streq (क्रमादेशः, "md5sum") ? "cksum"
                            : streq (क्रमादेशः, "sha1sum") ? "cksum"
                            : streq (क्रमादेशः, "sha224sum") ? "cksum"
                            : streq (क्रमादेशः, "sha256sum") ? "cksum"
                            : streq (क्रमादेशः, "sha384sum") ? "cksum"
                            : streq (क्रमादेशः, "sha512sum") ? "cksum"
                            : क्रमादेशः;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  यदि (STREQ_LEN (option_text, "--help", 6)
      || STREQ_LEN (option_text, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      प्रारूप_लिख ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_क्रमादेशः, url_क्रमादेशः, (पूर्णाङ्क) anchor_len, option_text);
    }
  अन्यथा
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      प्रारूप_लिख ("\033]8;;%s#%s%.*s", MANUAL_URL, url_क्रमादेशः,
              (पूर्णाङ्क) anchor_len, option_text);
    }
  fputs ("\033\\", stdout);
#यदि_अन्तः
#यदि_परिभाषितम् BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#यदि_अन्तः
  /* first_word != option_text for test(1).  */
  fwrite (first_word, 1, desc_text - first_word, stdout);
#यदि_परिभाषितम् BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#यदि_अन्तः
#यदि_परिभाषितम् MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#यदि_अन्तः

  /* write description.  */
  fputs (desc_text, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#परिभाषय oprintf(...) oprintf_ (क्रमादेशः_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((प्रारूप_लिख, 2, 3))
स्थिर अन्तःस्थ शून्य
oprintf_ (वर्ण ध्रुव *क्रमादेशः, वर्ण ध्रुव *सन्देशः_2, ...)
{
  va_list तर्काः;
  वर्ण *buf;
  पूर्णाङ्क buflen = -1;

#यदि defined MANUAL_URL || defined BOLD_MAN_REFS
  va_start (तर्काः, सन्देशः_2);
  buflen = vasprintf (&buf, सन्देशः_2, तर्काः);
  va_अन्तः (तर्काः);
#यदि_अन्तः

  यदि (buflen < 0)
    {
      va_start (तर्काः, सन्देशः_2);
      vprintf (सन्देशः_2, तर्काः);
      va_अन्तः (तर्काः);
      प्रत्यागम;
    }

  oputs_ (क्रमादेशः, buf);
  मुक्त (buf);
}

स्थिर अन्तःस्थ शून्य
emit_stdin_note (शून्य)
{
  fputs (_("\n\
With no FILE, or when FILE is -, पठ standard निवेशः.\n\
"), stdout);
}
स्थिर अन्तःस्थ शून्य
emit_mandatory_arg_note (शून्य)
{
  fputs (_("\n\
Mandatory arguments गन्तव्यम् दीर्घ options are mandatory प्रति ह्रस्व options too.\n\
"), stdout);
}

स्थिर अन्तःस्थ शून्य
emit_आकार_note (शून्य)
{
  fputs (_("\n\
The आकार_2 तर्कः is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers सम्बन्धि 1024) or KB,MB,... (powers सम्बन्धि 1000).\n\
द्विमान_2 prefixes can be प्रयुक्तम्, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

स्थिर अन्तःस्थ शून्य
emit_blocksize_note (वर्ण ध्रुव *क्रमादेशः)
{
  प्रारूप_लिख (_("\n\
Display मानानि are अन्तर् units सम्बन्धि the first उपलब्धम् आकार_2 from --खण्डम्-आकार,\n\
and the %s_खण्डम्_आकार, खण्डम्_आकार_2 and BLOCKSIZE environment variables.\n\
Otherwise, units स्वभाव गन्तव्यम् 1024 अष्टकानि (or 512 यदि POSIXLY_CORRECT is समुच्चयः_2).\n\
"), क्रमादेशः);
}

स्थिर अन्तःस्थ शून्य
emit_update_parameters_note (शून्य)
{
  fputs (_("\
\n\
UPDATE controls which existing files अन्तर् the destination are replaced.\n\
'all' is the स्वभाव operation when an --update option is not specified,\n\
and results अन्तर् all existing files अन्तर् the destination being replaced.\n\
'none' is like the --no-clobber option, अन्तर् that no files अन्तर् the\n\
destination are replaced, and skipped files कुरु not induce a failure.\n\
'none-fail' also ensures no files are replaced अन्तर् the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the स्वभाव operation when --update is specified, and results\n\
अन्तर् files being replaced यदि they're older than the corresponding source file.\n\
"), stdout);
}

स्थिर अन्तःस्थ शून्य
emit_backup_suffix_note (शून्य)
{
  fputs (_("\
\n\
The backup suffix is '~', unless समुच्चयः_2 with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The संस्करणम्_3 control विधानम्_2 may be selected द्वारा the --backup option or through\n\
the संस्करणम्_CONTROL environment variable.  अत्र_2 are the मानानि:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even यदि --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered यदि numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#परिभाषय emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (क्रमादेशः_NAME, default_opt)
स्थिर अन्तःस्थ शून्य
emit_symlink_recurse_options_ (वर्ण ध्रुव *क्रमादेशः, वर्ण ध्रुव *default_opt)
{
      प्रारूप_लिख (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If अधिकम् than one is specified, only the final\n\
one takes effect. %s is the स्वभाव.\n\
\n\
"), default_opt);
      oputs_ (क्रमादेशः, _("\
  -H\n\
         यदि a command पङ्क्तिः तर्कः is a symlink गन्तव्यम् a directory, traverse it\n\
"));
      oputs_ (क्रमादेशः, _("\
  -L\n\
         traverse every symbolic link गन्तव्यम् a directory encountered\n\
"));
      oputs_ (क्रमादेशः, _("\
  -P\n\
         कुरु not traverse any symbolic links\n\
\n\
"));
}

स्थिर अन्तःस्थ शून्य
emit_exec_स्थितिः (वर्ण ध्रुव *क्रमादेशः)
{
      प्रारूप_लिख (_("\n\
Exit स्थितिः:\n\
  125  यदि the %s command itself fails\n\
  126  यदि COMMAND is प्राप्तम् but cannot be invoked\n\
  127  यदि COMMAND cannot be प्राप्तम्\n\
  -    the निर्गच्छ स्थितिः सम्बन्धि COMMAND otherwise\n\
"), क्रमादेशः);
}

स्थिर अन्तःस्थ शून्य
emit_ancillary_info (वर्ण ध्रुव *क्रमादेशः)
{
  संरचना infomap { वर्ण ध्रुव *क्रमादेशः; वर्ण ध्रुव *ग्रन्थिः; } ध्रुव infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { रिक्त, रिक्त }
  };

  वर्ण ध्रुव *ग्रन्थिः = क्रमादेशः;
  संरचना infomap ध्रुव *map_prog = infomap;

  यावत् (map_prog->क्रमादेशः && ! streq (क्रमादेशः, map_prog->क्रमादेशः))
    map_prog++;

  यदि (map_prog->ग्रन्थिः)
    ग्रन्थिः = map_prog->ग्रन्थिः;

  emit_bug_reporting_स्थानाङ्कः ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  वर्ण ध्रुव *url_क्रमादेशः = streq (क्रमादेशः, "[") ? "test" : क्रमादेशः;
  प्रारूप_लिख (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_क्रमादेशः);
  प्रारूप_लिख (_("or available locally via: info '(coreutils) %s%s'\n"),
          ग्रन्थिः, ग्रन्थिः == क्रमादेशः ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#परिभाषय emit_try_help() \
  कुरु \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               क्रमादेशः_name); \
    } \
  यावत् (0)

#अन्तर्भाव "inttostr.h"

स्थिर अन्तःस्थ वर्ण *
timetostr (time_t t, वर्ण *buf)
{
  प्रत्यागम (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

स्थिर अन्तःस्थ वर्ण *
bad_cast (वर्ण ध्रुव *s)
{
  प्रत्यागम (वर्ण *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
स्थिर अन्तःस्थ bool
usable_st_आकार (संरचना stat ध्रुव *sb)
{
  प्रत्यागम (S_ISREG (sb->st_बहुलकम्) || S_ISLNK (sb->st_बहुलकम्)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn शून्य usage (पूर्णाङ्क स्थितिः);

#अन्तर्भाव <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#परिभाषय devmsg(...)			\
  कुरु					\
    {					\
      यदि (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  यावत् (0)

#परिभाषय emit_आवृत्तिः_warning(file_name)	\
  कुरु					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part सम्बन्धि the आवृत्तिः:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  यावत् (0)

/* exit with a _single_ "write error" diagnostic.  */

स्थिर अन्तःस्थ शून्य
write_error (शून्य)
{
  पूर्णाङ्क saved_errno = errno;
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
स्थिर अन्तःस्थ वर्ण *
stzncpy (वर्ण *सीमित dest, वर्ण ध्रुव *सीमित src, size_t len)
{
  प्रति (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  प्रत्यागम dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
स्थिर अन्तःस्थ bool
is_ENOTSUP (पूर्णाङ्क err)
{
  प्रत्यागम err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#अन्तर्भाव "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#परिभाषय quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#परिभाषय quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#परिभाषय quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#परिभाषय quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#परिभाषय quoteN(x) quotearg_style (get_quoting_style (रिक्त), x)
#परिभाषय quoteN_mem(x, आकार)                                     \
  quotearg_style_mem (get_quoting_style (रिक्त), x, आकार)

#यदि_परिभाषितम् ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
स्थिर अन्तःस्थ पूर्णाङ्क
getenv_quoting_style (शून्य)
{
  वर्ण ध्रुव *q_style = getenv ("QUOTING_STYLE");
  यदि (!q_style)
    प्रत्यागम -1;

  पूर्णाङ्क i = ARGMATCH (q_style, quoting_style_तर्काः, quoting_style_vals);
  यदि (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      प्रत्यागम -1;
    }
  प्रत्यागम quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
स्थिर अन्तःस्थ ptrdiff_t
x_timestyle_मेलनम् (वर्ण ध्रुव * style, bool allow_posix,
                   वर्ण ध्रुव *ध्रुव * timestyle_तर्काः,
                   वर्ण ध्रुव * timestyle_types,
                   size_t timestyle_types_आकार,
                   पूर्णाङ्क fail_स्थितिः)
{
  ptrdiff_t res = argmatch (style, timestyle_तर्काः,
                            timestyle_types,
                            timestyle_types_आकार);
  यदि (res < 0)
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
      वर्ण ध्रुव *ध्रुव *p = timestyle_तर्काः;
      वर्ण ध्रुव *posix_उपसर्गः = allow_posix ? "[posix-]" : "";
      यावत् (*p)
        fprintf (stderr, "  - %s%s\n", posix_उपसर्गः, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_स्थितिः);
    }

  प्रत्यागम res;
}
#यदि_अन्तः
