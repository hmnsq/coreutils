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

#شامل <attribute.h>

#شامل <alloca.h>

#شامل <ctype.h>

#شامل <sys/stat.h>

/* Commonly used file permission combination.  */
#تعریف_کرو منوال_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#اگر HAVE_SYS_PARAM_H
# include <sys/param.h>
#اگر_ختم

#شامل <unistd.h>

#شامل <limits.h>

#شامل "pathmax.h"
#اگر_غیر_تعریف_شدہ PATH_MAX
# define PATH_MAX 8192
#اگر_ختم

#شامل "configmake.h"

#شامل <sys/time.h>
#شامل <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#اگر MAJOR_اندر_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#اگر_ختم
#اگر MAJOR_اندر_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#اگر_ختم
#اگر_تعریف_شدہ major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#اگر_ختم

#اگر_غیر_تعریف_شدہ HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#اگر_ختم
#تعریف_منسوخ HAVE_MAJOR

#اگر ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#اگر_ختم

#شامل <stdckdint.h>
#شامل <stdcountof.h>
#شامل <stddef.h>
#شامل <string.h>
#شامل <uchar.h>
#شامل <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#اگر_غیر_تعریف_شدہ ENODATA
# define ENODATA (-1)
#اگر_ختم

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
ساکن اندرونی bool
ignorable_traversal_errno (صحیح errnum) { واپس errnum == ENOENT; }

#شامل <stdlib.h>
#شامل "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
شمار
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#شامل "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
ساکن اندرونی خالی
initialize_exit_failure (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_FAILURE)
    exit_failure = حالت_2;
}

#شامل <fcntl.h>
#اگر_تعریف_شدہ O_راستہ
شمار { O_PATHSEARCH = O_راستہ };
#ورنہ
شمار { O_PATHSEARCH = O_تلاش };
#اگر_ختم

#شامل <dirent.h>
#اگر_غیر_تعریف_شدہ _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) لڑی_لمبائی ((dp)->d_name)
#اگر_ختم

شمار
{
  NOT_AN_INODE_نمبر = 0
};

#اگر_تعریف_شدہ D_INO_اندر_DIRENT
# define D_INO(dp) (dp)->d_ino
#ورنہ
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_نمبر
#اگر_ختم

/* include here for SIZE_MAX.  */
#شامل <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#اگر_غیر_تعریف_شدہ initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# ورنہ
#  define initialize_main(ac, av) \
     کرو { _wildcard (ac, av); _response (ac, av); } جب (0)
# endif
#اگر_ختم

#شامل "stat-macros.h"

#شامل "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
ساکن اندرونی غیرعلامتی حرف مقصد_uchar (حرف ch) { واپس ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_خالص
ساکن اندرونی صحیح
c32isnbspace (char32_t wc)
{
  واپس wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_خالص
ساکن اندرونی صحیح
c32isnewline (char32_t wc)
{
  واپس wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_خالص
ساکن اندرونی صحیح
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  واپس c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_خالص
ساکن اندرونی bool
c32issep (char32_t wc)
{
#اگر defined __GLIBC__
  واپس !! c32isblank (wc);
#ورنہ
  واپس c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#اگر_ختم
}

#شامل <locale.h>

/* Take care of NLS matters.  */

#شامل "gettext.h"
#اگر ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /*خالی_خانے*/
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /*خالی_خانے*/
#اگر_ختم

#تعریف_کرو _(msgid) gettext (msgid)
#تعریف_کرو N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
ساکن اندرونی غیرعلامتی طویل صحیح
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  شمار { PLURAL_REDUCER = 1000000 };
  واپس (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#تعریف_کرو STREQ_LEN(a, b, n) (لڑی_موازنہ (a, b, n) == 0) /* n==-1 means unbounded */
#تعریف_کرو STRPREFIX(a, b) (لڑی_موازنہ (a, b, لڑی_لمبائی (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#تعریف_کرو STRNCMP_LIT(s, lit) لڑی_موازنہ (s, "" lit "", حجم (lit) - 1)

#اگر !HAVE_DECL_GETLOGIN
حرف *getlogin (خالی);
#اگر_ختم

#اگر !HAVE_DECL_TTYNAME
حرف *ttyname (صحیح);
#اگر_ختم

#اگر !HAVE_DECL_GETEUID
uid_t geteuid (خالی);
#اگر_ختم

#اگر !HAVE_DECL_GETPWUID
ساخت passwd *getpwuid (uid_t);
#اگر_ختم

#اگر !HAVE_DECL_GETGRGID
ساخت گروہ *getgrgid (gid_t);
#اگر_ختم

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#اگر ! HAVE_SETGROUPS
# اگر HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# اگر HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# اگر HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#اگر_ختم

#اگر !HAVE_DECL_GETUID
uid_t getuid (خالی);
#اگر_ختم

#شامل "idx.h"
#شامل "xalloc.h"
#شامل "verify.h"
#شامل "unlocked-io.h"
#شامل "same-inode.h"

#شامل "dirname.h"
#شامل "openat.h"

ساکن اندرونی bool
dot_or_dotdot (حرف ثابت *file_name)
{
  اگر (file_name[0] == '.')
    {
      حرف sep = file_name[(file_name[1] == '.') + 1];
      واپس (! sep || ISSLASH (sep));
    }
  ورنہ
    واپس false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
ساکن اندرونی ساخت dirent ثابت *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  جب (true)
    {
      ساخت dirent ثابت *dp = readdir (dirp);
      اگر (dp == تہی || ! dot_or_dotdot (dp->d_name))
        واپس dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
شمار {
    DS_UNKNOWN = -2,
    DS_خالی_خانے = -1,
    DS_NONEMPTY = 0,
};
ساکن اندرونی صحیح
directory_حالت (صحیح fd_cwd, حرف ثابت *dir)
{
  DIR *dirp;
  bool no_direntries;
  صحیح saved_errno;
  صحیح fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  اگر (fd < 0)
    واپس errno;

  dirp = fdopendir (fd);
  اگر (dirp == تہی)
    {
      saved_errno = errno;
      close (fd);
      واپس saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  واپس no_direntries && saved_errno == 0 ? DS_خالی_خانے : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
شمار
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_نسخہ_CHAR = (CHAR_MIN - 3)
};

#تعریف_کرو GETOPT_HELP_OPTION_DECL \
  "help", no_دلیل, تہی, GETOPT_HELP_CHAR
#تعریف_کرو GETOPT_نسخہ_OPTION_DECL \
  "version", no_دلیل, تہی, GETOPT_نسخہ_CHAR
#تعریف_کرو GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_دلیل, تہی, 'Z'

#تعریف_کرو case_GETOPT_HELP_CHAR			\
  حالت GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    توڑو;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#تعریف_کرو USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#تعریف_کرو HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#تعریف_کرو نسخہ_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#شامل "closein.h"
#شامل "closeout.h"

#شامل "version-etc.h"

#شامل "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#تعریف_کرو proper_name(x) proper_name_lite (x, x)

#شامل "progname.h"

#تعریف_کرو case_GETOPT_نسخہ_CHAR(پروگرام_name_2, Authors)			\
  حالت GETOPT_نسخہ_CHAR:						\
    نسخہ_etc (stdout, پروگرام_name_2, PACKAGE_NAME, نسخہ, Authors,	\
                 (حرف *) تہی);					\
    باہر_نکلو (EXIT_SUCCESS);						\
    توڑو;

#شامل "minmax.h"
#شامل "intprops.h"

#اگر_غیر_تعریف_شدہ SSIZE_MAX
# define SSIZE_MAX TYPE_زیادہ_سے_زیادہ (ssize_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ OFF_T_MAX
# define OFF_T_MAX TYPE_زیادہ_سے_زیادہ (off_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ UID_T_MAX
# define UID_T_MAX TYPE_زیادہ_سے_زیادہ (uid_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ GID_T_MAX
# define GID_T_MAX TYPE_زیادہ_سے_زیادہ (gid_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ PID_T_MAX
# define PID_T_MAX TYPE_زیادہ_سے_زیادہ (pid_t)
#اگر_ختم

/* Use this to suppress gcc warnings.  */
#اگر_تعریف_شدہ lint
# define IF_LINT(رمز_2) رمز_2
#ورنہ
# define IF_LINT(رمز_2) /*خالی_خانے*/
#اگر_ختم

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#اگر_تعریف_شدہ lint
# define main_exit(حالت_2) باہر_نکلو (حالت_2)
#ورنہ
# define main_exit(حالت_2) واپس حالت_2
#اگر_ختم

#اگر_تعریف_شدہ __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#ورنہ
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#اگر_ختم


#اگر defined strdupa
# define تفویض_STRDUPA(DEST, S)		\
  کرو { DEST = strdupa (S); } جب (0)
#ورنہ
# define تفویض_STRDUPA(DEST, S)		\
  کرو						\
    {						\
      حرف ثابت *s_ = (S);			\
      size_t len_ = لڑی_لمبائی (s_) + 1;		\
      حرف *tmp_dest_ = alloca (len_);		\
      DEST = حافظہ_نقل (tmp_dest_, s_, len_);	\
    }						\
  جب (0)
#اگر_ختم

#اگر ! HAVE_SYNC
# define sync() /*خالی_خانے*/
#اگر_ختم

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
ساکن اندرونی size_t
عاد_اعظم (size_t u, size_t v)
{
  کرو
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  جب (v);

  واپس u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
ساکن اندرونی size_t
lcm (size_t u, size_t v)
{
  واپس u * (v / عاد_اعظم (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

ساکن اندرونی خالی *
ptr_align (خالی ثابت *ptr, size_t alignment)
{
  حرف ثابت *p0 = ptr;
  حرف ثابت *p1 = p0 + alignment - 1;
  واپس (خالی *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_خالص
ساکن اندرونی bool
is_nul (خالی ثابت *buf, size_t لمبائی)
{
  ثابت غیرعلامتی حرف *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#اگر 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  غیرعلامتی طویل لفظ_2;
#ورنہ
  غیرعلامتی حرف لفظ_2;
#اگر_ختم

  اگر (! لمبائی)
    واپس true;

  /* Check len bytes not aligned on a word.  */
  جب (UNLIKELY (لمبائی & (حجم لفظ_2 - 1)))
    {
      اگر (*p)
        واپس false;
      p++;
      لمبائی--;
      اگر (! لمبائی)
        واپس true;
   }

  /* Check up to 16 bytes a word at a time.  */
  برائے (;;)
    {
      حافظہ_نقل (&لفظ_2, p, حجم لفظ_2);
      اگر (لفظ_2)
        واپس false;
      p += حجم لفظ_2;
      لمبائی -= حجم لفظ_2;
      اگر (! لمبائی)
        واپس true;
      اگر (UNLIKELY (لمبائی & 15) == 0)
        توڑو;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  واپس memeq (buf, p, لمبائی);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#تعریف_کرو DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_جوڑو (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#تعریف_کرو oputs(option) oputs_ (پروگرام_NAME, option)
ساکن اندرونی خالی
oputs_ (MAYBE_غیر_مستعمل حرف ثابت *پروگرام, حرف ثابت *option)
{
  ساکن صحیح help_markup =
#اگر defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#ورنہ
    0;   /* Disable.  */
#اگر_ختم
  اگر (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      حرف ثابت *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != تہی);
    }
  اگر (! help_markup)
    {
      fputs (option, stdout);
      واپس;
    }

  bool double_space = true;
  حرف ثابت *پہلا_لفظ = option + strspn (option, " \t\n");
  حرف ثابت *option_متن = strchr (option, '-');
  اگر (!option_متن)                    /* for dd(1) option syntax.  */
    {
      option_متن = پہلا_لفظ;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  ورنہ اگر (option_متن != پہلا_لفظ)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      حرف ثابت *s = پہلا_لفظ;
      size_t spaces = 0;
      جب (s < option_متن && spaces < 2)
        spaces += !!خلا_ہے (مقصد_uchar (*s++));
      اگر (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_متن = پہلا_لفظ;
          double_space = false;
        }
    }

  size_t anchor_len = strcspn (option_متن, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  حرف ثابت *desc_متن = option_متن + anchor_len;
  جب (*desc_متن && *desc_متن != '\n')
    {
      اگر (*desc_متن == '-' && *(desc_متن + 1) == '-')
        double_space = false;
      اگر (خلا_ہے (مقصد_uchar (*desc_متن)))
        {
          اگر (*desc_متن == '\t' || خلا_ہے (مقصد_uchar (*(desc_متن + 1))))
            توڑو;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          اگر (!double_space && *(desc_متن + 1) != '-')
            توڑو;
        }

      desc_متن++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, پہلا_لفظ - option, stdout);

  /* write option text.  */
#اگر_تعریف_شدہ MANUAL_URL
  حرف ثابت *url_پروگرام =   streq (پروگرام, "[") ? "test"
                            : streq (پروگرام, "dir") ? "ls"
                            : streq (پروگرام, "vdir") ? "ls"
                            : streq (پروگرام, "b2sum") ? "cksum"
                            : streq (پروگرام, "md5sum") ? "cksum"
                            : streq (پروگرام, "sha1sum") ? "cksum"
                            : streq (پروگرام, "sha224sum") ? "cksum"
                            : streq (پروگرام, "sha256sum") ? "cksum"
                            : streq (پروگرام, "sha384sum") ? "cksum"
                            : streq (پروگرام, "sha512sum") ? "cksum"
                            : پروگرام;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  اگر (STREQ_LEN (option_متن, "--help", 6)
      || STREQ_LEN (option_متن, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      لکھو ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_پروگرام, url_پروگرام, (صحیح) anchor_len, option_متن);
    }
  ورنہ
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      لکھو ("\033]8;;%s#%s%.*s", MANUAL_URL, url_پروگرام,
              (صحیح) anchor_len, option_متن);
    }
  fputs ("\033\\", stdout);
#اگر_ختم
#اگر_تعریف_شدہ BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#اگر_ختم
  /* first_word != option_text for test(1).  */
  fwrite (پہلا_لفظ, 1, desc_متن - پہلا_لفظ, stdout);
#اگر_تعریف_شدہ BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#اگر_ختم
#اگر_تعریف_شدہ MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#اگر_ختم

  /* write description.  */
  fputs (desc_متن, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#تعریف_کرو oprintf(...) oprintf_ (پروگرام_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((لکھو, 2, 3))
ساکن اندرونی خالی
oprintf_ (حرف ثابت *پروگرام, حرف ثابت *پیغام_2, ...)
{
  va_list دلائل;
  حرف *buf;
  صحیح buflen = -1;

#اگر defined MANUAL_URL || defined BOLD_MAN_REFS
  va_آغاز (دلائل, پیغام_2);
  buflen = vasprintf (&buf, پیغام_2, دلائل);
  va_انتہا (دلائل);
#اگر_ختم

  اگر (buflen < 0)
    {
      va_آغاز (دلائل, پیغام_2);
      vprintf (پیغام_2, دلائل);
      va_انتہا (دلائل);
      واپس;
    }

  oputs_ (پروگرام, buf);
  آزاد (buf);
}

ساکن اندرونی خالی
emit_stdin_note (خالی)
{
  fputs (_("\n\
With no FILE, or when FILE is -, اقرأ standard داخلہ.\n\
"), stdout);
}
ساکن اندرونی خالی
emit_mandatory_arg_note (خالی)
{
  fputs (_("\n\
Mandatory arguments مقصد طویل options are mandatory برائے مختصر options too.\n\
"), stdout);
}

ساکن اندرونی خالی
emit_سائز_note (خالی)
{
  fputs (_("\n\
The سائز_2 دلیل is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers کا 1024) or KB,MB,... (powers کا 1000).\n\
ثنائی_2 prefixes can be مستعمل, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

ساکن اندرونی خالی
emit_blocksize_note (حرف ثابت *پروگرام)
{
  لکھو (_("\n\
Display اقدار are اندر units کا the پہلا دستیاب سائز_2 from --بلاک-سائز,\n\
and the %s_بلاک_سائز, بلاک_سائز_2 and BLOCKSIZE environment variables.\n\
Otherwise, units طےشدہ مقصد 1024 بائٹیں (or 512 اگر POSIXLY_CORRECT is سیٹ_2).\n\
"), پروگرام);
}

ساکن اندرونی خالی
emit_update_parameters_note (خالی)
{
  fputs (_("\
\n\
UPDATE controls which existing files اندر the destination are replaced.\n\
'all' is the طےشدہ operation when an --update option is not specified,\n\
and results اندر all existing files اندر the destination being replaced.\n\
'none' is like the --no-clobber option, اندر that no files اندر the\n\
destination are replaced, and skipped files کرو not induce a failure.\n\
'none-fail' also ensures no files are replaced اندر the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the طےشدہ operation when --update is specified, and results\n\
اندر files being replaced اگر they're older than the corresponding ماخذ_2 file.\n\
"), stdout);
}

ساکن اندرونی خالی
emit_backup_suffix_note (خالی)
{
  fputs (_("\
\n\
The backup suffix is '~', unless سیٹ_2 with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The نسخہ_3 control طریقہ_2 may be selected بذریعہ the --backup option or through\n\
the نسخہ_CONTROL environment variable.  یہاں_2 are the اقدار:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even اگر --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered اگر numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#تعریف_کرو emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (پروگرام_NAME, default_opt)
ساکن اندرونی خالی
emit_symlink_recurse_options_ (حرف ثابت *پروگرام, حرف ثابت *default_opt)
{
      لکھو (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If مزید than one is specified, only the final\n\
one takes effect. %s is the طےشدہ.\n\
\n\
"), default_opt);
      oputs_ (پروگرام, _("\
  -H\n\
         اگر a command سطر دلیل is a symlink مقصد a directory, traverse it\n\
"));
      oputs_ (پروگرام, _("\
  -L\n\
         traverse every symbolic link مقصد a directory encountered\n\
"));
      oputs_ (پروگرام, _("\
  -P\n\
         کرو not traverse any symbolic links\n\
\n\
"));
}

ساکن اندرونی خالی
emit_exec_حالت (حرف ثابت *پروگرام)
{
      لکھو (_("\n\
Exit حالت_2:\n\
  125  اگر the %s command itself fails\n\
  126  اگر COMMAND is ملا but cannot be invoked\n\
  127  اگر COMMAND cannot be ملا\n\
  -    the باہر_نکلو حالت_2 کا COMMAND otherwise\n\
"), پروگرام);
}

ساکن اندرونی خالی
emit_ancillary_info (حرف ثابت *پروگرام)
{
  ساخت infomap { حرف ثابت *پروگرام; حرف ثابت *گرہ; } ثابت infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { تہی, تہی }
  };

  حرف ثابت *گرہ = پروگرام;
  ساخت infomap ثابت *map_prog = infomap;

  جب (map_prog->پروگرام && ! streq (پروگرام, map_prog->پروگرام))
    map_prog++;

  اگر (map_prog->گرہ)
    گرہ = map_prog->گرہ;

  emit_bug_reporting_پتہ ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  حرف ثابت *url_پروگرام = streq (پروگرام, "[") ? "test" : پروگرام;
  لکھو (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_پروگرام);
  لکھو (_("or available locally via: info '(coreutils) %s%s'\n"),
          گرہ, گرہ == پروگرام ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#تعریف_کرو emit_try_help() \
  کرو \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               پروگرام_name); \
    } \
  جب (0)

#شامل "inttostr.h"

ساکن اندرونی حرف *
timetostr (time_t t, حرف *buf)
{
  واپس (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

ساکن اندرونی حرف *
bad_cast (حرف ثابت *s)
{
  واپس (حرف *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
ساکن اندرونی bool
usable_st_سائز (ساخت stat ثابت *sb)
{
  واپس (S_ISREG (sb->st_منوال) || S_ISLNK (sb->st_منوال)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn خالی usage (صحیح حالت_2);

#شامل <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#تعریف_کرو devmsg(...)			\
  کرو					\
    {					\
      اگر (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  جب (0)

#تعریف_کرو emit_چکر_warning(file_name)	\
  کرو					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part کا the چکر:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  جب (0)

/* exit with a _single_ "write error" diagnostic.  */

ساکن اندرونی خالی
write_error (خالی)
{
  صحیح saved_errno = errno;
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
ساکن اندرونی حرف *
stzncpy (حرف *محدود dest, حرف ثابت *محدود src, size_t len)
{
  برائے (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  واپس dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
ساکن اندرونی bool
is_ENOTSUP (صحیح err)
{
  واپس err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#شامل "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#تعریف_کرو quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#تعریف_کرو quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#تعریف_کرو quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#تعریف_کرو quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#تعریف_کرو quoteN(x) quotearg_style (get_quoting_style (تہی), x)
#تعریف_کرو quoteN_mem(x, سائز)                                     \
  quotearg_style_mem (get_quoting_style (تہی), x, سائز)

#اگر_تعریف_شدہ ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
ساکن اندرونی صحیح
getenv_quoting_style (خالی)
{
  حرف ثابت *q_style = getenv ("QUOTING_STYLE");
  اگر (!q_style)
    واپس -1;

  صحیح i = ARGMATCH (q_style, quoting_style_دلائل, quoting_style_vals);
  اگر (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      واپس -1;
    }
  واپس quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
ساکن اندرونی ptrdiff_t
x_timestyle_مطابقت (حرف ثابت * style, bool allow_posix,
                   حرف ثابت *ثابت * timestyle_دلائل,
                   حرف ثابت * timestyle_types,
                   size_t timestyle_types_سائز,
                   صحیح fail_حالت)
{
  ptrdiff_t res = argmatch (style, timestyle_دلائل,
                            timestyle_types,
                            timestyle_types_سائز);
  اگر (res < 0)
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
      حرف ثابت *ثابت *p = timestyle_دلائل;
      حرف ثابت *posix_سابقہ = allow_posix ? "[posix-]" : "";
      جب (*p)
        fprintf (stderr, "  - %s%s\n", posix_سابقہ, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_حالت);
    }

  واپس res;
}
#اگر_ختم
