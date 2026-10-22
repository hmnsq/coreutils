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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <argz.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <c-ctype.h>
#अन्तर्भाव <signal.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "hash.h"
#अन्तर्भाव "hashcode-mem.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "operand2sig.h"
#अन्तर्भाव "printenv.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "read-file.h"
#अन्तर्भाव "sig2str.h"
#अन्तर्भाव "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
स्थिर वर्ण ध्रुव **usvars;
स्थिर idx_t usvars_alloc;
स्थिर idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
स्थिर वर्ण *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
संरचना env_vector
{
  वर्ण **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
स्थिर bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
स्थिर वर्ण *varname;
स्थिर idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
गणना SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
स्थिर गणना SIGNAL_MODE *signals;

/* Set of signals to block.  */
स्थिर sigset_t block_signals;

/* Set of signals to unblock.  */
स्थिर sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
स्थिर bool sig_mask_changed;

/* Whether to list non default handling.  */
स्थिर bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

स्थिर वर्ण ध्रुव shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"argv0", required_argument, रिक्त, 'a'},
  {"ignore-environment", no_argument, रिक्त, 'i'},
  {"env0-from", required_argument, रिक्त, ENV0_FROM_OPTION},
  {"null", no_argument, रिक्त, '0'},
  {"unset", required_argument, रिक्त, 'u'},
  {"chdir", required_argument, रिक्त, 'C'},
  {"default-signal", optional_argument, रिक्त, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, रिक्त, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, रिक्त, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, रिक्त,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, रिक्त, 'v'},
  {"quoting-style", required_argument, रिक्त, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, रिक्त, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=मानम्_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to मानम्_2 in the environment and run COMMAND.\n\
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
         पठ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output पङ्क्तिः with NUL, not newline\n\
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
         use quoting style WORD अत्थाय names and मानानि:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang पङ्क्तयः\n\
"));
      oputs (_("\
      --block-सङ्केत[=SIG]\n\
         block delivery of SIG सङ्केत(s) to COMMAND\n\
"));
      oputs (_("\
      --स्वभाव-सङ्केत[=SIG]\n\
         reset handling of SIG सङ्केत(s) to the स्वभाव\n\
"));
      oputs (_("\
      --ignore-सङ्केत[=SIG]\n\
         set handling of SIG सङ्केत(s) to करोहि nothing\n\
"));
      oputs (_("\
      --list-सङ्केत-handling\n\
         list non स्वभाव सङ्केत handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information अत्थाय each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a सङ्केत name like 'PIPE', or a सङ्केत सङ्ख्या like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

स्थिर शून्य
append_unset_var (वर्ण ध्रुव *var)
{
  सचे (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, परिमाण *usvars);
  usvars[usvars_used++] = var;
}

स्थिर शून्य
unset_envvars (शून्य)
{
  अत्थाय (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      सचे (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
स्थिर bool
entry_has_name (वर्ण ध्रुव *entry, वर्ण ध्रुव *name, idx_t name_length)
{
  वर्ण ध्रुव *eq = strchr (entry, '=');
  पटिनिवत्त (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#सचे ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
स्थिर size_t
env_vector_slot_hash (शून्य ध्रुव *x, size_t table_size)
{
  वर्ण *ध्रुव *slot = x;
  वर्ण ध्रुव *entry = *slot;
  वर्ण ध्रुव *eq = strchr (entry, '=');
  पटिनिवत्त hash_pjw_bare (entry, eq - entry) % table_size;
}

स्थिर bool
env_vector_slot_compare (शून्य ध्रुव *x, शून्य ध्रुव *y)
{
  वर्ण ध्रुव *a = * (वर्ण *ध्रुव *) x;
  वर्ण ध्रुव *b = * (वर्ण *ध्रुव *) y;
  वर्ण ध्रुव *b_eq = strchr (b, '=');
  पटिनिवत्त entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
स्थिर शून्य
unset_envvars_in_vector (संरचना env_vector *env)
{
  अत्थाय (idx_t i = 0; i < usvars_used; ++i)
    {
      वर्ण ध्रुव *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      सचे (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      अत्थाय (idx_t source = 0; source < env->used; ++source)
        सचे (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = रिक्त;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
स्थिर शून्य
set_envvar (वर्ण *assignment, वर्ण *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  सचे (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
स्थिर शून्य
set_envvar_in_vector (संरचना env_vector *env, वर्ण *assignment, वर्ण *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  अत्थाय (idx_t i = 0; i < env->used; ++i)
    सचे (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        पटिनिवत्त;
      }

  सचे (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               परिमाण *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = रिक्त;
}

/* Read the NUL-delimited environment entries in FILE.  */
स्थिर size_t
read_env_file (वर्ण ध्रुव *file)
{
  size_t size;
  सचे (streq (file, "-"))
    {
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
      env0_from_buffer = fread_file (stdin, RF_द्विमान, &size);
    }
  अञ्ञथा
    env0_from_buffer = read_file (file, RF_द्विमान, &size);

  सचे (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  सचे (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  पटिनिवत्त size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
स्थिर bool
set_envvars_from_file (संरचना env_vector *env, वर्ण ध्रुव *file)
{
  size_t size = read_env_file (file);

#सचे defined _WIN32 && ! defined __CYGWIN__
  (शून्य) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  वर्ण *assignment = रिक्त;
  याव ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      वर्ण *eq = strchr (assignment, '=');
      सचे (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  पटिनिवत्त false;
#अञ्ञथा
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_गणनम् = argz_गणनम् (env0_from_buffer, size);

  सचे (file_गणनम् == 0)
    पटिनिवत्त false;

  idx_t inherited_गणनम् = 0;
  याव (environ[inherited_गणनम्])
    ++inherited_गणनम्;

  सचे (IDX_MAX - inherited_गणनम् <= file_गणनम्)
    xalloc_die ();
  idx_t max_गणनम् = inherited_गणनम् + file_गणनम्;
  वर्ण **merged = xnmalloc (max_गणनम् + 1, परिमाण *merged);
  memcpy (merged, environ, inherited_गणनम् * परिमाण *merged);
  merged[inherited_गणनम्] = रिक्त;

  Hash_table *slot_table
    = hash_initialize (max_गणनम्, रिक्त, env_vector_slot_hash,
                       env_vector_slot_compare, रिक्त);
  सचे (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  अत्थाय (idx_t i = 0; i < inherited_गणनम्; ++i)
    {
      वर्ण *eq = strchr (merged[i], '=');
      सचे (! eq)
        अनुवर्त;

      सचे (! hash_निवेशय (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_गणनम् = inherited_गणनम्;
  वर्ण *assignment = रिक्त;
  याव ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      वर्ण *eq = strchr (assignment, '=');
      सचे (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_गणनम्++] = assignment;
        }
      अञ्ञथा
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          वर्ण **slot = hash_lookup (slot_table, &assignment);
          सचे (slot)
            *slot = assignment;
          अञ्ञथा
            {
              merged[merged_गणनम्] = assignment;
              सचे (! hash_निवेशय (slot_table, &merged[merged_गणनम्]))
                xalloc_die ();
              ++merged_गणनम्;
            }
        }
    }

  merged[merged_गणनम्] = रिक्त;
  env->variable = merged;
  env->used = merged_गणनम्;
  env->allocated = max_गणनम् + 1;
  environ = merged;
  hash_free (slot_table);
  पटिनिवत्त true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
स्थिर शून्य
set_raw_environment_from_file (संरचना env_vector *env,
                               वर्ण ध्रुव *file)
{
  size_t size = read_env_file (file);
  idx_t entry_गणनम् = argz_गणनम् (env0_from_buffer, size);

  env->allocated = entry_गणनम् + 1;
  env->variable = xnmalloc (env->allocated, परिमाण *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_गणनम्;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
स्थिर वर्ण ध्रुव *
scan_varname (वर्ण ध्रुव *str)
{
  सचे (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      वर्ण ध्रुव *end = str + 3;
      याव (c_isalnum (*end) || *end == '_')
        ++end;
      सचे (*end == '}')
        पटिनिवत्त end;
    }

  पटिनिवत्त रिक्त;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
स्थिर वर्ण *
extract_varname (वर्ण ध्रुव *str)
{
  वर्ण ध्रुव *p = scan_varname (str);

  सचे (!p)
    पटिनिवत्त रिक्त;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  सचे (i >= vnlen)
    {
      मुक्त (varname);
      varname = xpalloc (रिक्त, &vnlen, i + 1 - vnlen, -1, परिमाण *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  पटिनिवत्त varname;
}

/* Temporary buffer used by --split-string processing.  */
संरचना splitbuf
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
  वर्ण **argv;
  पूर्णाङ्क argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  पूर्णाङ्क extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
स्थिर शून्य
splitbuf_grow (संरचना splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * परिमाण *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
स्थिर शून्य
splitbuf_append_byte (संरचना splitbuf *ss, वर्ण c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  सचे (ss->half_alloc * परिमाण *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((वर्ण *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (वर्ण *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
स्थिर शून्य
check_start_new_arg (संरचना splitbuf *ss)
{
  सचे (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      पूर्णाङ्क argc = ss->argc;
      सचे (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
स्थिर वर्ण **
splitbuf_finishup (संरचना splitbuf *ss)
{
  पूर्णाङ्क argc = ss->argc;
  वर्ण **argv = ss->argv;
  वर्ण *stringbase = (वर्ण *) (ss->argv + ss->half_alloc);
  अत्थाय (पूर्णाङ्क i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  पटिनिवत्त argv;
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
स्थिर वर्ण **
build_argv (वर्ण ध्रुव *str, पूर्णाङ्क extra_argc, पूर्णाङ्क *argc)
{
  bool dq = false, sq = false;
  संरचना splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * परिमाण *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = रिक्त;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  याव (*str)
    {
      वर्ण newc = *str; /* Default: add the next character.  */

      विकल्प (*str)
        {
        पक्ष '\'':
          सचे (dq)
            भङ्ग;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          अनुवर्त;

        पक्ष '"':
          सचे (sq)
            भङ्ग;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          अनुवर्त;

        पक्ष ' ': पक्ष '\t': पक्ष '\n': पक्ष '\v': पक्ष '\f': पक्ष '\r':
          /* Start a new argument if outside quotes.  */
          सचे (sq || dq)
            भङ्ग;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          अनुवर्त;

        पक्ष '#':
          सचे (!ss.sep)
            भङ्ग;
          गच्छ eos; /* '#' as first char terminates the string.  */

        पक्ष '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          सचे (sq && str[1] != '\\' && str[1] != '\'')
            भङ्ग;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          विकल्प (newc)
            {
            पक्ष '"': पक्ष '#': पक्ष '$': पक्ष '\'': पक्ष '\\':
              /* Pass escaped character as-is.  */
              भङ्ग;

            पक्ष '_':
              सचे (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  अनुवर्त;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              भङ्ग;

            पक्ष 'c':
              सचे (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              गच्छ eos; /* '\c' terminates the string.  */

            पक्ष 'f': newc = '\f'; भङ्ग;
            पक्ष 'n': newc = '\n'; भङ्ग;
            पक्ष 'r': newc = '\r'; भङ्ग;
            पक्ष 't': newc = '\t'; भङ्ग;
            पक्ष 'v': newc = '\v'; भङ्ग;

            पक्ष '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            स्वभाव:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          भङ्ग;

        पक्ष '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          सचे (sq)
            भङ्ग;

          /* Store the ${VARNAME} value.  */
          {
            वर्ण *n = extract_varname (str);
            सचे (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            वर्ण *v = getenv (n);
            सचे (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                अत्थाय (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            अञ्ञथा
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            अनुवर्त;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  सचे (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  पटिनिवत्त splitbuf_finishup (&ss);
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
स्थिर शून्य
parse_split_string (वर्ण ध्रुव *str, पूर्णाङ्क *orig_optind,
                    पूर्णाङ्क *orig_argc, वर्ण ***orig_argv)
{
  पूर्णाङ्क extra_argc = *orig_argc - *orig_optind, newargc;
  वर्ण **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  सचे (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      अत्थाय (पूर्णाङ्क i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * परिमाण *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

स्थिर शून्य
parse_signal_action_params (वर्ण ध्रुव *arg, bool set_default)
{
  सचे (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      अत्थाय (पूर्णाङ्क i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      पटिनिवत्त;
    }

  वर्ण *optarg_writable = xstrdup (arg);

  वर्ण *opt_sig = strtok (optarg_writable, ",");
  याव (opt_sig)
    {
      पूर्णाङ्क signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      सचे (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      सचे (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (रिक्त, ",");
    }

  मुक्त (optarg_writable);
}

स्थिर शून्य
reset_signal_handlers (शून्य)
{
  अत्थाय (पूर्णाङ्क i = 1; i <= SIGNUM_BOUND; i++)
    {
      संरचना sigaction act;

      सचे (signals[i] == UNCHANGED)
        अनुवर्त;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      पूर्णाङ्क sig_err = sigaction (i, रिक्त, &act);

      सचे (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      सचे (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, रिक्त);
          सचे (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      सचे (dev_debug)
        {
          वर्ण signame[SIG2STR_MAX];
          सचे (sig2str (i, signame) != 0)
            snprintf (signame, परिमाण signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


स्थिर शून्य
parse_block_signal_params (वर्ण ध्रुव *arg, bool block)
{
  सचे (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  अञ्ञथा सचे (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  सचे (! arg)
    पटिनिवत्त;

  वर्ण *optarg_writable = xstrdup (arg);

  वर्ण *opt_sig = strtok (optarg_writable, ",");
  याव (opt_sig)
    {
      पूर्णाङ्क signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      सचे (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      सचे (signum <= 0)
        usage (exit_failure);

      सचे (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          सचे (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      अञ्ञथा
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (रिक्त, ",");
    }

  मुक्त (optarg_writable);
}

स्थिर शून्य
set_signal_proc_mask (शून्य)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  सचे (sigprocmask (0, रिक्त, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  अत्थाय (पूर्णाङ्क i = 1; i <= SIGNUM_BOUND; i++)
    {
      वर्ण ध्रुव *debug_act = रिक्त;

      सचे (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      अञ्ञथा सचे (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      सचे (dev_debug && debug_act)
        {
          वर्ण signame[SIG2STR_MAX];
          सचे (sig2str (i, signame) != 0)
            snprintf (signame, परिमाण signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  सचे (sigprocmask (SIG_SETMASK, &set, रिक्त))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

स्थिर शून्य
list_signal_handling (शून्य)
{
  sigset_t set;

  sigemptyset (&set);
  सचे (sigprocmask (0, रिक्त, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  अत्थाय (पूर्णाङ्क i = 1; i <= SIGNUM_BOUND; i++)
    {
      संरचना sigaction act;
      सचे (sigaction (i, रिक्त, &act))
        अनुवर्त;

      वर्ण ध्रुव *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      वर्ण ध्रुव *blocked = sigismember (&set, i) ? "BLOCK" : "";
      वर्ण ध्रुव *connect = *ignored && *blocked ? "," : "";

      सचे (! *ignored && ! *blocked)
        अनुवर्त;

      वर्ण signame[SIG2STR_MAX];
      सचे (sig2str (i, signame) != 0)
        snprintf (signame, परिमाण signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

स्थिर शून्य
initialize_signals (शून्य)
{
  signals = xmalloc ((परिमाण *signals) * (SIGNUM_BOUND + 1));

  अत्थाय (पूर्णाङ्क i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  पूर्णाङ्क quoting_style_opt = -1;
  वर्ण ध्रुव *newdir = रिक्त;
  वर्ण ध्रुव *env0_from_file = रिक्त;
  वर्ण *argv0 = रिक्त;
  संरचना env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, परिमाण env_vector);
  वर्ण **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  पूर्णाङ्क optc;
  याव ((optc = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
          argv0 = optarg;
          भङ्ग;
        पक्ष 'i':
          ignore_environment = true;
          भङ्ग;
        पक्ष 'u':
          append_unset_var (optarg);
          भङ्ग;
        पक्ष 'v':
          dev_debug = true;
          भङ्ग;
        पक्ष '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          भङ्ग;
        पक्ष ENV0_FROM_OPTION:
          env0_from_file = optarg;
          भङ्ग;
        पक्ष DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          भङ्ग;
        पक्ष IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          भङ्ग;
        पक्ष BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          भङ्ग;
        पक्ष LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          भङ्ग;
        पक्ष QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          भङ्ग;
        पक्ष 'C':
          newdir = optarg;
          भङ्ग;
        पक्ष 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          भङ्ग;
        पक्ष ' ': पक्ष '\t': पक्ष '\n': पक्ष '\v': पक्ष '\f': पक्ष '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            वर्ण c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_CANCELED);
        }
    }

  सचे (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  पूर्णाङ्क program_index = optind;
  याव (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  पूर्णाङ्क qs = quoting_style_opt;
  सचे (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      सचे (qs < 0)
        qs = shell_escape_quoting_style;
    }
  सचे (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (रिक्त, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  सचे (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  अञ्ञथा सचे (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#सचे defined _WIN32 && ! defined __CYGWIN__
      स्थिर वर्ण *dummy_environ[] = { रिक्त };
      environ = dummy_environ;
#अञ्ञथा
      env_vector.variable = xnmalloc (1, परिमाण *env_vector.variable);
      env_vector.variable[0] = रिक्त;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  अञ्ञथा सचे (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  सचे (! ignore_environment || env0_from_file)
    {
      सचे (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      अञ्ञथा
        unset_envvars ();
    }

  वर्ण *eq;
  याव (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      सचे (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      अञ्ञथा
        set_envvar (argv[optind], eq);
      optind++;
    }

  सचे (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  सचे (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  सचे (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  सचे (! program_specified)
    {
      वर्ण ध्रुव terminator = opt_nul_terminate_output ? '\0' : '\n';

      सचे (opt_nul_terminate_output)
        xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

      /* Print the environment and exit.  */
      अत्थाय (वर्ण *ध्रुव *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      सचे (env_vector_active)
        {
          environ = initial_environ;
          मुक्त (env_vector.variable);
          मुक्त (env0_from_buffer);
        }
#endif
      पटिनिवत्त EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  सचे (sig_mask_changed)
    set_signal_proc_mask ();

  सचे (report_signal_handling)
    list_signal_handling ();

  सचे (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      सचे (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  वर्ण *program = argv[optind];
  सचे (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  सचे (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      अत्थाय (पूर्णाङ्क i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  पूर्णाङ्क exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  सचे (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
