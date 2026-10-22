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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <argz.h>
#شامل <getopt.h>
#شامل <c-ctype.h>
#شامل <signal.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "hash.h"
#شامل "hashcode-mem.h"
#شامل "system.h"
#شامل "operand2sig.h"
#شامل "printenv.h"
#شامل "quote.h"
#شامل "read-file.h"
#شامل "sig2str.h"
#شامل "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "env"

#تعریف_کرو AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ساکن حرف ثابت **usvars;
ساکن idx_t usvars_alloc;
ساکن idx_t usvars_مستعمل;

/* Storage for entries read by --env0-from.  */
ساکن حرف *env0_from_بفر;

/* An environment vector managed directly to preserve raw entries.  */
ساخت env_سمتیہ
{
  حرف **variable;
  idx_t مستعمل;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ساکن bool dev_debug;
#تعریف_کرو vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ساکن حرف *varname;
ساکن idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#تعریف_منسوخ IGNORE

/* Possible actions on each signal.  */
شمار SIGNAL_منوال {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ساکن شمار SIGNAL_منوال *اشارات;

/* Set of signals to block.  */
ساکن sigset_t بلاک_اشارات;

/* Set of signals to unblock.  */
ساکن sigset_t unblock_اشارات;

/* Whether signal mask adjustment requested.  */
ساکن bool sig_نقاب_بدلا;

/* Whether to list non default handling.  */
ساکن bool report_signal_handling;

/* The isspace characters in the C locale.  */
#تعریف_کرو C_ISSPACE_CHARS " \t\n\v\f\r"

ساکن حرف ثابت shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  بلاک_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ساکن ساخت option ثابت longopts[] =
{
  {"argv0", required_دلیل, تہی, 'a'},
  {"ignore-environment", no_دلیل, تہی, 'i'},
  {"env0-from", required_دلیل, تہی, ENV0_FROM_OPTION},
  {"null", no_دلیل, تہی, '0'},
  {"unset", required_دلیل, تہی, 'u'},
  {"chdir", required_دلیل, تہی, 'C'},
  {"default-signal", optional_دلیل, تہی, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_دلیل, تہی, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_دلیل, تہی, بلاک_SIGNAL_OPTION},
  {"list-signal-handling", no_دلیل, تہی,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_دلیل, تہی, 'v'},
  {"quoting-style", required_دلیل, تہی, QUOTING_STYLE_OPTION},
  {"split-string", required_دلیل, تہی, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [-] [NAME=قدر_2]... [COMMAND [ARG]...]\n"),
              پروگرام_name);
      fputs (_("\
سیٹ_3 each NAME مقصد قدر_2 اندر the environment and تسلسل_2 COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth دلیل کا COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         آغاز with an خالی_خانے environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         اقرأ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         انتہا each output سطر with NUL, not newline\n\
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
      --quoting-style=لفظ\n\
         use quoting style لفظ برائے نام and اقدار:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --بانٹو-string=S\n\
         process and بانٹو S into separate arguments;\n\
         مستعمل مقصد pass مضرب arguments on shebang سطریں\n\
"));
      oputs (_("\
      --بلاک-اشارہ[=SIG]\n\
         بلاک delivery کا SIG اشارہ(s) مقصد COMMAND\n\
"));
      oputs (_("\
      --طےشدہ-اشارہ[=SIG]\n\
         reset handling کا SIG اشارہ(s) مقصد the طےشدہ\n\
"));
      oputs (_("\
      --ignore-اشارہ[=SIG]\n\
         سیٹ_2 handling کا SIG اشارہ(s) مقصد کرو nothing\n\
"));
      oputs (_("\
      --list-اشارہ-handling\n\
         list non طےشدہ اشارہ handling مقصد standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information برائے each processing قدم\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - مستلزم -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a اشارہ name like 'PIPE', or a اشارہ نمبر like '13'.\n\
Without SIG, all معلوم اشارات are included.  مضرب_2 اشارات can be\n\
comma-separated.  An خالی_خانے SIG دلیل is a no-op.\n\
"), stdout);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن خالی
append_unset_var (حرف ثابت *var)
{
  اگر (usvars_مستعمل == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, حجم *usvars);
  usvars[usvars_مستعمل++] = var;
}

ساکن خالی
unset_envvars (خالی)
{
  برائے (idx_t i = 0; i < usvars_مستعمل; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      اگر (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ساکن bool
اندراج_has_name (حرف ثابت *اندراج_2, حرف ثابت *name, idx_t name_لمبائی)
{
  حرف ثابت *eq = strchr (اندراج_2, '=');
  واپس (eq && eq - اندراج_2 == name_لمبائی
          && memeq (اندراج_2, name, name_لمبائی));
}

#اگر ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ساکن size_t
env_سمتیہ_خانہ_hash (خالی ثابت *x, size_t جدول_سائز)
{
  حرف *ثابت *خانہ = x;
  حرف ثابت *اندراج_2 = *خانہ;
  حرف ثابت *eq = strchr (اندراج_2, '=');
  واپس hash_pjw_bare (اندراج_2, eq - اندراج_2) % جدول_سائز;
}

ساکن bool
env_سمتیہ_خانہ_compare (خالی ثابت *x, خالی ثابت *y)
{
  حرف ثابت *a = * (حرف *ثابت *) x;
  حرف ثابت *b = * (حرف *ثابت *) y;
  حرف ثابت *b_eq = strchr (b, '=');
  واپس اندراج_has_name (a, b, b_eq - b);
}
#اگر_ختم

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ساکن خالی
unset_envvars_اندر_سمتیہ (ساخت env_سمتیہ *env)
{
  برائے (idx_t i = 0; i < usvars_مستعمل; ++i)
    {
      حرف ثابت *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      اگر (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_لمبائی = لڑی_لمبائی (name);
      idx_t dest = 0;
      برائے (idx_t ماخذ_2 = 0; ماخذ_2 < env->مستعمل; ++ماخذ_2)
        اگر (! اندراج_has_name (env->variable[ماخذ_2], name, name_لمبائی))
          env->variable[dest++] = env->variable[ماخذ_2];

      env->مستعمل = dest;
      env->variable[dest] = تہی;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ساکن خالی
سیٹ_envvar (حرف *assignment, حرف *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  اگر (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
ساکن خالی
سیٹ_envvar_اندر_سمتیہ (ساخت env_سمتیہ *env, حرف *assignment, حرف *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_لمبائی = eq - assignment;
  برائے (idx_t i = 0; i < env->مستعمل; ++i)
    اگر (اندراج_has_name (env->variable[i], assignment, name_لمبائی))
      {
        env->variable[i] = assignment;
        واپس;
      }

  اگر (env->مستعمل + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               حجم *env->variable);
      environ = env->variable;
    }

  env->variable[env->مستعمل++] = assignment;
  env->variable[env->مستعمل] = تہی;
}

/* Read the NUL-delimited environment entries in FILE.  */
ساکن size_t
read_env_file (حرف ثابت *file)
{
  size_t سائز;
  اگر (streq (file, "-"))
    {
      xset_ثنائی_منوال (STDIN_FILENO, O_ثنائی);
      env0_from_بفر = fread_file (stdin, RF_ثنائی, &سائز);
    }
  ورنہ
    env0_from_بفر = read_file (file, RF_ثنائی, &سائز);

  اگر (! env0_from_بفر)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  اگر (سائز && env0_from_بفر[سائز - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  واپس سائز;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ساکن bool
سیٹ_envvars_from_file (ساخت env_سمتیہ *env, حرف ثابت *file)
{
  size_t سائز = read_env_file (file);

#اگر defined _WIN32 && ! defined __CYGWIN__
  (خالی) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  حرف *assignment = تہی;
  جب ((assignment = argz_اگلا (env0_from_بفر, سائز, assignment)))
    {
      حرف *eq = strchr (assignment, '=');
      اگر (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      سیٹ_envvar (assignment, eq);
    }
  واپس false;
#ورنہ
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_گنتی = argz_گنتی (env0_from_بفر, سائز);

  اگر (file_گنتی == 0)
    واپس false;

  idx_t inherited_گنتی = 0;
  جب (environ[inherited_گنتی])
    ++inherited_گنتی;

  اگر (IDX_MAX - inherited_گنتی <= file_گنتی)
    xalloc_die ();
  idx_t max_گنتی = inherited_گنتی + file_گنتی;
  حرف **merged = xnmalloc (max_گنتی + 1, حجم *merged);
  حافظہ_نقل (merged, environ, inherited_گنتی * حجم *merged);
  merged[inherited_گنتی] = تہی;

  Hash_جدول *خانہ_جدول
    = hash_initialize (max_گنتی, تہی, env_سمتیہ_خانہ_hash,
                       env_سمتیہ_خانہ_compare, تہی);
  اگر (! خانہ_جدول)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  برائے (idx_t i = 0; i < inherited_گنتی; ++i)
    {
      حرف *eq = strchr (merged[i], '=');
      اگر (! eq)
        جاری;

      اگر (! hash_داخل_کرو (خانہ_جدول, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_گنتی = inherited_گنتی;
  حرف *assignment = تہی;
  جب ((assignment = argz_اگلا (env0_from_بفر, سائز, assignment)))
    {
      حرف *eq = strchr (assignment, '=');
      اگر (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_گنتی++] = assignment;
        }
      ورنہ
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          حرف **خانہ = hash_lookup (خانہ_جدول, &assignment);
          اگر (خانہ)
            *خانہ = assignment;
          ورنہ
            {
              merged[merged_گنتی] = assignment;
              اگر (! hash_داخل_کرو (خانہ_جدول, &merged[merged_گنتی]))
                xalloc_die ();
              ++merged_گنتی;
            }
        }
    }

  merged[merged_گنتی] = تہی;
  env->variable = merged;
  env->مستعمل = merged_گنتی;
  env->allocated = max_گنتی + 1;
  environ = merged;
  hash_free (خانہ_جدول);
  واپس true;
#اگر_ختم
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ساکن خالی
سیٹ_raw_environment_from_file (ساخت env_سمتیہ *env,
                               حرف ثابت *file)
{
  size_t سائز = read_env_file (file);
  idx_t اندراج_گنتی = argz_گنتی (env0_from_بفر, سائز);

  env->allocated = اندراج_گنتی + 1;
  env->variable = xnmalloc (env->allocated, حجم *env->variable);
  argz_نکالو (env0_from_بفر, سائز, env->variable);
  env->مستعمل = اندراج_گنتی;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_خالص
ساکن حرف ثابت *
جائزہ_varname (حرف ثابت *str)
{
  اگر (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      حرف ثابت *انتہا = str + 3;
      جب (c_isalnum (*انتہا) || *انتہا == '_')
        ++انتہا;
      اگر (*انتہا == '}')
        واپس انتہا;
    }

  واپس تہی;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ساکن حرف *
نکالو_varname (حرف ثابت *str)
{
  حرف ثابت *p = جائزہ_varname (str);

  اگر (!p)
    واپس تہی;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  اگر (i >= vnlen)
    {
      آزاد (varname);
      varname = xpalloc (تہی, &vnlen, i + 1 - vnlen, -1, حجم *varname);
    }

  حافظہ_نقل (varname, str + 2, i);
  varname[i] = 0;

  واپس varname;
}

/* Temporary buffer used by --split-string processing.  */
ساخت splitbuf
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
  صحیح argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  صحیح extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
ساکن خالی
splitbuf_grow (ساخت splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_بائٹیں = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (زیادہ_سے_زیادہ_عدد, IDX_MAX), 2 * حجم *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_بائٹیں);
}

/* In SS, append C to the last string.  */
ساکن خالی
splitbuf_append_byte (ساخت splitbuf *ss, حرف c)
{
  idx_t string_بائٹیں = (intptr_t) ss->argv[ss->argc];
  اگر (ss->half_alloc * حجم *ss->argv <= string_بائٹیں)
    splitbuf_grow (ss);
  ((حرف *) (ss->argv + ss->half_alloc))[string_بائٹیں] = c;
  ss->argv[ss->argc] = (حرف *) (intptr_t) {string_بائٹیں + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ساکن خالی
check_آغاز_new_arg (ساخت splitbuf *ss)
{
  اگر (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      صحیح argc = ss->argc;
      اگر (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
ساکن حرف **
splitbuf_finishup (ساخت splitbuf *ss)
{
  صحیح argc = ss->argc;
  حرف **argv = ss->argv;
  حرف *stringbase = (حرف *) (ss->argv + ss->half_alloc);
  برائے (صحیح i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  واپس argv;
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
ساکن حرف **
build_argv (حرف ثابت *str, صحیح extra_argc, صحیح *argc)
{
  bool dq = false, sq = false;
  ساخت splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * حجم *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = تہی;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  جب (*str)
    {
      حرف newc = *str; /* Default: add the next character.  */

      انتخاب (*str)
        {
        حالت '\'':
          اگر (dq)
            توڑو;
          sq = !sq;
          check_آغاز_new_arg (&ss);
          ++str;
          جاری;

        حالت '"':
          اگر (sq)
            توڑو;
          dq = !dq;
          check_آغاز_new_arg (&ss);
          ++str;
          جاری;

        حالت ' ': حالت '\t': حالت '\n': حالت '\v': حالت '\f': حالت '\r':
          /* Start a new argument if outside quotes.  */
          اگر (sq || dq)
            توڑو;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          جاری;

        حالت '#':
          اگر (!ss.sep)
            توڑو;
          جاؤ eos; /* '#' as first char terminates the string.  */

        حالت '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          اگر (sq && str[1] != '\\' && str[1] != '\'')
            توڑو;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          انتخاب (newc)
            {
            حالت '"': حالت '#': حالت '$': حالت '\'': حالت '\\':
              /* Pass escaped character as-is.  */
              توڑو;

            حالت '_':
              اگر (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  جاری;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              توڑو;

            حالت 'c':
              اگر (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              جاؤ eos; /* '\c' terminates the string.  */

            حالت 'f': newc = '\f'; توڑو;
            حالت 'n': newc = '\n'; توڑو;
            حالت 'r': newc = '\r'; توڑو;
            حالت 't': newc = '\t'; توڑو;
            حالت 'v': newc = '\v'; توڑو;

            حالت '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            طےشدہ:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          توڑو;

        حالت '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          اگر (sq)
            توڑو;

          /* Store the ${VARNAME} value.  */
          {
            حرف *n = نکالو_varname (str);
            اگر (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            حرف *v = getenv (n);
            اگر (v)
              {
                check_آغاز_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                برائے (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            ورنہ
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            جاری;
          }
        }

      check_آغاز_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  اگر (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  واپس splitbuf_finishup (&ss);
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
ساکن خالی
parse_بانٹو_string (حرف ثابت *str, صحیح *orig_optind,
                    صحیح *orig_argc, حرف ***orig_argv)
{
  صحیح extra_argc = *orig_argc - *orig_optind, newargc;
  حرف **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  اگر (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      برائے (صحیح i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  حافظہ_نقل (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * حجم *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ساکن خالی
parse_signal_action_params (حرف ثابت *arg, bool سیٹ_default)
{
  اگر (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      برائے (صحیح i = 1 ; i <= SIGNUM_BOUND; i++)
        اشارات[i] = سیٹ_default ? DEFAULT_NOERR : IGNORE_NOERR;
      واپس;
    }

  حرف *optarg_writable = xstrdup (arg);

  حرف *opt_sig = strtok (optarg_writable, ",");
  جب (opt_sig)
    {
      صحیح signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      اگر (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      اگر (signum <= 0)
        usage (exit_failure);

      اشارات[signum] = سیٹ_default ? DEFAULT : IGNORE;

      opt_sig = strtok (تہی, ",");
    }

  آزاد (optarg_writable);
}

ساکن خالی
reset_signal_handlers (خالی)
{
  برائے (صحیح i = 1; i <= SIGNUM_BOUND; i++)
    {
      ساخت sigaction act;

      اگر (اشارات[i] == UNCHANGED)
        جاری;

      bool ignore_errors = (اشارات[i] == DEFAULT_NOERR
                            || اشارات[i] == IGNORE_NOERR);

      bool سیٹ_مقصد_default = (اشارات[i] == DEFAULT
                             || اشارات[i] == DEFAULT_NOERR);

      صحیح sig_err = sigaction (i, تہی, &act);

      اگر (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      اگر (! sig_err)
        {
          act.sa_handler = سیٹ_مقصد_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, تہی);
          اگر (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      اگر (dev_debug)
        {
          حرف signame[SIG2STR_MAX];
          اگر (sig2str (i, signame) != 0)
            snprintf (signame, حجم signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  سیٹ_مقصد_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ساکن خالی
parse_بلاک_signal_params (حرف ثابت *arg, bool بلاک)
{
  اگر (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (بلاک ? &بلاک_اشارات : &unblock_اشارات);
      sigemptyset (بلاک ? &unblock_اشارات : &بلاک_اشارات);
    }
  ورنہ اگر (! sig_نقاب_بدلا)
    {
      /* Initialize the sets.  */
      sigemptyset (&بلاک_اشارات);
      sigemptyset (&unblock_اشارات);
    }

  sig_نقاب_بدلا = true;

  اگر (! arg)
    واپس;

  حرف *optarg_writable = xstrdup (arg);

  حرف *opt_sig = strtok (optarg_writable, ",");
  جب (opt_sig)
    {
      صحیح signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      اگر (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      اگر (signum <= 0)
        usage (exit_failure);

      اگر (sigaddset (بلاک ? &بلاک_اشارات : &unblock_اشارات, signum) == -1)
        {
          اگر (بلاک)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      ورنہ
        sigdelset (بلاک ? &unblock_اشارات : &بلاک_اشارات, signum);

      opt_sig = strtok (تہی, ",");
    }

  آزاد (optarg_writable);
}

ساکن خالی
سیٹ_signal_proc_نقاب (خالی)
{
  /* Get the existing signal mask */
  sigset_t سیٹ_2;

  sigemptyset (&سیٹ_2);

  اگر (sigprocmask (0, تہی, &سیٹ_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  برائے (صحیح i = 1; i <= SIGNUM_BOUND; i++)
    {
      حرف ثابت *debug_act = تہی;

      اگر (sigismember (&بلاک_اشارات, i))
        {
          sigaddset (&سیٹ_2, i);
          debug_act = "BLOCK";
        }
      ورنہ اگر (sigismember (&unblock_اشارات, i))
        {
          sigdelset (&سیٹ_2, i);
          debug_act = "UNBLOCK";
        }

      اگر (dev_debug && debug_act)
        {
          حرف signame[SIG2STR_MAX];
          اگر (sig2str (i, signame) != 0)
            snprintf (signame, حجم signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  اگر (sigprocmask (SIG_SETMASK, &سیٹ_2, تہی))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ساکن خالی
list_signal_handling (خالی)
{
  sigset_t سیٹ_2;

  sigemptyset (&سیٹ_2);
  اگر (sigprocmask (0, تہی, &سیٹ_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  برائے (صحیح i = 1; i <= SIGNUM_BOUND; i++)
    {
      ساخت sigaction act;
      اگر (sigaction (i, تہی, &act))
        جاری;

      حرف ثابت *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      حرف ثابت *blocked = sigismember (&سیٹ_2, i) ? "BLOCK" : "";
      حرف ثابت *connect = *ignored && *blocked ? "," : "";

      اگر (! *ignored && ! *blocked)
        جاری;

      حرف signame[SIG2STR_MAX];
      اگر (sig2str (i, signame) != 0)
        snprintf (signame, حجم signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ساکن خالی
initialize_اشارات (خالی)
{
  اشارات = xmalloc ((حجم *اشارات) * (SIGNUM_BOUND + 1));

  برائے (صحیح i = 0 ; i <= SIGNUM_BOUND; i++)
    اشارات[i] = UNCHANGED;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  صحیح quoting_style_opt = -1;
  حرف ثابت *newdir = تہی;
  حرف ثابت *env0_from_file = تہی;
  حرف *argv0 = تہی;
  ساخت env_سمتیہ env_سمتیہ;
#اگر_تعریف_شدہ lint
  memset (&env_سمتیہ, 0, حجم env_سمتیہ);
  حرف **initial_environ = environ;
#اگر_ختم

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_اشارات ();

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, shortopts, longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'a':
          argv0 = optarg;
          توڑو;
        حالت 'i':
          ignore_environment = true;
          توڑو;
        حالت 'u':
          append_unset_var (optarg);
          توڑو;
        حالت 'v':
          dev_debug = true;
          توڑو;
        حالت '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          توڑو;
        حالت ENV0_FROM_OPTION:
          env0_from_file = optarg;
          توڑو;
        حالت DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_بلاک_signal_params (optarg, false);
          توڑو;
        حالت IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          توڑو;
        حالت بلاک_SIGNAL_OPTION:
          parse_بلاک_signal_params (optarg, true);
          توڑو;
        حالت LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          توڑو;
        حالت QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_دلائل,
                                         quoting_style_vals);
          توڑو;
        حالت 'C':
          newdir = optarg;
          توڑو;
        حالت 'S':
          parse_بانٹو_string (optarg, &optind, &argc, &argv);
          توڑو;
        حالت ' ': حالت '\t': حالت '\n': حالت '\v': حالت '\f': حالت '\r':
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
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_CANCELED);
        }
    }

  اگر (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  صحیح پروگرام_اشاریہ = optind;
  جب (پروگرام_اشاریہ < argc && strchr (argv[پروگرام_اشاریہ], '='))
    پروگرام_اشاریہ++;
  bool پروگرام_specified = پروگرام_اشاریہ < argc;

  bool quote_output = false;
  صحیح qs = quoting_style_opt;
  اگر (! پروگرام_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      اگر (qs < 0)
        qs = shell_escape_quoting_style;
    }
  اگر (0 <= qs && qs != literal_quoting_style)
    {
      سیٹ_quoting_style (تہی, qs);
      quote_output = true;
    }

  bool env_سمتیہ_فعال = false;
  اگر (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      سیٹ_raw_environment_from_file (&env_سمتیہ, env0_from_file);
      env_سمتیہ_فعال = true;
    }
  ورنہ اگر (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#اگر defined _WIN32 && ! defined __CYGWIN__
      ساکن حرف *dummy_environ[] = { تہی };
      environ = dummy_environ;
#ورنہ
      env_سمتیہ.variable = xnmalloc (1, حجم *env_سمتیہ.variable);
      env_سمتیہ.variable[0] = تہی;
      env_سمتیہ.مستعمل = 0;
      env_سمتیہ.allocated = 1;
      environ = env_سمتیہ.variable;
      env_سمتیہ_فعال = true;
#اگر_ختم
    }
  ورنہ اگر (env0_from_file)
    env_سمتیہ_فعال = سیٹ_envvars_from_file (&env_سمتیہ, env0_from_file);

  اگر (! ignore_environment || env0_from_file)
    {
      اگر (env_سمتیہ_فعال)
        unset_envvars_اندر_سمتیہ (&env_سمتیہ);
      ورنہ
        unset_envvars ();
    }

  حرف *eq;
  جب (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      اگر (env_سمتیہ_فعال)
        سیٹ_envvar_اندر_سمتیہ (&env_سمتیہ, argv[optind], eq);
      ورنہ
        سیٹ_envvar (argv[optind], eq);
      optind++;
    }

  اگر (opt_nul_terminate_output && پروگرام_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  اگر (newdir && ! پروگرام_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  اگر (argv0 && ! پروگرام_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  اگر (! پروگرام_specified)
    {
      حرف ثابت terminator = opt_nul_terminate_output ? '\0' : '\n';

      اگر (opt_nul_terminate_output)
        xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);

      /* Print the environment and exit.  */
      برائے (حرف *ثابت *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#اگر_تعریف_شدہ lint
      اگر (env_سمتیہ_فعال)
        {
          environ = initial_environ;
          آزاد (env_سمتیہ.variable);
          آزاد (env0_from_بفر);
        }
#اگر_ختم
      واپس EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  اگر (sig_نقاب_بدلا)
    سیٹ_signal_proc_نقاب ();

  اگر (report_signal_handling)
    list_signal_handling ();

  اگر (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      اگر (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  حرف *پروگرام = argv[optind];
  اگر (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  اگر (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (پروگرام));
      برائے (صحیح i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (پروگرام, &argv[optind]);

  صحیح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (پروگرام));

  اگر (exit_حالت == EXIT_ENOENT && strpbrk (پروگرام, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_حالت);
}
