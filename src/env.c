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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <argz.h>
#include <getopt.h>
#include <c-ctype.h>
#include <signal.h>

#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "hash.h"
#include "hashcode-mem.h"
#include "system.h"
#include "operand2sig.h"
#include "printenv.h"
#include "quote.h"
#include "read-file.h"
#include "sig2str.h"
#include "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
static char const **usvars;
static idx_t usvars_alloc;
static idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
static char *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
struct env_vector
{
  char **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
static bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
static char *varname;
static idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
enum SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
static enum SIGNAL_MODE *signals;

/* Set of signals to block.  */
static sigset_t block_signals;

/* Set of signals to unblock.  */
static sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
static bool sig_mask_changed;

/* Whether to list non default handling.  */
static bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

static char const shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

static struct option const longopts[] =
{
  {"argv0", required_argument, NULL, 'a'},
  {"ignore-environment", no_argument, NULL, 'i'},
  {"env0-from", required_argument, NULL, ENV0_FROM_OPTION},
  {"null", no_argument, NULL, '0'},
  {"unset", required_argument, NULL, 'u'},
  {"chdir", required_argument, NULL, 'C'},
  {"default-signal", optional_argument, NULL, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, NULL, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, NULL, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, NULL,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, NULL, 'v'},
  {"quoting-style", required_argument, NULL, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, NULL, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (整数 status)
{
  如果 (status != EXIT_SUCCESS)
    emit_try_help ();
  否则
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=VALUE]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to VALUE in the environment and run COMMAND.\n\
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
         read NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output line with NUL, not newline\n\
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
         use quoting style WORD 为 names and values:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang lines\n\
"));
      oputs (_("\
      --block-signal[=SIG]\n\
         block delivery of SIG signal(s) to COMMAND\n\
"));
      oputs (_("\
      --default-signal[=SIG]\n\
         reset handling of SIG signal(s) to the default\n\
"));
      oputs (_("\
      --ignore-signal[=SIG]\n\
         set handling of SIG signal(s) to do nothing\n\
"));
      oputs (_("\
      --list-signal-handling\n\
         list non default signal handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information 为 each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a signal name like 'PIPE', or a signal number like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

static void
append_unset_var (char const *var)
{
  如果 (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, sizeof *usvars);
  usvars[usvars_used++] = var;
}

static void
unset_envvars (void)
{
  为 (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      如果 (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
static bool
entry_has_name (char const *entry, char const *name, idx_t name_length)
{
  char const *eq = strchr (entry, '=');
  返回 (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#如果 ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
static size_t
env_vector_slot_hash (void const *x, size_t table_size)
{
  char *const *slot = x;
  char const *entry = *slot;
  char const *eq = strchr (entry, '=');
  返回 hash_pjw_bare (entry, eq - entry) % table_size;
}

static bool
env_vector_slot_compare (void const *x, void const *y)
{
  char const *a = * (char *const *) x;
  char const *b = * (char *const *) y;
  char const *b_eq = strchr (b, '=');
  返回 entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
static void
unset_envvars_in_vector (struct env_vector *env)
{
  为 (idx_t i = 0; i < usvars_used; ++i)
    {
      char const *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      如果 (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      为 (idx_t source = 0; source < env->used; ++source)
        如果 (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = NULL;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
static void
set_envvar (char *assignment, char *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  如果 (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
static void
set_envvar_in_vector (struct env_vector *env, char *assignment, char *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  为 (idx_t i = 0; i < env->used; ++i)
    如果 (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        返回;
      }

  如果 (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               sizeof *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = NULL;
}

/* Read the NUL-delimited environment entries in FILE.  */
static size_t
read_env_file (char const *file)
{
  size_t size;
  如果 (streq (file, "-"))
    {
      xset_binary_mode (STDIN_FILENO, O_BINARY);
      env0_from_buffer = fread_file (stdin, RF_BINARY, &size);
    }
  否则
    env0_from_buffer = read_file (file, RF_BINARY, &size);

  如果 (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  如果 (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  返回 size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
static bool
set_envvars_from_file (struct env_vector *env, char const *file)
{
  size_t size = read_env_file (file);

#如果 defined _WIN32 && ! defined __CYGWIN__
  (void) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  char *assignment = NULL;
  当 ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      char *eq = strchr (assignment, '=');
      如果 (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  返回 false;
#否则
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_count = argz_count (env0_from_buffer, size);

  如果 (file_count == 0)
    返回 false;

  idx_t inherited_count = 0;
  当 (environ[inherited_count])
    ++inherited_count;

  如果 (IDX_MAX - inherited_count <= file_count)
    xalloc_die ();
  idx_t max_count = inherited_count + file_count;
  char **merged = xnmalloc (max_count + 1, sizeof *merged);
  memcpy (merged, environ, inherited_count * sizeof *merged);
  merged[inherited_count] = NULL;

  Hash_table *slot_table
    = hash_initialize (max_count, NULL, env_vector_slot_hash,
                       env_vector_slot_compare, NULL);
  如果 (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  为 (idx_t i = 0; i < inherited_count; ++i)
    {
      char *eq = strchr (merged[i], '=');
      如果 (! eq)
        continue;

      如果 (! hash_insert (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_count = inherited_count;
  char *assignment = NULL;
  当 ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      char *eq = strchr (assignment, '=');
      如果 (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_count++] = assignment;
        }
      否则
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          char **slot = hash_lookup (slot_table, &assignment);
          如果 (slot)
            *slot = assignment;
          否则
            {
              merged[merged_count] = assignment;
              如果 (! hash_insert (slot_table, &merged[merged_count]))
                xalloc_die ();
              ++merged_count;
            }
        }
    }

  merged[merged_count] = NULL;
  env->variable = merged;
  env->used = merged_count;
  env->allocated = max_count + 1;
  environ = merged;
  hash_free (slot_table);
  返回 true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
static void
set_raw_environment_from_file (struct env_vector *env,
                               char const *file)
{
  size_t size = read_env_file (file);
  idx_t entry_count = argz_count (env0_from_buffer, size);

  env->allocated = entry_count + 1;
  env->variable = xnmalloc (env->allocated, sizeof *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_count;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
static char const *
scan_varname (char const *str)
{
  如果 (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      char const *end = str + 3;
      当 (c_isalnum (*end) || *end == '_')
        ++end;
      如果 (*end == '}')
        返回 end;
    }

  返回 NULL;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
static char *
extract_varname (char const *str)
{
  char const *p = scan_varname (str);

  如果 (!p)
    返回 NULL;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  如果 (i >= vnlen)
    {
      free (varname);
      varname = xpalloc (NULL, &vnlen, i + 1 - vnlen, -1, sizeof *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  返回 varname;
}

/* Temporary buffer used by --split-string processing.  */
struct splitbuf
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
  char **argv;
  整数 argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  整数 extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
static void
splitbuf_grow (struct splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * sizeof *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
static void
splitbuf_append_byte (struct splitbuf *ss, char c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  如果 (ss->half_alloc * sizeof *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((char *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (char *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
static void
check_start_new_arg (struct splitbuf *ss)
{
  如果 (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      整数 argc = ss->argc;
      如果 (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
static char **
splitbuf_finishup (struct splitbuf *ss)
{
  整数 argc = ss->argc;
  char **argv = ss->argv;
  char *stringbase = (char *) (ss->argv + ss->half_alloc);
  为 (整数 i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  返回 argv;
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
static char **
build_argv (char const *str, 整数 extra_argc, 整数 *argc)
{
  bool dq = false, sq = false;
  struct splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * sizeof *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = NULL;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  当 (*str)
    {
      char newc = *str; /* Default: add the next character.  */

      switch (*str)
        {
        case '\'':
          如果 (dq)
            break;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          continue;

        case '"':
          如果 (sq)
            break;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          continue;

        case ' ': case '\t': case '\n': case '\v': case '\f': case '\r':
          /* Start a new argument if outside quotes.  */
          如果 (sq || dq)
            break;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          continue;

        case '#':
          如果 (!ss.sep)
            break;
          goto eos; /* '#' as first char terminates the string.  */

        case '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          如果 (sq && str[1] != '\\' && str[1] != '\'')
            break;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          switch (newc)
            {
            case '"': case '#': case '$': case '\'': case '\\':
              /* Pass escaped character as-is.  */
              break;

            case '_':
              如果 (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  continue;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              break;

            case 'c':
              如果 (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              goto eos; /* '\c' terminates the string.  */

            case 'f': newc = '\f'; break;
            case 'n': newc = '\n'; break;
            case 'r': newc = '\r'; break;
            case 't': newc = '\t'; break;
            case 'v': newc = '\v'; break;

            case '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            default:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          break;

        case '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          如果 (sq)
            break;

          /* Store the ${VARNAME} value.  */
          {
            char *n = extract_varname (str);
            如果 (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            char *v = getenv (n);
            如果 (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                为 (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            否则
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            continue;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  如果 (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  返回 splitbuf_finishup (&ss);
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
static void
parse_split_string (char const *str, 整数 *orig_optind,
                    整数 *orig_argc, char ***orig_argv)
{
  整数 extra_argc = *orig_argc - *orig_optind, newargc;
  char **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  如果 (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      为 (整数 i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * sizeof *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

static void
parse_signal_action_params (char const *arg, bool set_default)
{
  如果 (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      为 (整数 i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      返回;
    }

  char *optarg_writable = xstrdup (arg);

  char *opt_sig = strtok (optarg_writable, ",");
  当 (opt_sig)
    {
      整数 signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      如果 (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      如果 (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (NULL, ",");
    }

  free (optarg_writable);
}

static void
reset_signal_handlers (void)
{
  为 (整数 i = 1; i <= SIGNUM_BOUND; i++)
    {
      struct sigaction act;

      如果 (signals[i] == UNCHANGED)
        continue;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      整数 sig_err = sigaction (i, NULL, &act);

      如果 (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      如果 (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, NULL);
          如果 (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      如果 (dev_debug)
        {
          char signame[SIG2STR_MAX];
          如果 (sig2str (i, signame) != 0)
            snprintf (signame, sizeof signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


static void
parse_block_signal_params (char const *arg, bool block)
{
  如果 (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  否则 如果 (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  如果 (! arg)
    返回;

  char *optarg_writable = xstrdup (arg);

  char *opt_sig = strtok (optarg_writable, ",");
  当 (opt_sig)
    {
      整数 signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      如果 (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      如果 (signum <= 0)
        usage (exit_failure);

      如果 (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          如果 (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      否则
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (NULL, ",");
    }

  free (optarg_writable);
}

static void
set_signal_proc_mask (void)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  如果 (sigprocmask (0, NULL, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  为 (整数 i = 1; i <= SIGNUM_BOUND; i++)
    {
      char const *debug_act = NULL;

      如果 (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      否则 如果 (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      如果 (dev_debug && debug_act)
        {
          char signame[SIG2STR_MAX];
          如果 (sig2str (i, signame) != 0)
            snprintf (signame, sizeof signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  如果 (sigprocmask (SIG_SETMASK, &set, NULL))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

static void
list_signal_handling (void)
{
  sigset_t set;

  sigemptyset (&set);
  如果 (sigprocmask (0, NULL, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  为 (整数 i = 1; i <= SIGNUM_BOUND; i++)
    {
      struct sigaction act;
      如果 (sigaction (i, NULL, &act))
        continue;

      char const *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      char const *blocked = sigismember (&set, i) ? "BLOCK" : "";
      char const *connect = *ignored && *blocked ? "," : "";

      如果 (! *ignored && ! *blocked)
        continue;

      char signame[SIG2STR_MAX];
      如果 (sig2str (i, signame) != 0)
        snprintf (signame, sizeof signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

static void
initialize_signals (void)
{
  signals = xmalloc ((sizeof *signals) * (SIGNUM_BOUND + 1));

  为 (整数 i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

整数
主要 (整数 argc, char **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  整数 quoting_style_opt = -1;
  char const *newdir = NULL;
  char const *env0_from_file = NULL;
  char *argv0 = NULL;
  struct env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, sizeof env_vector);
  char **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  整数 optc;
  当 ((optc = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      switch (optc)
        {
        case 'a':
          argv0 = optarg;
          break;
        case 'i':
          ignore_environment = true;
          break;
        case 'u':
          append_unset_var (optarg);
          break;
        case 'v':
          dev_debug = true;
          break;
        case '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          break;
        case ENV0_FROM_OPTION:
          env0_from_file = optarg;
          break;
        case DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          break;
        case IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          break;
        case BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          break;
        case LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          break;
        case QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          break;
        case 'C':
          newdir = optarg;
          break;
        case 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          break;
        case ' ': case '\t': case '\n': case '\v': case '\f': case '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            char c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_CANCELED);
        }
    }

  如果 (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  整数 program_index = optind;
  当 (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  整数 qs = quoting_style_opt;
  如果 (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      如果 (qs < 0)
        qs = shell_escape_quoting_style;
    }
  如果 (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (NULL, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  如果 (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  否则 如果 (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#如果 defined _WIN32 && ! defined __CYGWIN__
      static char *dummy_environ[] = { NULL };
      environ = dummy_environ;
#否则
      env_vector.variable = xnmalloc (1, sizeof *env_vector.variable);
      env_vector.variable[0] = NULL;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  否则 如果 (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  如果 (! ignore_environment || env0_from_file)
    {
      如果 (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      否则
        unset_envvars ();
    }

  char *eq;
  当 (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      如果 (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      否则
        set_envvar (argv[optind], eq);
      optind++;
    }

  如果 (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  如果 (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  如果 (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  如果 (! program_specified)
    {
      char const terminator = opt_nul_terminate_output ? '\0' : '\n';

      如果 (opt_nul_terminate_output)
        xset_binary_mode (STDOUT_FILENO, O_BINARY);

      /* Print the environment and exit.  */
      为 (char *const *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      如果 (env_vector_active)
        {
          environ = initial_environ;
          free (env_vector.variable);
          free (env0_from_buffer);
        }
#endif
      返回 EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  如果 (sig_mask_changed)
    set_signal_proc_mask ();

  如果 (report_signal_handling)
    list_signal_handling ();

  如果 (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      如果 (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  char *program = argv[optind];
  如果 (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  如果 (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      为 (整数 i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  整数 exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  如果 (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
