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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <argz.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <c-ctype.h>
#அந்தர்பாவ <signal.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "hashcode-mem.h"
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "operand2sig.h"
#அந்தர்பாவ "printenv.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "read-file.h"
#அந்தர்பாவ "sig2str.h"
#அந்தர்பாவ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
நிலையான எழுத்து மாறிலி **usvars;
நிலையான idx_t usvars_alloc;
நிலையான idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
நிலையான எழுத்து *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
கட்டமைப்பு env_vector
{
  எழுத்து **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
நிலையான bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
நிலையான எழுத்து *varname;
நிலையான idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
எண்ணிக்கை SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
நிலையான எண்ணிக்கை SIGNAL_MODE *signals;

/* Set of signals to block.  */
நிலையான sigset_t block_signals;

/* Set of signals to unblock.  */
நிலையான sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
நிலையான bool sig_mask_changed;

/* Whether to list non default handling.  */
நிலையான bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

நிலையான எழுத்து மாறிலி shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"argv0", required_argument, வெற்று, 'a'},
  {"ignore-environment", no_argument, வெற்று, 'i'},
  {"env0-from", required_argument, வெற்று, ENV0_FROM_OPTION},
  {"null", no_argument, வெற்று, '0'},
  {"unset", required_argument, வெற்று, 'u'},
  {"chdir", required_argument, வெற்று, 'C'},
  {"default-signal", optional_argument, வெற்று, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, வெற்று, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, வெற்று, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, வெற்று,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, வெற்று, 'v'},
  {"quoting-style", required_argument, வெற்று, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, வெற்று, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=மதிப்பு_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to மதிப்பு_2 in the environment and run COMMAND.\n\
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
         பட NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output வரி with NUL, not newline\n\
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
         use quoting style WORD க்கு names and மதிப்புகள்:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang வரிகள்\n\
"));
      oputs (_("\
      --block-சமிக்ஞை[=SIG]\n\
         block delivery of SIG சமிக்ஞை(s) to COMMAND\n\
"));
      oputs (_("\
      --இயல்பு-சமிக்ஞை[=SIG]\n\
         reset handling of SIG சமிக்ஞை(s) to the இயல்பு\n\
"));
      oputs (_("\
      --ignore-சமிக்ஞை[=SIG]\n\
         set handling of SIG சமிக்ஞை(s) to செய் nothing\n\
"));
      oputs (_("\
      --list-சமிக்ஞை-handling\n\
         list non இயல்பு சமிக்ஞை handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information க்கு each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a சமிக்ஞை name like 'PIPE', or a சமிக்ஞை எண் like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

நிலையான வெறுமை
append_unset_var (எழுத்து மாறிலி *var)
{
  எனில் (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, அளவு *usvars);
  usvars[usvars_used++] = var;
}

நிலையான வெறுமை
unset_envvars (வெறுமை)
{
  க்கு (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      எனில் (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
நிலையான bool
entry_has_name (எழுத்து மாறிலி *entry, எழுத்து மாறிலி *name, idx_t name_length)
{
  எழுத்து மாறிலி *eq = strchr (entry, '=');
  திரும்பு (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#எனில் ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
நிலையான size_t
env_vector_slot_hash (வெறுமை மாறிலி *x, size_t table_size)
{
  எழுத்து *மாறிலி *slot = x;
  எழுத்து மாறிலி *entry = *slot;
  எழுத்து மாறிலி *eq = strchr (entry, '=');
  திரும்பு hash_pjw_bare (entry, eq - entry) % table_size;
}

நிலையான bool
env_vector_slot_compare (வெறுமை மாறிலி *x, வெறுமை மாறிலி *y)
{
  எழுத்து மாறிலி *a = * (எழுத்து *மாறிலி *) x;
  எழுத்து மாறிலி *b = * (எழுத்து *மாறிலி *) y;
  எழுத்து மாறிலி *b_eq = strchr (b, '=');
  திரும்பு entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
நிலையான வெறுமை
unset_envvars_in_vector (கட்டமைப்பு env_vector *env)
{
  க்கு (idx_t i = 0; i < usvars_used; ++i)
    {
      எழுத்து மாறிலி *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      எனில் (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = strlen (name);
      idx_t dest = 0;
      க்கு (idx_t source = 0; source < env->used; ++source)
        எனில் (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = வெற்று;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
நிலையான வெறுமை
set_envvar (எழுத்து *assignment, எழுத்து *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  எனில் (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
நிலையான வெறுமை
set_envvar_in_vector (கட்டமைப்பு env_vector *env, எழுத்து *assignment, எழுத்து *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  க்கு (idx_t i = 0; i < env->used; ++i)
    எனில் (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        திரும்பு;
      }

  எனில் (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               அளவு *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = வெற்று;
}

/* Read the NUL-delimited environment entries in FILE.  */
நிலையான size_t
read_env_file (எழுத்து மாறிலி *file)
{
  size_t size;
  எனில் (streq (file, "-"))
    {
      xset_இருமம்_mode (STDIN_FILENO, O_இருமம்);
      env0_from_buffer = fread_file (stdin, RF_இருமம், &size);
    }
  இல்லையெனில்
    env0_from_buffer = read_file (file, RF_இருமம், &size);

  எனில் (! env0_from_buffer)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  எனில் (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  திரும்பு size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
நிலையான bool
set_envvars_from_file (கட்டமைப்பு env_vector *env, எழுத்து மாறிலி *file)
{
  size_t size = read_env_file (file);

#எனில் defined _WIN32 && ! defined __CYGWIN__
  (வெறுமை) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  எழுத்து *assignment = வெற்று;
  போது ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      எழுத்து *eq = strchr (assignment, '=');
      எனில் (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  திரும்பு false;
#இல்லையெனில்
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_மொத்தம் = argz_மொத்தம் (env0_from_buffer, size);

  எனில் (file_மொத்தம் == 0)
    திரும்பு false;

  idx_t inherited_மொத்தம் = 0;
  போது (environ[inherited_மொத்தம்])
    ++inherited_மொத்தம்;

  எனில் (IDX_MAX - inherited_மொத்தம் <= file_மொத்தம்)
    xalloc_die ();
  idx_t max_மொத்தம் = inherited_மொத்தம் + file_மொத்தம்;
  எழுத்து **merged = xnmalloc (max_மொத்தம் + 1, அளவு *merged);
  memcpy (merged, environ, inherited_மொத்தம் * அளவு *merged);
  merged[inherited_மொத்தம்] = வெற்று;

  Hash_table *slot_table
    = hash_initialize (max_மொத்தம், வெற்று, env_vector_slot_hash,
                       env_vector_slot_compare, வெற்று);
  எனில் (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  க்கு (idx_t i = 0; i < inherited_மொத்தம்; ++i)
    {
      எழுத்து *eq = strchr (merged[i], '=');
      எனில் (! eq)
        தொடர்;

      எனில் (! hash_செருகு (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_மொத்தம் = inherited_மொத்தம்;
  எழுத்து *assignment = வெற்று;
  போது ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      எழுத்து *eq = strchr (assignment, '=');
      எனில் (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_மொத்தம்++] = assignment;
        }
      இல்லையெனில்
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          எழுத்து **slot = hash_lookup (slot_table, &assignment);
          எனில் (slot)
            *slot = assignment;
          இல்லையெனில்
            {
              merged[merged_மொத்தம்] = assignment;
              எனில் (! hash_செருகு (slot_table, &merged[merged_மொத்தம்]))
                xalloc_die ();
              ++merged_மொத்தம்;
            }
        }
    }

  merged[merged_மொத்தம்] = வெற்று;
  env->variable = merged;
  env->used = merged_மொத்தம்;
  env->allocated = max_மொத்தம் + 1;
  environ = merged;
  hash_free (slot_table);
  திரும்பு true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
நிலையான வெறுமை
set_raw_environment_from_file (கட்டமைப்பு env_vector *env,
                               எழுத்து மாறிலி *file)
{
  size_t size = read_env_file (file);
  idx_t entry_மொத்தம் = argz_மொத்தம் (env0_from_buffer, size);

  env->allocated = entry_மொத்தம் + 1;
  env->variable = xnmalloc (env->allocated, அளவு *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_மொத்தம்;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
நிலையான எழுத்து மாறிலி *
scan_varname (எழுத்து மாறிலி *str)
{
  எனில் (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      எழுத்து மாறிலி *end = str + 3;
      போது (c_isalnum (*end) || *end == '_')
        ++end;
      எனில் (*end == '}')
        திரும்பு end;
    }

  திரும்பு வெற்று;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
நிலையான எழுத்து *
extract_varname (எழுத்து மாறிலி *str)
{
  எழுத்து மாறிலி *p = scan_varname (str);

  எனில் (!p)
    திரும்பு வெற்று;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  எனில் (i >= vnlen)
    {
      விடுவி (varname);
      varname = xpalloc (வெற்று, &vnlen, i + 1 - vnlen, -1, அளவு *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  திரும்பு varname;
}

/* Temporary buffer used by --split-string processing.  */
கட்டமைப்பு splitbuf
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
  எழுத்து **argv;
  முழு argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  முழு extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
நிலையான வெறுமை
splitbuf_grow (கட்டமைப்பு splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * அளவு *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
நிலையான வெறுமை
splitbuf_append_byte (கட்டமைப்பு splitbuf *ss, எழுத்து c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  எனில் (ss->half_alloc * அளவு *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((எழுத்து *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (எழுத்து *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
நிலையான வெறுமை
check_start_new_arg (கட்டமைப்பு splitbuf *ss)
{
  எனில் (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      முழு argc = ss->argc;
      எனில் (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
நிலையான எழுத்து **
splitbuf_finishup (கட்டமைப்பு splitbuf *ss)
{
  முழு argc = ss->argc;
  எழுத்து **argv = ss->argv;
  எழுத்து *stringbase = (எழுத்து *) (ss->argv + ss->half_alloc);
  க்கு (முழு i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  திரும்பு argv;
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
நிலையான எழுத்து **
build_argv (எழுத்து மாறிலி *str, முழு extra_argc, முழு *argc)
{
  bool dq = false, sq = false;
  கட்டமைப்பு splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * அளவு *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = வெற்று;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  போது (*str)
    {
      எழுத்து newc = *str; /* Default: add the next character.  */

      தேர்வு (*str)
        {
        நிலை '\'':
          எனில் (dq)
            நிறுத்து;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          தொடர்;

        நிலை '"':
          எனில் (sq)
            நிறுத்து;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          தொடர்;

        நிலை ' ': நிலை '\t': நிலை '\n': நிலை '\v': நிலை '\f': நிலை '\r':
          /* Start a new argument if outside quotes.  */
          எனில் (sq || dq)
            நிறுத்து;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          தொடர்;

        நிலை '#':
          எனில் (!ss.sep)
            நிறுத்து;
          செல் eos; /* '#' as first char terminates the string.  */

        நிலை '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          எனில் (sq && str[1] != '\\' && str[1] != '\'')
            நிறுத்து;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          தேர்வு (newc)
            {
            நிலை '"': நிலை '#': நிலை '$': நிலை '\'': நிலை '\\':
              /* Pass escaped character as-is.  */
              நிறுத்து;

            நிலை '_':
              எனில் (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  தொடர்;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              நிறுத்து;

            நிலை 'c':
              எனில் (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              செல் eos; /* '\c' terminates the string.  */

            நிலை 'f': newc = '\f'; நிறுத்து;
            நிலை 'n': newc = '\n'; நிறுத்து;
            நிலை 'r': newc = '\r'; நிறுத்து;
            நிலை 't': newc = '\t'; நிறுத்து;
            நிலை 'v': newc = '\v'; நிறுத்து;

            நிலை '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            இயல்பு:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          நிறுத்து;

        நிலை '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          எனில் (sq)
            நிறுத்து;

          /* Store the ${VARNAME} value.  */
          {
            எழுத்து *n = extract_varname (str);
            எனில் (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            எழுத்து *v = getenv (n);
            எனில் (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                க்கு (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            இல்லையெனில்
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            தொடர்;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  எனில் (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  திரும்பு splitbuf_finishup (&ss);
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
நிலையான வெறுமை
parse_split_string (எழுத்து மாறிலி *str, முழு *orig_optind,
                    முழு *orig_argc, எழுத்து ***orig_argv)
{
  முழு extra_argc = *orig_argc - *orig_optind, newargc;
  எழுத்து **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  எனில் (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      க்கு (முழு i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * அளவு *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

நிலையான வெறுமை
parse_signal_action_params (எழுத்து மாறிலி *arg, bool set_default)
{
  எனில் (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      க்கு (முழு i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      திரும்பு;
    }

  எழுத்து *optarg_writable = xstrdup (arg);

  எழுத்து *opt_sig = strtok (optarg_writable, ",");
  போது (opt_sig)
    {
      முழு signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      எனில் (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      எனில் (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (வெற்று, ",");
    }

  விடுவி (optarg_writable);
}

நிலையான வெறுமை
reset_signal_handlers (வெறுமை)
{
  க்கு (முழு i = 1; i <= SIGNUM_BOUND; i++)
    {
      கட்டமைப்பு sigaction act;

      எனில் (signals[i] == UNCHANGED)
        தொடர்;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      முழு sig_err = sigaction (i, வெற்று, &act);

      எனில் (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      எனில் (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, வெற்று);
          எனில் (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      எனில் (dev_debug)
        {
          எழுத்து signame[SIG2STR_MAX];
          எனில் (sig2str (i, signame) != 0)
            snprintf (signame, அளவு signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


நிலையான வெறுமை
parse_block_signal_params (எழுத்து மாறிலி *arg, bool block)
{
  எனில் (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  இல்லையெனில் எனில் (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  எனில் (! arg)
    திரும்பு;

  எழுத்து *optarg_writable = xstrdup (arg);

  எழுத்து *opt_sig = strtok (optarg_writable, ",");
  போது (opt_sig)
    {
      முழு signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      எனில் (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      எனில் (signum <= 0)
        usage (exit_failure);

      எனில் (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          எனில் (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      இல்லையெனில்
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (வெற்று, ",");
    }

  விடுவி (optarg_writable);
}

நிலையான வெறுமை
set_signal_proc_mask (வெறுமை)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  எனில் (sigprocmask (0, வெற்று, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  க்கு (முழு i = 1; i <= SIGNUM_BOUND; i++)
    {
      எழுத்து மாறிலி *debug_act = வெற்று;

      எனில் (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      இல்லையெனில் எனில் (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      எனில் (dev_debug && debug_act)
        {
          எழுத்து signame[SIG2STR_MAX];
          எனில் (sig2str (i, signame) != 0)
            snprintf (signame, அளவு signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  எனில் (sigprocmask (SIG_SETMASK, &set, வெற்று))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

நிலையான வெறுமை
list_signal_handling (வெறுமை)
{
  sigset_t set;

  sigemptyset (&set);
  எனில் (sigprocmask (0, வெற்று, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  க்கு (முழு i = 1; i <= SIGNUM_BOUND; i++)
    {
      கட்டமைப்பு sigaction act;
      எனில் (sigaction (i, வெற்று, &act))
        தொடர்;

      எழுத்து மாறிலி *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      எழுத்து மாறிலி *blocked = sigismember (&set, i) ? "BLOCK" : "";
      எழுத்து மாறிலி *connect = *ignored && *blocked ? "," : "";

      எனில் (! *ignored && ! *blocked)
        தொடர்;

      எழுத்து signame[SIG2STR_MAX];
      எனில் (sig2str (i, signame) != 0)
        snprintf (signame, அளவு signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

நிலையான வெறுமை
initialize_signals (வெறுமை)
{
  signals = xmalloc ((அளவு *signals) * (SIGNUM_BOUND + 1));

  க்கு (முழு i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  முழு quoting_style_opt = -1;
  எழுத்து மாறிலி *newdir = வெற்று;
  எழுத்து மாறிலி *env0_from_file = வெற்று;
  எழுத்து *argv0 = வெற்று;
  கட்டமைப்பு env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, அளவு env_vector);
  எழுத்து **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  முழு optc;
  போது ((optc = getopt_long (argc, argv, shortopts, longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'a':
          argv0 = optarg;
          நிறுத்து;
        நிலை 'i':
          ignore_environment = true;
          நிறுத்து;
        நிலை 'u':
          append_unset_var (optarg);
          நிறுத்து;
        நிலை 'v':
          dev_debug = true;
          நிறுத்து;
        நிலை '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          நிறுத்து;
        நிலை ENV0_FROM_OPTION:
          env0_from_file = optarg;
          நிறுத்து;
        நிலை DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          நிறுத்து;
        நிலை IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          நிறுத்து;
        நிலை BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          நிறுத்து;
        நிலை LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          நிறுத்து;
        நிலை QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          நிறுத்து;
        நிலை 'C':
          newdir = optarg;
          நிறுத்து;
        நிலை 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          நிறுத்து;
        நிலை ' ': நிலை '\t': நிலை '\n': நிலை '\v': நிலை '\f': நிலை '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            எழுத்து c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_CANCELED);
        }
    }

  எனில் (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  முழு program_index = optind;
  போது (program_index < argc && strchr (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  முழு qs = quoting_style_opt;
  எனில் (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      எனில் (qs < 0)
        qs = shell_escape_quoting_style;
    }
  எனில் (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (வெற்று, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  எனில் (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  இல்லையெனில் எனில் (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#எனில் defined _WIN32 && ! defined __CYGWIN__
      நிலையான எழுத்து *dummy_environ[] = { வெற்று };
      environ = dummy_environ;
#இல்லையெனில்
      env_vector.variable = xnmalloc (1, அளவு *env_vector.variable);
      env_vector.variable[0] = வெற்று;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  இல்லையெனில் எனில் (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  எனில் (! ignore_environment || env0_from_file)
    {
      எனில் (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      இல்லையெனில்
        unset_envvars ();
    }

  எழுத்து *eq;
  போது (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      எனில் (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      இல்லையெனில்
        set_envvar (argv[optind], eq);
      optind++;
    }

  எனில் (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  எனில் (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  எனில் (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  எனில் (! program_specified)
    {
      எழுத்து மாறிலி terminator = opt_nul_terminate_output ? '\0' : '\n';

      எனில் (opt_nul_terminate_output)
        xset_இருமம்_mode (STDOUT_FILENO, O_இருமம்);

      /* Print the environment and exit.  */
      க்கு (எழுத்து *மாறிலி *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      எனில் (env_vector_active)
        {
          environ = initial_environ;
          விடுவி (env_vector.variable);
          விடுவி (env0_from_buffer);
        }
#endif
      திரும்பு EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  எனில் (sig_mask_changed)
    set_signal_proc_mask ();

  எனில் (report_signal_handling)
    list_signal_handling ();

  எனில் (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      எனில் (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  எழுத்து *program = argv[optind];
  எனில் (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  எனில் (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      க்கு (முழு i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  முழு exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  எனில் (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
