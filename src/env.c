/* env - run a program in a modified environment
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

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

/* Richard Mlynarik and David MacKenzie */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <argz.h>
#ضمن <getopt.h>
#ضمن <c-ctype.h>
#ضمن <signal.h>

#ضمن "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ضمن "hash.h"
#ضمن "hashcode-mem.h"
#ضمن "system.h"
#ضمن "operand2sig.h"
#ضمن "printenv.h"
#ضمن "quote.h"
#ضمن "read-file.h"
#ضمن "sig2str.h"
#ضمن "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "env"

#تعریف_کن AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ساكن حرف ثابت **usvars;
ساكن idx_t usvars_alloc;
ساكن idx_t usvars_استفاده_شده;

/* Storage for entries read by --env0-from.  */
ساكن حرف *env0_from_میانگیر;

/* An environment vector managed directly to preserve raw entries.  */
بنية env_وکتور
{
  حرف **variable;
  idx_t استفاده_شده;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ساكن bool dev_debug;
#تعریف_کن vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ساكن حرف *varname;
ساكن idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#لغو_تعریف IGNORE

/* Possible actions on each signal.  */
تعداد SIGNAL_نما {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ساكن تعداد SIGNAL_نما *سیگنالها;

/* Set of signals to block.  */
ساكن sigset_t بلوک_سیگنالها;

/* Set of signals to unblock.  */
ساكن sigset_t unblock_سیگنالها;

/* Whether signal mask adjustment requested.  */
ساكن bool sig_ماسک_تغییریافته;

/* Whether to list non default handling.  */
ساكن bool report_signal_handling;

/* The isspace characters in the C locale.  */
#تعریف_کن C_ISSPACE_CHARS " \t\n\v\f\r"

ساكن حرف ثابت shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  بلوک_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ساكن بنية option ثابت longopts[] =
{
  {"argv0", required_آرگومان, عدم, 'a'},
  {"ignore-environment", no_آرگومان, عدم, 'i'},
  {"env0-from", required_آرگومان, عدم, ENV0_FROM_OPTION},
  {"null", no_آرگومان, عدم, '0'},
  {"unset", required_آرگومان, عدم, 'u'},
  {"chdir", required_آرگومان, عدم, 'C'},
  {"default-signal", optional_آرگومان, عدم, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_آرگومان, عدم, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_آرگومان, عدم, بلوک_SIGNAL_OPTION},
  {"list-signal-handling", no_آرگومان, عدم,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_آرگومان, عدم, 'v'},
  {"quoting-style", required_آرگومان, عدم, QUOTING_STYLE_OPTION},
  {"split-string", required_آرگومان, عدم, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [-] [NAME=مقدار_2]... [COMMAND [ARG]...]\n"),
              برنامه_name);
      fputs (_("\
مجموعه_3 each NAME مقصد مقدار_2 درون the environment and پیاپی_2 COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth آرگومان از COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         شروع with an خالی environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         اقرأ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         انتها each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         remove variable from the environment\n\
"));
      oputs (_("\
  -C, --chdir=DIR\n\
         change working directory مقصد DIR\n\
"));
      oputs (_("\
      --quoting-style=کلمه\n\
         use quoting style کلمه لكل نامها and مقادیر:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --تقسیم-string=S\n\
         process and تقسیم S into separate arguments;\n\
         استفاده_شده مقصد pass مضرب arguments on shebang سطرها\n\
"));
      oputs (_("\
      --بلوک-إشارة[=SIG]\n\
         بلوک delivery از SIG إشارة(s) مقصد COMMAND\n\
"));
      oputs (_("\
      --افتراضي-إشارة[=SIG]\n\
         reset handling از SIG إشارة(s) مقصد the افتراضي\n\
"));
      oputs (_("\
      --ignore-إشارة[=SIG]\n\
         مجموعه_2 handling از SIG إشارة(s) مقصد افعل nothing\n\
"));
      oputs (_("\
      --list-إشارة-handling\n\
         list non افتراضي إشارة handling مقصد standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information لكل each processing گام\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - نتیجه_میدهد -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a إشارة name like 'PIPE', or a إشارة عدد like '13'.\n\
Without SIG, all معلوم سیگنالها are included.  مضرب_2 سیگنالها can be\n\
comma-separated.  An خالی SIG آرگومان is a no-op.\n\
"), stdout);
      emit_exec_وضعیت (برنامه_NAME);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

ساكن فراغ
append_unset_var (حرف ثابت *var)
{
  إذا (usvars_استفاده_شده == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, حجم *usvars);
  usvars[usvars_استفاده_شده++] = var;
}

ساكن فراغ
unset_envvars (فراغ)
{
  لكل (idx_t i = 0; i < usvars_استفاده_شده; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      إذا (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ساكن bool
مدخل_has_name (حرف ثابت *مدخل, حرف ثابت *name, idx_t name_درازی)
{
  حرف ثابت *eq = strchr (مدخل, '=');
  أرجع (eq && eq - مدخل == name_درازی
          && memeq (مدخل, name, name_درازی));
}

#إذا ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ساكن size_t
env_وکتور_جایگاه_hash (فراغ ثابت *x, size_t جدول_اندازه)
{
  حرف *ثابت *جایگاه = x;
  حرف ثابت *مدخل = *جایگاه;
  حرف ثابت *eq = strchr (مدخل, '=');
  أرجع hash_pjw_bare (مدخل, eq - مدخل) % جدول_اندازه;
}

ساكن bool
env_وکتور_جایگاه_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  حرف ثابت *a = * (حرف *ثابت *) x;
  حرف ثابت *b = * (حرف *ثابت *) y;
  حرف ثابت *b_eq = strchr (b, '=');
  أرجع مدخل_has_name (a, b, b_eq - b);
}
#پایان_اگر

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ساكن فراغ
unset_envvars_درون_وکتور (بنية env_وکتور *env)
{
  لكل (idx_t i = 0; i < usvars_استفاده_شده; ++i)
    {
      حرف ثابت *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      إذا (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_درازی = طول_رشته (name);
      idx_t dest = 0;
      لكل (idx_t منبع_2 = 0; منبع_2 < env->استفاده_شده; ++منبع_2)
        إذا (! مدخل_has_name (env->variable[منبع_2], name, name_درازی))
          env->variable[dest++] = env->variable[منبع_2];

      env->استفاده_شده = dest;
      env->variable[dest] = عدم;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ساكن فراغ
مجموعه_envvar (حرف *assignment, حرف *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  إذا (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
ساكن فراغ
مجموعه_envvar_درون_وکتور (بنية env_وکتور *env, حرف *assignment, حرف *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_درازی = eq - assignment;
  لكل (idx_t i = 0; i < env->استفاده_شده; ++i)
    إذا (مدخل_has_name (env->variable[i], assignment, name_درازی))
      {
        env->variable[i] = assignment;
        أرجع;
      }

  إذا (env->استفاده_شده + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               حجم *env->variable);
      environ = env->variable;
    }

  env->variable[env->استفاده_شده++] = assignment;
  env->variable[env->استفاده_شده] = عدم;
}

/* Read the NUL-delimited environment entries in FILE.  */
ساكن size_t
read_env_file (حرف ثابت *file)
{
  size_t اندازه;
  إذا (streq (file, "-"))
    {
      xset_دودویی_نما (STDIN_FILENO, O_دودویی);
      env0_from_میانگیر = fread_file (stdin, RF_دودویی, &اندازه);
    }
  وإلا
    env0_from_میانگیر = read_file (file, RF_دودویی, &اندازه);

  إذا (! env0_from_میانگیر)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  إذا (اندازه && env0_from_میانگیر[اندازه - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  أرجع اندازه;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ساكن bool
مجموعه_envvars_from_file (بنية env_وکتور *env, حرف ثابت *file)
{
  size_t اندازه = read_env_file (file);

#إذا defined _WIN32 && ! defined __CYGWIN__
  (فراغ) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  حرف *assignment = عدم;
  طالما ((assignment = argz_بعدی (env0_from_میانگیر, اندازه, assignment)))
    {
      حرف *eq = strchr (assignment, '=');
      إذا (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      مجموعه_envvar (assignment, eq);
    }
  أرجع false;
#وإلا
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_شمار = argz_شمار (env0_from_میانگیر, اندازه);

  إذا (file_شمار == 0)
    أرجع false;

  idx_t inherited_شمار = 0;
  طالما (environ[inherited_شمار])
    ++inherited_شمار;

  إذا (IDX_MAX - inherited_شمار <= file_شمار)
    xalloc_die ();
  idx_t max_شمار = inherited_شمار + file_شمار;
  حرف **merged = xnmalloc (max_شمار + 1, حجم *merged);
  کپی_حافظه (merged, environ, inherited_شمار * حجم *merged);
  merged[inherited_شمار] = عدم;

  Hash_جدول *جایگاه_جدول
    = hash_initialize (max_شمار, عدم, env_وکتور_جایگاه_hash,
                       env_وکتور_جایگاه_compare, عدم);
  إذا (! جایگاه_جدول)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  لكل (idx_t i = 0; i < inherited_شمار; ++i)
    {
      حرف *eq = strchr (merged[i], '=');
      إذا (! eq)
        استمر;

      إذا (! hash_درج (جایگاه_جدول, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_شمار = inherited_شمار;
  حرف *assignment = عدم;
  طالما ((assignment = argz_بعدی (env0_from_میانگیر, اندازه, assignment)))
    {
      حرف *eq = strchr (assignment, '=');
      إذا (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_شمار++] = assignment;
        }
      وإلا
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          حرف **جایگاه = hash_lookup (جایگاه_جدول, &assignment);
          إذا (جایگاه)
            *جایگاه = assignment;
          وإلا
            {
              merged[merged_شمار] = assignment;
              إذا (! hash_درج (جایگاه_جدول, &merged[merged_شمار]))
                xalloc_die ();
              ++merged_شمار;
            }
        }
    }

  merged[merged_شمار] = عدم;
  env->variable = merged;
  env->استفاده_شده = merged_شمار;
  env->allocated = max_شمار + 1;
  environ = merged;
  hash_free (جایگاه_جدول);
  أرجع true;
#پایان_اگر
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ساكن فراغ
مجموعه_raw_environment_from_file (بنية env_وکتور *env,
                               حرف ثابت *file)
{
  size_t اندازه = read_env_file (file);
  idx_t مدخل_شمار = argz_شمار (env0_from_میانگیر, اندازه);

  env->allocated = مدخل_شمار + 1;
  env->variable = xnmalloc (env->allocated, حجم *env->variable);
  argz_استخراج (env0_from_میانگیر, اندازه, env->variable);
  env->استفاده_شده = مدخل_شمار;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_خالص
ساكن حرف ثابت *
پیمایش_varname (حرف ثابت *str)
{
  إذا (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      حرف ثابت *انتها = str + 3;
      طالما (c_isalnum (*انتها) || *انتها == '_')
        ++انتها;
      إذا (*انتها == '}')
        أرجع انتها;
    }

  أرجع عدم;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ساكن حرف *
استخراج_varname (حرف ثابت *str)
{
  حرف ثابت *p = پیمایش_varname (str);

  إذا (!p)
    أرجع عدم;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  إذا (i >= vnlen)
    {
      حرر (varname);
      varname = xpalloc (عدم, &vnlen, i + 1 - vnlen, -1, حجم *varname);
    }

  کپی_حافظه (varname, str + 2, i);
  varname[i] = 0;

  أرجع varname;
}

/* Temporary buffer used by --split-string processing.  */
بنية splitbuf
{
  /* Buffer address, arg count, and half the number of elements in the buffer.
     ARGC and ARGV are as in 'main', and ARGC + 1 <= HALF_ALLOC so
     that the upper half of ARGV can be used for string contents.
     This may waste up to half the space but keeps the code simple,
     which is better for this rarely-used but security-sensitive code.

     ARGV[0] is not initialized; that is the caller's responsibility
     after finalization.

     During assembly, ARGV[I] (where 0 < I < ARGC) contains the offset
     of the Ith string (relative to ARGV + HALF_ALLOC), so that
     reallocating ARGV does not change the validity of its contents.
     The integer offset is cast to char * during assembly, and is
     converted to a true char * pointer on finalization.

     During assembly, ARGV[ARGC] contains the offset of the first
     unused string byte (relative to ARGV + HALF_ALLOC).  */
  حرف **argv;
  صحيح argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  صحيح extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
ساكن فراغ
splitbuf_grow (بنية splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_بایتها = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (بیشترین_صحیح, IDX_MAX), 2 * حجم *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_بایتها);
}

/* In SS, append C to the last string.  */
ساكن فراغ
splitbuf_append_byte (بنية splitbuf *ss, حرف c)
{
  idx_t string_بایتها = (intptr_t) ss->argv[ss->argc];
  إذا (ss->half_alloc * حجم *ss->argv <= string_بایتها)
    splitbuf_grow (ss);
  ((حرف *) (ss->argv + ss->half_alloc))[string_بایتها] = c;
  ss->argv[ss->argc] = (حرف *) (intptr_t) {string_بایتها + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ساكن فراغ
check_شروع_new_arg (بنية splitbuf *ss)
{
  إذا (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      صحيح argc = ss->argc;
      إذا (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
ساكن حرف **
splitbuf_finishup (بنية splitbuf *ss)
{
  صحيح argc = ss->argc;
  حرف **argv = ss->argv;
  حرف *stringbase = (حرف *) (ss->argv + ss->half_alloc);
  لكل (صحيح i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  أرجع argv;
}

/* Return a newly-allocated argv-like array,
   by parsing and splitting the input 'str'.

   'extra_argc' is the number of additional elements to allocate
   in the array (on top of the number of args required to split 'str').

   Store into *argc the number of arguments found (plus 1 for
   the program name).

   Example:
     int argc;
     char **argv = build_argv ("A=B uname -k', 3, &argc);
   Results in:
     argc = 4
     argv[0] = [not initialized]
     argv[1] = "A=B"
     argv[2] = "uname"
     argv[3] = "-k"
     argv[4,5,6,7] = [allocated due to extra_argc + 1, but not initialized]

   To free allocated memory:
     free (argv);
   However, 'env' does not free since it's about to exec or exit anyway
   and the complexity of keeping track of the storage that may have been
   allocated via multiple calls to build_argv is not worth the hassle.  */
ساكن حرف **
build_argv (حرف ثابت *str, صحيح extra_argc, صحيح *argc)
{
  bool dq = false, sq = false;
  بنية splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * حجم *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = عدم;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  طالما (*str)
    {
      حرف newc = *str; /* Default: add the next character.  */

      بدل (*str)
        {
        حالة '\'':
          إذا (dq)
            اكسر;
          sq = !sq;
          check_شروع_new_arg (&ss);
          ++str;
          استمر;

        حالة '"':
          إذا (sq)
            اكسر;
          dq = !dq;
          check_شروع_new_arg (&ss);
          ++str;
          استمر;

        حالة ' ': حالة '\t': حالة '\n': حالة '\v': حالة '\f': حالة '\r':
          /* Start a new argument if outside quotes.  */
          إذا (sq || dq)
            اكسر;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          استمر;

        حالة '#':
          إذا (!ss.sep)
            اكسر;
          اذهب eos; /* '#' as first char terminates the string.  */

        حالة '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          إذا (sq && str[1] != '\\' && str[1] != '\'')
            اكسر;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          بدل (newc)
            {
            حالة '"': حالة '#': حالة '$': حالة '\'': حالة '\\':
              /* Pass escaped character as-is.  */
              اكسر;

            حالة '_':
              إذا (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  استمر;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              اكسر;

            حالة 'c':
              إذا (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              اذهب eos; /* '\c' terminates the string.  */

            حالة 'f': newc = '\f'; اكسر;
            حالة 'n': newc = '\n'; اكسر;
            حالة 'r': newc = '\r'; اكسر;
            حالة 't': newc = '\t'; اكسر;
            حالة 'v': newc = '\v'; اكسر;

            حالة '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            افتراضي:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          اكسر;

        حالة '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          إذا (sq)
            اكسر;

          /* Store the ${VARNAME} value.  */
          {
            حرف *n = استخراج_varname (str);
            إذا (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            حرف *v = getenv (n);
            إذا (v)
              {
                check_شروع_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                لكل (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            وإلا
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            استمر;
          }
        }

      check_شروع_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  إذا (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  أرجع splitbuf_finishup (&ss);
}

/* Process an "-S" string and create the corresponding argv array.
   Update the given argc/argv parameters with the new argv.

   Example: if executed as:
      $ env -S"-i -C/tmp A=B" foo bar
   The input argv is:
      argv[0] = "env"
      argv[1] = "-S-i -C/tmp A=B"
      argv[2] = "foo"
      argv[3] = "bar"
      argv[4] = NULL
   This function will modify argv to be:
      argv[0] = "env"
      argv[1] = "-i"
      argv[2] = "-C/tmp"
      argv[3] = "A=B"
      argv[4] = "foo"
      argv[5] = "bar"
      argv[6] = NULL
   argc will be updated from 4 to 6.
   optind will be reset to 0 to force getopt_long to rescan all arguments.  */
ساكن فراغ
parse_تقسیم_string (حرف ثابت *str, صحيح *orig_optind,
                    صحيح *orig_argc, حرف ***orig_argv)
{
  صحيح extra_argc = *orig_argc - *orig_optind, newargc;
  حرف **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  إذا (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      لكل (صحيح i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  کپی_حافظه (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * حجم *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ساكن فراغ
parse_signal_action_params (حرف ثابت *arg, bool مجموعه_default)
{
  إذا (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      لكل (صحيح i = 1 ; i <= SIGNUM_BOUND; i++)
        سیگنالها[i] = مجموعه_default ? DEFAULT_NOERR : IGNORE_NOERR;
      أرجع;
    }

  حرف *optarg_writable = xstrdup (arg);

  حرف *opt_sig = strtok (optarg_writable, ",");
  طالما (opt_sig)
    {
      صحيح signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      إذا (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      إذا (signum <= 0)
        usage (exit_failure);

      سیگنالها[signum] = مجموعه_default ? DEFAULT : IGNORE;

      opt_sig = strtok (عدم, ",");
    }

  حرر (optarg_writable);
}

ساكن فراغ
reset_signal_handlers (فراغ)
{
  لكل (صحيح i = 1; i <= SIGNUM_BOUND; i++)
    {
      بنية sigaction act;

      إذا (سیگنالها[i] == UNCHANGED)
        استمر;

      bool ignore_errors = (سیگنالها[i] == DEFAULT_NOERR
                            || سیگنالها[i] == IGNORE_NOERR);

      bool مجموعه_مقصد_default = (سیگنالها[i] == DEFAULT
                             || سیگنالها[i] == DEFAULT_NOERR);

      صحيح sig_err = sigaction (i, عدم, &act);

      إذا (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      إذا (! sig_err)
        {
          act.sa_handler = مجموعه_مقصد_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, عدم);
          إذا (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      إذا (dev_debug)
        {
          حرف signame[SIG2STR_MAX];
          إذا (sig2str (i, signame) != 0)
            snprintf (signame, حجم signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  مجموعه_مقصد_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ساكن فراغ
parse_بلوک_signal_params (حرف ثابت *arg, bool بلوک)
{
  إذا (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (بلوک ? &بلوک_سیگنالها : &unblock_سیگنالها);
      sigemptyset (بلوک ? &unblock_سیگنالها : &بلوک_سیگنالها);
    }
  وإلا إذا (! sig_ماسک_تغییریافته)
    {
      /* Initialize the sets.  */
      sigemptyset (&بلوک_سیگنالها);
      sigemptyset (&unblock_سیگنالها);
    }

  sig_ماسک_تغییریافته = true;

  إذا (! arg)
    أرجع;

  حرف *optarg_writable = xstrdup (arg);

  حرف *opt_sig = strtok (optarg_writable, ",");
  طالما (opt_sig)
    {
      صحيح signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      إذا (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      إذا (signum <= 0)
        usage (exit_failure);

      إذا (sigaddset (بلوک ? &بلوک_سیگنالها : &unblock_سیگنالها, signum) == -1)
        {
          إذا (بلوک)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      وإلا
        sigdelset (بلوک ? &unblock_سیگنالها : &بلوک_سیگنالها, signum);

      opt_sig = strtok (عدم, ",");
    }

  حرر (optarg_writable);
}

ساكن فراغ
مجموعه_signal_proc_ماسک (فراغ)
{
  /* Get the existing signal mask */
  sigset_t مجموعه_2;

  sigemptyset (&مجموعه_2);

  إذا (sigprocmask (0, عدم, &مجموعه_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  لكل (صحيح i = 1; i <= SIGNUM_BOUND; i++)
    {
      حرف ثابت *debug_act = عدم;

      إذا (sigismember (&بلوک_سیگنالها, i))
        {
          sigaddset (&مجموعه_2, i);
          debug_act = "BLOCK";
        }
      وإلا إذا (sigismember (&unblock_سیگنالها, i))
        {
          sigdelset (&مجموعه_2, i);
          debug_act = "UNBLOCK";
        }

      إذا (dev_debug && debug_act)
        {
          حرف signame[SIG2STR_MAX];
          إذا (sig2str (i, signame) != 0)
            snprintf (signame, حجم signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  إذا (sigprocmask (SIG_SETMASK, &مجموعه_2, عدم))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ساكن فراغ
list_signal_handling (فراغ)
{
  sigset_t مجموعه_2;

  sigemptyset (&مجموعه_2);
  إذا (sigprocmask (0, عدم, &مجموعه_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  لكل (صحيح i = 1; i <= SIGNUM_BOUND; i++)
    {
      بنية sigaction act;
      إذا (sigaction (i, عدم, &act))
        استمر;

      حرف ثابت *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      حرف ثابت *blocked = sigismember (&مجموعه_2, i) ? "BLOCK" : "";
      حرف ثابت *connect = *ignored && *blocked ? "," : "";

      إذا (! *ignored && ! *blocked)
        استمر;

      حرف signame[SIG2STR_MAX];
      إذا (sig2str (i, signame) != 0)
        snprintf (signame, حجم signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ساكن فراغ
initialize_سیگنالها (فراغ)
{
  سیگنالها = xmalloc ((حجم *سیگنالها) * (SIGNUM_BOUND + 1));

  لكل (صحيح i = 0 ; i <= SIGNUM_BOUND; i++)
    سیگنالها[i] = UNCHANGED;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  صحيح quoting_style_opt = -1;
  حرف ثابت *newdir = عدم;
  حرف ثابت *env0_from_file = عدم;
  حرف *argv0 = عدم;
  بنية env_وکتور env_وکتور;
#اگر_تعریف_شده lint
  memset (&env_وکتور, 0, حجم env_وکتور);
  حرف **initial_environ = environ;
#پایان_اگر

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_سیگنالها ();

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, shortopts, longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'a':
          argv0 = optarg;
          اكسر;
        حالة 'i':
          ignore_environment = true;
          اكسر;
        حالة 'u':
          append_unset_var (optarg);
          اكسر;
        حالة 'v':
          dev_debug = true;
          اكسر;
        حالة '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          اكسر;
        حالة ENV0_FROM_OPTION:
          env0_from_file = optarg;
          اكسر;
        حالة DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_بلوک_signal_params (optarg, false);
          اكسر;
        حالة IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          اكسر;
        حالة بلوک_SIGNAL_OPTION:
          parse_بلوک_signal_params (optarg, true);
          اكسر;
        حالة LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          اكسر;
        حالة QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_آرگومانها,
                                         quoting_style_vals);
          اكسر;
        حالة 'C':
          newdir = optarg;
          اكسر;
        حالة 'S':
          parse_تقسیم_string (optarg, &optind, &argc, &argv);
          اكسر;
        حالة ' ': حالة '\t': حالة '\n': حالة '\v': حالة '\f': حالة '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            حرف c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_CANCELED);
        }
    }

  إذا (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  صحيح برنامه_شاخص = optind;
  طالما (برنامه_شاخص < argc && strchr (argv[برنامه_شاخص], '='))
    برنامه_شاخص++;
  bool برنامه_specified = برنامه_شاخص < argc;

  bool quote_output = false;
  صحيح qs = quoting_style_opt;
  إذا (! برنامه_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      إذا (qs < 0)
        qs = shell_escape_quoting_style;
    }
  إذا (0 <= qs && qs != literal_quoting_style)
    {
      مجموعه_quoting_style (عدم, qs);
      quote_output = true;
    }

  bool env_وکتور_فعال = false;
  إذا (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      مجموعه_raw_environment_from_file (&env_وکتور, env0_from_file);
      env_وکتور_فعال = true;
    }
  وإلا إذا (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#إذا defined _WIN32 && ! defined __CYGWIN__
      ساكن حرف *dummy_environ[] = { عدم };
      environ = dummy_environ;
#وإلا
      env_وکتور.variable = xnmalloc (1, حجم *env_وکتور.variable);
      env_وکتور.variable[0] = عدم;
      env_وکتور.استفاده_شده = 0;
      env_وکتور.allocated = 1;
      environ = env_وکتور.variable;
      env_وکتور_فعال = true;
#پایان_اگر
    }
  وإلا إذا (env0_from_file)
    env_وکتور_فعال = مجموعه_envvars_from_file (&env_وکتور, env0_from_file);

  إذا (! ignore_environment || env0_from_file)
    {
      إذا (env_وکتور_فعال)
        unset_envvars_درون_وکتور (&env_وکتور);
      وإلا
        unset_envvars ();
    }

  حرف *eq;
  طالما (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      إذا (env_وکتور_فعال)
        مجموعه_envvar_درون_وکتور (&env_وکتور, argv[optind], eq);
      وإلا
        مجموعه_envvar (argv[optind], eq);
      optind++;
    }

  إذا (opt_nul_terminate_output && برنامه_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  إذا (newdir && ! برنامه_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  إذا (argv0 && ! برنامه_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  إذا (! برنامه_specified)
    {
      حرف ثابت terminator = opt_nul_terminate_output ? '\0' : '\n';

      إذا (opt_nul_terminate_output)
        xset_دودویی_نما (STDOUT_FILENO, O_دودویی);

      /* Print the environment and exit.  */
      لكل (حرف *ثابت *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#اگر_تعریف_شده lint
      إذا (env_وکتور_فعال)
        {
          environ = initial_environ;
          حرر (env_وکتور.variable);
          حرر (env0_from_میانگیر);
        }
#پایان_اگر
      أرجع EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  إذا (sig_ماسک_تغییریافته)
    مجموعه_signal_proc_ماسک ();

  إذا (report_signal_handling)
    list_signal_handling ();

  إذا (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      إذا (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  حرف *برنامه = argv[optind];
  إذا (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  إذا (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (برنامه));
      لكل (صحيح i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (برنامه, &argv[optind]);

  صحيح exit_وضعیت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (برنامه));

  إذا (exit_وضعیت == EXIT_ENOENT && strpbrk (برنامه, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_وضعیت);
}
