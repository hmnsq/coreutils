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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <argz.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <c-ctype.h>
#അന്തര്ഭാവ <signal.h>

#അന്തര്ഭാവ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "hashcode-mem.h"
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "operand2sig.h"
#അന്തര്ഭാവ "printenv.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "read-file.h"
#അന്തര്ഭാവ "sig2str.h"
#അന്തര്ഭാവ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം **usvars;
സ്ഥിതം idx_t usvars_alloc;
സ്ഥിതം idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
സ്ഥിതം അക്ഷരം *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
ഘടന env_vector
{
  അക്ഷരം **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
സ്ഥിതം bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
സ്ഥിതം അക്ഷരം *varname;
സ്ഥിതം idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
എണ്ണൽ SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
സ്ഥിതം എണ്ണൽ SIGNAL_MODE *signals;

/* Set of signals to block.  */
സ്ഥിതം sigset_t block_signals;

/* Set of signals to unblock.  */
സ്ഥിതം sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
സ്ഥിതം bool sig_mask_changed;

/* Whether to list non default handling.  */
സ്ഥിതം bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

സ്ഥിതം അക്ഷരം സ്ഥിരം shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"argv0", required_argument, ശൂന്യ, 'a'},
  {"ignore-environment", no_argument, ശൂന്യ, 'i'},
  {"env0-from", required_argument, ശൂന്യ, ENV0_FROM_OPTION},
  {"null", no_argument, ശൂന്യ, '0'},
  {"unset", required_argument, ശൂന്യ, 'u'},
  {"chdir", required_argument, ശൂന്യ, 'C'},
  {"default-signal", optional_argument, ശൂന്യ, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, ശൂന്യ, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, ശൂന്യ, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, ശൂന്യ,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, ശൂന്യ, 'v'},
  {"quoting-style", required_argument, ശൂന്യ, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, ശൂന്യ, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=മൂല്യം_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to മൂല്യം_2 in the environment and run COMMAND.\n\
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
         പഠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output വരി with NUL, not newline\n\
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
         use quoting style WORD വേണ്ടി names and മൂല്യങ്ങൾ:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang വരികൾ\n\
"));
      oputs (_("\
      --block-സൂചന[=SIG]\n\
         block delivery of SIG സൂചന(s) to COMMAND\n\
"));
      oputs (_("\
      --സ്ഥിരസ്ഥിതി-സൂചന[=SIG]\n\
         reset handling of SIG സൂചന(s) to the സ്ഥിരസ്ഥിതി\n\
"));
      oputs (_("\
      --ignore-സൂചന[=SIG]\n\
         set handling of SIG സൂചന(s) to ചെയ്യുക nothing\n\
"));
      oputs (_("\
      --list-സൂചന-handling\n\
         list non സ്ഥിരസ്ഥിതി സൂചന handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information വേണ്ടി each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a സൂചന name like 'PIPE', or a സൂചന സംഖ്യ like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

സ്ഥിതം ശൂന്യം
append_unset_var (അക്ഷരം സ്ഥിരം *var)
{
  എങ്കിൽ (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, വലുപ്പം *usvars);
  usvars[usvars_used++] = var;
}

സ്ഥിതം ശൂന്യം
unset_envvars (ശൂന്യം)
{
  വേണ്ടി (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      എങ്കിൽ (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
സ്ഥിതം bool
entry_has_name (അക്ഷരം സ്ഥിരം *entry, അക്ഷരം സ്ഥിരം *name, idx_t name_length)
{
  അക്ഷരം സ്ഥിരം *eq = strchr (entry, '=');
  തിരികെ (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#എങ്കിൽ ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
സ്ഥിതം size_t
env_vector_slot_hash (ശൂന്യം സ്ഥിരം *x, size_t table_size)
{
  അക്ഷരം *സ്ഥിരം *slot = x;
  അക്ഷരം സ്ഥിരം *entry = *slot;
  അക്ഷരം സ്ഥിരം *eq = strchr (entry, '=');
  തിരികെ hash_pjw_bare (entry, eq - entry) % table_size;
}

സ്ഥിതം bool
env_vector_slot_compare (ശൂന്യം സ്ഥിരം *x, ശൂന്യം സ്ഥിരം *y)
{
  അക്ഷരം സ്ഥിരം *a = * (അക്ഷരം *സ്ഥിരം *) x;
  അക്ഷരം സ്ഥിരം *b = * (അക്ഷരം *സ്ഥിരം *) y;
  അക്ഷരം സ്ഥിരം *b_eq = strchr (b, '=');
  തിരികെ entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
സ്ഥിതം ശൂന്യം
unset_envvars_in_vector (ഘടന env_vector *env)
{
  വേണ്ടി (idx_t i = 0; i < usvars_used; ++i)
    {
      അക്ഷരം സ്ഥിരം *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      എങ്കിൽ (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      വേണ്ടി (idx_t source = 0; source < env->used; ++source)
        എങ്കിൽ (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = ശൂന്യ;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
സ്ഥിതം ശൂന്യം
set_envvar (അക്ഷരം *assignment, അക്ഷരം *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  എങ്കിൽ (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
സ്ഥിതം ശൂന്യം
set_envvar_in_vector (ഘടന env_vector *env, അക്ഷരം *assignment, അക്ഷരം *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  വേണ്ടി (idx_t i = 0; i < env->used; ++i)
    എങ്കിൽ (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        തിരികെ;
      }

  എങ്കിൽ (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               വലുപ്പം *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = ശൂന്യ;
}

/* Read the NUL-delimited environment entries in FILE.  */
സ്ഥിതം size_t
read_env_file (അക്ഷരം സ്ഥിരം *file)
{
  size_t size;
  എങ്കിൽ (streq (file, "-"))
    {
      xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);
      env0_from_buffer = fread_file (stdin, RF_ദ്വിമാന, &size);
    }
  അല്ലെങ്കിൽ
    env0_from_buffer = read_file (file, RF_ദ്വിമാന, &size);

  എങ്കിൽ (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  എങ്കിൽ (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  തിരികെ size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
സ്ഥിതം bool
set_envvars_from_file (ഘടന env_vector *env, അക്ഷരം സ്ഥിരം *file)
{
  size_t size = read_env_file (file);

#എങ്കിൽ defined _WIN32 && ! defined __CYGWIN__
  (ശൂന്യം) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  അക്ഷരം *assignment = ശൂന്യ;
  ആകുമ്പോൾ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      അക്ഷരം *eq = strchr (assignment, '=');
      എങ്കിൽ (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  തിരികെ false;
#അല്ലെങ്കിൽ
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_എണ്ണം = argz_എണ്ണം (env0_from_buffer, size);

  എങ്കിൽ (file_എണ്ണം == 0)
    തിരികെ false;

  idx_t inherited_എണ്ണം = 0;
  ആകുമ്പോൾ (environ[inherited_എണ്ണം])
    ++inherited_എണ്ണം;

  എങ്കിൽ (IDX_MAX - inherited_എണ്ണം <= file_എണ്ണം)
    xalloc_die ();
  idx_t max_എണ്ണം = inherited_എണ്ണം + file_എണ്ണം;
  അക്ഷരം **merged = xnmalloc (max_എണ്ണം + 1, വലുപ്പം *merged);
  memcpy (merged, environ, inherited_എണ്ണം * വലുപ്പം *merged);
  merged[inherited_എണ്ണം] = ശൂന്യ;

  Hash_table *slot_table
    = hash_initialize (max_എണ്ണം, ശൂന്യ, env_vector_slot_hash,
                       env_vector_slot_compare, ശൂന്യ);
  എങ്കിൽ (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  വേണ്ടി (idx_t i = 0; i < inherited_എണ്ണം; ++i)
    {
      അക്ഷരം *eq = strchr (merged[i], '=');
      എങ്കിൽ (! eq)
        തുടരുക;

      എങ്കിൽ (! hash_തിരുകുക (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_എണ്ണം = inherited_എണ്ണം;
  അക്ഷരം *assignment = ശൂന്യ;
  ആകുമ്പോൾ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      അക്ഷരം *eq = strchr (assignment, '=');
      എങ്കിൽ (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_എണ്ണം++] = assignment;
        }
      അല്ലെങ്കിൽ
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          അക്ഷരം **slot = hash_lookup (slot_table, &assignment);
          എങ്കിൽ (slot)
            *slot = assignment;
          അല്ലെങ്കിൽ
            {
              merged[merged_എണ്ണം] = assignment;
              എങ്കിൽ (! hash_തിരുകുക (slot_table, &merged[merged_എണ്ണം]))
                xalloc_die ();
              ++merged_എണ്ണം;
            }
        }
    }

  merged[merged_എണ്ണം] = ശൂന്യ;
  env->variable = merged;
  env->used = merged_എണ്ണം;
  env->allocated = max_എണ്ണം + 1;
  environ = merged;
  hash_free (slot_table);
  തിരികെ true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
സ്ഥിതം ശൂന്യം
set_raw_environment_from_file (ഘടന env_vector *env,
                               അക്ഷരം സ്ഥിരം *file)
{
  size_t size = read_env_file (file);
  idx_t entry_എണ്ണം = argz_എണ്ണം (env0_from_buffer, size);

  env->allocated = entry_എണ്ണം + 1;
  env->variable = xnmalloc (env->allocated, വലുപ്പം *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_എണ്ണം;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
സ്ഥിതം അക്ഷരം സ്ഥിരം *
scan_varname (അക്ഷരം സ്ഥിരം *str)
{
  എങ്കിൽ (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      അക്ഷരം സ്ഥിരം *end = str + 3;
      ആകുമ്പോൾ (c_isalnum (*end) || *end == '_')
        ++end;
      എങ്കിൽ (*end == '}')
        തിരികെ end;
    }

  തിരികെ ശൂന്യ;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
സ്ഥിതം അക്ഷരം *
extract_varname (അക്ഷരം സ്ഥിരം *str)
{
  അക്ഷരം സ്ഥിരം *p = scan_varname (str);

  എങ്കിൽ (!p)
    തിരികെ ശൂന്യ;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  എങ്കിൽ (i >= vnlen)
    {
      സ്വതന്ത്രം (varname);
      varname = xpalloc (ശൂന്യ, &vnlen, i + 1 - vnlen, -1, വലുപ്പം *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  തിരികെ varname;
}

/* Temporary buffer used by --split-string processing.  */
ഘടന splitbuf
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
  അക്ഷരം **argv;
  പൂർണ്ണസംഖ്യ argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  പൂർണ്ണസംഖ്യ extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
സ്ഥിതം ശൂന്യം
splitbuf_grow (ഘടന splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * വലുപ്പം *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
സ്ഥിതം ശൂന്യം
splitbuf_append_byte (ഘടന splitbuf *ss, അക്ഷരം c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  എങ്കിൽ (ss->half_alloc * വലുപ്പം *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((അക്ഷരം *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (അക്ഷരം *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
സ്ഥിതം ശൂന്യം
check_start_new_arg (ഘടന splitbuf *ss)
{
  എങ്കിൽ (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      പൂർണ്ണസംഖ്യ argc = ss->argc;
      എങ്കിൽ (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
സ്ഥിതം അക്ഷരം **
splitbuf_finishup (ഘടന splitbuf *ss)
{
  പൂർണ്ണസംഖ്യ argc = ss->argc;
  അക്ഷരം **argv = ss->argv;
  അക്ഷരം *stringbase = (അക്ഷരം *) (ss->argv + ss->half_alloc);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  തിരികെ argv;
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
സ്ഥിതം അക്ഷരം **
build_argv (അക്ഷരം സ്ഥിരം *str, പൂർണ്ണസംഖ്യ extra_argc, പൂർണ്ണസംഖ്യ *argc)
{
  bool dq = false, sq = false;
  ഘടന splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * വലുപ്പം *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ശൂന്യ;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  ആകുമ്പോൾ (*str)
    {
      അക്ഷരം newc = *str; /* Default: add the next character.  */

      തിരഞ്ഞെടുപ്പ് (*str)
        {
        അവസ്ഥ '\'':
          എങ്കിൽ (dq)
            നിർത്തുക;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          തുടരുക;

        അവസ്ഥ '"':
          എങ്കിൽ (sq)
            നിർത്തുക;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          തുടരുക;

        അവസ്ഥ ' ': അവസ്ഥ '\t': അവസ്ഥ '\n': അവസ്ഥ '\v': അവസ്ഥ '\f': അവസ്ഥ '\r':
          /* Start a new argument if outside quotes.  */
          എങ്കിൽ (sq || dq)
            നിർത്തുക;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          തുടരുക;

        അവസ്ഥ '#':
          എങ്കിൽ (!ss.sep)
            നിർത്തുക;
          പോകുക eos; /* '#' as first char terminates the string.  */

        അവസ്ഥ '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          എങ്കിൽ (sq && str[1] != '\\' && str[1] != '\'')
            നിർത്തുക;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          തിരഞ്ഞെടുപ്പ് (newc)
            {
            അവസ്ഥ '"': അവസ്ഥ '#': അവസ്ഥ '$': അവസ്ഥ '\'': അവസ്ഥ '\\':
              /* Pass escaped character as-is.  */
              നിർത്തുക;

            അവസ്ഥ '_':
              എങ്കിൽ (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  തുടരുക;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              നിർത്തുക;

            അവസ്ഥ 'c':
              എങ്കിൽ (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              പോകുക eos; /* '\c' terminates the string.  */

            അവസ്ഥ 'f': newc = '\f'; നിർത്തുക;
            അവസ്ഥ 'n': newc = '\n'; നിർത്തുക;
            അവസ്ഥ 'r': newc = '\r'; നിർത്തുക;
            അവസ്ഥ 't': newc = '\t'; നിർത്തുക;
            അവസ്ഥ 'v': newc = '\v'; നിർത്തുക;

            അവസ്ഥ '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            സ്ഥിരസ്ഥിതി:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          നിർത്തുക;

        അവസ്ഥ '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          എങ്കിൽ (sq)
            നിർത്തുക;

          /* Store the ${VARNAME} value.  */
          {
            അക്ഷരം *n = extract_varname (str);
            എങ്കിൽ (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            അക്ഷരം *v = getenv (n);
            എങ്കിൽ (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                വേണ്ടി (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            അല്ലെങ്കിൽ
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            തുടരുക;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  എങ്കിൽ (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  തിരികെ splitbuf_finishup (&ss);
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
സ്ഥിതം ശൂന്യം
parse_split_string (അക്ഷരം സ്ഥിരം *str, പൂർണ്ണസംഖ്യ *orig_optind,
                    പൂർണ്ണസംഖ്യ *orig_argc, അക്ഷരം ***orig_argv)
{
  പൂർണ്ണസംഖ്യ extra_argc = *orig_argc - *orig_optind, newargc;
  അക്ഷരം **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  എങ്കിൽ (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * വലുപ്പം *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

സ്ഥിതം ശൂന്യം
parse_signal_action_params (അക്ഷരം സ്ഥിരം *arg, bool set_default)
{
  എങ്കിൽ (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      തിരികെ;
    }

  അക്ഷരം *optarg_writable = xstrdup (arg);

  അക്ഷരം *opt_sig = strtok (optarg_writable, ",");
  ആകുമ്പോൾ (opt_sig)
    {
      പൂർണ്ണസംഖ്യ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      എങ്കിൽ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      എങ്കിൽ (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ശൂന്യ, ",");
    }

  സ്വതന്ത്രം (optarg_writable);
}

സ്ഥിതം ശൂന്യം
reset_signal_handlers (ശൂന്യം)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ഘടന sigaction act;

      എങ്കിൽ (signals[i] == UNCHANGED)
        തുടരുക;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      പൂർണ്ണസംഖ്യ sig_err = sigaction (i, ശൂന്യ, &act);

      എങ്കിൽ (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      എങ്കിൽ (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ശൂന്യ);
          എങ്കിൽ (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      എങ്കിൽ (dev_debug)
        {
          അക്ഷരം signame[SIG2STR_MAX];
          എങ്കിൽ (sig2str (i, signame) != 0)
            snprintf (signame, വലുപ്പം signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


സ്ഥിതം ശൂന്യം
parse_block_signal_params (അക്ഷരം സ്ഥിരം *arg, bool block)
{
  എങ്കിൽ (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  എങ്കിൽ (! arg)
    തിരികെ;

  അക്ഷരം *optarg_writable = xstrdup (arg);

  അക്ഷരം *opt_sig = strtok (optarg_writable, ",");
  ആകുമ്പോൾ (opt_sig)
    {
      പൂർണ്ണസംഖ്യ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      എങ്കിൽ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      എങ്കിൽ (signum <= 0)
        usage (exit_failure);

      എങ്കിൽ (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          എങ്കിൽ (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      അല്ലെങ്കിൽ
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (ശൂന്യ, ",");
    }

  സ്വതന്ത്രം (optarg_writable);
}

സ്ഥിതം ശൂന്യം
set_signal_proc_mask (ശൂന്യം)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  എങ്കിൽ (sigprocmask (0, ശൂന്യ, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= SIGNUM_BOUND; i++)
    {
      അക്ഷരം സ്ഥിരം *debug_act = ശൂന്യ;

      എങ്കിൽ (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      അല്ലെങ്കിൽ എങ്കിൽ (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      എങ്കിൽ (dev_debug && debug_act)
        {
          അക്ഷരം signame[SIG2STR_MAX];
          എങ്കിൽ (sig2str (i, signame) != 0)
            snprintf (signame, വലുപ്പം signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  എങ്കിൽ (sigprocmask (SIG_SETMASK, &set, ശൂന്യ))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

സ്ഥിതം ശൂന്യം
list_signal_handling (ശൂന്യം)
{
  sigset_t set;

  sigemptyset (&set);
  എങ്കിൽ (sigprocmask (0, ശൂന്യ, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ഘടന sigaction act;
      എങ്കിൽ (sigaction (i, ശൂന്യ, &act))
        തുടരുക;

      അക്ഷരം സ്ഥിരം *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      അക്ഷരം സ്ഥിരം *blocked = sigismember (&set, i) ? "BLOCK" : "";
      അക്ഷരം സ്ഥിരം *connect = *ignored && *blocked ? "," : "";

      എങ്കിൽ (! *ignored && ! *blocked)
        തുടരുക;

      അക്ഷരം signame[SIG2STR_MAX];
      എങ്കിൽ (sig2str (i, signame) != 0)
        snprintf (signame, വലുപ്പം signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

സ്ഥിതം ശൂന്യം
initialize_signals (ശൂന്യം)
{
  signals = xmalloc ((വലുപ്പം *signals) * (SIGNUM_BOUND + 1));

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  പൂർണ്ണസംഖ്യ quoting_style_opt = -1;
  അക്ഷരം സ്ഥിരം *newdir = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *env0_from_file = ശൂന്യ;
  അക്ഷരം *argv0 = ശൂന്യ;
  ഘടന env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, വലുപ്പം env_vector);
  അക്ഷരം **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, shortopts, longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'a':
          argv0 = optarg;
          നിർത്തുക;
        അവസ്ഥ 'i':
          ignore_environment = true;
          നിർത്തുക;
        അവസ്ഥ 'u':
          append_unset_var (optarg);
          നിർത്തുക;
        അവസ്ഥ 'v':
          dev_debug = true;
          നിർത്തുക;
        അവസ്ഥ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          നിർത്തുക;
        അവസ്ഥ ENV0_FROM_OPTION:
          env0_from_file = optarg;
          നിർത്തുക;
        അവസ്ഥ DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          നിർത്തുക;
        അവസ്ഥ IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          നിർത്തുക;
        അവസ്ഥ BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          നിർത്തുക;
        അവസ്ഥ LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          നിർത്തുക;
        അവസ്ഥ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          നിർത്തുക;
        അവസ്ഥ 'C':
          newdir = optarg;
          നിർത്തുക;
        അവസ്ഥ 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          നിർത്തുക;
        അവസ്ഥ ' ': അവസ്ഥ '\t': അവസ്ഥ '\n': അവസ്ഥ '\v': അവസ്ഥ '\f': അവസ്ഥ '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            അക്ഷരം c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_CANCELED);
        }
    }

  എങ്കിൽ (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  പൂർണ്ണസംഖ്യ program_index = optind;
  ആകുമ്പോൾ (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  പൂർണ്ണസംഖ്യ qs = quoting_style_opt;
  എങ്കിൽ (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      എങ്കിൽ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  എങ്കിൽ (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ശൂന്യ, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  എങ്കിൽ (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#എങ്കിൽ defined _WIN32 && ! defined __CYGWIN__
      സ്ഥിതം അക്ഷരം *dummy_environ[] = { ശൂന്യ };
      environ = dummy_environ;
#അല്ലെങ്കിൽ
      env_vector.variable = xnmalloc (1, വലുപ്പം *env_vector.variable);
      env_vector.variable[0] = ശൂന്യ;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  അല്ലെങ്കിൽ എങ്കിൽ (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  എങ്കിൽ (! ignore_environment || env0_from_file)
    {
      എങ്കിൽ (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      അല്ലെങ്കിൽ
        unset_envvars ();
    }

  അക്ഷരം *eq;
  ആകുമ്പോൾ (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      എങ്കിൽ (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      അല്ലെങ്കിൽ
        set_envvar (argv[optind], eq);
      optind++;
    }

  എങ്കിൽ (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  എങ്കിൽ (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  എങ്കിൽ (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  എങ്കിൽ (! program_specified)
    {
      അക്ഷരം സ്ഥിരം terminator = opt_nul_terminate_output ? '\0' : '\n';

      എങ്കിൽ (opt_nul_terminate_output)
        xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);

      /* Print the environment and exit.  */
      വേണ്ടി (അക്ഷരം *സ്ഥിരം *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      എങ്കിൽ (env_vector_active)
        {
          environ = initial_environ;
          സ്വതന്ത്രം (env_vector.variable);
          സ്വതന്ത്രം (env0_from_buffer);
        }
#endif
      തിരികെ EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  എങ്കിൽ (sig_mask_changed)
    set_signal_proc_mask ();

  എങ്കിൽ (report_signal_handling)
    list_signal_handling ();

  എങ്കിൽ (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      എങ്കിൽ (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  അക്ഷരം *program = argv[optind];
  എങ്കിൽ (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  എങ്കിൽ (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      വേണ്ടി (പൂർണ്ണസംഖ്യ i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  പൂർണ്ണസംഖ്യ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  എങ്കിൽ (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
