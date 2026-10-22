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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <argz.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <c-ctype.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "hashcode-mem.h"
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "operand2sig.h"
#ଅନ୍ତର୍ଭାଵ "printenv.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "read-file.h"
#ଅନ୍ତର୍ଭାଵ "sig2str.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ **usvars;
ସ୍ଥିର idx_t usvars_alloc;
ସ୍ଥିର idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
ସ୍ଥିର ଅକ୍ଷର *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
ସଂରଚନା env_vector
{
  ଅକ୍ଷର **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ସ୍ଥିର bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ସ୍ଥିର ଅକ୍ଷର *varname;
ସ୍ଥିର idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
ଗଣନା SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ସ୍ଥିର ଗଣନା SIGNAL_MODE *signals;

/* Set of signals to block.  */
ସ୍ଥିର sigset_t block_signals;

/* Set of signals to unblock.  */
ସ୍ଥିର sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
ସ୍ଥିର bool sig_mask_changed;

/* Whether to list non default handling.  */
ସ୍ଥିର bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"argv0", required_argument, ଖାଲି, 'a'},
  {"ignore-environment", no_argument, ଖାଲି, 'i'},
  {"env0-from", required_argument, ଖାଲି, ENV0_FROM_OPTION},
  {"null", no_argument, ଖାଲି, '0'},
  {"unset", required_argument, ଖାଲି, 'u'},
  {"chdir", required_argument, ଖାଲି, 'C'},
  {"default-signal", optional_argument, ଖାଲି, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, ଖାଲି, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, ଖାଲି, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, ଖାଲି,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, ଖାଲି, 'v'},
  {"quoting-style", required_argument, ଖାଲି, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, ଖାଲି, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=ମୂଲ୍ୟ_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to ମୂଲ୍ୟ_2 in the environment and run COMMAND.\n\
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
         ପଠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output ଧାଡ଼ି with NUL, not newline\n\
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
         use quoting style WORD ପାଇଁ names and ମୂଲ୍ୟଗୁଡ଼ିକ:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
      oputs (_("\
      --block-ସଙ୍କେତ[=SIG]\n\
         block delivery of SIG ସଙ୍କେତ(s) to COMMAND\n\
"));
      oputs (_("\
      --ପୂର୍ବନିର୍ଦ୍ଧାରିତ-ସଙ୍କେତ[=SIG]\n\
         reset handling of SIG ସଙ୍କେତ(s) to the ପୂର୍ବନିର୍ଦ୍ଧାରିତ\n\
"));
      oputs (_("\
      --ignore-ସଙ୍କେତ[=SIG]\n\
         set handling of SIG ସଙ୍କେତ(s) to କର nothing\n\
"));
      oputs (_("\
      --list-ସଙ୍କେତ-handling\n\
         list non ପୂର୍ବନିର୍ଦ୍ଧାରିତ ସଙ୍କେତ handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information ପାଇଁ each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a ସଙ୍କେତ name like 'PIPE', or a ସଙ୍କେତ ସଂଖ୍ୟା like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ସ୍ଥିର ଶୂନ୍ୟ
append_unset_var (ଅକ୍ଷର ଅଚଳ *var)
{
  ଯଦି (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, ଆକାର *usvars);
  usvars[usvars_used++] = var;
}

ସ୍ଥିର ଶୂନ୍ୟ
unset_envvars (ଶୂନ୍ୟ)
{
  ପାଇଁ (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      ଯଦି (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ସ୍ଥିର bool
entry_has_name (ଅକ୍ଷର ଅଚଳ *entry, ଅକ୍ଷର ଅଚଳ *name, idx_t name_length)
{
  ଅକ୍ଷର ଅଚଳ *eq = strchr (entry, '=');
  ଫେରନ୍ତୁ (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#ଯଦି ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ସ୍ଥିର size_t
env_vector_slot_hash (ଶୂନ୍ୟ ଅଚଳ *x, size_t table_size)
{
  ଅକ୍ଷର *ଅଚଳ *slot = x;
  ଅକ୍ଷର ଅଚଳ *entry = *slot;
  ଅକ୍ଷର ଅଚଳ *eq = strchr (entry, '=');
  ଫେରନ୍ତୁ hash_pjw_bare (entry, eq - entry) % table_size;
}

ସ୍ଥିର bool
env_vector_slot_compare (ଶୂନ୍ୟ ଅଚଳ *x, ଶୂନ୍ୟ ଅଚଳ *y)
{
  ଅକ୍ଷର ଅଚଳ *a = * (ଅକ୍ଷର *ଅଚଳ *) x;
  ଅକ୍ଷର ଅଚଳ *b = * (ଅକ୍ଷର *ଅଚଳ *) y;
  ଅକ୍ଷର ଅଚଳ *b_eq = strchr (b, '=');
  ଫେରନ୍ତୁ entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ସ୍ଥିର ଶୂନ୍ୟ
unset_envvars_in_vector (ସଂରଚନା env_vector *env)
{
  ପାଇଁ (idx_t i = 0; i < usvars_used; ++i)
    {
      ଅକ୍ଷର ଅଚଳ *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      ଯଦି (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      ପାଇଁ (idx_t source = 0; source < env->used; ++source)
        ଯଦି (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = ଖାଲି;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ସ୍ଥିର ଶୂନ୍ୟ
set_envvar (ଅକ୍ଷର *assignment, ଅକ୍ଷର *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  ଯଦି (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
ସ୍ଥିର ଶୂନ୍ୟ
set_envvar_in_vector (ସଂରଚନା env_vector *env, ଅକ୍ଷର *assignment, ଅକ୍ଷର *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  ପାଇଁ (idx_t i = 0; i < env->used; ++i)
    ଯଦି (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        ଫେରନ୍ତୁ;
      }

  ଯଦି (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               ଆକାର *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = ଖାଲି;
}

/* Read the NUL-delimited environment entries in FILE.  */
ସ୍ଥିର size_t
read_env_file (ଅକ୍ଷର ଅଚଳ *file)
{
  size_t size;
  ଯଦି (streq (file, "-"))
    {
      xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
      env0_from_buffer = fread_file (stdin, RF_ଦ୍ୱିମିକ, &size);
    }
  ଅନ୍ୟଥା
    env0_from_buffer = read_file (file, RF_ଦ୍ୱିମିକ, &size);

  ଯଦି (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  ଯଦି (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  ଫେରନ୍ତୁ size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ସ୍ଥିର bool
set_envvars_from_file (ସଂରଚନା env_vector *env, ଅକ୍ଷର ଅଚଳ *file)
{
  size_t size = read_env_file (file);

#ଯଦି defined _WIN32 && ! defined __CYGWIN__
  (ଶୂନ୍ୟ) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  ଅକ୍ଷର *assignment = ଖାଲି;
  ଯେତେବେଳେ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      ଅକ୍ଷର *eq = strchr (assignment, '=');
      ଯଦି (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  ଫେରନ୍ତୁ false;
#ଅନ୍ୟଥା
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_ଗଣନା = argz_ଗଣନା (env0_from_buffer, size);

  ଯଦି (file_ଗଣନା == 0)
    ଫେରନ୍ତୁ false;

  idx_t inherited_ଗଣନା = 0;
  ଯେତେବେଳେ (environ[inherited_ଗଣନା])
    ++inherited_ଗଣନା;

  ଯଦି (IDX_MAX - inherited_ଗଣନା <= file_ଗଣନା)
    xalloc_die ();
  idx_t max_ଗଣନା = inherited_ଗଣନା + file_ଗଣନା;
  ଅକ୍ଷର **merged = xnmalloc (max_ଗଣନା + 1, ଆକାର *merged);
  memcpy (merged, environ, inherited_ଗଣନା * ଆକାର *merged);
  merged[inherited_ଗଣନା] = ଖାଲି;

  Hash_table *slot_table
    = hash_initialize (max_ଗଣନା, ଖାଲି, env_vector_slot_hash,
                       env_vector_slot_compare, ଖାଲି);
  ଯଦି (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  ପାଇଁ (idx_t i = 0; i < inherited_ଗଣନା; ++i)
    {
      ଅକ୍ଷର *eq = strchr (merged[i], '=');
      ଯଦି (! eq)
        ଜାରି;

      ଯଦି (! hash_ଭର୍ତ୍ତି (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_ଗଣନା = inherited_ଗଣନା;
  ଅକ୍ଷର *assignment = ଖାଲି;
  ଯେତେବେଳେ ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      ଅକ୍ଷର *eq = strchr (assignment, '=');
      ଯଦି (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_ଗଣନା++] = assignment;
        }
      ଅନ୍ୟଥା
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          ଅକ୍ଷର **slot = hash_lookup (slot_table, &assignment);
          ଯଦି (slot)
            *slot = assignment;
          ଅନ୍ୟଥା
            {
              merged[merged_ଗଣନା] = assignment;
              ଯଦି (! hash_ଭର୍ତ୍ତି (slot_table, &merged[merged_ଗଣନା]))
                xalloc_die ();
              ++merged_ଗଣନା;
            }
        }
    }

  merged[merged_ଗଣନା] = ଖାଲି;
  env->variable = merged;
  env->used = merged_ଗଣନା;
  env->allocated = max_ଗଣନା + 1;
  environ = merged;
  hash_free (slot_table);
  ଫେରନ୍ତୁ true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ସ୍ଥିର ଶୂନ୍ୟ
set_raw_environment_from_file (ସଂରଚନା env_vector *env,
                               ଅକ୍ଷର ଅଚଳ *file)
{
  size_t size = read_env_file (file);
  idx_t entry_ଗଣନା = argz_ଗଣନା (env0_from_buffer, size);

  env->allocated = entry_ଗଣନା + 1;
  env->variable = xnmalloc (env->allocated, ଆକାର *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_ଗଣନା;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
scan_varname (ଅକ୍ଷର ଅଚଳ *str)
{
  ଯଦି (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      ଅକ୍ଷର ଅଚଳ *end = str + 3;
      ଯେତେବେଳେ (c_isalnum (*end) || *end == '_')
        ++end;
      ଯଦି (*end == '}')
        ଫେରନ୍ତୁ end;
    }

  ଫେରନ୍ତୁ ଖାଲି;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ସ୍ଥିର ଅକ୍ଷର *
extract_varname (ଅକ୍ଷର ଅଚଳ *str)
{
  ଅକ୍ଷର ଅଚଳ *p = scan_varname (str);

  ଯଦି (!p)
    ଫେରନ୍ତୁ ଖାଲି;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  ଯଦି (i >= vnlen)
    {
      ମୁକ୍ତ (varname);
      varname = xpalloc (ଖାଲି, &vnlen, i + 1 - vnlen, -1, ଆକାର *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  ଫେରନ୍ତୁ varname;
}

/* Temporary buffer used by --split-string processing.  */
ସଂରଚନା splitbuf
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
  ଅକ୍ଷର **argv;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
ସ୍ଥିର ଶୂନ୍ୟ
splitbuf_grow (ସଂରଚନା splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * ଆକାର *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
ସ୍ଥିର ଶୂନ୍ୟ
splitbuf_append_byte (ସଂରଚନା splitbuf *ss, ଅକ୍ଷର c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ଯଦି (ss->half_alloc * ଆକାର *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((ଅକ୍ଷର *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (ଅକ୍ଷର *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ସ୍ଥିର ଶୂନ୍ୟ
check_start_new_arg (ସଂରଚନା splitbuf *ss)
{
  ଯଦି (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc = ss->argc;
      ଯଦି (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
ସ୍ଥିର ଅକ୍ଷର **
splitbuf_finishup (ସଂରଚନା splitbuf *ss)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc = ss->argc;
  ଅକ୍ଷର **argv = ss->argv;
  ଅକ୍ଷର *stringbase = (ଅକ୍ଷର *) (ss->argv + ss->half_alloc);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  ଫେରନ୍ତୁ argv;
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
ସ୍ଥିର ଅକ୍ଷର **
build_argv (ଅକ୍ଷର ଅଚଳ *str, ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_argc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *argc)
{
  bool dq = false, sq = false;
  ସଂରଚନା splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * ଆକାର *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ଖାଲି;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  ଯେତେବେଳେ (*str)
    {
      ଅକ୍ଷର newc = *str; /* Default: add the next character.  */

      ବାଛ (*str)
        {
        ସ୍ଥିତି '\'':
          ଯଦି (dq)
            ଭାଙ୍ଗ;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          ଜାରି;

        ସ୍ଥିତି '"':
          ଯଦି (sq)
            ଭାଙ୍ଗ;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          ଜାରି;

        ସ୍ଥିତି ' ': ସ୍ଥିତି '\t': ସ୍ଥିତି '\n': ସ୍ଥିତି '\v': ସ୍ଥିତି '\f': ସ୍ଥିତି '\r':
          /* Start a new argument if outside quotes.  */
          ଯଦି (sq || dq)
            ଭାଙ୍ଗ;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          ଜାରି;

        ସ୍ଥିତି '#':
          ଯଦି (!ss.sep)
            ଭାଙ୍ଗ;
          ଯାଅ eos; /* '#' as first char terminates the string.  */

        ସ୍ଥିତି '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          ଯଦି (sq && str[1] != '\\' && str[1] != '\'')
            ଭାଙ୍ଗ;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          ବାଛ (newc)
            {
            ସ୍ଥିତି '"': ସ୍ଥିତି '#': ସ୍ଥିତି '$': ସ୍ଥିତି '\'': ସ୍ଥିତି '\\':
              /* Pass escaped character as-is.  */
              ଭାଙ୍ଗ;

            ସ୍ଥିତି '_':
              ଯଦି (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  ଜାରି;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'c':
              ଯଦି (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              ଯାଅ eos; /* '\c' terminates the string.  */

            ସ୍ଥିତି 'f': newc = '\f'; ଭାଙ୍ଗ;
            ସ୍ଥିତି 'n': newc = '\n'; ଭାଙ୍ଗ;
            ସ୍ଥିତି 'r': newc = '\r'; ଭାଙ୍ଗ;
            ସ୍ଥିତି 't': newc = '\t'; ଭାଙ୍ଗ;
            ସ୍ଥିତି 'v': newc = '\v'; ଭାଙ୍ଗ;

            ସ୍ଥିତି '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          ଯଦି (sq)
            ଭାଙ୍ଗ;

          /* Store the ${VARNAME} value.  */
          {
            ଅକ୍ଷର *n = extract_varname (str);
            ଯଦି (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            ଅକ୍ଷର *v = getenv (n);
            ଯଦି (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                ପାଇଁ (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            ଅନ୍ୟଥା
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            ଜାରି;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  ଯଦି (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  ଫେରନ୍ତୁ splitbuf_finishup (&ss);
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
ସ୍ଥିର ଶୂନ୍ୟ
parse_split_string (ଅକ୍ଷର ଅଚଳ *str, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *orig_optind,
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *orig_argc, ଅକ୍ଷର ***orig_argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_argc = *orig_argc - *orig_optind, newargc;
  ଅକ୍ଷର **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  ଯଦି (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * ଆକାର *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_signal_action_params (ଅକ୍ଷର ଅଚଳ *arg, bool set_default)
{
  ଯଦି (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      ଫେରନ୍ତୁ;
    }

  ଅକ୍ଷର *optarg_writable = xstrdup (arg);

  ଅକ୍ଷର *opt_sig = strtok (optarg_writable, ",");
  ଯେତେବେଳେ (opt_sig)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ଯଦି (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ଯଦି (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ଖାଲି, ",");
    }

  ମୁକ୍ତ (optarg_writable);
}

ସ୍ଥିର ଶୂନ୍ୟ
reset_signal_handlers (ଶୂନ୍ୟ)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= SIGNUM_BOUND; i++)
    {
      ସଂରଚନା sigaction act;

      ଯଦି (signals[i] == UNCHANGED)
        ଜାରି;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig_err = sigaction (i, ଖାଲି, &act);

      ଯଦି (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      ଯଦି (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ଖାଲି);
          ଯଦି (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      ଯଦି (dev_debug)
        {
          ଅକ୍ଷର signame[SIG2STR_MAX];
          ଯଦି (sig2str (i, signame) != 0)
            snprintf (signame, ଆକାର signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ସ୍ଥିର ଶୂନ୍ୟ
parse_block_signal_params (ଅକ୍ଷର ଅଚଳ *arg, bool block)
{
  ଯଦି (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  ଅନ୍ୟଥା ଯଦି (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  ଯଦି (! arg)
    ଫେରନ୍ତୁ;

  ଅକ୍ଷର *optarg_writable = xstrdup (arg);

  ଅକ୍ଷର *opt_sig = strtok (optarg_writable, ",");
  ଯେତେବେଳେ (opt_sig)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ଯଦି (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ଯଦି (signum <= 0)
        usage (exit_failure);

      ଯଦି (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          ଯଦି (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      ଅନ୍ୟଥା
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (ଖାଲି, ",");
    }

  ମୁକ୍ତ (optarg_writable);
}

ସ୍ଥିର ଶୂନ୍ୟ
set_signal_proc_mask (ଶୂନ୍ୟ)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  ଯଦି (sigprocmask (0, ଖାଲି, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= SIGNUM_BOUND; i++)
    {
      ଅକ୍ଷର ଅଚଳ *debug_act = ଖାଲି;

      ଯଦି (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      ଅନ୍ୟଥା ଯଦି (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      ଯଦି (dev_debug && debug_act)
        {
          ଅକ୍ଷର signame[SIG2STR_MAX];
          ଯଦି (sig2str (i, signame) != 0)
            snprintf (signame, ଆକାର signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  ଯଦି (sigprocmask (SIG_SETMASK, &set, ଖାଲି))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ସ୍ଥିର ଶୂନ୍ୟ
list_signal_handling (ଶୂନ୍ୟ)
{
  sigset_t set;

  sigemptyset (&set);
  ଯଦି (sigprocmask (0, ଖାଲି, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i <= SIGNUM_BOUND; i++)
    {
      ସଂରଚନା sigaction act;
      ଯଦି (sigaction (i, ଖାଲି, &act))
        ଜାରି;

      ଅକ୍ଷର ଅଚଳ *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      ଅକ୍ଷର ଅଚଳ *blocked = sigismember (&set, i) ? "BLOCK" : "";
      ଅକ୍ଷର ଅଚଳ *connect = *ignored && *blocked ? "," : "";

      ଯଦି (! *ignored && ! *blocked)
        ଜାରି;

      ଅକ୍ଷର signame[SIG2STR_MAX];
      ଯଦି (sig2str (i, signame) != 0)
        snprintf (signame, ଆକାର signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
initialize_signals (ଶୂନ୍ୟ)
{
  signals = xmalloc ((ଆକାର *signals) * (SIGNUM_BOUND + 1));

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା quoting_style_opt = -1;
  ଅକ୍ଷର ଅଚଳ *newdir = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *env0_from_file = ଖାଲି;
  ଅକ୍ଷର *argv0 = ଖାଲି;
  ସଂରଚନା env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, ଆକାର env_vector);
  ଅକ୍ଷର **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, shortopts, longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'a':
          argv0 = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'i':
          ignore_environment = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'u':
          append_unset_var (optarg);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          dev_debug = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ENV0_FROM_OPTION:
          env0_from_file = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'C':
          newdir = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          ଭାଙ୍ଗ;
        ସ୍ଥିତି ' ': ସ୍ଥିତି '\t': ସ୍ଥିତି '\n': ସ୍ଥିତି '\v': ସ୍ଥିତି '\f': ସ୍ଥିତି '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            ଅକ୍ଷର c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_CANCELED);
        }
    }

  ଯଦି (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା program_index = optind;
  ଯେତେବେଳେ (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା qs = quoting_style_opt;
  ଯଦି (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      ଯଦି (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ଯଦି (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ଖାଲି, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  ଯଦି (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  ଅନ୍ୟଥା ଯଦି (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#ଯଦି defined _WIN32 && ! defined __CYGWIN__
      ସ୍ଥିର ଅକ୍ଷର *dummy_environ[] = { ଖାଲି };
      environ = dummy_environ;
#ଅନ୍ୟଥା
      env_vector.variable = xnmalloc (1, ଆକାର *env_vector.variable);
      env_vector.variable[0] = ଖାଲି;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  ଅନ୍ୟଥା ଯଦି (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  ଯଦି (! ignore_environment || env0_from_file)
    {
      ଯଦି (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      ଅନ୍ୟଥା
        unset_envvars ();
    }

  ଅକ୍ଷର *eq;
  ଯେତେବେଳେ (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      ଯଦି (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      ଅନ୍ୟଥା
        set_envvar (argv[optind], eq);
      optind++;
    }

  ଯଦି (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  ଯଦି (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  ଯଦି (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  ଯଦି (! program_specified)
    {
      ଅକ୍ଷର ଅଚଳ terminator = opt_nul_terminate_output ? '\0' : '\n';

      ଯଦି (opt_nul_terminate_output)
        xset_ଦ୍ୱିମିକ_mode (STDOUT_FILENO, O_ଦ୍ୱିମିକ);

      /* Print the environment and exit.  */
      ପାଇଁ (ଅକ୍ଷର *ଅଚଳ *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      ଯଦି (env_vector_active)
        {
          environ = initial_environ;
          ମୁକ୍ତ (env_vector.variable);
          ମୁକ୍ତ (env0_from_buffer);
        }
#endif
      ଫେରନ୍ତୁ EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  ଯଦି (sig_mask_changed)
    set_signal_proc_mask ();

  ଯଦି (report_signal_handling)
    list_signal_handling ();

  ଯଦି (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      ଯଦି (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  ଅକ୍ଷର *program = argv[optind];
  ଯଦି (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  ଯଦି (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  ଯଦି (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
