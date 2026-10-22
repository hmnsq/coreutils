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

#అన్తర్భావ <attribute.h>

#అన్తర్భావ <alloca.h>

#అన్తర్భావ <ctype.h>

#అన్తర్భావ <sys/stat.h>

/* Commonly used file permission combination.  */
#నిర్వచించు బహుళకం_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#ఒకవేళ HAVE_SYS_PARAM_H
# include <sys/param.h>
#అయితే_ముగింపు

#అన్తర్భావ <unistd.h>

#అన్తర్భావ <limits.h>

#అన్తర్భావ "pathmax.h"
#నిర్వచితం_కాకపోతే PATH_MAX
# define PATH_MAX 8192
#అయితే_ముగింపు

#అన్తర్భావ "configmake.h"

#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#ఒకవేళ MAJOR_లోపల_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#అయితే_ముగింపు
#ఒకవేళ MAJOR_లోపల_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#అయితే_ముగింపు
#నిర్వచితమైతే major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#అయితే_ముగింపు
#నిర్వచనం_రద్దు HAVE_MAJOR

#ఒకవేళ ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#అయితే_ముగింపు

#అన్తర్భావ <stdckdint.h>
#అన్తర్భావ <stdcountof.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <uchar.h>
#అన్తర్భావ <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#నిర్వచితం_కాకపోతే ENODATA
# define ENODATA (-1)
#అయితే_ముగింపు

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
స్థిర అంతఃవరుస bool
ignorable_traversal_errno (పూర్ణాంకం errnum) { తిరిగి errnum == ENOENT; }

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
గణన
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#అన్తర్భావ "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
స్థిర అంతఃవరుస శూన్య
initialize_exit_failure (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_FAILURE)
    exit_failure = స్థితి_2;
}

#అన్తర్భావ <fcntl.h>
#నిర్వచితమైతే O_మార్గం
గణన { O_PATHSEARCH = O_మార్గం };
#లేకపోతే
గణన { O_PATHSEARCH = O_శోధన };
#అయితే_ముగింపు

#అన్తర్భావ <dirent.h>
#నిర్వచితం_కాకపోతే _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) అక్షరశ్రేణి_పొడవు ((dp)->d_name)
#అయితే_ముగింపు

గణన
{
  NOT_AN_INODE_సంఖ్య = 0
};

#నిర్వచితమైతే D_INO_లోపల_DIRENT
# define D_INO(dp) (dp)->d_ino
#లేకపోతే
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_సంఖ్య
#అయితే_ముగింపు

/* include here for SIZE_MAX.  */
#అన్తర్భావ <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#నిర్వచితం_కాకపోతే initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# లేకపోతే
#  define initialize_main(ac, av) \
     చేయి { _wildcard (ac, av); _response (ac, av); } అయితే (0)
# endif
#అయితే_ముగింపు

#అన్తర్భావ "stat-macros.h"

#అన్తర్భావ "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
స్థిర అంతఃవరుస గుర్తులేని అక్షరం గమ్యం_uchar (అక్షరం ch) { తిరిగి ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_శుద్ధ
స్థిర అంతఃవరుస పూర్ణాంకం
c32isnbspace (char32_t wc)
{
  తిరిగి wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_శుద్ధ
స్థిర అంతఃవరుస పూర్ణాంకం
c32isnewline (char32_t wc)
{
  తిరిగి wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_శుద్ధ
స్థిర అంతఃవరుస పూర్ణాంకం
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  తిరిగి c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_శుద్ధ
స్థిర అంతఃవరుస bool
c32issep (char32_t wc)
{
#ఒకవేళ defined __GLIBC__
  తిరిగి !! c32isblank (wc);
#లేకపోతే
  తిరిగి c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#అయితే_ముగింపు
}

#అన్తర్భావ <locale.h>

/* Take care of NLS matters.  */

#అన్తర్భావ "gettext.h"
#ఒకవేళ ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /*ఖాళీలు*/
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /*ఖాళీలు*/
#అయితే_ముగింపు

#నిర్వచించు _(msgid) gettext (msgid)
#నిర్వచించు N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ పూర్ణాంకం
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  గణన { PLURAL_REDUCER = 1000000 };
  తిరిగి (n <= ULONG_MAX ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#నిర్వచించు STREQ_LEN(a, b, n) (అక్షరశ్రేణి_పోలిక (a, b, n) == 0) /* n==-1 means unbounded */
#నిర్వచించు STRPREFIX(a, b) (అక్షరశ్రేణి_పోలిక (a, b, అక్షరశ్రేణి_పొడవు (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#నిర్వచించు STRNCMP_LIT(s, lit) అక్షరశ్రేణి_పోలిక (s, "" lit "", పరిమాణం (lit) - 1)

#ఒకవేళ !HAVE_DECL_GETLOGIN
అక్షరం *getlogin (శూన్య);
#అయితే_ముగింపు

#ఒకవేళ !HAVE_DECL_TTYNAME
అక్షరం *ttyname (పూర్ణాంకం);
#అయితే_ముగింపు

#ఒకవేళ !HAVE_DECL_GETEUID
uid_t geteuid (శూన్య);
#అయితే_ముగింపు

#ఒకవేళ !HAVE_DECL_GETPWUID
నిర్మాణం passwd *getpwuid (uid_t);
#అయితే_ముగింపు

#ఒకవేళ !HAVE_DECL_GETGRGID
నిర్మాణం సమూహం *getgrgid (gid_t);
#అయితే_ముగింపు

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#ఒకవేళ ! HAVE_SETGROUPS
# ఒకవేళ HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# ఒకవేళ HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# ఒకవేళ HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#అయితే_ముగింపు

#ఒకవేళ !HAVE_DECL_GETUID
uid_t getuid (శూన్య);
#అయితే_ముగింపు

#అన్తర్భావ "idx.h"
#అన్తర్భావ "xalloc.h"
#అన్తర్భావ "verify.h"
#అన్తర్భావ "unlocked-io.h"
#అన్తర్భావ "same-inode.h"

#అన్తర్భావ "dirname.h"
#అన్తర్భావ "openat.h"

స్థిర అంతఃవరుస bool
dot_or_dotdot (అక్షరం స్థిరరాశి *file_name)
{
  ఒకవేళ (file_name[0] == '.')
    {
      అక్షరం sep = file_name[(file_name[1] == '.') + 1];
      తిరిగి (! sep || ISSLASH (sep));
    }
  లేకపోతే
    తిరిగి false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
స్థిర అంతఃవరుస నిర్మాణం dirent స్థిరరాశి *
readdir_ignoring_dot_and_dotdot (DIR *dirp)
{
  అయితే (true)
    {
      నిర్మాణం dirent స్థిరరాశి *dp = readdir (dirp);
      ఒకవేళ (dp == ఖాళీ || ! dot_or_dotdot (dp->d_name))
        తిరిగి dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
గణన {
    DS_UNKNOWN = -2,
    DS_ఖాళీలు = -1,
    DS_NONEMPTY = 0,
};
స్థిర అంతఃవరుస పూర్ణాంకం
directory_స్థితి (పూర్ణాంకం fd_cwd, అక్షరం స్థిరరాశి *dir)
{
  DIR *dirp;
  bool no_direntries;
  పూర్ణాంకం saved_errno;
  పూర్ణాంకం fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  ఒకవేళ (fd < 0)
    తిరిగి errno;

  dirp = fdopendir (fd);
  ఒకవేళ (dirp == ఖాళీ)
    {
      saved_errno = errno;
      close (fd);
      తిరిగి saved_errno;
    }

  errno = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = errno;
  closedir (dirp);
  తిరిగి no_direntries && saved_errno == 0 ? DS_ఖాళీలు : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
గణన
{
  GETOPT_HELP_CHAR = (CHAR_MIN - 2),
  GETOPT_సంస్కరణ_CHAR = (CHAR_MIN - 3)
};

#నిర్వచించు GETOPT_HELP_OPTION_DECL \
  "help", no_వాదం, ఖాళీ, GETOPT_HELP_CHAR
#నిర్వచించు GETOPT_సంస్కరణ_OPTION_DECL \
  "version", no_వాదం, ఖాళీ, GETOPT_సంస్కరణ_CHAR
#నిర్వచించు GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_వాదం, ఖాళీ, 'Z'

#నిర్వచించు case_GETOPT_HELP_CHAR			\
  స్థితి GETOPT_HELP_CHAR:			\
    usage (EXIT_SUCCESS);			\
    ఆపు;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#నిర్వచించు USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#నిర్వచించు HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#నిర్వచించు సంస్కరణ_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#అన్తర్భావ "closein.h"
#అన్తర్భావ "closeout.h"

#అన్తర్భావ "version-etc.h"

#అన్తర్భావ "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#నిర్వచించు proper_name(x) proper_name_lite (x, x)

#అన్తర్భావ "progname.h"

#నిర్వచించు case_GETOPT_సంస్కరణ_CHAR(కార్యక్రమం_name_2, Authors)			\
  స్థితి GETOPT_సంస్కరణ_CHAR:						\
    సంస్కరణ_etc (stdout, కార్యక్రమం_name_2, PACKAGE_NAME, సంస్కరణ, Authors,	\
                 (అక్షరం *) ఖాళీ);					\
    నిష్క్రమించు (EXIT_SUCCESS);						\
    ఆపు;

#అన్తర్భావ "minmax.h"
#అన్తర్భావ "intprops.h"

#నిర్వచితం_కాకపోతే SSIZE_MAX
# define SSIZE_MAX TYPE_గరిష్ఠ (ssize_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OFF_T_MAX
# define OFF_T_MAX TYPE_గరిష్ఠ (off_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే UID_T_MAX
# define UID_T_MAX TYPE_గరిష్ఠ (uid_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GID_T_MAX
# define GID_T_MAX TYPE_గరిష్ఠ (gid_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే PID_T_MAX
# define PID_T_MAX TYPE_గరిష్ఠ (pid_t)
#అయితే_ముగింపు

/* Use this to suppress gcc warnings.  */
#నిర్వచితమైతే lint
# define IF_LINT(సంకేతం_3) సంకేతం_3
#లేకపోతే
# define IF_LINT(సంకేతం_3) /*ఖాళీలు*/
#అయితే_ముగింపు

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#నిర్వచితమైతే lint
# define main_exit(స్థితి_2) నిష్క్రమించు (స్థితి_2)
#లేకపోతే
# define main_exit(స్థితి_2) తిరిగి స్థితి_2
#అయితే_ముగింపు

#నిర్వచితమైతే __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#లేకపోతే
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#అయితే_ముగింపు


#ఒకవేళ defined strdupa
# define కేటాయించు_STRDUPA(DEST, S)		\
  చేయి { DEST = strdupa (S); } అయితే (0)
#లేకపోతే
# define కేటాయించు_STRDUPA(DEST, S)		\
  చేయి						\
    {						\
      అక్షరం స్థిరరాశి *s_ = (S);			\
      size_t len_ = అక్షరశ్రేణి_పొడవు (s_) + 1;		\
      అక్షరం *tmp_dest_ = alloca (len_);		\
      DEST = జ్ఞాపకం_నకలు (tmp_dest_, s_, len_);	\
    }						\
  అయితే (0)
#అయితే_ముగింపు

#ఒకవేళ ! HAVE_SYNC
# define sync() /*ఖాళీలు*/
#అయితే_ముగింపు

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
స్థిర అంతఃవరుస size_t
గరిష్ఠ_సామాన్య_భాజకం (size_t u, size_t v)
{
  చేయి
    {
      size_t t = u % v;
      u = v;
      v = t;
    }
  అయితే (v);

  తిరిగి u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
స్థిర అంతఃవరుస size_t
lcm (size_t u, size_t v)
{
  తిరిగి u * (v / గరిష్ఠ_సామాన్య_భాజకం (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

స్థిర అంతఃవరుస శూన్య *
ptr_align (శూన్య స్థిరరాశి *ptr, size_t alignment)
{
  అక్షరం స్థిరరాశి *p0 = ptr;
  అక్షరం స్థిరరాశి *p1 = p0 + alignment - 1;
  తిరిగి (శూన్య *) (p1 - (size_t) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_శుద్ధ
స్థిర అంతఃవరుస bool
is_nul (శూన్య స్థిరరాశి *buf, size_t నిడివి)
{
  స్థిరరాశి గుర్తులేని అక్షరం *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#ఒకవేళ 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  గుర్తులేని దీర్ఘ మాట_2;
#లేకపోతే
  గుర్తులేని అక్షరం మాట_2;
#అయితే_ముగింపు

  ఒకవేళ (! నిడివి)
    తిరిగి true;

  /* Check len bytes not aligned on a word.  */
  అయితే (UNLIKELY (నిడివి & (పరిమాణం మాట_2 - 1)))
    {
      ఒకవేళ (*p)
        తిరిగి false;
      p++;
      నిడివి--;
      ఒకవేళ (! నిడివి)
        తిరిగి true;
   }

  /* Check up to 16 bytes a word at a time.  */
  కొరకు (;;)
    {
      జ్ఞాపకం_నకలు (&మాట_2, p, పరిమాణం మాట_2);
      ఒకవేళ (మాట_2)
        తిరిగి false;
      p += పరిమాణం మాట_2;
      నిడివి -= పరిమాణం మాట_2;
      ఒకవేళ (! నిడివి)
        తిరిగి true;
      ఒకవేళ (UNLIKELY (నిడివి & 15) == 0)
        ఆపు;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  తిరిగి memeq (buf, p, నిడివి);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#నిర్వచించు DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_కూడు (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#నిర్వచించు oputs(option) oputs_ (కార్యక్రమం_NAME, option)
స్థిర అంతఃవరుస శూన్య
oputs_ (MAYBE_వాడని అక్షరం స్థిరరాశి *కార్యక్రమం, అక్షరం స్థిరరాశి *option)
{
  స్థిర పూర్ణాంకం help_markup =
#ఒకవేళ defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#లేకపోతే
    0;   /* Disable.  */
#అయితే_ముగింపు
  ఒకవేళ (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      అక్షరం స్థిరరాశి *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = getenv ("TERM")) && *te && !streq (te, "dumb"))
                     : getenv ("IN_HELP2MAN") != ఖాళీ);
    }
  ఒకవేళ (! help_markup)
    {
      fputs (option, stdout);
      తిరిగి;
    }

  bool double_space = true;
  అక్షరం స్థిరరాశి *మొదటి_మాట = option + strspn (option, " \t\n");
  అక్షరం స్థిరరాశి *option_పాఠ్యం = strchr (option, '-');
  ఒకవేళ (!option_పాఠ్యం)                    /* for dd(1) option syntax.  */
    {
      option_పాఠ్యం = మొదటి_మాట;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  లేకపోతే ఒకవేళ (option_పాఠ్యం != మొదటి_మాట)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      అక్షరం స్థిరరాశి *s = మొదటి_మాట;
      size_t spaces = 0;
      అయితే (s < option_పాఠ్యం && spaces < 2)
        spaces += !!ఖాళీనా (గమ్యం_uchar (*s++));
      ఒకవేళ (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_పాఠ్యం = మొదటి_మాట;
          double_space = false;
        }
    }

  size_t anchor_len = strcspn (option_పాఠ్యం, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  అక్షరం స్థిరరాశి *desc_పాఠ్యం = option_పాఠ్యం + anchor_len;
  అయితే (*desc_పాఠ్యం && *desc_పాఠ్యం != '\n')
    {
      ఒకవేళ (*desc_పాఠ్యం == '-' && *(desc_పాఠ్యం + 1) == '-')
        double_space = false;
      ఒకవేళ (ఖాళీనా (గమ్యం_uchar (*desc_పాఠ్యం)))
        {
          ఒకవేళ (*desc_పాఠ్యం == '\t' || ఖాళీనా (గమ్యం_uchar (*(desc_పాఠ్యం + 1))))
            ఆపు;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          ఒకవేళ (!double_space && *(desc_పాఠ్యం + 1) != '-')
            ఆపు;
        }

      desc_పాఠ్యం++;
    }

  /* write spaces before option text. */
  fwrite (option, 1, మొదటి_మాట - option, stdout);

  /* write option text.  */
#నిర్వచితమైతే MANUAL_URL
  అక్షరం స్థిరరాశి *url_కార్యక్రమం =   streq (కార్యక్రమం, "[") ? "test"
                            : streq (కార్యక్రమం, "dir") ? "ls"
                            : streq (కార్యక్రమం, "vdir") ? "ls"
                            : streq (కార్యక్రమం, "b2sum") ? "cksum"
                            : streq (కార్యక్రమం, "md5sum") ? "cksum"
                            : streq (కార్యక్రమం, "sha1sum") ? "cksum"
                            : streq (కార్యక్రమం, "sha224sum") ? "cksum"
                            : streq (కార్యక్రమం, "sha256sum") ? "cksum"
                            : streq (కార్యక్రమం, "sha384sum") ? "cksum"
                            : streq (కార్యక్రమం, "sha512sum") ? "cksum"
                            : కార్యక్రమం;
  /* Note we don't add the hostname to the links below, because
     it's unused with http:// links and not usually useful with file:// links.
     Also there is the possibility that local (hostname agnostic) links
     would work if remotely accessing a similar system to the local one.  */
  ఒకవేళ (STREQ_LEN (option_పాఠ్యం, "--help", 6)
      || STREQ_LEN (option_పాఠ్యం, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      రాయి ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_కార్యక్రమం, url_కార్యక్రమం, (పూర్ణాంకం) anchor_len, option_పాఠ్యం);
    }
  లేకపోతే
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      రాయి ("\033]8;;%s#%s%.*s", MANUAL_URL, url_కార్యక్రమం,
              (పూర్ణాంకం) anchor_len, option_పాఠ్యం);
    }
  fputs ("\033\\", stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  fputs ("\033[1m", stdout);
#అయితే_ముగింపు
  /* first_word != option_text for test(1).  */
  fwrite (మొదటి_మాట, 1, desc_పాఠ్యం - మొదటి_మాట, stdout);
#నిర్వచితమైతే BOLD_MAN_REFS
  fputs ("\033[0m", stdout);
#అయితే_ముగింపు
#నిర్వచితమైతే MANUAL_URL
  fputs ("\033]8;;\033\\", stdout);
#అయితే_ముగింపు

  /* write description.  */
  fputs (desc_పాఠ్యం, stdout);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#నిర్వచించు oprintf(...) oprintf_ (కార్యక్రమం_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((రాయి, 2, 3))
స్థిర అంతఃవరుస శూన్య
oprintf_ (అక్షరం స్థిరరాశి *కార్యక్రమం, అక్షరం స్థిరరాశి *సందేశం_2, ...)
{
  va_list వాదనలు;
  అక్షరం *buf;
  పూర్ణాంకం buflen = -1;

#ఒకవేళ defined MANUAL_URL || defined BOLD_MAN_REFS
  va_ప్రారంభం (వాదనలు, సందేశం_2);
  buflen = vasprintf (&buf, సందేశం_2, వాదనలు);
  va_చివర (వాదనలు);
#అయితే_ముగింపు

  ఒకవేళ (buflen < 0)
    {
      va_ప్రారంభం (వాదనలు, సందేశం_2);
      vprintf (సందేశం_2, వాదనలు);
      va_చివర (వాదనలు);
      తిరిగి;
    }

  oputs_ (కార్యక్రమం, buf);
  విడుదల (buf);
}

స్థిర అంతఃవరుస శూన్య
emit_stdin_note (శూన్య)
{
  fputs (_("\n\
With no FILE, or when FILE is -, పఠ standard ఆదానం.\n\
"), stdout);
}
స్థిర అంతఃవరుస శూన్య
emit_mandatory_arg_note (శూన్య)
{
  fputs (_("\n\
Mandatory arguments గమ్యం దీర్ఘ options are mandatory కొరకు పొట్టి options too.\n\
"), stdout);
}

స్థిర అంతఃవరుస శూన్య
emit_కొలత_note (శూన్య)
{
  fputs (_("\n\
The కొలత_2 వాదం is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers యొక్క 1024) or KB,MB,... (powers యొక్క 1000).\n\
ద్విమాన_2 prefixes can be వాడిన, too: KiB=K, MiB=M, and so on.\n\
"), stdout);
}

స్థిర అంతఃవరుస శూన్య
emit_blocksize_note (అక్షరం స్థిరరాశి *కార్యక్రమం)
{
  రాయి (_("\n\
Display విలువలు are లోపల units యొక్క the మొదటి అందుబాటు కొలత_2 from --ఖండం-కొలత,\n\
and the %s_ఖండం_కొలత, ఖండం_కొలత_2 and BLOCKSIZE environment variables.\n\
Otherwise, units అప్రమేయ గమ్యం 1024 బైట్లు (or 512 ఒకవేళ POSIXLY_CORRECT is సమితి_2).\n\
"), కార్యక్రమం);
}

స్థిర అంతఃవరుస శూన్య
emit_update_parameters_note (శూన్య)
{
  fputs (_("\
\n\
UPDATE controls which existing files లోపల the destination are replaced.\n\
'all' is the అప్రమేయ operation when an --update option is not specified,\n\
and results లోపల all existing files లోపల the destination being replaced.\n\
'none' is like the --no-clobber option, లోపల that no files లోపల the\n\
destination are replaced, and skipped files చేయి not induce a failure.\n\
'none-fail' also ensures no files are replaced లోపల the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the అప్రమేయ operation when --update is specified, and results\n\
లోపల files being replaced ఒకవేళ they're older than the corresponding ఆధారం_2 file.\n\
"), stdout);
}

స్థిర అంతఃవరుస శూన్య
emit_backup_suffix_note (శూన్య)
{
  fputs (_("\
\n\
The backup suffix is '~', unless సమితి_2 with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The సంస్కరణ_3 control పద్ధతి_2 may be selected ద్వారా the --backup option or through\n\
the సంస్కరణ_CONTROL environment variable.  ఇక్కడ_2 are the విలువలు:\n\
\n\
"), stdout);
  fputs (_("\
  none, off       never make backups (even ఒకవేళ --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered ఒకవేళ numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), stdout);
}

#నిర్వచించు emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (కార్యక్రమం_NAME, default_opt)
స్థిర అంతఃవరుస శూన్య
emit_symlink_recurse_options_ (అక్షరం స్థిరరాశి *కార్యక్రమం, అక్షరం స్థిరరాశి *default_opt)
{
      రాయి (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If మరిన్ని than one is specified, only the final\n\
one takes effect. %s is the అప్రమేయ.\n\
\n\
"), default_opt);
      oputs_ (కార్యక్రమం, _("\
  -H\n\
         ఒకవేళ a command పంక్తి వాదం is a symlink గమ్యం a directory, traverse it\n\
"));
      oputs_ (కార్యక్రమం, _("\
  -L\n\
         traverse every symbolic link గమ్యం a directory encountered\n\
"));
      oputs_ (కార్యక్రమం, _("\
  -P\n\
         చేయి not traverse any symbolic links\n\
\n\
"));
}

స్థిర అంతఃవరుస శూన్య
emit_exec_స్థితి (అక్షరం స్థిరరాశి *కార్యక్రమం)
{
      రాయి (_("\n\
Exit స్థితి_2:\n\
  125  ఒకవేళ the %s command itself fails\n\
  126  ఒకవేళ COMMAND is దొరికింది but cannot be invoked\n\
  127  ఒకవేళ COMMAND cannot be దొరికింది\n\
  -    the నిష్క్రమించు స్థితి_2 యొక్క COMMAND otherwise\n\
"), కార్యక్రమం);
}

స్థిర అంతఃవరుస శూన్య
emit_ancillary_info (అక్షరం స్థిరరాశి *కార్యక్రమం)
{
  నిర్మాణం infomap { అక్షరం స్థిరరాశి *కార్యక్రమం; అక్షరం స్థిరరాశి *కణుపు; } స్థిరరాశి infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { ఖాళీ, ఖాళీ }
  };

  అక్షరం స్థిరరాశి *కణుపు = కార్యక్రమం;
  నిర్మాణం infomap స్థిరరాశి *map_prog = infomap;

  అయితే (map_prog->కార్యక్రమం && ! streq (కార్యక్రమం, map_prog->కార్యక్రమం))
    map_prog++;

  ఒకవేళ (map_prog->కణుపు)
    కణుపు = map_prog->కణుపు;

  emit_bug_reporting_చిరునామా ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  అక్షరం స్థిరరాశి *url_కార్యక్రమం = streq (కార్యక్రమం, "[") ? "test" : కార్యక్రమం;
  రాయి (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_కార్యక్రమం);
  రాయి (_("or available locally via: info '(coreutils) %s%s'\n"),
          కణుపు, కణుపు == కార్యక్రమం ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#నిర్వచించు emit_try_help() \
  చేయి \
    { \
      fprintf (stderr, _("Try '%s --help' for more information.\n"), \
               కార్యక్రమం_name); \
    } \
  అయితే (0)

#అన్తర్భావ "inttostr.h"

స్థిర అంతఃవరుస అక్షరం *
timetostr (time_t t, అక్షరం *buf)
{
  తిరిగి (TYPE_SIGNED (time_t)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

స్థిర అంతఃవరుస అక్షరం *
bad_cast (అక్షరం స్థిరరాశి *s)
{
  తిరిగి (అక్షరం *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
స్థిర అంతఃవరుస bool
usable_st_కొలత (నిర్మాణం stat స్థిరరాశి *sb)
{
  తిరిగి (S_ISREG (sb->st_బహుళకం) || S_ISLNK (sb->st_బహుళకం)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn శూన్య usage (పూర్ణాంకం స్థితి_2);

#అన్తర్భావ <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#నిర్వచించు devmsg(...)			\
  చేయి					\
    {					\
      ఒకవేళ (dev_debug)			\
        fprintf (stderr, __VA_ARGS__);	\
    }					\
  అయితే (0)

#నిర్వచించు emit_చక్రం_warning(file_name)	\
  చేయి					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file system.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part యొక్క the చక్రం:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  అయితే (0)

/* exit with a _single_ "write error" diagnostic.  */

స్థిర అంతఃవరుస శూన్య
write_error (శూన్య)
{
  పూర్ణాంకం saved_errno = errno;
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
స్థిర అంతఃవరుస అక్షరం *
stzncpy (అక్షరం *పరిమిత dest, అక్షరం స్థిరరాశి *పరిమిత src, size_t len)
{
  కొరకు (size_t i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  తిరిగి dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
స్థిర అంతఃవరుస bool
is_ENOTSUP (పూర్ణాంకం err)
{
  తిరిగి err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#అన్తర్భావ "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#నిర్వచించు quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#నిర్వచించు quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#నిర్వచించు quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#నిర్వచించు quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#నిర్వచించు quoteN(x) quotearg_style (get_quoting_style (ఖాళీ), x)
#నిర్వచించు quoteN_mem(x, కొలత)                                     \
  quotearg_style_mem (get_quoting_style (ఖాళీ), x, కొలత)

#నిర్వచితమైతే ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
స్థిర అంతఃవరుస పూర్ణాంకం
getenv_quoting_style (శూన్య)
{
  అక్షరం స్థిరరాశి *q_style = getenv ("QUOTING_STYLE");
  ఒకవేళ (!q_style)
    తిరిగి -1;

  పూర్ణాంకం i = ARGMATCH (q_style, quoting_style_వాదనలు, quoting_style_vals);
  ఒకవేళ (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      తిరిగి -1;
    }
  తిరిగి quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
స్థిర అంతఃవరుస ptrdiff_t
x_timestyle_సరిపోలిక (అక్షరం స్థిరరాశి * style, bool allow_posix,
                   అక్షరం స్థిరరాశి *స్థిరరాశి * timestyle_వాదనలు,
                   అక్షరం స్థిరరాశి * timestyle_types,
                   size_t timestyle_types_కొలత,
                   పూర్ణాంకం fail_స్థితి)
{
  ptrdiff_t res = argmatch (style, timestyle_వాదనలు,
                            timestyle_types,
                            timestyle_types_కొలత);
  ఒకవేళ (res < 0)
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
      అక్షరం స్థిరరాశి *స్థిరరాశి *p = timestyle_వాదనలు;
      అక్షరం స్థిరరాశి *posix_ఉపసర్గ = allow_posix ? "[posix-]" : "";
      అయితే (*p)
        fprintf (stderr, "  - %s%s\n", posix_ఉపసర్గ, *p++);
      fputs (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), stderr);
      usage (fail_స్థితి);
    }

  తిరిగి res;
}
#అయితే_ముగింపు
