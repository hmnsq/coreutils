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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <argz.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <c-ctype.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hashcode-mem.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "operand2sig.h"
#ಅನ್ತರ್ಭಾವ "printenv.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "read-file.h"
#ಅನ್ತರ್ಭಾವ "sig2str.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ **usvars;
ಸ್ಥಿರ idx_t usvars_alloc;
ಸ್ಥಿರ idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
ಸ್ಥಿರ ವರ್ಣ *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
ಸಂರಚನಾ env_vector
{
  ವರ್ಣ **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ಸ್ಥಿರ bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ಸ್ಥಿರ ವರ್ಣ *varname;
ಸ್ಥಿರ idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
ಗಣನಾ SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ಸ್ಥಿರ ಗಣನಾ SIGNAL_MODE *signals;

/* Set of signals to block.  */
ಸ್ಥಿರ sigset_t block_signals;

/* Set of signals to unblock.  */
ಸ್ಥಿರ sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
ಸ್ಥಿರ bool sig_mask_changed;

/* Whether to list non default handling.  */
ಸ್ಥಿರ bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"argv0", required_argument, ರಿಕ್ತ, 'a'},
  {"ignore-environment", no_argument, ರಿಕ್ತ, 'i'},
  {"env0-from", required_argument, ರಿಕ್ತ, ENV0_FROM_OPTION},
  {"null", no_argument, ರಿಕ್ತ, '0'},
  {"unset", required_argument, ರಿಕ್ತ, 'u'},
  {"chdir", required_argument, ರಿಕ್ತ, 'C'},
  {"default-signal", optional_argument, ರಿಕ್ತ, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, ರಿಕ್ತ, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, ರಿಕ್ತ, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, ರಿಕ್ತ,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, ರಿಕ್ತ, 'v'},
  {"quoting-style", required_argument, ರಿಕ್ತ, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, ರಿಕ್ತ, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=ಮೌಲ್ಯ_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to ಮೌಲ್ಯ_2 in the environment and run COMMAND.\n\
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
         ಪಠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output ಸಾಲು with NUL, not newline\n\
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
         use quoting style WORD ಪ್ರತಿ names and ಮೌಲ್ಯಗಳು:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang ಸಾಲುಗಳು\n\
"));
      oputs (_("\
      --block-ಸಙ್ಕೇತ[=SIG]\n\
         block delivery of SIG ಸಙ್ಕೇತ(s) to COMMAND\n\
"));
      oputs (_("\
      --ಸ್ವಭಾವ-ಸಙ್ಕೇತ[=SIG]\n\
         reset handling of SIG ಸಙ್ಕೇತ(s) to the ಸ್ವಭಾವ\n\
"));
      oputs (_("\
      --ignore-ಸಙ್ಕೇತ[=SIG]\n\
         set handling of SIG ಸಙ್ಕೇತ(s) to ಕುರು nothing\n\
"));
      oputs (_("\
      --list-ಸಙ್ಕೇತ-handling\n\
         list non ಸ್ವಭಾವ ಸಙ್ಕೇತ handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information ಪ್ರತಿ each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a ಸಙ್ಕೇತ name like 'PIPE', or a ಸಙ್ಕೇತ ಸಂಖ್ಯೆ like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_unset_var (ವರ್ಣ ಧ್ರುವ *var)
{
  ಯದಿ (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, ಪರಿಮಾಣ *usvars);
  usvars[usvars_used++] = var;
}

ಸ್ಥಿರ ಶೂನ್ಯ
unset_envvars (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      ಯದಿ (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ಸ್ಥಿರ bool
entry_has_name (ವರ್ಣ ಧ್ರುವ *entry, ವರ್ಣ ಧ್ರುವ *name, idx_t name_length)
{
  ವರ್ಣ ಧ್ರುವ *eq = strchr (entry, '=');
  ಪ್ರತ್ಯಾಗಮ (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#ಯದಿ ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ಸ್ಥಿರ size_t
env_vector_slot_hash (ಶೂನ್ಯ ಧ್ರುವ *x, size_t table_size)
{
  ವರ್ಣ *ಧ್ರುವ *slot = x;
  ವರ್ಣ ಧ್ರುವ *entry = *slot;
  ವರ್ಣ ಧ್ರುವ *eq = strchr (entry, '=');
  ಪ್ರತ್ಯಾಗಮ hash_pjw_bare (entry, eq - entry) % table_size;
}

ಸ್ಥಿರ bool
env_vector_slot_compare (ಶೂನ್ಯ ಧ್ರುವ *x, ಶೂನ್ಯ ಧ್ರುವ *y)
{
  ವರ್ಣ ಧ್ರುವ *a = * (ವರ್ಣ *ಧ್ರುವ *) x;
  ವರ್ಣ ಧ್ರುವ *b = * (ವರ್ಣ *ಧ್ರುವ *) y;
  ವರ್ಣ ಧ್ರುವ *b_eq = strchr (b, '=');
  ಪ್ರತ್ಯಾಗಮ entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ಸ್ಥಿರ ಶೂನ್ಯ
unset_envvars_in_vector (ಸಂರಚನಾ env_vector *env)
{
  ಪ್ರತಿ (idx_t i = 0; i < usvars_used; ++i)
    {
      ವರ್ಣ ಧ್ರುವ *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      ಯದಿ (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      ಪ್ರತಿ (idx_t source = 0; source < env->used; ++source)
        ಯದಿ (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = ರಿಕ್ತ;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ಸ್ಥಿರ ಶೂನ್ಯ
set_envvar (ವರ್ಣ *assignment, ವರ್ಣ *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  ಯದಿ (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
ಸ್ಥಿರ ಶೂನ್ಯ
set_envvar_in_vector (ಸಂರಚನಾ env_vector *env, ವರ್ಣ *assignment, ವರ್ಣ *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  ಪ್ರತಿ (idx_t i = 0; i < env->used; ++i)
    ಯದಿ (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        ಪ್ರತ್ಯಾಗಮ;
      }

  ಯದಿ (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               ಪರಿಮಾಣ *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = ರಿಕ್ತ;
}

/* Read the NUL-delimited environment entries in FILE.  */
ಸ್ಥಿರ size_t
read_env_file (ವರ್ಣ ಧ್ರುವ *file)
{
  size_t size;
  ಯದಿ (streq (file, "-"))
    {
      xset_ದ್ವಿಮಾನ_mode (STDIN_FILENO, O_ದ್ವಿಮಾನ);
      env0_from_buffer = fread_file (stdin, RF_ದ್ವಿಮಾನ, &size);
    }
  ಅನ್ಯಥಾ
    env0_from_buffer = read_file (file, RF_ದ್ವಿಮಾನ, &size);

  ಯದಿ (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  ಯದಿ (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  ಪ್ರತ್ಯಾಗಮ size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ಸ್ಥಿರ bool
set_envvars_from_file (ಸಂರಚನಾ env_vector *env, ವರ್ಣ ಧ್ರುವ *file)
{
  size_t size = read_env_file (file);

#ಯದಿ defined _WIN32 && ! defined __CYGWIN__
  (ಶೂನ್ಯ) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  ವರ್ಣ *assignment = ರಿಕ್ತ;
  ಯಾವತ್ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      ವರ್ಣ *eq = strchr (assignment, '=');
      ಯದಿ (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  ಪ್ರತ್ಯಾಗಮ false;
#ಅನ್ಯಥಾ
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_ಎಣಿಕೆ = argz_ಎಣಿಕೆ (env0_from_buffer, size);

  ಯದಿ (file_ಎಣಿಕೆ == 0)
    ಪ್ರತ್ಯಾಗಮ false;

  idx_t inherited_ಎಣಿಕೆ = 0;
  ಯಾವತ್ (environ[inherited_ಎಣಿಕೆ])
    ++inherited_ಎಣಿಕೆ;

  ಯದಿ (IDX_MAX - inherited_ಎಣಿಕೆ <= file_ಎಣಿಕೆ)
    xalloc_die ();
  idx_t max_ಎಣಿಕೆ = inherited_ಎಣಿಕೆ + file_ಎಣಿಕೆ;
  ವರ್ಣ **merged = xnmalloc (max_ಎಣಿಕೆ + 1, ಪರಿಮಾಣ *merged);
  memcpy (merged, environ, inherited_ಎಣಿಕೆ * ಪರಿಮಾಣ *merged);
  merged[inherited_ಎಣಿಕೆ] = ರಿಕ್ತ;

  Hash_table *slot_table
    = hash_initialize (max_ಎಣಿಕೆ, ರಿಕ್ತ, env_vector_slot_hash,
                       env_vector_slot_compare, ರಿಕ್ತ);
  ಯದಿ (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  ಪ್ರತಿ (idx_t i = 0; i < inherited_ಎಣಿಕೆ; ++i)
    {
      ವರ್ಣ *eq = strchr (merged[i], '=');
      ಯದಿ (! eq)
        ಅನುವರ್ತ;

      ಯದಿ (! hash_ಸೇರಿಸು (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_ಎಣಿಕೆ = inherited_ಎಣಿಕೆ;
  ವರ್ಣ *assignment = ರಿಕ್ತ;
  ಯಾವತ್ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      ವರ್ಣ *eq = strchr (assignment, '=');
      ಯದಿ (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_ಎಣಿಕೆ++] = assignment;
        }
      ಅನ್ಯಥಾ
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          ವರ್ಣ **slot = hash_lookup (slot_table, &assignment);
          ಯದಿ (slot)
            *slot = assignment;
          ಅನ್ಯಥಾ
            {
              merged[merged_ಎಣಿಕೆ] = assignment;
              ಯದಿ (! hash_ಸೇರಿಸು (slot_table, &merged[merged_ಎಣಿಕೆ]))
                xalloc_die ();
              ++merged_ಎಣಿಕೆ;
            }
        }
    }

  merged[merged_ಎಣಿಕೆ] = ರಿಕ್ತ;
  env->variable = merged;
  env->used = merged_ಎಣಿಕೆ;
  env->allocated = max_ಎಣಿಕೆ + 1;
  environ = merged;
  hash_free (slot_table);
  ಪ್ರತ್ಯಾಗಮ true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ಸ್ಥಿರ ಶೂನ್ಯ
set_raw_environment_from_file (ಸಂರಚನಾ env_vector *env,
                               ವರ್ಣ ಧ್ರುವ *file)
{
  size_t size = read_env_file (file);
  idx_t entry_ಎಣಿಕೆ = argz_ಎಣಿಕೆ (env0_from_buffer, size);

  env->allocated = entry_ಎಣಿಕೆ + 1;
  env->variable = xnmalloc (env->allocated, ಪರಿಮಾಣ *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_ಎಣಿಕೆ;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
scan_varname (ವರ್ಣ ಧ್ರುವ *str)
{
  ಯದಿ (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      ವರ್ಣ ಧ್ರುವ *end = str + 3;
      ಯಾವತ್ (c_isalnum (*end) || *end == '_')
        ++end;
      ಯದಿ (*end == '}')
        ಪ್ರತ್ಯಾಗಮ end;
    }

  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ಸ್ಥಿರ ವರ್ಣ *
extract_varname (ವರ್ಣ ಧ್ರುವ *str)
{
  ವರ್ಣ ಧ್ರುವ *p = scan_varname (str);

  ಯದಿ (!p)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  ಯದಿ (i >= vnlen)
    {
      ಮುಕ್ತ (varname);
      varname = xpalloc (ರಿಕ್ತ, &vnlen, i + 1 - vnlen, -1, ಪರಿಮಾಣ *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  ಪ್ರತ್ಯಾಗಮ varname;
}

/* Temporary buffer used by --split-string processing.  */
ಸಂರಚನಾ splitbuf
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
  ವರ್ಣ **argv;
  ಪೂರ್ಣಾಙ್ಕ argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  ಪೂರ್ಣಾಙ್ಕ extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
ಸ್ಥಿರ ಶೂನ್ಯ
splitbuf_grow (ಸಂರಚನಾ splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * ಪರಿಮಾಣ *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
ಸ್ಥಿರ ಶೂನ್ಯ
splitbuf_append_byte (ಸಂರಚನಾ splitbuf *ss, ವರ್ಣ c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ಯದಿ (ss->half_alloc * ಪರಿಮಾಣ *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((ವರ್ಣ *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (ವರ್ಣ *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ಸ್ಥಿರ ಶೂನ್ಯ
check_start_new_arg (ಸಂರಚನಾ splitbuf *ss)
{
  ಯದಿ (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      ಪೂರ್ಣಾಙ್ಕ argc = ss->argc;
      ಯದಿ (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
ಸ್ಥಿರ ವರ್ಣ **
splitbuf_finishup (ಸಂರಚನಾ splitbuf *ss)
{
  ಪೂರ್ಣಾಙ್ಕ argc = ss->argc;
  ವರ್ಣ **argv = ss->argv;
  ವರ್ಣ *stringbase = (ವರ್ಣ *) (ss->argv + ss->half_alloc);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  ಪ್ರತ್ಯಾಗಮ argv;
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
ಸ್ಥಿರ ವರ್ಣ **
build_argv (ವರ್ಣ ಧ್ರುವ *str, ಪೂರ್ಣಾಙ್ಕ extra_argc, ಪೂರ್ಣಾಙ್ಕ *argc)
{
  bool dq = false, sq = false;
  ಸಂರಚನಾ splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * ಪರಿಮಾಣ *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ರಿಕ್ತ;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  ಯಾವತ್ (*str)
    {
      ವರ್ಣ newc = *str; /* Default: add the next character.  */

      ವಿಕಲ್ಪ (*str)
        {
        ಪಕ್ಷ '\'':
          ಯದಿ (dq)
            ಭಙ್ಗ;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          ಅನುವರ್ತ;

        ಪಕ್ಷ '"':
          ಯದಿ (sq)
            ಭಙ್ಗ;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          ಅನುವರ್ತ;

        ಪಕ್ಷ ' ': ಪಕ್ಷ '\t': ಪಕ್ಷ '\n': ಪಕ್ಷ '\v': ಪಕ್ಷ '\f': ಪಕ್ಷ '\r':
          /* Start a new argument if outside quotes.  */
          ಯದಿ (sq || dq)
            ಭಙ್ಗ;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          ಅನುವರ್ತ;

        ಪಕ್ಷ '#':
          ಯದಿ (!ss.sep)
            ಭಙ್ಗ;
          ಗಚ್ಛ eos; /* '#' as first char terminates the string.  */

        ಪಕ್ಷ '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          ಯದಿ (sq && str[1] != '\\' && str[1] != '\'')
            ಭಙ್ಗ;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          ವಿಕಲ್ಪ (newc)
            {
            ಪಕ್ಷ '"': ಪಕ್ಷ '#': ಪಕ್ಷ '$': ಪಕ್ಷ '\'': ಪಕ್ಷ '\\':
              /* Pass escaped character as-is.  */
              ಭಙ್ಗ;

            ಪಕ್ಷ '_':
              ಯದಿ (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  ಅನುವರ್ತ;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              ಭಙ್ಗ;

            ಪಕ್ಷ 'c':
              ಯದಿ (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              ಗಚ್ಛ eos; /* '\c' terminates the string.  */

            ಪಕ್ಷ 'f': newc = '\f'; ಭಙ್ಗ;
            ಪಕ್ಷ 'n': newc = '\n'; ಭಙ್ಗ;
            ಪಕ್ಷ 'r': newc = '\r'; ಭಙ್ಗ;
            ಪಕ್ಷ 't': newc = '\t'; ಭಙ್ಗ;
            ಪಕ್ಷ 'v': newc = '\v'; ಭಙ್ಗ;

            ಪಕ್ಷ '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            ಸ್ವಭಾವ:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          ಯದಿ (sq)
            ಭಙ್ಗ;

          /* Store the ${VARNAME} value.  */
          {
            ವರ್ಣ *n = extract_varname (str);
            ಯದಿ (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            ವರ್ಣ *v = getenv (n);
            ಯದಿ (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                ಪ್ರತಿ (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            ಅನ್ಯಥಾ
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            ಅನುವರ್ತ;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  ಯದಿ (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  ಪ್ರತ್ಯಾಗಮ splitbuf_finishup (&ss);
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
ಸ್ಥಿರ ಶೂನ್ಯ
parse_split_string (ವರ್ಣ ಧ್ರುವ *str, ಪೂರ್ಣಾಙ್ಕ *orig_optind,
                    ಪೂರ್ಣಾಙ್ಕ *orig_argc, ವರ್ಣ ***orig_argv)
{
  ಪೂರ್ಣಾಙ್ಕ extra_argc = *orig_argc - *orig_optind, newargc;
  ವರ್ಣ **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  ಯದಿ (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * ಪರಿಮಾಣ *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_signal_action_params (ವರ್ಣ ಧ್ರುವ *arg, bool set_default)
{
  ಯದಿ (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      ಪ್ರತ್ಯಾಗಮ;
    }

  ವರ್ಣ *optarg_writable = xstrdup (arg);

  ವರ್ಣ *opt_sig = strtok (optarg_writable, ",");
  ಯಾವತ್ (opt_sig)
    {
      ಪೂರ್ಣಾಙ್ಕ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ಯದಿ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ಯದಿ (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ರಿಕ್ತ, ",");
    }

  ಮುಕ್ತ (optarg_writable);
}

ಸ್ಥಿರ ಶೂನ್ಯ
reset_signal_handlers (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ಸಂರಚನಾ sigaction act;

      ಯದಿ (signals[i] == UNCHANGED)
        ಅನುವರ್ತ;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      ಪೂರ್ಣಾಙ್ಕ sig_err = sigaction (i, ರಿಕ್ತ, &act);

      ಯದಿ (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      ಯದಿ (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ರಿಕ್ತ);
          ಯದಿ (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      ಯದಿ (dev_debug)
        {
          ವರ್ಣ signame[SIG2STR_MAX];
          ಯದಿ (sig2str (i, signame) != 0)
            snprintf (signame, ಪರಿಮಾಣ signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
parse_block_signal_params (ವರ್ಣ ಧ್ರುವ *arg, bool block)
{
  ಯದಿ (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  ಅನ್ಯಥಾ ಯದಿ (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  ಯದಿ (! arg)
    ಪ್ರತ್ಯಾಗಮ;

  ವರ್ಣ *optarg_writable = xstrdup (arg);

  ವರ್ಣ *opt_sig = strtok (optarg_writable, ",");
  ಯಾವತ್ (opt_sig)
    {
      ಪೂರ್ಣಾಙ್ಕ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ಯದಿ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ಯದಿ (signum <= 0)
        usage (exit_failure);

      ಯದಿ (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          ಯದಿ (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      ಅನ್ಯಥಾ
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (ರಿಕ್ತ, ",");
    }

  ಮುಕ್ತ (optarg_writable);
}

ಸ್ಥಿರ ಶೂನ್ಯ
set_signal_proc_mask (ಶೂನ್ಯ)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  ಯದಿ (sigprocmask (0, ರಿಕ್ತ, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ವರ್ಣ ಧ್ರುವ *debug_act = ರಿಕ್ತ;

      ಯದಿ (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      ಅನ್ಯಥಾ ಯದಿ (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      ಯದಿ (dev_debug && debug_act)
        {
          ವರ್ಣ signame[SIG2STR_MAX];
          ಯದಿ (sig2str (i, signame) != 0)
            snprintf (signame, ಪರಿಮಾಣ signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  ಯದಿ (sigprocmask (SIG_SETMASK, &set, ರಿಕ್ತ))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
list_signal_handling (ಶೂನ್ಯ)
{
  sigset_t set;

  sigemptyset (&set);
  ಯದಿ (sigprocmask (0, ರಿಕ್ತ, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ಸಂರಚನಾ sigaction act;
      ಯದಿ (sigaction (i, ರಿಕ್ತ, &act))
        ಅನುವರ್ತ;

      ವರ್ಣ ಧ್ರುವ *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      ವರ್ಣ ಧ್ರುವ *blocked = sigismember (&set, i) ? "BLOCK" : "";
      ವರ್ಣ ಧ್ರುವ *connect = *ignored && *blocked ? "," : "";

      ಯದಿ (! *ignored && ! *blocked)
        ಅನುವರ್ತ;

      ವರ್ಣ signame[SIG2STR_MAX];
      ಯದಿ (sig2str (i, signame) != 0)
        snprintf (signame, ಪರಿಮಾಣ signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
initialize_signals (ಶೂನ್ಯ)
{
  signals = xmalloc ((ಪರಿಮಾಣ *signals) * (SIGNUM_BOUND + 1));

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  ಪೂರ್ಣಾಙ್ಕ quoting_style_opt = -1;
  ವರ್ಣ ಧ್ರುವ *newdir = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *env0_from_file = ರಿಕ್ತ;
  ವರ್ಣ *argv0 = ರಿಕ್ತ;
  ಸಂರಚನಾ env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, ಪರಿಮಾಣ env_vector);
  ವರ್ಣ **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, shortopts, longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'a':
          argv0 = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'i':
          ignore_environment = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'u':
          append_unset_var (optarg);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          dev_debug = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ಭಙ್ಗ;
        ಪಕ್ಷ ENV0_FROM_OPTION:
          env0_from_file = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          ಭಙ್ಗ;
        ಪಕ್ಷ IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          ಭಙ್ಗ;
        ಪಕ್ಷ BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          ಭಙ್ಗ;
        ಪಕ್ಷ LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ಭಙ್ಗ;
        ಪಕ್ಷ 'C':
          newdir = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          ಭಙ್ಗ;
        ಪಕ್ಷ ' ': ಪಕ್ಷ '\t': ಪಕ್ಷ '\n': ಪಕ್ಷ '\v': ಪಕ್ಷ '\f': ಪಕ್ಷ '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            ವರ್ಣ c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_CANCELED);
        }
    }

  ಯದಿ (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  ಪೂರ್ಣಾಙ್ಕ program_index = optind;
  ಯಾವತ್ (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  ಪೂರ್ಣಾಙ್ಕ qs = quoting_style_opt;
  ಯದಿ (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      ಯದಿ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ಯದಿ (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ರಿಕ್ತ, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  ಯದಿ (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  ಅನ್ಯಥಾ ಯದಿ (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#ಯದಿ defined _WIN32 && ! defined __CYGWIN__
      ಸ್ಥಿರ ವರ್ಣ *dummy_environ[] = { ರಿಕ್ತ };
      environ = dummy_environ;
#ಅನ್ಯಥಾ
      env_vector.variable = xnmalloc (1, ಪರಿಮಾಣ *env_vector.variable);
      env_vector.variable[0] = ರಿಕ್ತ;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  ಅನ್ಯಥಾ ಯದಿ (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  ಯದಿ (! ignore_environment || env0_from_file)
    {
      ಯದಿ (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      ಅನ್ಯಥಾ
        unset_envvars ();
    }

  ವರ್ಣ *eq;
  ಯಾವತ್ (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      ಯದಿ (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      ಅನ್ಯಥಾ
        set_envvar (argv[optind], eq);
      optind++;
    }

  ಯದಿ (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  ಯದಿ (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  ಯದಿ (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  ಯದಿ (! program_specified)
    {
      ವರ್ಣ ಧ್ರುವ terminator = opt_nul_terminate_output ? '\0' : '\n';

      ಯದಿ (opt_nul_terminate_output)
        xset_ದ್ವಿಮಾನ_mode (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

      /* Print the environment and exit.  */
      ಪ್ರತಿ (ವರ್ಣ *ಧ್ರುವ *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      ಯದಿ (env_vector_active)
        {
          environ = initial_environ;
          ಮುಕ್ತ (env_vector.variable);
          ಮುಕ್ತ (env0_from_buffer);
        }
#endif
      ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  ಯದಿ (sig_mask_changed)
    set_signal_proc_mask ();

  ಯದಿ (report_signal_handling)
    list_signal_handling ();

  ಯದಿ (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      ಯದಿ (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  ವರ್ಣ *program = argv[optind];
  ಯದಿ (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  ಯದಿ (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  ಪೂರ್ಣಾಙ್ಕ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  ಯದಿ (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
