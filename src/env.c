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
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ایستا نویسه ثابت **usvars;
ایستا idx_t usvars_alloc;
ایستا idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
ایستا نویسه *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
ساختار env_vector
{
  نویسه **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ایستا bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ایستا نویسه *varname;
ایستا idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
شمارش SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ایستا شمارش SIGNAL_MODE *signals;

/* Set of signals to block.  */
ایستا sigset_t block_signals;

/* Set of signals to unblock.  */
ایستا sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
ایستا bool sig_mask_changed;

/* Whether to list non default handling.  */
ایستا bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

ایستا نویسه ثابت shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ایستا ساختار option ثابت longopts[] =
{
  {"argv0", required_argument, پوچ, 'a'},
  {"ignore-environment", no_argument, پوچ, 'i'},
  {"env0-from", required_argument, پوچ, ENV0_FROM_OPTION},
  {"null", no_argument, پوچ, '0'},
  {"unset", required_argument, پوچ, 'u'},
  {"chdir", required_argument, پوچ, 'C'},
  {"default-signal", optional_argument, پوچ, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, پوچ, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, پوچ, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, پوچ,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, پوچ, 'v'},
  {"quoting-style", required_argument, پوچ, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, پوچ, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=مقدار_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to مقدار_2 in the environment and run COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth argument of COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         start with an empty environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         اقرأ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         remove variable from the environment\n\
"));
      oputs (_("\
  -C, --chdir=DIR\n\
         change working directory to DIR\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD برای names and مقادیر:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang سطرها\n\
"));
      oputs (_("\
      --block-سیگنال[=SIG]\n\
         block delivery of SIG سیگنال(s) to COMMAND\n\
"));
      oputs (_("\
      --پیشفرض-سیگنال[=SIG]\n\
         reset handling of SIG سیگنال(s) to the پیشفرض\n\
"));
      oputs (_("\
      --ignore-سیگنال[=SIG]\n\
         set handling of SIG سیگنال(s) to انجامبده nothing\n\
"));
      oputs (_("\
      --list-سیگنال-handling\n\
         list non پیشفرض سیگنال handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information برای each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a سیگنال name like 'PIPE', or a سیگنال عدد like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ایستا تهی
append_unset_var (نویسه ثابت *var)
{
  اگر (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, اندازه *usvars);
  usvars[usvars_used++] = var;
}

ایستا تهی
unset_envvars (تهی)
{
  برای (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      اگر (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ایستا bool
entry_has_name (نویسه ثابت *entry, نویسه ثابت *name, idx_t name_length)
{
  نویسه ثابت *eq = strchr (entry, '=');
  بازگشت (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#اگر ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ایستا size_t
env_vector_slot_hash (تهی ثابت *x, size_t table_size)
{
  نویسه *ثابت *slot = x;
  نویسه ثابت *entry = *slot;
  نویسه ثابت *eq = strchr (entry, '=');
  بازگشت hash_pjw_bare (entry, eq - entry) % table_size;
}

ایستا bool
env_vector_slot_compare (تهی ثابت *x, تهی ثابت *y)
{
  نویسه ثابت *a = * (نویسه *ثابت *) x;
  نویسه ثابت *b = * (نویسه *ثابت *) y;
  نویسه ثابت *b_eq = strchr (b, '=');
  بازگشت entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ایستا تهی
unset_envvars_in_vector (ساختار env_vector *env)
{
  برای (idx_t i = 0; i < usvars_used; ++i)
    {
      نویسه ثابت *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      اگر (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      برای (idx_t source = 0; source < env->used; ++source)
        اگر (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = پوچ;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ایستا تهی
set_envvar (نویسه *assignment, نویسه *eq)
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
ایستا تهی
set_envvar_in_vector (ساختار env_vector *env, نویسه *assignment, نویسه *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  برای (idx_t i = 0; i < env->used; ++i)
    اگر (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        بازگشت;
      }

  اگر (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               اندازه *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = پوچ;
}

/* Read the NUL-delimited environment entries in FILE.  */
ایستا size_t
read_env_file (نویسه ثابت *file)
{
  size_t size;
  اگر (streq (file, "-"))
    {
      xset_دودویی_mode (STDIN_FILENO, O_دودویی);
      env0_from_buffer = fread_file (stdin, RF_دودویی, &size);
    }
  وگرنه
    env0_from_buffer = read_file (file, RF_دودویی, &size);

  اگر (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  اگر (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  بازگشت size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ایستا bool
set_envvars_from_file (ساختار env_vector *env, نویسه ثابت *file)
{
  size_t size = read_env_file (file);

#اگر defined _WIN32 && ! defined __CYGWIN__
  (تهی) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  نویسه *assignment = پوچ;
  وقتی ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      نویسه *eq = strchr (assignment, '=');
      اگر (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  بازگشت false;
#وگرنه
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_شمار = argz_شمار (env0_from_buffer, size);

  اگر (file_شمار == 0)
    بازگشت false;

  idx_t inherited_شمار = 0;
  وقتی (environ[inherited_شمار])
    ++inherited_شمار;

  اگر (IDX_MAX - inherited_شمار <= file_شمار)
    xalloc_die ();
  idx_t max_شمار = inherited_شمار + file_شمار;
  نویسه **merged = xnmalloc (max_شمار + 1, اندازه *merged);
  memcpy (merged, environ, inherited_شمار * اندازه *merged);
  merged[inherited_شمار] = پوچ;

  Hash_table *slot_table
    = hash_initialize (max_شمار, پوچ, env_vector_slot_hash,
                       env_vector_slot_compare, پوچ);
  اگر (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  برای (idx_t i = 0; i < inherited_شمار; ++i)
    {
      نویسه *eq = strchr (merged[i], '=');
      اگر (! eq)
        ادامه;

      اگر (! hash_درج (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_شمار = inherited_شمار;
  نویسه *assignment = پوچ;
  وقتی ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      نویسه *eq = strchr (assignment, '=');
      اگر (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_شمار++] = assignment;
        }
      وگرنه
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          نویسه **slot = hash_lookup (slot_table, &assignment);
          اگر (slot)
            *slot = assignment;
          وگرنه
            {
              merged[merged_شمار] = assignment;
              اگر (! hash_درج (slot_table, &merged[merged_شمار]))
                xalloc_die ();
              ++merged_شمار;
            }
        }
    }

  merged[merged_شمار] = پوچ;
  env->variable = merged;
  env->used = merged_شمار;
  env->allocated = max_شمار + 1;
  environ = merged;
  hash_free (slot_table);
  بازگشت true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ایستا تهی
set_raw_environment_from_file (ساختار env_vector *env,
                               نویسه ثابت *file)
{
  size_t size = read_env_file (file);
  idx_t entry_شمار = argz_شمار (env0_from_buffer, size);

  env->allocated = entry_شمار + 1;
  env->variable = xnmalloc (env->allocated, اندازه *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_شمار;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
ایستا نویسه ثابت *
scan_varname (نویسه ثابت *str)
{
  اگر (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      نویسه ثابت *end = str + 3;
      وقتی (c_isalnum (*end) || *end == '_')
        ++end;
      اگر (*end == '}')
        بازگشت end;
    }

  بازگشت پوچ;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ایستا نویسه *
extract_varname (نویسه ثابت *str)
{
  نویسه ثابت *p = scan_varname (str);

  اگر (!p)
    بازگشت پوچ;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  اگر (i >= vnlen)
    {
      آزاد (varname);
      varname = xpalloc (پوچ, &vnlen, i + 1 - vnlen, -1, اندازه *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  بازگشت varname;
}

/* Temporary buffer used by --split-string processing.  */
ساختار splitbuf
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
  نویسه **argv;
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
ایستا تهی
splitbuf_grow (ساختار splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * اندازه *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
ایستا تهی
splitbuf_append_byte (ساختار splitbuf *ss, نویسه c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  اگر (ss->half_alloc * اندازه *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((نویسه *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (نویسه *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ایستا تهی
check_start_new_arg (ساختار splitbuf *ss)
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
ایستا نویسه **
splitbuf_finishup (ساختار splitbuf *ss)
{
  صحیح argc = ss->argc;
  نویسه **argv = ss->argv;
  نویسه *stringbase = (نویسه *) (ss->argv + ss->half_alloc);
  برای (صحیح i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  بازگشت argv;
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
ایستا نویسه **
build_argv (نویسه ثابت *str, صحیح extra_argc, صحیح *argc)
{
  bool dq = false, sq = false;
  ساختار splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * اندازه *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = پوچ;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  وقتی (*str)
    {
      نویسه newc = *str; /* Default: add the next character.  */

      انتخاب (*str)
        {
        حالت '\'':
          اگر (dq)
            بشکن;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          ادامه;

        حالت '"':
          اگر (sq)
            بشکن;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          ادامه;

        حالت ' ': حالت '\t': حالت '\n': حالت '\v': حالت '\f': حالت '\r':
          /* Start a new argument if outside quotes.  */
          اگر (sq || dq)
            بشکن;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          ادامه;

        حالت '#':
          اگر (!ss.sep)
            بشکن;
          برو eos; /* '#' as first char terminates the string.  */

        حالت '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          اگر (sq && str[1] != '\\' && str[1] != '\'')
            بشکن;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          انتخاب (newc)
            {
            حالت '"': حالت '#': حالت '$': حالت '\'': حالت '\\':
              /* Pass escaped character as-is.  */
              بشکن;

            حالت '_':
              اگر (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  ادامه;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              بشکن;

            حالت 'c':
              اگر (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              برو eos; /* '\c' terminates the string.  */

            حالت 'f': newc = '\f'; بشکن;
            حالت 'n': newc = '\n'; بشکن;
            حالت 'r': newc = '\r'; بشکن;
            حالت 't': newc = '\t'; بشکن;
            حالت 'v': newc = '\v'; بشکن;

            حالت '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            پیشفرض:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          بشکن;

        حالت '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          اگر (sq)
            بشکن;

          /* Store the ${VARNAME} value.  */
          {
            نویسه *n = extract_varname (str);
            اگر (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            نویسه *v = getenv (n);
            اگر (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                برای (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            وگرنه
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            ادامه;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  اگر (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  بازگشت splitbuf_finishup (&ss);
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
ایستا تهی
parse_split_string (نویسه ثابت *str, صحیح *orig_optind,
                    صحیح *orig_argc, نویسه ***orig_argv)
{
  صحیح extra_argc = *orig_argc - *orig_optind, newargc;
  نویسه **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  اگر (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      برای (صحیح i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * اندازه *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ایستا تهی
parse_signal_action_params (نویسه ثابت *arg, bool set_default)
{
  اگر (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      برای (صحیح i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      بازگشت;
    }

  نویسه *optarg_writable = xstrdup (arg);

  نویسه *opt_sig = strtok (optarg_writable, ",");
  وقتی (opt_sig)
    {
      صحیح signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      اگر (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      اگر (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (پوچ, ",");
    }

  آزاد (optarg_writable);
}

ایستا تهی
reset_signal_handlers (تهی)
{
  برای (صحیح i = 1; i <= SIGNUM_BOUND; i++)
    {
      ساختار sigaction act;

      اگر (signals[i] == UNCHANGED)
        ادامه;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      صحیح sig_err = sigaction (i, پوچ, &act);

      اگر (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      اگر (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, پوچ);
          اگر (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      اگر (dev_debug)
        {
          نویسه signame[SIG2STR_MAX];
          اگر (sig2str (i, signame) != 0)
            snprintf (signame, اندازه signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ایستا تهی
parse_block_signal_params (نویسه ثابت *arg, bool block)
{
  اگر (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  وگرنه اگر (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  اگر (! arg)
    بازگشت;

  نویسه *optarg_writable = xstrdup (arg);

  نویسه *opt_sig = strtok (optarg_writable, ",");
  وقتی (opt_sig)
    {
      صحیح signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      اگر (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      اگر (signum <= 0)
        usage (exit_failure);

      اگر (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          اگر (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      وگرنه
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (پوچ, ",");
    }

  آزاد (optarg_writable);
}

ایستا تهی
set_signal_proc_mask (تهی)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  اگر (sigprocmask (0, پوچ, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  برای (صحیح i = 1; i <= SIGNUM_BOUND; i++)
    {
      نویسه ثابت *debug_act = پوچ;

      اگر (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      وگرنه اگر (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      اگر (dev_debug && debug_act)
        {
          نویسه signame[SIG2STR_MAX];
          اگر (sig2str (i, signame) != 0)
            snprintf (signame, اندازه signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  اگر (sigprocmask (SIG_SETMASK, &set, پوچ))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ایستا تهی
list_signal_handling (تهی)
{
  sigset_t set;

  sigemptyset (&set);
  اگر (sigprocmask (0, پوچ, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  برای (صحیح i = 1; i <= SIGNUM_BOUND; i++)
    {
      ساختار sigaction act;
      اگر (sigaction (i, پوچ, &act))
        ادامه;

      نویسه ثابت *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      نویسه ثابت *blocked = sigismember (&set, i) ? "BLOCK" : "";
      نویسه ثابت *connect = *ignored && *blocked ? "," : "";

      اگر (! *ignored && ! *blocked)
        ادامه;

      نویسه signame[SIG2STR_MAX];
      اگر (sig2str (i, signame) != 0)
        snprintf (signame, اندازه signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ایستا تهی
initialize_signals (تهی)
{
  signals = xmalloc ((اندازه *signals) * (SIGNUM_BOUND + 1));

  برای (صحیح i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  صحیح quoting_style_opt = -1;
  نویسه ثابت *newdir = پوچ;
  نویسه ثابت *env0_from_file = پوچ;
  نویسه *argv0 = پوچ;
  ساختار env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, اندازه env_vector);
  نویسه **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, shortopts, longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'a':
          argv0 = optarg;
          بشکن;
        حالت 'i':
          ignore_environment = true;
          بشکن;
        حالت 'u':
          append_unset_var (optarg);
          بشکن;
        حالت 'v':
          dev_debug = true;
          بشکن;
        حالت '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          بشکن;
        حالت ENV0_FROM_OPTION:
          env0_from_file = optarg;
          بشکن;
        حالت DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          بشکن;
        حالت IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          بشکن;
        حالت BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          بشکن;
        حالت LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          بشکن;
        حالت QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          بشکن;
        حالت 'C':
          newdir = optarg;
          بشکن;
        حالت 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          بشکن;
        حالت ' ': حالت '\t': حالت '\n': حالت '\v': حالت '\f': حالت '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            نویسه c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_CANCELED);
        }
    }

  اگر (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  صحیح program_index = optind;
  وقتی (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  صحیح qs = quoting_style_opt;
  اگر (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      اگر (qs < 0)
        qs = shell_escape_quoting_style;
    }
  اگر (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (پوچ, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  اگر (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  وگرنه اگر (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#اگر defined _WIN32 && ! defined __CYGWIN__
      ایستا نویسه *dummy_environ[] = { پوچ };
      environ = dummy_environ;
#وگرنه
      env_vector.variable = xnmalloc (1, اندازه *env_vector.variable);
      env_vector.variable[0] = پوچ;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  وگرنه اگر (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  اگر (! ignore_environment || env0_from_file)
    {
      اگر (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      وگرنه
        unset_envvars ();
    }

  نویسه *eq;
  وقتی (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      اگر (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      وگرنه
        set_envvar (argv[optind], eq);
      optind++;
    }

  اگر (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  اگر (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  اگر (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  اگر (! program_specified)
    {
      نویسه ثابت terminator = opt_nul_terminate_output ? '\0' : '\n';

      اگر (opt_nul_terminate_output)
        xset_دودویی_mode (STDOUT_FILENO, O_دودویی);

      /* Print the environment and exit.  */
      برای (نویسه *ثابت *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      اگر (env_vector_active)
        {
          environ = initial_environ;
          آزاد (env_vector.variable);
          آزاد (env0_from_buffer);
        }
#endif
      بازگشت EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  اگر (sig_mask_changed)
    set_signal_proc_mask ();

  اگر (report_signal_handling)
    list_signal_handling ();

  اگر (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      اگر (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  نویسه *program = argv[optind];
  اگر (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  اگر (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      برای (صحیح i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  صحیح exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  اگر (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
