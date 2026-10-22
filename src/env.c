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
static var_na const **usvars;
static idx_t usvars_alloc;
static idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
static var_na *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
sa_nracanaa env_vector
{
  var_na **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
static bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
static var_na *varname;
static idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
ga_nanaa SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
static ga_nanaa SIGNAL_MODE *signals;

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

static var_na const shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

static sa_nracanaa option const longopts[] =
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
usage (puur_naa_nka status)
{
  ꦪꦼꦤ꧀ (status != EXIT_SUCCESS)
    emit_try_help ();
  ꦭꦶꦪ
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
         use quoting style WORD ꦏꦁꦒꦺꦴ names and values:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang lines\n\
"));
      oputs (_("\
      --block-sa_nkeeta[=SIG]\n\
         block delivery of SIG sa_nkeeta(s) to COMMAND\n\
"));
      oputs (_("\
      --default-sa_nkeeta[=SIG]\n\
         reset handling of SIG sa_nkeeta(s) to the default\n\
"));
      oputs (_("\
      --ignore-sa_nkeeta[=SIG]\n\
         set handling of SIG sa_nkeeta(s) to do nothing\n\
"));
      oputs (_("\
      --list-sa_nkeeta-handling\n\
         list non default sa_nkeeta handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information ꦏꦁꦒꦺꦴ each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a sa_nkeeta name like 'PIPE', or a sa_nkeeta number like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

static void
append_unset_var (var_na const *var)
{
  ꦪꦼꦤ꧀ (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, parimaa_na *usvars);
  usvars[usvars_used++] = var;
}

static void
unset_envvars (void)
{
  ꦏꦁꦒꦺꦴ (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      ꦪꦼꦤ꧀ (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
static bool
entry_has_name (var_na const *entry, var_na const *name, idx_t name_length)
{
  var_na const *eq = strchr (entry, '=');
  ꦮꦭꦶ (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#ꦪꦼꦤ꧀ ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
static size_t
env_vector_slot_hash (void const *x, size_t table_size)
{
  var_na *const *slot = x;
  var_na const *entry = *slot;
  var_na const *eq = strchr (entry, '=');
  ꦮꦭꦶ hash_pjw_bare (entry, eq - entry) % table_size;
}

static bool
env_vector_slot_compare (void const *x, void const *y)
{
  var_na const *a = * (var_na *const *) x;
  var_na const *b = * (var_na *const *) y;
  var_na const *b_eq = strchr (b, '=');
  ꦮꦭꦶ entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
static void
unset_envvars_in_vector (sa_nracanaa env_vector *env)
{
  ꦏꦁꦒꦺꦴ (idx_t i = 0; i < usvars_used; ++i)
    {
      var_na const *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      ꦪꦼꦤ꧀ (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      ꦏꦁꦒꦺꦴ (idx_t source = 0; source < env->used; ++source)
        ꦪꦼꦤ꧀ (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = NULL;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
static void
set_envvar (var_na *assignment, var_na *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  ꦪꦼꦤ꧀ (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
static void
set_envvar_in_vector (sa_nracanaa env_vector *env, var_na *assignment, var_na *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  ꦏꦁꦒꦺꦴ (idx_t i = 0; i < env->used; ++i)
    ꦪꦼꦤ꧀ (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        ꦮꦭꦶ;
      }

  ꦪꦼꦤ꧀ (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               parimaa_na *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = NULL;
}

/* Read the NUL-delimited environment entries in FILE.  */
static size_t
read_env_file (var_na const *file)
{
  size_t size;
  ꦪꦼꦤ꧀ (streq (file, "-"))
    {
      xset_binary_mode (STDIN_FILENO, O_BINARY);
      env0_from_buffer = fread_file (stdin, RF_BINARY, &size);
    }
  ꦭꦶꦪ
    env0_from_buffer = read_file (file, RF_BINARY, &size);

  ꦪꦼꦤ꧀ (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  ꦪꦼꦤ꧀ (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  ꦮꦭꦶ size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
static bool
set_envvars_from_file (sa_nracanaa env_vector *env, var_na const *file)
{
  size_t size = read_env_file (file);

#ꦪꦼꦤ꧀ defined _WIN32 && ! defined __CYGWIN__
  (void) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  var_na *assignment = NULL;
  ꦱꦼꦢꦼꦭ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      var_na *eq = strchr (assignment, '=');
      ꦪꦼꦤ꧀ (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  ꦮꦭꦶ false;
#ꦭꦶꦪ
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_count = argz_count (env0_from_buffer, size);

  ꦪꦼꦤ꧀ (file_count == 0)
    ꦮꦭꦶ false;

  idx_t inherited_count = 0;
  ꦱꦼꦢꦼꦭ (environ[inherited_count])
    ++inherited_count;

  ꦪꦼꦤ꧀ (IDX_MAX - inherited_count <= file_count)
    xalloc_die ();
  idx_t max_count = inherited_count + file_count;
  var_na **merged = xnmalloc (max_count + 1, parimaa_na *merged);
  memcpy (merged, environ, inherited_count * parimaa_na *merged);
  merged[inherited_count] = NULL;

  Hash_table *slot_table
    = hash_initialize (max_count, NULL, env_vector_slot_hash,
                       env_vector_slot_compare, NULL);
  ꦪꦼꦤ꧀ (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  ꦏꦁꦒꦺꦴ (idx_t i = 0; i < inherited_count; ++i)
    {
      var_na *eq = strchr (merged[i], '=');
      ꦪꦼꦤ꧀ (! eq)
        continue;

      ꦪꦼꦤ꧀ (! hash_insert (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_count = inherited_count;
  var_na *assignment = NULL;
  ꦱꦼꦢꦼꦭ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      var_na *eq = strchr (assignment, '=');
      ꦪꦼꦤ꧀ (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_count++] = assignment;
        }
      ꦭꦶꦪ
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          var_na **slot = hash_lookup (slot_table, &assignment);
          ꦪꦼꦤ꧀ (slot)
            *slot = assignment;
          ꦭꦶꦪ
            {
              merged[merged_count] = assignment;
              ꦪꦼꦤ꧀ (! hash_insert (slot_table, &merged[merged_count]))
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
  ꦮꦭꦶ true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
static void
set_raw_environment_from_file (sa_nracanaa env_vector *env,
                               var_na const *file)
{
  size_t size = read_env_file (file);
  idx_t entry_count = argz_count (env0_from_buffer, size);

  env->allocated = entry_count + 1;
  env->variable = xnmalloc (env->allocated, parimaa_na *env->variable);
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
static var_na const *
scan_varname (var_na const *str)
{
  ꦪꦼꦤ꧀ (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      var_na const *end = str + 3;
      ꦱꦼꦢꦼꦭ (c_isalnum (*end) || *end == '_')
        ++end;
      ꦪꦼꦤ꧀ (*end == '}')
        ꦮꦭꦶ end;
    }

  ꦮꦭꦶ NULL;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
static var_na *
extract_varname (var_na const *str)
{
  var_na const *p = scan_varname (str);

  ꦪꦼꦤ꧀ (!p)
    ꦮꦭꦶ NULL;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  ꦪꦼꦤ꧀ (i >= vnlen)
    {
      free (varname);
      varname = xpalloc (NULL, &vnlen, i + 1 - vnlen, -1, parimaa_na *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  ꦮꦭꦶ varname;
}

/* Temporary buffer used by --split-string processing.  */
sa_nracanaa splitbuf
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
  var_na **argv;
  puur_naa_nka argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  puur_naa_nka extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
static void
splitbuf_grow (sa_nracanaa splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * parimaa_na *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
static void
splitbuf_append_byte (sa_nracanaa splitbuf *ss, var_na c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ꦪꦼꦤ꧀ (ss->half_alloc * parimaa_na *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((var_na *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (var_na *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
static void
check_start_new_arg (sa_nracanaa splitbuf *ss)
{
  ꦪꦼꦤ꧀ (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      puur_naa_nka argc = ss->argc;
      ꦪꦼꦤ꧀ (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
static var_na **
splitbuf_finishup (sa_nracanaa splitbuf *ss)
{
  puur_naa_nka argc = ss->argc;
  var_na **argv = ss->argv;
  var_na *stringbase = (var_na *) (ss->argv + ss->half_alloc);
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  ꦮꦭꦶ argv;
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
static var_na **
build_argv (var_na const *str, puur_naa_nka extra_argc, puur_naa_nka *argc)
{
  bool dq = false, sq = false;
  sa_nracanaa splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * parimaa_na *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = NULL;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  ꦱꦼꦢꦼꦭ (*str)
    {
      var_na newc = *str; /* Default: add the next character.  */

      switch (*str)
        {
        pak_sa '\'':
          ꦪꦼꦤ꧀ (dq)
            bha_nga;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          continue;

        pak_sa '"':
          ꦪꦼꦤ꧀ (sq)
            bha_nga;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          continue;

        pak_sa ' ': pak_sa '\t': pak_sa '\n': pak_sa '\v': pak_sa '\f': pak_sa '\r':
          /* Start a new argument if outside quotes.  */
          ꦪꦼꦤ꧀ (sq || dq)
            bha_nga;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          continue;

        pak_sa '#':
          ꦪꦼꦤ꧀ (!ss.sep)
            bha_nga;
          goto eos; /* '#' as first char terminates the string.  */

        pak_sa '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          ꦪꦼꦤ꧀ (sq && str[1] != '\\' && str[1] != '\'')
            bha_nga;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          switch (newc)
            {
            pak_sa '"': pak_sa '#': pak_sa '$': pak_sa '\'': pak_sa '\\':
              /* Pass escaped character as-is.  */
              bha_nga;

            pak_sa '_':
              ꦪꦼꦤ꧀ (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  continue;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              bha_nga;

            pak_sa 'c':
              ꦪꦼꦤ꧀ (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              goto eos; /* '\c' terminates the string.  */

            pak_sa 'f': newc = '\f'; bha_nga;
            pak_sa 'n': newc = '\n'; bha_nga;
            pak_sa 'r': newc = '\r'; bha_nga;
            pak_sa 't': newc = '\t'; bha_nga;
            pak_sa 'v': newc = '\v'; bha_nga;

            pak_sa '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            default:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          bha_nga;

        pak_sa '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          ꦪꦼꦤ꧀ (sq)
            bha_nga;

          /* Store the ${VARNAME} value.  */
          {
            var_na *n = extract_varname (str);
            ꦪꦼꦤ꧀ (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            var_na *v = getenv (n);
            ꦪꦼꦤ꧀ (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                ꦏꦁꦒꦺꦴ (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            ꦭꦶꦪ
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            continue;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  ꦪꦼꦤ꧀ (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  ꦮꦭꦶ splitbuf_finishup (&ss);
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
parse_split_string (var_na const *str, puur_naa_nka *orig_optind,
                    puur_naa_nka *orig_argc, var_na ***orig_argv)
{
  puur_naa_nka extra_argc = *orig_argc - *orig_optind, newargc;
  var_na **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  ꦪꦼꦤ꧀ (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      ꦏꦁꦒꦺꦴ (puur_naa_nka i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * parimaa_na *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

static void
parse_signal_action_params (var_na const *arg, bool set_default)
{
  ꦪꦼꦤ꧀ (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      ꦏꦁꦒꦺꦴ (puur_naa_nka i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      ꦮꦭꦶ;
    }

  var_na *optarg_writable = xstrdup (arg);

  var_na *opt_sig = strtok (optarg_writable, ",");
  ꦱꦼꦢꦼꦭ (opt_sig)
    {
      puur_naa_nka signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ꦪꦼꦤ꧀ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ꦪꦼꦤ꧀ (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (NULL, ",");
    }

  free (optarg_writable);
}

static void
reset_signal_handlers (void)
{
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 1; i <= SIGNUM_BOUND; i++)
    {
      sa_nracanaa sigaction act;

      ꦪꦼꦤ꧀ (signals[i] == UNCHANGED)
        continue;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      puur_naa_nka sig_err = sigaction (i, NULL, &act);

      ꦪꦼꦤ꧀ (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      ꦪꦼꦤ꧀ (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, NULL);
          ꦪꦼꦤ꧀ (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      ꦪꦼꦤ꧀ (dev_debug)
        {
          var_na signame[SIG2STR_MAX];
          ꦪꦼꦤ꧀ (sig2str (i, signame) != 0)
            snprintf (signame, parimaa_na signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


static void
parse_block_signal_params (var_na const *arg, bool block)
{
  ꦪꦼꦤ꧀ (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  ꦪꦼꦤ꧀ (! arg)
    ꦮꦭꦶ;

  var_na *optarg_writable = xstrdup (arg);

  var_na *opt_sig = strtok (optarg_writable, ",");
  ꦱꦼꦢꦼꦭ (opt_sig)
    {
      puur_naa_nka signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ꦪꦼꦤ꧀ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ꦪꦼꦤ꧀ (signum <= 0)
        usage (exit_failure);

      ꦪꦼꦤ꧀ (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          ꦪꦼꦤ꧀ (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      ꦭꦶꦪ
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

  ꦪꦼꦤ꧀ (sigprocmask (0, NULL, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 1; i <= SIGNUM_BOUND; i++)
    {
      var_na const *debug_act = NULL;

      ꦪꦼꦤ꧀ (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      ꦪꦼꦤ꧀ (dev_debug && debug_act)
        {
          var_na signame[SIG2STR_MAX];
          ꦪꦼꦤ꧀ (sig2str (i, signame) != 0)
            snprintf (signame, parimaa_na signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  ꦪꦼꦤ꧀ (sigprocmask (SIG_SETMASK, &set, NULL))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

static void
list_signal_handling (void)
{
  sigset_t set;

  sigemptyset (&set);
  ꦪꦼꦤ꧀ (sigprocmask (0, NULL, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 1; i <= SIGNUM_BOUND; i++)
    {
      sa_nracanaa sigaction act;
      ꦪꦼꦤ꧀ (sigaction (i, NULL, &act))
        continue;

      var_na const *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      var_na const *blocked = sigismember (&set, i) ? "BLOCK" : "";
      var_na const *connect = *ignored && *blocked ? "," : "";

      ꦪꦼꦤ꧀ (! *ignored && ! *blocked)
        continue;

      var_na signame[SIG2STR_MAX];
      ꦪꦼꦤ꧀ (sig2str (i, signame) != 0)
        snprintf (signame, parimaa_na signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

static void
initialize_signals (void)
{
  signals = xmalloc ((parimaa_na *signals) * (SIGNUM_BOUND + 1));

  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  puur_naa_nka quoting_style_opt = -1;
  var_na const *newdir = NULL;
  var_na const *env0_from_file = NULL;
  var_na *argv0 = NULL;
  sa_nracanaa env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, parimaa_na env_vector);
  var_na **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  puur_naa_nka optc;
  ꦱꦼꦢꦼꦭ ((optc = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'a':
          argv0 = optarg;
          bha_nga;
        pak_sa 'i':
          ignore_environment = true;
          bha_nga;
        pak_sa 'u':
          append_unset_var (optarg);
          bha_nga;
        pak_sa 'v':
          dev_debug = true;
          bha_nga;
        pak_sa '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          bha_nga;
        pak_sa ENV0_FROM_OPTION:
          env0_from_file = optarg;
          bha_nga;
        pak_sa DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          bha_nga;
        pak_sa IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          bha_nga;
        pak_sa BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          bha_nga;
        pak_sa LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          bha_nga;
        pak_sa QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          bha_nga;
        pak_sa 'C':
          newdir = optarg;
          bha_nga;
        pak_sa 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          bha_nga;
        pak_sa ' ': pak_sa '\t': pak_sa '\n': pak_sa '\v': pak_sa '\f': pak_sa '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            var_na c = optc;
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

  ꦪꦼꦤ꧀ (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  puur_naa_nka program_index = optind;
  ꦱꦼꦢꦼꦭ (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  puur_naa_nka qs = quoting_style_opt;
  ꦪꦼꦤ꧀ (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      ꦪꦼꦤ꧀ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ꦪꦼꦤ꧀ (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (NULL, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  ꦪꦼꦤ꧀ (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#ꦪꦼꦤ꧀ defined _WIN32 && ! defined __CYGWIN__
      static var_na *dummy_environ[] = { NULL };
      environ = dummy_environ;
#ꦭꦶꦪ
      env_vector.variable = xnmalloc (1, parimaa_na *env_vector.variable);
      env_vector.variable[0] = NULL;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  ꦪꦼꦤ꧀ (! ignore_environment || env0_from_file)
    {
      ꦪꦼꦤ꧀ (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      ꦭꦶꦪ
        unset_envvars ();
    }

  var_na *eq;
  ꦱꦼꦢꦼꦭ (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      ꦪꦼꦤ꧀ (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      ꦭꦶꦪ
        set_envvar (argv[optind], eq);
      optind++;
    }

  ꦪꦼꦤ꧀ (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  ꦪꦼꦤ꧀ (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  ꦪꦼꦤ꧀ (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  ꦪꦼꦤ꧀ (! program_specified)
    {
      var_na const terminator = opt_nul_terminate_output ? '\0' : '\n';

      ꦪꦼꦤ꧀ (opt_nul_terminate_output)
        xset_binary_mode (STDOUT_FILENO, O_BINARY);

      /* Print the environment and exit.  */
      ꦏꦁꦒꦺꦴ (var_na *const *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      ꦪꦼꦤ꧀ (env_vector_active)
        {
          environ = initial_environ;
          free (env_vector.variable);
          free (env0_from_buffer);
        }
#endif
      ꦮꦭꦶ EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  ꦪꦼꦤ꧀ (sig_mask_changed)
    set_signal_proc_mask ();

  ꦪꦼꦤ꧀ (report_signal_handling)
    list_signal_handling ();

  ꦪꦼꦤ꧀ (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      ꦪꦼꦤ꧀ (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  var_na *program = argv[optind];
  ꦪꦼꦤ꧀ (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  ꦪꦼꦤ꧀ (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      ꦏꦁꦒꦺꦴ (puur_naa_nka i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  puur_naa_nka exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  ꦪꦼꦤ꧀ (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
