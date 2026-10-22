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

#ضمن <attribute.h>

#ضمن <alloca.h>

#ضمن <ctype.h>

#ضمن <sys/stat.h>

/* Commonly used file permission combination.  */
#تعریف_کن نما_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#إذا HAVE_SYS_PARAM_H
# include <sys/param.h>
#پایان_اگر

#ضمن <unistd.h>

#ضمن <limits.h>

#ضمن "pathmax.h"
#اگر_تعریف_نشده PATH_MAX
# define PATH_MAX 8192
#پایان_اگر

#ضمن "configmake.h"

#ضمن <sys/time.h>
#ضمن <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#إذا MAJOR_درون_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#پایان_اگر
#إذا MAJOR_درون_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#پایان_اگر
#اگر_تعریف_شده major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#پایان_اگر

#اگر_تعریف_نشده HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#پایان_اگر
#لغو_تعریف HAVE_MAJOR

#إذا ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#پایان_اگر

#ضمن <stdckdint.h>
#ضمن <stdcountof.h>
#ضمن <stddef.h>
#ضمن <string.h>
#ضمن <uchar.h>
#ضمن <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#اگر_تعریف_نشده ENODATA
# define ENODATA (-1)
#پایان_اگر

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
ساكن مضمن bool
ignorable_traversal_errno (صحيح errnum) { أرجع errnum == ENOENT; }

#ضمن <stdlib.h>
#ضمن "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
تعداد
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#ضمن "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
ساكن مضمن فراغ
initialize_exit_failure (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_FAILURE)
    exit_failure = وضعیت;
}

#ضمن <fcntl.h>
#اگر_تعریف_شده O_مسیر
تعداد { O_PATHSEARCH = O_مسیر };
#وإلا
تعداد { O_PATHSEARCH = O_جستجو };
#پایان_اگر

#ضمن <dirent.h>
#اگر_تعریف_نشده _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) طول_رشته ((dp)->d_name)
#پایان_اگر

تعداد
{
  NOT_AN_INODE_عدد = 0
};

#اگر_تعریف_شده D_INO_درون_DIRENT
# define D_INO(dp) (dp)->d_ino
#وإلا
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_عدد
#پایان_اگر

/* include here for SIZE_MAX.  */
#ضمن <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#اگر_تعریف_نشده initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# وإلا
#  define initialize_main(ac, av) \
     افعل { _wildcard (ac, av); _response (ac, av); } طالما (0)
# endif
#پایان_اگر

#ضمن "stat-macros.h"

#ضمن "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
ساكن مضمن بیعلامت حرف مقصد_uchar (حرف ch) { أرجع ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_خالص
ساكن مضمن صحيح
c32isnbspace (char32_t wc)
{
  أرجع wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_خالص
ساكن مضمن صحيح
c32isnewline (char32_t wc)
{
  أرجع wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_خالص
ساكن مضمن صحيح
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  أرجع c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_خالص
ساكن مضمن bool
c32issep (char32_t wc)
{
#إذا defined __GLIBC__
  أرجع !! c32isblank (wc);
#وإلا
  أرجع c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#پایان_اگر
}

#ضمن <locale.h>

/* Take care of NLS matters.  */

#ضمن "gettext.h"
#إذا ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /*خالی*/
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /*خالی*/
#پایان_اگر

#تعریف_کن _(msgid) gettext (msgid)
#تعریف_کن N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
ساكن مضمن بیعلامت دراز صحيح
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  تعداد { PLURAL_REDUCER = 1000000 };
  أرجع (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#تعریف_کن STREQ_LEN(a, b, n) (مقایسه_رشته (a, b, n) == 0) /* n==-1 means unbounded */
#تعریف_کن STRPREFIX(a, b) (مقایسه_رشته (a, b, طول_رشته (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#تعریف_کن STRNCMP_LIT(s, lit) مقایسه_رشته (s, "" lit "", حجم (lit) - 1)

#إذا !HAVE_DECL_GETLOGIN
حرف *getlogin (فراغ);
#پایان_اگر

#إذا !HAVE_DECL_TTYNAME
حرف *ttyname (صحيح);
#پایان_اگر

#إذا !HAVE_DECL_GETEUID
uid_t geteuid (فراغ);
#پایان_اگر

#إذا !HAVE_DECL_GETPWUID
بنية passwd *getpwuid (uid_t);
#پایان_اگر

#إذا !HAVE_DECL_GETGRGID
بنية گروه *getgrgid (gid_t);
#پایان_اگر

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#إذا ! HAVE_SETGROUPS
# إذا HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# إذا HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# إذا HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#پایان_اگر

#إذا !HAVE_DECL_GETUID
uid_t getuid (فراغ);
#پایان_اگر

#ضمن "idx.h"
#ضمن "xalloc.h"
#ضمن "verify.h"
#ضمن "unlocked-io.h"
#ضمن "same-inode.h"

#ضمن "dirname.h"
#ضمن "openat.h"

ساكن مضمن bool
dot_or_dotdot (حرف ثابت *file_name)
{
  إذا (file_name[0] == '.')
    {
      حرف sep = file_name[(file_name[1] == '.') + 1];
      أرجع (! sep || ISSLASH (sep));
    }
  وإلا
    أرجع false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
ساكن مضمن بنية dirent ثابت *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  طالما (true)
    {
      بنية dirent ثابت *dp = readdir (dirp);
      إذا (dp == عدم || ! dot_or_dotdot (dp->d_name))
        أرجع dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
تعداد {
    DS_UNKNOWN = -2,
    DS_خالی = -1,
    DS_NONEMPTY = 0,
};
ساكن مضمن صحيح
directory_وضعیت (صحيح fd_cwd, حرف ثابت *dir)
{
  DIR *dirp;
  bool no_direntries;
  صحيح saved_errno;
  صحيح fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  إذا (fd < 0)
    أرجع errno;

  dirp = fdopendir (fd);
  إذا (dirp == عدم)
    {
      saved_errno = errno;
      close (fd);
      أرجع saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  أرجع no_direntries && saved_errno == 0 ? DS_خالی : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
تعداد
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_نسخه_CHAR = (CHAR_MIN - 3)
};

#تعریف_کن GETOPT_HELP_OPTION_DECL \
  "help", no_آرگومان, عدم, GETOPT_HELP_CHAR
#تعریف_کن GETOPT_نسخه_OPTION_DECL \
  "version", no_آرگومان, عدم, GETOPT_نسخه_CHAR
#تعریف_کن GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_آرگومان, عدم, 'Z'

#تعریف_کن case_GETOPT_HELP_CHAR			\
  حالة GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    اكسر;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#تعریف_کن USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#تعریف_کن HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#تعریف_کن نسخه_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#ضمن "closein.h"
#ضمن "closeout.h"

#ضمن "version-etc.h"

#ضمن "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#تعریف_کن proper_name(x) proper_name_lite (x, x)

#ضمن "progname.h"

#تعریف_کن case_GETOPT_نسخه_CHAR(برنامه_name_2, Authors)			\
  حالة GETOPT_نسخه_CHAR:						\
    نسخه_etc (stdout, برنامه_name_2, PACKAGE_NAME, نسخه, Authors,	\
                 (حرف *) عدم);					\
    خروج (EXIT_SUCCESS);						\
    اكسر;

#ضمن "minmax.h"
#ضمن "intprops.h"

#اگر_تعریف_نشده SSIZE_MAX
# define SSIZE_MAX TYPE_بیشینه (ssize_t)
#پایان_اگر

#اگر_تعریف_نشده OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#پایان_اگر

#اگر_تعریف_نشده OFF_T_MAX
# define OFF_T_MAX TYPE_بیشینه (off_t)
#پایان_اگر

#اگر_تعریف_نشده UID_T_MAX
# define UID_T_MAX TYPE_بیشینه (uid_t)
#پایان_اگر

#اگر_تعریف_نشده GID_T_MAX
# define GID_T_MAX TYPE_بیشینه (gid_t)
#پایان_اگر

#اگر_تعریف_نشده PID_T_MAX
# define PID_T_MAX TYPE_بیشینه (pid_t)
#پایان_اگر

/* Use this to suppress gcc warnings.  */
#اگر_تعریف_شده lint
# define IF_LINT(کد_2) کد_2
#وإلا
# define IF_LINT(کد_2) /*خالی*/
#پایان_اگر

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#اگر_تعریف_شده lint
# define main_exit(وضعیت) خروج (وضعیت)
#وإلا
# define main_exit(وضعیت) أرجع وضعیت
#پایان_اگر

#اگر_تعریف_شده __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#وإلا
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#پایان_اگر


#إذا defined strdupa
# define تخصیص_بده_STRDUPA(DEST, S)		\
  افعل { DEST = strdupa (S); } طالما (0)
#وإلا
# define تخصیص_بده_STRDUPA(DEST, S)		\
  افعل						\
    {						\
      حرف ثابت *s_ = (S);			\
      size_t len_ = طول_رشته (s_) + 1;		\
      حرف *tmp_dest_ = alloca (len_);		\
      DEST = کپی_حافظه (tmp_dest_, s_, len_);	\
    }						\
  طالما (0)
#پایان_اگر

#إذا ! HAVE_SYNC
# define sync() /*خالی*/
#پایان_اگر

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
ساكن مضمن size_t
بزرگترین_مقسوم_علیه (size_t u, size_t v)
{
  افعل
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  طالما (v);

  أرجع u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
ساكن مضمن size_t
lcm (size_t u, size_t v)
{
  أرجع u * (v / بزرگترین_مقسوم_علیه (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

ساكن مضمن فراغ *
ptr_align (فراغ ثابت *ptr, size_t alignment)
{
  حرف ثابت *p0 = ptr;
  حرف ثابت *p1 = p0 + alignment - 1;
  أرجع (فراغ *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_خالص
ساكن مضمن bool
is_nul (فراغ ثابت *buf, size_t درازی)
{
  ثابت بیعلامت حرف *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#إذا 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  بیعلامت دراز کلمه_2;
#وإلا
  بیعلامت حرف کلمه_2;
#پایان_اگر

  إذا (! درازی)
    أرجع true;

  /* Check len bytes not aligned on a word.  */
  طالما (UNLIKELY (درازی & (حجم کلمه_2 - 1)))
    {
      إذا (*p)
        أرجع false;
      p++;
      درازی--;
      إذا (! درازی)
        أرجع true;
   }

  /* Check up to 16 bytes a word at a time.  */
  لكل (;;)
    {
      کپی_حافظه (&کلمه_2, p, حجم کلمه_2);
      إذا (کلمه_2)
        أرجع false;
      p += حجم کلمه_2;
      درازی -= حجم کلمه_2;
      إذا (! درازی)
        أرجع true;
      إذا (UNLIKELY (درازی & 15) == 0)
        اكسر;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  أرجع memeq (buf, p, درازی);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#تعریف_کن DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_جمع_کن (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#تعریف_کن oputs(option) oputs_ (برنامه_NAME, option)
ساكن مضمن فراغ
oputs_ (MAYBE_استفادهنشده حرف ثابت *برنامه, حرف ثابت *option)
{
  ساكن صحيح help_markup =
#إذا defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#وإلا
    0;   /* Disable.  */
#پایان_اگر
  إذا (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      حرف ثابت *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != عدم);
    }
  إذا (! help_markup)
    {
      fputs (option, stdout);
      أرجع;
    }

  bool double_space = true;
  حرف ثابت *اولی_کلمه = option + strspn (option, " \t\n");
  حرف ثابت *option_متن = strchr (option, '-');
  إذا (!option_متن)                    /* for dd(1) option syntax.  */
    {
      option_متن = اولی_کلمه;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  وإلا إذا (option_متن != اولی_کلمه)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      حرف ثابت *s = اولی_کلمه;
      size_t spaces = 0;
      طالما (s < option_متن && spaces < 2)
        spaces += !!آیا_فاصله (مقصد_uchar (*s++));
      إذا (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_متن = اولی_کلمه;
          double_space = false;
        }
    }

  size_t anchor_len = strcspn (option_متن, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  حرف ثابت *desc_متن = option_متن + anchor_len;
  طالما (*desc_متن && *desc_متن != '\n')
    {
      إذا (*desc_متن == '-' && *(desc_متن + 1) == '-')
        double_space = false;
      إذا (آیا_فاصله (مقصد_uchar (*desc_متن)))
        {
          إذا (*desc_متن == '\t' || آیا_فاصله (مقصد_uchar (*(desc_متن + 1))))
            اكسر;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          إذا (!double_space && *(desc_متن + 1) != '-')
            اكسر;
        }

      desc_متن++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, اولی_کلمه - option, stdout);

  /* write option text.  */
#اگر_تعریف_شده MANUAL_URL
  حرف ثابت *url_برنامه =   streq (برنامه, "[") ? "test"
                            : streq (برنامه, "dir") ? "ls"
                            : streq (برنامه, "vdir") ? "ls"
                            : streq (برنامه, "b2sum") ? "cksum"
                            : streq (برنامه, "md5sum") ? "cksum"
                            : streq (برنامه, "sha1sum") ? "cksum"
                            : streq (برنامه, "sha224sum") ? "cksum"
                            : streq (برنامه, "sha256sum") ? "cksum"
                            : streq (برنامه, "sha384sum") ? "cksum"
                            : streq (برنامه, "sha512sum") ? "cksum"
                            : برنامه;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  إذا (STREQ_LEN (option_متن, "--help", 6)
      || STREQ_LEN (option_متن, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      چاپ_کن ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_برنامه, url_برنامه, (صحيح) anchor_len, option_متن);
    }
  وإلا
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      چاپ_کن ("\033]8;;%s#%s%.*s", MANUAL_URL, url_برنامه,
              (صحيح) anchor_len, option_متن);
    }
  fputs ("\033\\", stdout);
#پایان_اگر
#اگر_تعریف_شده BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#پایان_اگر
  /* first_word != option_text for test(1).  */
  fwrite (اولی_کلمه, 1, desc_متن - اولی_کلمه, stdout);
#اگر_تعریف_شده BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#پایان_اگر
#اگر_تعریف_شده MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#پایان_اگر

  /* write description.  */
  fputs (desc_متن, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#تعریف_کن oprintf(...) oprintf_ (برنامه_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((چاپ_کن, 2, 3))
ساكن مضمن فراغ
oprintf_ (حرف ثابت *برنامه, حرف ثابت *پیام_2, ...)
{
  va_list آرگومانها;
  حرف *buf;
  صحيح buflen = -1;

#إذا defined MANUAL_URL || defined BOLD_MAN_REFS
  va_شروع (آرگومانها, پیام_2);
  buflen = vasprintf (&buf, پیام_2, آرگومانها);
  va_انتها (آرگومانها);
#پایان_اگر

  إذا (buflen < 0)
    {
      va_شروع (آرگومانها, پیام_2);
      vprintf (پیام_2, آرگومانها);
      va_انتها (آرگومانها);
      أرجع;
    }

  oputs_ (برنامه, buf);
  حرر (buf);
}

ساكن مضمن فراغ
emit_stdin_note (فراغ)
{
  fputs (_("\n\
With no FILE, or when FILE is -, اقرأ standard درون_داد.\n\
"), stdout);
}
ساكن مضمن فراغ
emit_mandatory_arg_note (فراغ)
{
  fputs (_("\n\
Mandatory arguments مقصد دراز options are mandatory لكل کوتاه options too.\n\
"), stdout);
}

ساكن مضمن فراغ
emit_اندازه_note (فراغ)
{
  fputs (_("\n\
The اندازه_2 آرگومان is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers از 1024) or KB,MB,... (powers از 1000).\n\
دودویی_2 prefixes can be استفاده_شده, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

ساكن مضمن فراغ
emit_blocksize_note (حرف ثابت *برنامه)
{
  چاپ_کن (_("\n\
Display مقادیر are درون units از the اولی موجود اندازه_2 from --بلوک-اندازه,\n\
and the %s_بلوک_اندازه, بلوک_اندازه_2 and BLOCKSIZE environment variables.\n\
Otherwise, units افتراضي مقصد 1024 بایتها (or 512 إذا POSIXLY_CORRECT is مجموعه_2).\n\
"), برنامه);
}

ساكن مضمن فراغ
emit_update_parameters_note (فراغ)
{
  fputs (_("\
\n\
UPDATE controls which existing files درون the destination are replaced.\n\
'all' is the افتراضي operation when an --update option is not specified,\n\
and results درون all existing files درون the destination being replaced.\n\
'none' is like the --no-clobber option, درون that no files درون the\n\
destination are replaced, and skipped files افعل not induce a failure.\n\
'none-fail' also ensures no files are replaced درون the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the افتراضي operation when --update is specified, and results\n\
درون files being replaced إذا they're older than the corresponding منبع_2 file.\n\
"), stdout);
}

ساكن مضمن فراغ
emit_backup_suffix_note (فراغ)
{
  fputs (_("\
\n\
The backup suffix is '~', unless مجموعه_2 with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The نسخه_3 control روش_2 may be selected از_طریق the --backup option or through\n\
the نسخه_CONTROL environment variable.  اینجا_2 are the مقادیر:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even إذا --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered إذا numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#تعریف_کن emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (برنامه_NAME, default_opt)
ساكن مضمن فراغ
emit_symlink_recurse_options_ (حرف ثابت *برنامه, حرف ثابت *default_opt)
{
      چاپ_کن (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If بیشتر than one is specified, only the final\n\
one takes effect. %s is the افتراضي.\n\
\n\
"), default_opt);
      oputs_ (برنامه, _("\
  -H\n\
         إذا a command سطر آرگومان is a symlink مقصد a directory, traverse it\n\
"));
      oputs_ (برنامه, _("\
  -L\n\
         traverse every symbolic link مقصد a directory encountered\n\
"));
      oputs_ (برنامه, _("\
  -P\n\
         افعل not traverse any symbolic links\n\
\n\
"));
}

ساكن مضمن فراغ
emit_exec_وضعیت (حرف ثابت *برنامه)
{
      چاپ_کن (_("\n\
Exit وضعیت:\n\
  125  إذا the %s command itself fails\n\
  126  إذا COMMAND is یافت but cannot be invoked\n\
  127  إذا COMMAND cannot be یافت\n\
  -    the خروج وضعیت از COMMAND otherwise\n\
"), برنامه);
}

ساكن مضمن فراغ
emit_ancillary_info (حرف ثابت *برنامه)
{
  بنية infomap { حرف ثابت *برنامه; حرف ثابت *گره; } ثابت infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { عدم, عدم }
  };

  حرف ثابت *گره = برنامه;
  بنية infomap ثابت *map_prog = infomap;

  طالما (map_prog->برنامه && ! streq (برنامه, map_prog->برنامه))
    map_prog++;

  إذا (map_prog->گره)
    گره = map_prog->گره;

  emit_bug_reporting_آدرس ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  حرف ثابت *url_برنامه = streq (برنامه, "[") ? "test" : برنامه;
  چاپ_کن (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_برنامه);
  چاپ_کن (_("or available locally via: info '(coreutils) %s%s'\n"),
          گره, گره == برنامه ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#تعریف_کن emit_try_help() \
  افعل \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               برنامه_name); \
    } \
  طالما (0)

#ضمن "inttostr.h"

ساكن مضمن حرف *
timetostr (time_t t, حرف *buf)
{
  أرجع (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

ساكن مضمن حرف *
bad_cast (حرف ثابت *s)
{
  أرجع (حرف *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
ساكن مضمن bool
usable_st_اندازه (بنية stat ثابت *sb)
{
  أرجع (S_ISREG (sb->st_نما) || S_ISLNK (sb->st_نما)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn فراغ usage (صحيح وضعیت);

#ضمن <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#تعریف_کن devmsg(...)			\
  افعل					\
    {					\
      إذا (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  طالما (0)

#تعریف_کن emit_چرخه_warning(file_name)	\
  افعل					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part از the چرخه:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  طالما (0)

/* exit with a _single_ "write error" diagnostic.  */

ساكن مضمن فراغ
write_error (فراغ)
{
  صحيح saved_errno = errno;
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
ساكن مضمن حرف *
stzncpy (حرف *محدود dest, حرف ثابت *محدود src, size_t len)
{
  لكل (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  أرجع dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
ساكن مضمن bool
is_ENOTSUP (صحيح err)
{
  أرجع err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#ضمن "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#تعریف_کن quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#تعریف_کن quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#تعریف_کن quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#تعریف_کن quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#تعریف_کن quoteN(x) quotearg_style (get_quoting_style (عدم), x)
#تعریف_کن quoteN_mem(x, اندازه)                                     \
  quotearg_style_mem (get_quoting_style (عدم), x, اندازه)

#اگر_تعریف_شده ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
ساكن مضمن صحيح
getenv_quoting_style (فراغ)
{
  حرف ثابت *q_style = getenv ("QUOTING_STYLE");
  إذا (!q_style)
    أرجع -1;

  صحيح i = ARGMATCH (q_style, quoting_style_آرگومانها, quoting_style_vals);
  إذا (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      أرجع -1;
    }
  أرجع quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
ساكن مضمن ptrdiff_t
x_timestyle_تطابق (حرف ثابت * style, bool allow_posix,
                   حرف ثابت *ثابت * timestyle_آرگومانها,
                   حرف ثابت * timestyle_types,
                   size_t timestyle_types_اندازه,
                   صحيح fail_وضعیت)
{
  ptrdiff_t res = argmatch (style, timestyle_آرگومانها,
                            timestyle_types,
                            timestyle_types_اندازه);
  إذا (res < 0)
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
      حرف ثابت *ثابت *p = timestyle_آرگومانها;
      حرف ثابت *posix_پیشوند = allow_posix ? "[posix-]" : "";
      طالما (*p)
        fprintf (stderr, "  - %s%s\n", posix_پیشوند, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_وضعیت);
    }

  أرجع res;
}
#پایان_اگر
