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

#समावेश <attribute.h>

#समावेश <alloca.h>

#समावेश <ctype.h>

#समावेश <sys/stat.h>

/* Commonly used file permission combination.  */
#घोषणा MODE_RW_UGO (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

#अगर HAVE_SYS_PARAM_H
# include <sys/param.h>
#पूर्ण_अगर

#समावेश <unistd.h>

#समावेश <limits.h>

#समावेश "pathmax.h"
#अगर_अघोषित PATH_MAX
# define PATH_MAX 8192
#पूर्ण_अगर

#समावेश "configmake.h"

#समावेश <sys/time.h>
#समावेश <time.h>

/* Since major is a function on SVR4, we can't use 'ifndef major'.  */
#अगर MAJOR_IN_MKDEV
# include <sys/mkdev.h>
# define HAVE_MAJOR
#पूर्ण_अगर
#अगर MAJOR_IN_SYSMACROS
# include <sys/sysmacros.h>
# define HAVE_MAJOR
#पूर्ण_अगर
#अगर_घोषित major			/* Might be defined in sys/types.h.  */
# define HAVE_MAJOR
#पूर्ण_अगर

#अगर_अघोषित HAVE_MAJOR
# define major(dev)  (((dev) >> 8) & 0xff)
# define minor(dev)  ((dev) & 0xff)
# define makedev(maj, min)  (((maj) << 8) | (min))
#पूर्ण_अगर
#अघोषित HAVE_MAJOR

#अगर ! defined makedev && defined mkdev
# define makedev(maj, min)  mkdev (maj, min)
#पूर्ण_अगर

#समावेश <stdckdint.h>
#समावेश <stdcountof.h>
#समावेश <stddef.h>
#समावेश <string.h>
#समावेश <uchar.h>
#समावेश <errno.h>

/* Some systems don't define this; POSIX mentions it but says it is
   obsolete.  gnulib defines it, but only on native Windows systems,
   and there only because MSVC 10 does.  */
#अगर_अघोषित ENODATA
# define ENODATA (-1)
#पूर्ण_अगर

/* Return true if ERRNUM indicates that a file disappeared during a
   directory traversal.  */
अटल अंतरभूत bool
ignorable_traversal_errno (पूर्णांक errnum) { वापस errnum == ENOENT; }

#समावेश <stdlib.h>
#समावेश "version.h"

/* Exit statuses for programs like 'env' that exec other programs.  */
क्रमागत
{
  EXIT_TIMEDOUT = 124, /* Time expired before child completed.  */
  EXIT_CANCELED = 125, /* Internal error prior to exec attempt.  */
  EXIT_CANNOT_INVOKE = 126, /* Program located, but not usable.  */
  EXIT_ENOENT = 127 /* Could not find program to exec.  */
};

#समावेश "exitfail.h"

/* Set exit_failure to STATUS if that's not the default already.  */
अटल अंतरभूत व्योम
initialize_exit_failure (पूर्णांक status)
{
  अगर (status != निकास_त्रुटि)
    exit_failure = status;
}

#समावेश <fcntl.h>
#अगर_घोषित O_PATH
क्रमागत { O_PATHSEARCH = O_PATH };
#अन्यथा
क्रमागत { O_PATHSEARCH = O_खोज };
#पूर्ण_अगर

#समावेश <dirent.h>
#अगर_अघोषित _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(dp) म_माप ((dp)->d_name)
#पूर्ण_अगर

क्रमागत
{
  NOT_AN_INODE_संख्या = 0
};

#अगर_घोषित D_INO_IN_DIRENT
# define D_INO(dp) (dp)->d_ino
#अन्यथा
/* Some systems don't have inodes, so fake them to avoid lots of ifdefs.  */
# define D_INO(dp) NOT_AN_INODE_संख्या
#पूर्ण_अगर

/* include here for SIZE_MAX.  */
#समावेश <inttypes.h>

/* Redirection and wildcarding when done by the utility itself.
   Generally a noop, but used in particular for OS/2.  */
#अगर_अघोषित initialize_main
# ifndef __OS2__
#  define initialize_main(ac, av)
# अन्यथा
#  define initialize_main(ac, av) \
     करो { _wildcard (ac, av); _response (ac, av); } जबतक (0)
# endif
#पूर्ण_अगर

#समावेश "stat-macros.h"

#समावेश "timespec.h"

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
अटल अंतरभूत अचिन्हित अक्षर to_uchar (अक्षर ch) { वापस ch; }

/* Return non zero if a non breaking space.  */
ATTRIBUTE_PURE
अटल अंतरभूत पूर्णांक
c32isnbspace (char32_t wc)
{
  वापस wc == 0x00A0 || wc == 0x2007 || wc == 0x202F || wc == 0x2060;
}

ATTRIBUTE_PURE
अटल अंतरभूत पूर्णांक
c32isnewline (char32_t wc)
{
  वापस wc == 0x000A || wc == 0x0085 || wc == 0x2028 || wc == 0x2029;
}

ATTRIBUTE_PURE
अटल अंतरभूत पूर्णांक
c32isvertspace (char32_t wc)
{
  /* 000D is backwards as opposed to vertical,
     but that's fine for this grouping.  */
  वापस c32isnewline (wc) || wc == 0x000B || wc == 0x000C || wc == 0x000D;
}

/* c32isblank() is too variable on non GLIBC platforms.
   E.g., does not include \u3000 ideographic space on musl.
   E.g., does include non-breaking space on Solaris and NetBSD.
   This equivalent is more consistent across systems.  */
ATTRIBUTE_PURE
अटल अंतरभूत bool
c32issep (char32_t wc)
{
#अगर defined __GLIBC__
  वापस !! c32isblank (wc);
#अन्यथा
  वापस c32isspace (wc) && ! c32isvertspace (wc) && ! c32isnbspace (wc);
#पूर्ण_अगर
}

#समावेश <locale.h>

/* Take care of NLS matters.  */

#समावेश "gettext.h"
#अगर ! ENABLE_NLS
# undef textdomain
# define textdomain(Domainname) /* empty */
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) /* empty */
#पूर्ण_अगर

#घोषणा _(msgid) gettext (msgid)
#घोषणा N_(msgid) msgid

/* Return a value that pluralizes the same way that N does, in all
   languages we know of.  */
अटल अंतरभूत अचिन्हित दीर्घ पूर्णांक
select_plural (uintmax_t n)
{
  /* Reduce by a power of ten, but keep it away from zero.  The
     gettext manual says 1000000 should be safe.  */
  क्रमागत { PLURAL_REDUCER = 1000000 };
  वापस (n <= अच_दीर्घ_उच्च ? n : n % PLURAL_REDUCER + PLURAL_REDUCER);
}

#घोषणा STREQ_LEN(a, b, n) (म_भेदन (a, b, n) == 0) /* n==-1 means unbounded */
#घोषणा STRPREFIX(a, b) (म_भेदन (a, b, म_माप (b)) == 0)

/* Just like strncmp, but the second argument must be a literal string
   and you don't specify the length;  that comes from the literal.  */
#घोषणा STRNCMP_LIT(s, lit) म_भेदन (s, "" lit "", माप (lit) - 1)

#अगर !HAVE_DECL_GETLOGIN
अक्षर *getlogin (व्योम);
#पूर्ण_अगर

#अगर !HAVE_DECL_TTYNAME
अक्षर *ttyname (पूर्णांक);
#पूर्ण_अगर

#अगर !HAVE_DECL_GETEUID
uid_t geteuid (व्योम);
#पूर्ण_अगर

#अगर !HAVE_DECL_GETPWUID
काष्ठा passwd *getpwuid (uid_t);
#पूर्ण_अगर

#अगर !HAVE_DECL_GETGRGID
काष्ठा group *getgrgid (gid_t);
#पूर्ण_अगर

/* Interix has replacements for getgr{gid,nam,ent}, that don't
   query the domain controller for group members when not required.
   This speeds up the calls tremendously (<1 ms vs. >3 s). */
/* To protect any system that could provide _nomembers functions
   other than interix, check for HAVE_SETGROUPS, as interix is
   one of the very few (the only?) platform that lacks it */
#अगर ! HAVE_SETGROUPS
# अगर HAVE_GETGRGID_NOMEMBERS
#  define getgrgid(gid) getgrgid_nomembers(gid)
# endif
# अगर HAVE_GETGRNAM_NOMEMBERS
#  define getgrnam(nam) getgrnam_nomembers(nam)
# endif
# अगर HAVE_GETGRENT_NOMEMBERS
#  define getgrent() getgrent_nomembers()
# endif
#पूर्ण_अगर

#अगर !HAVE_DECL_GETUID
uid_t getuid (व्योम);
#पूर्ण_अगर

#समावेश "idx.h"
#समावेश "xalloc.h"
#समावेश "verify.h"
#समावेश "unlocked-io.h"
#समावेश "same-inode.h"

#समावेश "dirname.h"
#समावेश "openat.h"

अटल अंतरभूत bool
dot_or_dotdot (अक्षर स्थिर *file_name)
{
  अगर (file_name[0] == '.')
    {
      अक्षर sep = file_name[(file_name[1] == '.') + 1];
      वापस (! sep || ISSLASH (sep));
    }
  अन्यथा
    वापस false;
}

/* A wrapper for readdir so that callers don't see entries for '.' or '..'.  */
अटल अंतरभूत काष्ठा dirent स्थिर *
readdir_ignoring_dot_and_dotdot (सूची *dirp)
{
  जबतक (true)
    {
      काष्ठा dirent स्थिर *dp = सूची_पढ़ो (dirp);
      अगर (dp == शून्य || ! dot_or_dotdot (dp->d_name))
        वापस dp;
    }
}

/* Return -1 if DIR is an empty directory,
   0 if DIR is a nonempty directory,
   and a positive error number if there was trouble determining
   whether DIR is an empty or nonempty directory.  */
क्रमागत {
    DS_UNKNOWN = -2,
    DS_EMPTY = -1,
    DS_NONEMPTY = 0,
};
अटल अंतरभूत पूर्णांक
directory_status (पूर्णांक fd_cwd, अक्षर स्थिर *dir)
{
  सूची *dirp;
  bool no_direntries;
  पूर्णांक saved_errno;
  पूर्णांक fd = openat (fd_cwd, dir,
                   (O_RDONLY | O_DIRECTORY
                    | O_NOCTTY | O_NOFOLLOW | O_NONBLOCK));

  अगर (fd < 0)
    वापस त्रुटि_सं;

  dirp = fdopendir (fd);
  अगर (dirp == शून्य)
    {
      saved_errno = त्रुटि_सं;
      बंद (fd);
      वापस saved_errno;
    }

  त्रुटि_सं = 0;
  no_direntries = !readdir_ignoring_dot_and_dotdot (dirp);
  saved_errno = त्रुटि_सं;
  बंद_सूची (dirp);
  वापस no_direntries && saved_errno == 0 ? DS_EMPTY : saved_errno;
}

/* Factor out some of the common --help and --version processing code.  */

/* These enum values cannot possibly conflict with the option values
   ordinarily used by commands, including CHAR_MAX + 1, etc.  Avoid
   CHAR_MIN - 1, as it may equal -1, the getopt end-of-options value.  */
क्रमागत
{
  GETOPT_HELP_CHAR = (अक्षर_न्यून - 2),
  GETOPT_VERSION_CHAR = (अक्षर_न्यून - 3)
};

#घोषणा GETOPT_HELP_OPTION_DECL \
  "help", no_argument, शून्य, GETOPT_HELP_CHAR
#घोषणा GETOPT_VERSION_OPTION_DECL \
  "version", no_argument, शून्य, GETOPT_VERSION_CHAR
#घोषणा GETOPT_SELINUX_CONTEXT_OPTION_DECL \
  "context", optional_argument, शून्य, 'Z'

#घोषणा case_GETOPT_HELP_CHAR			\
  हाल GETOPT_HELP_CHAR:			\
    usage (निकास_सफल);			\
    अवरोध;

/* Program_name must be a literal string.
   Usually it is just PROGRAM_NAME.  */
#घोषणा USAGE_BUILTIN_WARNING \
  _("\n" \
"Your shell may have its own version of %s, which usually supersedes\n" \
"the version described here.  Please refer to your shell's documentation\n" \
"for details about the options it supports.\n")

#घोषणा HELP_OPTION_DESCRIPTION \
  _("      --help\n         display this help and exit\n")
#घोषणा VERSION_OPTION_DESCRIPTION \
  _("      --version\n         output version information and exit\n")

#समावेश "closein.h"
#समावेश "closeout.h"

#समावेश "version-etc.h"

#समावेश "propername.h"
/* Define away proper_name, since it's not worth the cost of adding ~17KB to
   the x86_64 text size of every single program.  This avoids a 40%
   (almost ~2MB) increase in the file system space utilization for the set
   of the 100 binaries. */
#घोषणा proper_name(x) proper_name_lite (x, x)

#समावेश "progname.h"

#घोषणा case_GETOPT_VERSION_CHAR(Program_name, Authors)			\
  हाल GETOPT_VERSION_CHAR:						\
    version_etc (मानक_निकास, Program_name, PACKAGE_NAME, Version, Authors,	\
                 (अक्षर *) शून्य);					\
    निकास (निकास_सफल);						\
    अवरोध;

#समावेश "minmax.h"
#समावेश "intprops.h"

#अगर_अघोषित SSIZE_MAX
# define SSIZE_MAX TYPE_MAXIMUM (ssize_t)
#पूर्ण_अगर

#अगर_अघोषित OFF_T_MIN
# define OFF_T_MIN TYPE_MINIMUM (off_t)
#पूर्ण_अगर

#अगर_अघोषित OFF_T_MAX
# define OFF_T_MAX TYPE_MAXIMUM (off_t)
#पूर्ण_अगर

#अगर_अघोषित UID_T_MAX
# define UID_T_MAX TYPE_MAXIMUM (uid_t)
#पूर्ण_अगर

#अगर_अघोषित GID_T_MAX
# define GID_T_MAX TYPE_MAXIMUM (gid_t)
#पूर्ण_अगर

#अगर_अघोषित PID_T_MAX
# define PID_T_MAX TYPE_MAXIMUM (pid_t)
#पूर्ण_अगर

/* Use this to suppress gcc warnings.  */
#अगर_घोषित lint
# define IF_LINT(Code) Code
#अन्यथा
# define IF_LINT(Code) /* empty */
#पूर्ण_अगर

/* main_exit should be called only from the main function.  It is
   equivalent to 'exit'.  When checking for lint it calls 'exit', to
   pacify gcc -fsanitize=lint which would otherwise have false alarms
   for pointers in the main function's activation record.  Otherwise
   it simply returns from 'main'; this used to be what gcc's static
   checking preferred and may yet be again.  */
#अगर_घोषित lint
# define main_exit(status) निकास (status)
#अन्यथा
# define main_exit(status) वापस status
#पूर्ण_अगर

#अगर_घोषित __GNUC__
# define LIKELY(cond)    __builtin_expect ((cond), 1)
# define UNLIKELY(cond)  __builtin_expect ((cond), 0)
#अन्यथा
# define LIKELY(cond)    (cond)
# define UNLIKELY(cond)  (cond)
#पूर्ण_अगर


#अगर defined strdupa
# define ASSIGN_STRDUPA(DEST, S)		\
  करो { DEST = strdupa (S); } जबतक (0)
#अन्यथा
# define ASSIGN_STRDUPA(DEST, S)		\
  करो						\
    {						\
      अक्षर स्थिर *s_ = (S);			\
      माप_प्रकार len_ = म_माप (s_) + 1;		\
      अक्षर *tmp_dest_ = alloca (len_);		\
      DEST = स_नकल (tmp_dest_, s_, len_);	\
    }						\
  जबतक (0)
#पूर्ण_अगर

#अगर ! HAVE_SYNC
# define sync() /* empty */
#पूर्ण_अगर

/* Compute the greatest common divisor of U and V using Euclid's
   algorithm.  U and V must be nonzero.  */

ATTRIBUTE_CONST
अटल अंतरभूत माप_प्रकार
gcd (माप_प्रकार u, माप_प्रकार v)
{
  करो
    {
      माप_प्रकार t = u % v;
      u = v;
      v = t;
    }
  जबतक (v);

  वापस u;
}

/* Compute the least common multiple of U and V.  U and V must be
   nonzero.  There is no overflow checking, so callers should not
   specify outlandish sizes.  */

ATTRIBUTE_CONST
अटल अंतरभूत माप_प्रकार
lcm (माप_प्रकार u, माप_प्रकार v)
{
  वापस u * (v / gcd (u, v));
}

/* Return PTR, aligned upward to the next multiple of ALIGNMENT.
   ALIGNMENT must be nonzero.  The caller must arrange for ((char *)
   PTR) through ((char *) PTR + ALIGNMENT - 1) to be addressable
   locations.  */

अटल अंतरभूत व्योम *
ptr_align (व्योम स्थिर *ptr, माप_प्रकार alignment)
{
  अक्षर स्थिर *p0 = ptr;
  अक्षर स्थिर *p1 = p0 + alignment - 1;
  वापस (व्योम *) (p1 - (माप_प्रकार) p1 % alignment);
}

/* Return whether the buffer consists entirely of NULs.
   Based on memeqzero in CCAN by Rusty Russell under CC0 (Public domain).  */

ATTRIBUTE_PURE
अटल अंतरभूत bool
is_nul (व्योम स्थिर *buf, माप_प्रकार लंबाई)
{
  स्थिर अचिन्हित अक्षर *p = buf;
/* Using possibly unaligned access for the first 16 bytes
   saves about 30-40 cycles, though it is strictly undefined behavior
   and so would need __attribute__ ((__no_sanitize_undefined__))
   to avoid -fsanitize=undefined warnings.
   Considering coreutils is mainly concerned with relatively
   large buffers, we'll just use the defined behavior.  */
#अगर 0 && (_STRING_ARCH_unaligned || _STRING_INLINE_unaligned)
  अचिन्हित दीर्घ word;
#अन्यथा
  अचिन्हित अक्षर word;
#पूर्ण_अगर

  अगर (! लंबाई)
    वापस true;

  /* Check len bytes not aligned on a word.  */
  जबतक (UNLIKELY (लंबाई & (माप word - 1)))
    {
      अगर (*p)
        वापस false;
      p++;
      लंबाई--;
      अगर (! लंबाई)
        वापस true;
   }

  /* Check up to 16 bytes a word at a time.  */
  क्रम (;;)
    {
      स_नकल (&word, p, माप word);
      अगर (word)
        वापस false;
      p += माप word;
      लंबाई -= माप word;
      अगर (! लंबाई)
        वापस true;
      अगर (UNLIKELY (लंबाई & 15) == 0)
        अवरोध;
   }

  /* Now we know first 16 bytes are NUL, memeq with self.  */
  वापस memeq (buf, p, लंबाई);
}

/* Set Accum = 10*Accum + Digit_val and return true, where Accum is an
   integer object and Digit_val an integer expression.  However, if
   the result overflows, set Accum to an unspecified value and return
   false.  Accum and Digit_val may be evaluated multiple times.  */

#घोषणा DECIMAL_DIGIT_ACCUMULATE(Accum, Digit_val)			\
  (!ckd_mul (&(Accum), Accum, 10) && !ckd_add (&(Accum), Accum, Digit_val))


/* Output --option descriptions;
   formatted with ANSI format and hyperlink codes.
   Any postprocessors like help2man etc. are expected to handle this,
   though it can be disabled in edge cases with the TERM=dumb env var.  */

#घोषणा oputs(option) oputs_ (PROGRAM_NAME, option)
अटल अंतरभूत व्योम
oputs_ (MAYBE_UNUSED अक्षर स्थिर *program, अक्षर स्थिर *option)
{
  अटल पूर्णांक help_markup =
#अगर defined MANUAL_URL || defined BOLD_MAN_REFS
    -1;  /* Lookup.  */
#अन्यथा
    0;   /* Disable.  */
#पूर्ण_अगर
  अगर (help_markup == -1)
    {
      /* Note one generally would want markup when piping to grep/less etc.
         but we're conservative here for now, and only markup to tty.  */
      अक्षर स्थिर *te;
      help_markup = (isatty (STDOUT_FILENO)
                     ? ((te = दो_पर्या ("TERM")) && *te && !streq (te, "dumb"))
                     : दो_पर्या ("IN_HELP2MAN") != शून्य);
    }
  अगर (! help_markup)
    {
      ख_माला_दो (option, मानक_निकास);
      वापस;
    }

  bool double_space = true;
  अक्षर स्थिर *first_word = option + म_अखोज (option, " \t\n");
  अक्षर स्थिर *option_text = म_अक्षर (option, '-');
  अगर (!option_text)                    /* for dd(1) option syntax.  */
    {
      option_text = first_word;
      /* Just match first word to support single spaced
         translated dd "foo=bar description" format.  */
      double_space = false;
    }
  अन्यथा अगर (option_text != first_word)  /* for test(1) option syntax.  */
    {
      /* Ensure only a single space encountered before '-', to avoid
         matching within descriptions for translated dd option syntax.  */
      अक्षर स्थिर *s = first_word;
      माप_प्रकार spaces = 0;
      जबतक (s < option_text && spaces < 2)
        spaces += !!है_खाली (to_uchar (*s++));
      अगर (spaces == 2)
        {
          /* Probably mismatched dd format.  */
          option_text = first_word;
          double_space = false;
        }
    }

  माप_प्रकार anchor_len = म_खोज (option_text, ",=[ \n");

  /* Set highlighted text up to spacing after the full option text.
     Any single space is included in highlighted text,
     double space or TAB or newline terminates the option text.  */
  अक्षर स्थिर *desc_text = option_text + anchor_len;
  जबतक (*desc_text && *desc_text != '\n')
    {
      अगर (*desc_text == '-' && *(desc_text + 1) == '-')
        double_space = false;
      अगर (है_खाली (to_uchar (*desc_text)))
        {
          अगर (*desc_text == '\t' || है_खाली (to_uchar (*(desc_text + 1))))
            अवरोध;
          /* With long options we restrict the match as some translations
             delimit a long option and description with a single space.  */
          अगर (!double_space && *(desc_text + 1) != '-')
            अवरोध;
        }

      desc_text++;
    }

  /* write spaces before option text. */
  ख_डालो (option, 1, first_word - option, मानक_निकास);

  /* write option text.  */
#अगर_घोषित MANUAL_URL
  अक्षर स्थिर *url_program =   streq (program, "[") ? "test"
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
  अगर (STREQ_LEN (option_text, "--help", 6)
      || STREQ_LEN (option_text, "--version", 9))
    {
      /* We don't have --help and --version links for each command,
         and they wouldn't be useful to reference anyway.
         Instead use these to reference the single node manual.  */
      म_लिखो ("\033]8;;%s%s#%s%.*s", PACKAGE_URL,
              url_program, url_program, (पूर्णांक) anchor_len, option_text);
    }
  अन्यथा
    {
      /* The single node manual doesn't work for ls, cksum, md5sum, sha*sum,
         so we link to the full manual.  */
      म_लिखो ("\033]8;;%s#%s%.*s", MANUAL_URL, url_program,
              (पूर्णांक) anchor_len, option_text);
    }
  ख_माला_दो ("\033\\", मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित BOLD_MAN_REFS
  /* Note help2man strips this and will reinstate with --bold-refs.  */
  ख_माला_दो ("\033[1m", मानक_निकास);
#पूर्ण_अगर
  /* first_word != option_text for test(1).  */
  ख_डालो (first_word, 1, desc_text - first_word, मानक_निकास);
#अगर_घोषित BOLD_MAN_REFS
  ख_माला_दो ("\033[0m", मानक_निकास);
#पूर्ण_अगर
#अगर_घोषित MANUAL_URL
  ख_माला_दो ("\033]8;;\033\\", मानक_निकास);
#पूर्ण_अगर

  /* write description.  */
  ख_माला_दो (desc_text, मानक_निकास);
}

/* If required and possible,
   call oputs with printf formatted message.  */

#घोषणा oprintf(...) oprintf_ (PROGRAM_NAME, __VA_ARGS__)
ATTRIBUTE_FORMAT ((म_लिखो, 2, 3))
अटल अंतरभूत व्योम
oprintf_ (अक्षर स्थिर *program, अक्षर स्थिर *message, ...)
{
  बहु_सूची args;
  अक्षर *buf;
  पूर्णांक buflen = -1;

#अगर defined MANUAL_URL || defined BOLD_MAN_REFS
  बहु_शुरू (args, message);
  buflen = vasprintf (&buf, message, args);
  बहु_पूर्ण (args);
#पूर्ण_अगर

  अगर (buflen < 0)
    {
      बहु_शुरू (args, message);
      भ_लिखो (message, args);
      बहु_पूर्ण (args);
      वापस;
    }

  oputs_ (program, buf);
  मुक्त (buf);
}

अटल अंतरभूत व्योम
emit_stdin_note (व्योम)
{
  ख_माला_दो (_("\n\
With no खाता, or when खाता is -, पढ़ो standard निवेश.\n\
"), मानक_निकास);
}
अटल अंतरभूत व्योम
emit_mandatory_arg_note (व्योम)
{
  ख_माला_दो (_("\n\
Mandatory arguments to दीर्घ options are mandatory क्रम लघु options too.\n\
"), मानक_निकास);
}

अटल अंतरभूत व्योम
emit_आकार_note (व्योम)
{
  ख_माला_दो (_("\n\
The आकार_2 argument is an integer and optional unit (example: 10K is 10*1024).\n\
Units are K,M,G,T,P,E,Z,Y,R,Q (powers of 1024) or KB,MB,... (powers of 1000).\n\
द्विआधारी_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
"), मानक_निकास);
}

अटल अंतरभूत व्योम
emit_blocksize_note (अक्षर स्थिर *program)
{
  म_लिखो (_("\n\
Display मूल्यावली are in units of the first available आकार_2 from --block-आकार,\n\
and the %s_BLOCK_आकार, BLOCK_आकार and BLOCKSIZE environment variables.\n\
Otherwise, units शेष to 1024 bytes (or 512 अगर POSIXLY_CORRECT is set).\n\
"), program);
}

अटल अंतरभूत व्योम
emit_update_parameters_note (व्योम)
{
  ख_माला_दो (_("\
\n\
UPDATE controls which existing files in the destination are replaced.\n\
'all' is the शेष operation when an --update option is not specified,\n\
and results in all existing files in the destination being replaced.\n\
'none' is like the --no-clobber option, in that no files in the\n\
destination are replaced, and skipped files करो not induce a failure.\n\
'none-fail' also ensures no files are replaced in the destination,\n\
but any skipped files are diagnosed and induce a failure.\n\
'older' is the शेष operation when --update is specified, and results\n\
in files being replaced अगर they're older than the corresponding source file.\n\
"), मानक_निकास);
}

अटल अंतरभूत व्योम
emit_backup_suffix_note (व्योम)
{
  ख_माला_दो (_("\
\n\
The backup suffix is '~', unless set with --suffix or SIMPLE_BACKUP_SUFFIX.\n\
The version control method may be selected via the --backup option or through\n\
the VERSION_CONTROL environment variable.  Here are the मूल्यावली:\n\
\n\
"), मानक_निकास);
  ख_माला_दो (_("\
  none, off       never make backups (even अगर --backup is given)\n\
  numbered, t     make numbered backups\n\
  existing, nil   numbered अगर numbered backups exist, simple otherwise\n\
  simple, never   always make simple backups\n\
"), मानक_निकास);
}

#घोषणा emit_symlink_recurse_options(default_opt) \
 emit_symlink_recurse_options_ (PROGRAM_NAME, default_opt)
अटल अंतरभूत व्योम
emit_symlink_recurse_options_ (अक्षर स्थिर *program, अक्षर स्थिर *default_opt)
{
      म_लिखो (_("\
\n\
The following options modify how a hierarchy is traversed when the -R\n\
option is also specified.  If more than one is specified, only the final\n\
one takes effect. %s is the शेष.\n\
\n\
"), default_opt);
      oputs_ (program, _("\
  -H\n\
         अगर a command पंक्ति argument is a symlink to a directory, traverse it\n\
"));
      oputs_ (program, _("\
  -L\n\
         traverse every symbolic link to a directory encountered\n\
"));
      oputs_ (program, _("\
  -P\n\
         करो not traverse any symbolic links\n\
\n\
"));
}

अटल अंतरभूत व्योम
emit_exec_status (अक्षर स्थिर *program)
{
      म_लिखो (_("\n\
Exit status:\n\
  125  अगर the %s command itself fails\n\
  126  अगर COMMAND is found but cannot be invoked\n\
  127  अगर COMMAND cannot be found\n\
  -    the निकास status of COMMAND otherwise\n\
"), program);
}

अटल अंतरभूत व्योम
emit_ancillary_info (अक्षर स्थिर *program)
{
  काष्ठा infomap { अक्षर स्थिर *program; अक्षर स्थिर *गाँठ; } स्थिर infomap[] = {
    { "[", "test invocation" },
    { "coreutils", "Multi-call invocation" },
    { "sha224sum", "sha2 utilities" },
    { "sha256sum", "sha2 utilities" },
    { "sha384sum", "sha2 utilities" },
    { "sha512sum", "sha2 utilities" },
    { शून्य, शून्य }
  };

  अक्षर स्थिर *गाँठ = program;
  काष्ठा infomap स्थिर *map_prog = infomap;

  जबतक (map_prog->program && ! streq (program, map_prog->program))
    map_prog++;

  अगर (map_prog->गाँठ)
    गाँठ = map_prog->गाँठ;

  emit_bug_reporting_address ();

  /* .htaccess on the coreutils web site maps programs to the appropriate page,
     however we explicitly handle "[" -> "test" here as the "[" is not
     recognized as part of a URL by default in terminals.  */
  अक्षर स्थिर *url_program = streq (program, "[") ? "test" : program;
  म_लिखो (_("Full documentation <%s%s>\n"),
          PACKAGE_URL, url_program);
  म_लिखो (_("or available locally via: info '(coreutils) %s%s'\n"),
          गाँठ, गाँठ == program ? " invocation" : "");
}

/* Use a macro rather than an inline function, as this references
   the global program_name, which causes dynamic linking issues
   in libstdbuf.so on some systems where unused functions
   are not removed by the linker.  */
#घोषणा emit_try_help() \
  करो \
    { \
      ख_लिखो (मानक_त्रुटि, _("Try '%s --help' for more information.\n"), \
               program_name); \
    } \
  जबतक (0)

#समावेश "inttostr.h"

अटल अंतरभूत अक्षर *
timetostr (समय_प्रकार t, अक्षर *buf)
{
  वापस (TYPE_SIGNED (समय_प्रकार)
          ? imaxtostr (t, buf)
          : umaxtostr (t, buf));
}

अटल अंतरभूत अक्षर *
bad_cast (अक्षर स्थिर *s)
{
  वापस (अक्षर *) s;
}

/* Return a boolean indicating whether SB->st_size is defined.  */
अटल अंतरभूत bool
usable_st_आकार (काष्ठा stat स्थिर *sb)
{
  वापस (S_ISREG (sb->st_mode) || S_ISLNK (sb->st_mode)
          || S_TYPEISSHM (sb) || S_TYPEISTMO (sb));
}

_Noreturn व्योम usage (पूर्णांक status);

#समावेश <error.h>

/* Like error(0, 0, ...), but without an implicit newline.
   Also a noop unless the global DEV_DEBUG is set.  */
#घोषणा devmsg(...)			\
  करो					\
    {					\
      अगर (dev_debug)			\
        ख_लिखो (मानक_त्रुटि, __VA_ARGS__);	\
    }					\
  जबतक (0)

#घोषणा emit_cycle_warning(file_name)	\
  करो					\
    {					\
      error (0, 0, _("\
WARNING: Circular directory structure.\n\
This almost certainly means that you have a corrupted file प्रणाली.\n\
NOTIFY YOUR SYSTEM MANAGER.\n\
The following directory is part of the cycle:\n  %s\n"), \
             quotef (file_name));	\
    }					\
  जबतक (0)

/* exit with a _single_ "write error" diagnostic.  */

अटल अंतरभूत व्योम
write_error (व्योम)
{
  पूर्णांक saved_errno = त्रुटि_सं;
  ख_साफ (मानक_निकास);    /* Last attempt to write any buffered data.  */
  fpurge (मानक_निकास);    /* Ensure nothing buffered that might induce an error. */
  साफ_त्रुटि (मानक_निकास);  /* Avoid extraneous diagnostic from close_stdout.  */
  error (निकास_त्रुटि, saved_errno, _("write error"));
}

/* Like stpncpy, but do ensure that the result is NUL-terminated,
   and do not NUL-pad out to LEN.  I.e., when strnlen (src, len) == len,
   this function writes a NUL byte into dest[len].  Thus, the length
   of the destination buffer must be at least LEN + 1.
   The DEST and SRC buffers must not overlap.  */
अटल अंतरभूत अक्षर *
stzncpy (अक्षर *restrict dest, अक्षर स्थिर *restrict src, माप_प्रकार len)
{
  क्रम (माप_प्रकार i = 0; i < len && *src; i++)
    *dest++ = *src++;
  *dest = 0;
  वापस dest;
}

/* Return true if ERR is ENOTSUP or EOPNOTSUPP, otherwise false.
   This wrapper function avoids the redundant 'or'd comparison on
   systems like Linux for which they have the same value.  It also
   avoids the gcc warning to that effect.  */
अटल अंतरभूत bool
is_ENOTSUP (पूर्णांक err)
{
  वापस err == EOPNOTSUPP || (ENOTSUP != EOPNOTSUPP && err == ENOTSUP);
}


/* How coreutils quotes filenames, to minimize use of outer quotes,
   but also provide better support for copy and paste when used.  */
#समावेश "quotearg.h"

/* Use these to shell quote only when necessary,
   when the quoted item is already delimited with colons.  */
#घोषणा quotef(arg) \
  quotearg_n_style_colon (0, shell_escape_quoting_style, arg)
#घोषणा quotef_n(n, arg) \
  quotearg_n_style_colon (n, shell_escape_quoting_style, arg)

/* Use these when there are spaces around the file name,
   in the error message.  */
#घोषणा quoteaf(arg) \
  quotearg_style (shell_escape_always_quoting_style, arg)
#घोषणा quoteaf_n(n, arg) \
  quotearg_n_style (n, shell_escape_always_quoting_style, arg)

/* Equivalent to quotearg(), but explicit to avoid syntax checks.  */
#घोषणा quoteN(x) quotearg_style (get_quoting_style (शून्य), x)
#घोषणा quoteN_mem(x, आकार)                                     \
  quotearg_style_mem (get_quoting_style (शून्य), x, आकार)

#अगर_घोषित ARGMATCH
/* Return the quoting style specified by the environment variable
   QUOTING_STYLE if set and valid, -1 otherwise.  */
अटल अंतरभूत पूर्णांक
getenv_quoting_style (व्योम)
{
  अक्षर स्थिर *q_style = दो_पर्या ("QUOTING_STYLE");
  अगर (!q_style)
    वापस -1;

  पूर्णांक i = ARGMATCH (q_style, quoting_style_args, quoting_style_vals);
  अगर (i < 0)
    {
      error (0, 0, _("ignoring invalid value of environment "
                     "variable QUOTING_STYLE: %s"), quote (q_style));
      वापस -1;
    }
  वापस quoting_style_vals[i];
}

/* Used instead of XARGMATCH() to provide a custom error message.  */
अटल अंतरभूत सूचक_भेद_प्रकार
x_timestyle_match (अक्षर स्थिर * style, bool allow_posix,
                   अक्षर स्थिर *स्थिर * timestyle_args,
                   अक्षर स्थिर * timestyle_types,
                   माप_प्रकार timestyle_types_आकार,
                   पूर्णांक fail_status)
{
  सूचक_भेद_प्रकार res = argmatch (style, timestyle_args,
                            timestyle_types,
                            timestyle_types_आकार);
  अगर (res < 0)
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
      ख_माला_दो (_("Valid arguments are:\n"), मानक_त्रुटि);
      अक्षर स्थिर *स्थिर *p = timestyle_args;
      अक्षर स्थिर *posix_prefix = allow_posix ? "[posix-]" : "";
      जबतक (*p)
        ख_लिखो (मानक_त्रुटि, "  - %s%s\n", posix_prefix, *p++);
      ख_माला_दो (_("  - +FORMAT (e.g., +%H:%M) for a 'date'-style"
               " format\n"), मानक_त्रुटि);
      usage (fail_status);
    }

  वापस res;
}
#पूर्ण_अगर
