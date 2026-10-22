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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <argz.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <c-ctype.h>
#અન્તર્ભાવ <signal.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "hashcode-mem.h"
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "operand2sig.h"
#અન્તર્ભાવ "printenv.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "read-file.h"
#અન્તર્ભાવ "sig2str.h"
#અન્તર્ભાવ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
સ્થિર અક્ષર અચલ **usvars;
સ્થિર idx_t usvars_alloc;
સ્થિર idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
સ્થિર અક્ષર *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
સંરચના env_vector
{
  અક્ષર **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
સ્થિર bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
સ્થિર અક્ષર *varname;
સ્થિર idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
ગણના SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
સ્થિર ગણના SIGNAL_MODE *signals;

/* Set of signals to block.  */
સ્થિર sigset_t block_signals;

/* Set of signals to unblock.  */
સ્થિર sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
સ્થિર bool sig_mask_changed;

/* Whether to list non default handling.  */
સ્થિર bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

સ્થિર અક્ષર અચલ shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"argv0", required_argument, ખાલી, 'a'},
  {"ignore-environment", no_argument, ખાલી, 'i'},
  {"env0-from", required_argument, ખાલી, ENV0_FROM_OPTION},
  {"null", no_argument, ખાલી, '0'},
  {"unset", required_argument, ખાલી, 'u'},
  {"chdir", required_argument, ખાલી, 'C'},
  {"default-signal", optional_argument, ખાલી, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, ખાલી, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, ખાલી, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, ખાલી,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, ખાલી, 'v'},
  {"quoting-style", required_argument, ખાલી, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, ખાલી, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=કિંમત_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to કિંમત_2 in the environment and run COMMAND.\n\
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
         પઠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output લીટી with NUL, not newline\n\
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
         use quoting style WORD માટે names and કિંમતો:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang લીટીઓ\n\
"));
      oputs (_("\
      --block-સંકેત[=SIG]\n\
         block delivery of SIG સંકેત(s) to COMMAND\n\
"));
      oputs (_("\
      --મૂળભૂત-સંકેત[=SIG]\n\
         reset handling of SIG સંકેત(s) to the મૂળભૂત\n\
"));
      oputs (_("\
      --ignore-સંકેત[=SIG]\n\
         set handling of SIG સંકેત(s) to કરો nothing\n\
"));
      oputs (_("\
      --list-સંકેત-handling\n\
         list non મૂળભૂત સંકેત handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information માટે each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a સંકેત name like 'PIPE', or a સંકેત સંખ્યા like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

સ્થિર શૂન્ય
append_unset_var (અક્ષર અચલ *var)
{
  જો (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, કદ *usvars);
  usvars[usvars_used++] = var;
}

સ્થિર શૂન્ય
unset_envvars (શૂન્ય)
{
  માટે (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      જો (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
સ્થિર bool
entry_has_name (અક્ષર અચલ *entry, અક્ષર અચલ *name, idx_t name_length)
{
  અક્ષર અચલ *eq = strchr (entry, '=');
  પાછા (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#જો ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
સ્થિર size_t
env_vector_slot_hash (શૂન્ય અચલ *x, size_t table_size)
{
  અક્ષર *અચલ *slot = x;
  અક્ષર અચલ *entry = *slot;
  અક્ષર અચલ *eq = strchr (entry, '=');
  પાછા hash_pjw_bare (entry, eq - entry) % table_size;
}

સ્થિર bool
env_vector_slot_compare (શૂન્ય અચલ *x, શૂન્ય અચલ *y)
{
  અક્ષર અચલ *a = * (અક્ષર *અચલ *) x;
  અક્ષર અચલ *b = * (અક્ષર *અચલ *) y;
  અક્ષર અચલ *b_eq = strchr (b, '=');
  પાછા entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
સ્થિર શૂન્ય
unset_envvars_in_vector (સંરચના env_vector *env)
{
  માટે (idx_t i = 0; i < usvars_used; ++i)
    {
      અક્ષર અચલ *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      જો (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      માટે (idx_t source = 0; source < env->used; ++source)
        જો (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = ખાલી;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
સ્થિર શૂન્ય
set_envvar (અક્ષર *assignment, અક્ષર *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  જો (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
સ્થિર શૂન્ય
set_envvar_in_vector (સંરચના env_vector *env, અક્ષર *assignment, અક્ષર *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  માટે (idx_t i = 0; i < env->used; ++i)
    જો (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        પાછા;
      }

  જો (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               કદ *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = ખાલી;
}

/* Read the NUL-delimited environment entries in FILE.  */
સ્થિર size_t
read_env_file (અક્ષર અચલ *file)
{
  size_t size;
  જો (streq (file, "-"))
    {
      xset_દ્વિઅંકી_mode (STDIN_FILENO, O_દ્વિઅંકી);
      env0_from_buffer = fread_file (stdin, RF_દ્વિઅંકી, &size);
    }
  નહીંતર
    env0_from_buffer = read_file (file, RF_દ્વિઅંકી, &size);

  જો (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  જો (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  પાછા size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
સ્થિર bool
set_envvars_from_file (સંરચના env_vector *env, અક્ષર અચલ *file)
{
  size_t size = read_env_file (file);

#જો defined _WIN32 && ! defined __CYGWIN__
  (શૂન્ય) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  અક્ષર *assignment = ખાલી;
  જ્યારે ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      અક્ષર *eq = strchr (assignment, '=');
      જો (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  પાછા false;
#નહીંતર
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_ગણતરી = argz_ગણતરી (env0_from_buffer, size);

  જો (file_ગણતરી == 0)
    પાછા false;

  idx_t inherited_ગણતરી = 0;
  જ્યારે (environ[inherited_ગણતરી])
    ++inherited_ગણતરી;

  જો (IDX_MAX - inherited_ગણતરી <= file_ગણતરી)
    xalloc_die ();
  idx_t max_ગણતરી = inherited_ગણતરી + file_ગણતરી;
  અક્ષર **merged = xnmalloc (max_ગણતરી + 1, કદ *merged);
  memcpy (merged, environ, inherited_ગણતરી * કદ *merged);
  merged[inherited_ગણતરી] = ખાલી;

  Hash_table *slot_table
    = hash_initialize (max_ગણતરી, ખાલી, env_vector_slot_hash,
                       env_vector_slot_compare, ખાલી);
  જો (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  માટે (idx_t i = 0; i < inherited_ગણતરી; ++i)
    {
      અક્ષર *eq = strchr (merged[i], '=');
      જો (! eq)
        ચાલુ;

      જો (! hash_ઉમેરો (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_ગણતરી = inherited_ગણતરી;
  અક્ષર *assignment = ખાલી;
  જ્યારે ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      અક્ષર *eq = strchr (assignment, '=');
      જો (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_ગણતરી++] = assignment;
        }
      નહીંતર
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          અક્ષર **slot = hash_lookup (slot_table, &assignment);
          જો (slot)
            *slot = assignment;
          નહીંતર
            {
              merged[merged_ગણતરી] = assignment;
              જો (! hash_ઉમેરો (slot_table, &merged[merged_ગણતરી]))
                xalloc_die ();
              ++merged_ગણતરી;
            }
        }
    }

  merged[merged_ગણતરી] = ખાલી;
  env->variable = merged;
  env->used = merged_ગણતરી;
  env->allocated = max_ગણતરી + 1;
  environ = merged;
  hash_free (slot_table);
  પાછા true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
સ્થિર શૂન્ય
set_raw_environment_from_file (સંરચના env_vector *env,
                               અક્ષર અચલ *file)
{
  size_t size = read_env_file (file);
  idx_t entry_ગણતરી = argz_ગણતરી (env0_from_buffer, size);

  env->allocated = entry_ગણતરી + 1;
  env->variable = xnmalloc (env->allocated, કદ *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_ગણતરી;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
સ્થિર અક્ષર અચલ *
scan_varname (અક્ષર અચલ *str)
{
  જો (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      અક્ષર અચલ *end = str + 3;
      જ્યારે (c_isalnum (*end) || *end == '_')
        ++end;
      જો (*end == '}')
        પાછા end;
    }

  પાછા ખાલી;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
સ્થિર અક્ષર *
extract_varname (અક્ષર અચલ *str)
{
  અક્ષર અચલ *p = scan_varname (str);

  જો (!p)
    પાછા ખાલી;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  જો (i >= vnlen)
    {
      મુક્ત (varname);
      varname = xpalloc (ખાલી, &vnlen, i + 1 - vnlen, -1, કદ *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  પાછા varname;
}

/* Temporary buffer used by --split-string processing.  */
સંરચના splitbuf
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
  અક્ષર **argv;
  પૂર્ણાંક argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  પૂર્ણાંક extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
સ્થિર શૂન્ય
splitbuf_grow (સંરચના splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * કદ *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
સ્થિર શૂન્ય
splitbuf_append_byte (સંરચના splitbuf *ss, અક્ષર c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  જો (ss->half_alloc * કદ *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((અક્ષર *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (અક્ષર *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
સ્થિર શૂન્ય
check_start_new_arg (સંરચના splitbuf *ss)
{
  જો (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      પૂર્ણાંક argc = ss->argc;
      જો (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
સ્થિર અક્ષર **
splitbuf_finishup (સંરચના splitbuf *ss)
{
  પૂર્ણાંક argc = ss->argc;
  અક્ષર **argv = ss->argv;
  અક્ષર *stringbase = (અક્ષર *) (ss->argv + ss->half_alloc);
  માટે (પૂર્ણાંક i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  પાછા argv;
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
સ્થિર અક્ષર **
build_argv (અક્ષર અચલ *str, પૂર્ણાંક extra_argc, પૂર્ણાંક *argc)
{
  bool dq = false, sq = false;
  સંરચના splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * કદ *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ખાલી;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  જ્યારે (*str)
    {
      અક્ષર newc = *str; /* Default: add the next character.  */

      પસંદગી (*str)
        {
        સ્થિતિ '\'':
          જો (dq)
            તોડો;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          ચાલુ;

        સ્થિતિ '"':
          જો (sq)
            તોડો;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          ચાલુ;

        સ્થિતિ ' ': સ્થિતિ '\t': સ્થિતિ '\n': સ્થિતિ '\v': સ્થિતિ '\f': સ્થિતિ '\r':
          /* Start a new argument if outside quotes.  */
          જો (sq || dq)
            તોડો;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          ચાલુ;

        સ્થિતિ '#':
          જો (!ss.sep)
            તોડો;
          જાઓ eos; /* '#' as first char terminates the string.  */

        સ્થિતિ '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          જો (sq && str[1] != '\\' && str[1] != '\'')
            તોડો;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          પસંદગી (newc)
            {
            સ્થિતિ '"': સ્થિતિ '#': સ્થિતિ '$': સ્થિતિ '\'': સ્થિતિ '\\':
              /* Pass escaped character as-is.  */
              તોડો;

            સ્થિતિ '_':
              જો (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  ચાલુ;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              તોડો;

            સ્થિતિ 'c':
              જો (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              જાઓ eos; /* '\c' terminates the string.  */

            સ્થિતિ 'f': newc = '\f'; તોડો;
            સ્થિતિ 'n': newc = '\n'; તોડો;
            સ્થિતિ 'r': newc = '\r'; તોડો;
            સ્થિતિ 't': newc = '\t'; તોડો;
            સ્થિતિ 'v': newc = '\v'; તોડો;

            સ્થિતિ '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            મૂળભૂત:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          તોડો;

        સ્થિતિ '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          જો (sq)
            તોડો;

          /* Store the ${VARNAME} value.  */
          {
            અક્ષર *n = extract_varname (str);
            જો (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            અક્ષર *v = getenv (n);
            જો (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                માટે (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            નહીંતર
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            ચાલુ;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  જો (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  પાછા splitbuf_finishup (&ss);
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
સ્થિર શૂન્ય
parse_split_string (અક્ષર અચલ *str, પૂર્ણાંક *orig_optind,
                    પૂર્ણાંક *orig_argc, અક્ષર ***orig_argv)
{
  પૂર્ણાંક extra_argc = *orig_argc - *orig_optind, newargc;
  અક્ષર **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  જો (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      માટે (પૂર્ણાંક i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * કદ *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

સ્થિર શૂન્ય
parse_signal_action_params (અક્ષર અચલ *arg, bool set_default)
{
  જો (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      માટે (પૂર્ણાંક i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      પાછા;
    }

  અક્ષર *optarg_writable = xstrdup (arg);

  અક્ષર *opt_sig = strtok (optarg_writable, ",");
  જ્યારે (opt_sig)
    {
      પૂર્ણાંક signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      જો (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      જો (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ખાલી, ",");
    }

  મુક્ત (optarg_writable);
}

સ્થિર શૂન્ય
reset_signal_handlers (શૂન્ય)
{
  માટે (પૂર્ણાંક i = 1; i <= SIGNUM_BOUND; i++)
    {
      સંરચના sigaction act;

      જો (signals[i] == UNCHANGED)
        ચાલુ;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      પૂર્ણાંક sig_err = sigaction (i, ખાલી, &act);

      જો (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      જો (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ખાલી);
          જો (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      જો (dev_debug)
        {
          અક્ષર signame[SIG2STR_MAX];
          જો (sig2str (i, signame) != 0)
            snprintf (signame, કદ signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


સ્થિર શૂન્ય
parse_block_signal_params (અક્ષર અચલ *arg, bool block)
{
  જો (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  નહીંતર જો (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  જો (! arg)
    પાછા;

  અક્ષર *optarg_writable = xstrdup (arg);

  અક્ષર *opt_sig = strtok (optarg_writable, ",");
  જ્યારે (opt_sig)
    {
      પૂર્ણાંક signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      જો (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      જો (signum <= 0)
        usage (exit_failure);

      જો (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          જો (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      નહીંતર
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (ખાલી, ",");
    }

  મુક્ત (optarg_writable);
}

સ્થિર શૂન્ય
set_signal_proc_mask (શૂન્ય)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  જો (sigprocmask (0, ખાલી, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  માટે (પૂર્ણાંક i = 1; i <= SIGNUM_BOUND; i++)
    {
      અક્ષર અચલ *debug_act = ખાલી;

      જો (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      નહીંતર જો (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      જો (dev_debug && debug_act)
        {
          અક્ષર signame[SIG2STR_MAX];
          જો (sig2str (i, signame) != 0)
            snprintf (signame, કદ signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  જો (sigprocmask (SIG_SETMASK, &set, ખાલી))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

સ્થિર શૂન્ય
list_signal_handling (શૂન્ય)
{
  sigset_t set;

  sigemptyset (&set);
  જો (sigprocmask (0, ખાલી, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  માટે (પૂર્ણાંક i = 1; i <= SIGNUM_BOUND; i++)
    {
      સંરચના sigaction act;
      જો (sigaction (i, ખાલી, &act))
        ચાલુ;

      અક્ષર અચલ *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      અક્ષર અચલ *blocked = sigismember (&set, i) ? "BLOCK" : "";
      અક્ષર અચલ *connect = *ignored && *blocked ? "," : "";

      જો (! *ignored && ! *blocked)
        ચાલુ;

      અક્ષર signame[SIG2STR_MAX];
      જો (sig2str (i, signame) != 0)
        snprintf (signame, કદ signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

સ્થિર શૂન્ય
initialize_signals (શૂન્ય)
{
  signals = xmalloc ((કદ *signals) * (SIGNUM_BOUND + 1));

  માટે (પૂર્ણાંક i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  પૂર્ણાંક quoting_style_opt = -1;
  અક્ષર અચલ *newdir = ખાલી;
  અક્ષર અચલ *env0_from_file = ખાલી;
  અક્ષર *argv0 = ખાલી;
  સંરચના env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, કદ env_vector);
  અક્ષર **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, shortopts, longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'a':
          argv0 = optarg;
          તોડો;
        સ્થિતિ 'i':
          ignore_environment = true;
          તોડો;
        સ્થિતિ 'u':
          append_unset_var (optarg);
          તોડો;
        સ્થિતિ 'v':
          dev_debug = true;
          તોડો;
        સ્થિતિ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          તોડો;
        સ્થિતિ ENV0_FROM_OPTION:
          env0_from_file = optarg;
          તોડો;
        સ્થિતિ DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          તોડો;
        સ્થિતિ IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          તોડો;
        સ્થિતિ BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          તોડો;
        સ્થિતિ LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          તોડો;
        સ્થિતિ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          તોડો;
        સ્થિતિ 'C':
          newdir = optarg;
          તોડો;
        સ્થિતિ 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          તોડો;
        સ્થિતિ ' ': સ્થિતિ '\t': સ્થિતિ '\n': સ્થિતિ '\v': સ્થિતિ '\f': સ્થિતિ '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            અક્ષર c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_CANCELED);
        }
    }

  જો (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  પૂર્ણાંક program_index = optind;
  જ્યારે (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  પૂર્ણાંક qs = quoting_style_opt;
  જો (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      જો (qs < 0)
        qs = shell_escape_quoting_style;
    }
  જો (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ખાલી, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  જો (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  નહીંતર જો (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#જો defined _WIN32 && ! defined __CYGWIN__
      સ્થિર અક્ષર *dummy_environ[] = { ખાલી };
      environ = dummy_environ;
#નહીંતર
      env_vector.variable = xnmalloc (1, કદ *env_vector.variable);
      env_vector.variable[0] = ખાલી;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  નહીંતર જો (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  જો (! ignore_environment || env0_from_file)
    {
      જો (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      નહીંતર
        unset_envvars ();
    }

  અક્ષર *eq;
  જ્યારે (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      જો (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      નહીંતર
        set_envvar (argv[optind], eq);
      optind++;
    }

  જો (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  જો (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  જો (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  જો (! program_specified)
    {
      અક્ષર અચલ terminator = opt_nul_terminate_output ? '\0' : '\n';

      જો (opt_nul_terminate_output)
        xset_દ્વિઅંકી_mode (STDOUT_FILENO, O_દ્વિઅંકી);

      /* Print the environment and exit.  */
      માટે (અક્ષર *અચલ *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      જો (env_vector_active)
        {
          environ = initial_environ;
          મુક્ત (env_vector.variable);
          મુક્ત (env0_from_buffer);
        }
#endif
      પાછા EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  જો (sig_mask_changed)
    set_signal_proc_mask ();

  જો (report_signal_handling)
    list_signal_handling ();

  જો (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      જો (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  અક્ષર *program = argv[optind];
  જો (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  જો (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      માટે (પૂર્ણાંક i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  પૂર્ણાંક exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  જો (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
