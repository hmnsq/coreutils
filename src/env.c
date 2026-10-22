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
स्थिर अक्षर स्थिरांक **usvars;
स्थिर idx_t usvars_alloc;
स्थिर idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
स्थिर अक्षर *env0_from_buffer;

/* An environment vector managed directly to preserve raw entries.  */
संरचना env_vector
{
  अक्षर **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
स्थिर bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
स्थिर अक्षर *varname;
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

स्थिर अक्षर स्थिरांक shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  ENV0_FROM_OPTION = अक्षर_उच्च + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

स्थिर संरचना option स्थिरांक longopts[] =
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
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [-] [NAME=मूल्य_2]... [COMMAND [ARG]...]\n"),
              program_name);
      ख_माला_दो (_("\
Set each NAME to मूल्य_2 in the environment and run COMMAND.\n\
"), मानक_निकास);

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
      --env0-from=खाता\n\
         पठ NUL-delimited environment entries from खाता\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output ओळ with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         हटाओ variable from the environment\n\
"));
      oputs (_("\
  -C, --स_बदलो=सूची\n\
         change working directory to सूची\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD साठी names and मूल्ये:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang ओळी\n\
"));
      oputs (_("\
      --block-संकेत[=SIG]\n\
         block delivery of SIG संकेत(s) to COMMAND\n\
"));
      oputs (_("\
      --पूर्वनिर्धारित-संकेत[=SIG]\n\
         reset handling of SIG संकेत(s) to the पूर्वनिर्धारित\n\
"));
      oputs (_("\
      --ignore-संकेत[=SIG]\n\
         set handling of SIG संकेत(s) to करा nothing\n\
"));
      oputs (_("\
      --list-संकेत-handling\n\
         list non पूर्वनिर्धारित संकेत handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information साठी each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
SIG may be a संकेत name like 'PIPE', or a संकेत संख्या like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), मानक_निकास);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

स्थिर शून्य
append_unset_var (अक्षर स्थिरांक *var)
{
  जर (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, आकार *usvars);
  usvars[usvars_used++] = var;
}

स्थिर शून्य
unset_envvars (शून्य)
{
  साठी (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      जर (unsetenv (usvars[i]))
        error (EXIT_CANCELED, त्रुटि_सं, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
स्थिर bool
entry_has_name (अक्षर स्थिरांक *entry, अक्षर स्थिरांक *name, idx_t name_length)
{
  अक्षर स्थिरांक *eq = म_अक्षर (entry, '=');
  परत (eq && eq - entry == name_length
          && memeq (entry, name, name_length));
}

#जर ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
स्थिर माप_प्रकार
env_vector_slot_hash (शून्य स्थिरांक *x, माप_प्रकार table_size)
{
  अक्षर *स्थिरांक *slot = x;
  अक्षर स्थिरांक *entry = *slot;
  अक्षर स्थिरांक *eq = म_अक्षर (entry, '=');
  परत hash_pjw_bare (entry, eq - entry) % table_size;
}

स्थिर bool
env_vector_slot_compare (शून्य स्थिरांक *x, शून्य स्थिरांक *y)
{
  अक्षर स्थिरांक *a = * (अक्षर *स्थिरांक *) x;
  अक्षर स्थिरांक *b = * (अक्षर *स्थिरांक *) y;
  अक्षर स्थिरांक *b_eq = म_अक्षर (b, '=');
  परत entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
स्थिर शून्य
unset_envvars_in_vector (संरचना env_vector *env)
{
  साठी (idx_t i = 0; i < usvars_used; ++i)
    {
      अक्षर स्थिरांक *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      जर (! *name || म_अक्षर (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_length = म_माप (name);
      idx_t dest = 0;
      साठी (idx_t source = 0; source < env->used; ++source)
        जर (! entry_has_name (env->variable[source], name, name_length))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = रिक्त;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
स्थिर शून्य
set_envvar (अक्षर *assignment, अक्षर *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  जर (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, त्रुटि_सं, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
स्थिर शून्य
set_envvar_in_vector (संरचना env_vector *env, अक्षर *assignment, अक्षर *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_length = eq - assignment;
  साठी (idx_t i = 0; i < env->used; ++i)
    जर (entry_has_name (env->variable[i], assignment, name_length))
      {
        env->variable[i] = assignment;
        परत;
      }

  जर (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               आकार *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = रिक्त;
}

/* Read the NUL-delimited environment entries in FILE.  */
स्थिर माप_प्रकार
read_env_file (अक्षर स्थिरांक *file)
{
  माप_प्रकार size;
  जर (streq (file, "-"))
    {
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
      env0_from_buffer = fread_file (मानक_निवेश, RF_द्विमान, &size);
    }
  अन्यथा
    env0_from_buffer = read_file (file, RF_द्विमान, &size);

  जर (! env0_from_buffer)
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot read %s"), quoteaf (file));
  जर (size && env0_from_buffer[size - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  परत size;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
स्थिर bool
set_envvars_from_file (संरचना env_vector *env, अक्षर स्थिरांक *file)
{
  माप_प्रकार size = read_env_file (file);

#जर defined _WIN32 && ! defined __CYGWIN__
  (शून्य) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  अक्षर *assignment = रिक्त;
  पर्यंत ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      अक्षर *eq = म_अक्षर (assignment, '=');
      जर (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  परत false;
#अन्यथा
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_मोजणी = argz_मोजणी (env0_from_buffer, size);

  जर (file_मोजणी == 0)
    परत false;

  idx_t inherited_मोजणी = 0;
  पर्यंत (environ[inherited_मोजणी])
    ++inherited_मोजणी;

  जर (IDX_MAX - inherited_मोजणी <= file_मोजणी)
    xalloc_die ();
  idx_t max_मोजणी = inherited_मोजणी + file_मोजणी;
  अक्षर **merged = xnmalloc (max_मोजणी + 1, आकार *merged);
  स_नकल (merged, environ, inherited_मोजणी * आकार *merged);
  merged[inherited_मोजणी] = रिक्त;

  Hash_table *slot_table
    = hash_initialize (max_मोजणी, रिक्त, env_vector_slot_hash,
                       env_vector_slot_compare, रिक्त);
  जर (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  साठी (idx_t i = 0; i < inherited_मोजणी; ++i)
    {
      अक्षर *eq = म_अक्षर (merged[i], '=');
      जर (! eq)
        सुरूठेवा;

      जर (! hash_घाला (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_मोजणी = inherited_मोजणी;
  अक्षर *assignment = रिक्त;
  पर्यंत ((assignment = argz_next (env0_from_buffer, size, assignment)))
    {
      अक्षर *eq = म_अक्षर (assignment, '=');
      जर (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_मोजणी++] = assignment;
        }
      अन्यथा
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          अक्षर **slot = hash_lookup (slot_table, &assignment);
          जर (slot)
            *slot = assignment;
          अन्यथा
            {
              merged[merged_मोजणी] = assignment;
              जर (! hash_घाला (slot_table, &merged[merged_मोजणी]))
                xalloc_die ();
              ++merged_मोजणी;
            }
        }
    }

  merged[merged_मोजणी] = रिक्त;
  env->variable = merged;
  env->used = merged_मोजणी;
  env->allocated = max_मोजणी + 1;
  environ = merged;
  hash_free (slot_table);
  परत true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
स्थिर शून्य
set_raw_environment_from_file (संरचना env_vector *env,
                               अक्षर स्थिरांक *file)
{
  माप_प्रकार size = read_env_file (file);
  idx_t entry_मोजणी = argz_मोजणी (env0_from_buffer, size);

  env->allocated = entry_मोजणी + 1;
  env->variable = xnmalloc (env->allocated, आकार *env->variable);
  argz_extract (env0_from_buffer, size, env->variable);
  env->used = entry_मोजणी;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
स्थिर अक्षर स्थिरांक *
scan_varname (अक्षर स्थिरांक *str)
{
  जर (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      अक्षर स्थिरांक *end = str + 3;
      पर्यंत (c_isalnum (*end) || *end == '_')
        ++end;
      जर (*end == '}')
        परत end;
    }

  परत रिक्त;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
स्थिर अक्षर *
extract_varname (अक्षर स्थिरांक *str)
{
  अक्षर स्थिरांक *p = scan_varname (str);

  जर (!p)
    परत रिक्त;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  जर (i >= vnlen)
    {
      मुक्त (varname);
      varname = xpalloc (रिक्त, &vnlen, i + 1 - vnlen, -1, आकार *varname);
    }

  स_नकल (varname, str + 2, i);
  varname[i] = 0;

  परत varname;
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
  अक्षर **argv;
  पूर्णांक argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  पूर्णांक extra_argc;

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
                      MIN (पूर्णांक_उच्च, IDX_MAX), 2 * आकार *ss->argv);
  स_हटाओ (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
स्थिर शून्य
splitbuf_append_byte (संरचना splitbuf *ss, अक्षर c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  जर (ss->half_alloc * आकार *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((अक्षर *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (अक्षर *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
स्थिर शून्य
check_start_new_arg (संरचना splitbuf *ss)
{
  जर (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      पूर्णांक argc = ss->argc;
      जर (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
स्थिर अक्षर **
splitbuf_finishup (संरचना splitbuf *ss)
{
  पूर्णांक argc = ss->argc;
  अक्षर **argv = ss->argv;
  अक्षर *stringbase = (अक्षर *) (ss->argv + ss->half_alloc);
  साठी (पूर्णांक i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  परत argv;
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
स्थिर अक्षर **
build_argv (अक्षर स्थिरांक *str, पूर्णांक extra_argc, पूर्णांक *argc)
{
  bool dq = false, sq = false;
  संरचना splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * आकार *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = रिक्त;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  पर्यंत (*str)
    {
      अक्षर newc = *str; /* Default: add the next character.  */

      निवड (*str)
        {
        स्थिती '\'':
          जर (dq)
            थांबा;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          सुरूठेवा;

        स्थिती '"':
          जर (sq)
            थांबा;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          सुरूठेवा;

        स्थिती ' ': स्थिती '\t': स्थिती '\n': स्थिती '\v': स्थिती '\f': स्थिती '\r':
          /* Start a new argument if outside quotes.  */
          जर (sq || dq)
            थांबा;
          ss.sep = true;
          str += म_अखोज (str, C_ISSPACE_CHARS);
          सुरूठेवा;

        स्थिती '#':
          जर (!ss.sep)
            थांबा;
          जा eos; /* '#' as first char terminates the string.  */

        स्थिती '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          जर (sq && str[1] != '\\' && str[1] != '\'')
            थांबा;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          निवड (newc)
            {
            स्थिती '"': स्थिती '#': स्थिती '$': स्थिती '\'': स्थिती '\\':
              /* Pass escaped character as-is.  */
              थांबा;

            स्थिती '_':
              जर (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  सुरूठेवा;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              थांबा;

            स्थिती 'c':
              जर (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              जा eos; /* '\c' terminates the string.  */

            स्थिती 'f': newc = '\f'; थांबा;
            स्थिती 'n': newc = '\n'; थांबा;
            स्थिती 'r': newc = '\r'; थांबा;
            स्थिती 't': newc = '\t'; थांबा;
            स्थिती 'v': newc = '\v'; थांबा;

            स्थिती '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            पूर्वनिर्धारित:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          थांबा;

        स्थिती '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          जर (sq)
            थांबा;

          /* Store the ${VARNAME} value.  */
          {
            अक्षर *n = extract_varname (str);
            जर (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            अक्षर *v = दो_पर्या (n);
            जर (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                साठी (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            अन्यथा
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = म_अक्षर (str, '}') + 1;
            सुरूठेवा;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  जर (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  परत splitbuf_finishup (&ss);
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
parse_split_string (अक्षर स्थिरांक *str, पूर्णांक *orig_optind,
                    पूर्णांक *orig_argc, अक्षर ***orig_argv)
{
  पूर्णांक extra_argc = *orig_argc - *orig_optind, newargc;
  अक्षर **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  जर (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      साठी (पूर्णांक i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  स_नकल (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * आकार *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

स्थिर शून्य
parse_signal_action_params (अक्षर स्थिरांक *arg, bool set_default)
{
  जर (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      साठी (पूर्णांक i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      परत;
    }

  अक्षर *optarg_writable = xstrdup (arg);

  अक्षर *opt_sig = म_मोहर (optarg_writable, ",");
  पर्यंत (opt_sig)
    {
      पूर्णांक signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      जर (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      जर (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = म_मोहर (रिक्त, ",");
    }

  मुक्त (optarg_writable);
}

स्थिर शून्य
reset_signal_handlers (शून्य)
{
  साठी (पूर्णांक i = 1; i <= SIGNUM_BOUND; i++)
    {
      संरचना sigaction act;

      जर (signals[i] == UNCHANGED)
        सुरूठेवा;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      पूर्णांक sig_err = sigaction (i, रिक्त, &act);

      जर (sig_err && !ignore_errors)
        error (EXIT_CANCELED, त्रुटि_सं,
               _("failed to get signal action for signal %d"), i);

      जर (! sig_err)
        {
          act.sa_handler = set_to_default ? संक_पूर्व : संक_छोड़ो;
          sig_err = sigaction (i, &act, रिक्त);
          जर (sig_err && !ignore_errors)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to set signal action for signal %d"), i);
        }

      जर (dev_debug)
        {
          अक्षर signame[SIG2STR_MAX];
          जर (sig2str (i, signame) != 0)
            snprintf (signame, आकार signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


स्थिर शून्य
parse_block_signal_params (अक्षर स्थिरांक *arg, bool block)
{
  जर (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  अन्यथा जर (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  जर (! arg)
    परत;

  अक्षर *optarg_writable = xstrdup (arg);

  अक्षर *opt_sig = म_मोहर (optarg_writable, ",");
  पर्यंत (opt_sig)
    {
      पूर्णांक signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      जर (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      जर (signum <= 0)
        usage (exit_failure);

      जर (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          जर (block)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      अन्यथा
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = म_मोहर (रिक्त, ",");
    }

  मुक्त (optarg_writable);
}

स्थिर शून्य
set_signal_proc_mask (शून्य)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  जर (sigprocmask (0, रिक्त, &set))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to get signal process mask"));

  साठी (पूर्णांक i = 1; i <= SIGNUM_BOUND; i++)
    {
      अक्षर स्थिरांक *debug_act = रिक्त;

      जर (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      अन्यथा जर (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      जर (dev_debug && debug_act)
        {
          अक्षर signame[SIG2STR_MAX];
          जर (sig2str (i, signame) != 0)
            snprintf (signame, आकार signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  जर (sigprocmask (SIG_SETMASK, &set, रिक्त))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set signal process mask"));
}

स्थिर शून्य
list_signal_handling (शून्य)
{
  sigset_t set;

  sigemptyset (&set);
  जर (sigprocmask (0, रिक्त, &set))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to get signal process mask"));

  साठी (पूर्णांक i = 1; i <= SIGNUM_BOUND; i++)
    {
      संरचना sigaction act;
      जर (sigaction (i, रिक्त, &act))
        सुरूठेवा;

      अक्षर स्थिरांक *ignored = act.sa_handler == संक_छोड़ो ? "IGNORE" : "";
      अक्षर स्थिरांक *blocked = sigismember (&set, i) ? "BLOCK" : "";
      अक्षर स्थिरांक *connect = *ignored && *blocked ? "," : "";

      जर (! *ignored && ! *blocked)
        सुरूठेवा;

      अक्षर signame[SIG2STR_MAX];
      जर (sig2str (i, signame) != 0)
        snprintf (signame, आकार signame, "SIG%d", i);
      ख_लिखो (मानक_त्रुटि, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

स्थिर शून्य
initialize_signals (शून्य)
{
  signals = xmalloc ((आकार *signals) * (SIGNUM_BOUND + 1));

  साठी (पूर्णांक i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  पूर्णांक quoting_style_opt = -1;
  अक्षर स्थिरांक *newdir = रिक्त;
  अक्षर स्थिरांक *env0_from_file = रिक्त;
  अक्षर *argv0 = रिक्त;
  संरचना env_vector env_vector;
#ifdef lint
  स_रखो (&env_vector, 0, आकार env_vector);
  अक्षर **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  initialize_signals ();

  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      निवड (optc)
        {
        स्थिती 'a':
          argv0 = optarg;
          थांबा;
        स्थिती 'i':
          ignore_environment = true;
          थांबा;
        स्थिती 'u':
          append_unset_var (optarg);
          थांबा;
        स्थिती 'v':
          dev_debug = true;
          थांबा;
        स्थिती '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          थांबा;
        स्थिती ENV0_FROM_OPTION:
          env0_from_file = optarg;
          थांबा;
        स्थिती DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          थांबा;
        स्थिती IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          थांबा;
        स्थिती BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          थांबा;
        स्थिती LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          थांबा;
        स्थिती QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          थांबा;
        स्थिती 'C':
          newdir = optarg;
          थांबा;
        स्थिती 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          थांबा;
        स्थिती ' ': स्थिती '\t': स्थिती '\n': स्थिती '\v': स्थिती '\f': स्थिती '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            अक्षर c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (EXIT_CANCELED);
        }
    }

  जर (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  पूर्णांक program_index = optind;
  पर्यंत (program_index < argc && म_अक्षर (argv[program_index], '='))
    program_index++;
  bool program_specified = program_index < argc;

  bool quote_output = false;
  पूर्णांक qs = quoting_style_opt;
  जर (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      जर (qs < 0)
        qs = shell_escape_quoting_style;
    }
  जर (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (रिक्त, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  जर (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  अन्यथा जर (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#जर defined _WIN32 && ! defined __CYGWIN__
      स्थिर अक्षर *dummy_environ[] = { रिक्त };
      environ = dummy_environ;
#अन्यथा
      env_vector.variable = xnmalloc (1, आकार *env_vector.variable);
      env_vector.variable[0] = रिक्त;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  अन्यथा जर (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  जर (! ignore_environment || env0_from_file)
    {
      जर (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      अन्यथा
        unset_envvars ();
    }

  अक्षर *eq;
  पर्यंत (optind < argc && (eq = म_अक्षर (argv[optind], '=')))
    {
      जर (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      अन्यथा
        set_envvar (argv[optind], eq);
      optind++;
    }

  जर (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  जर (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  जर (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  जर (! program_specified)
    {
      अक्षर स्थिरांक terminator = opt_nul_terminate_output ? '\0' : '\n';

      जर (opt_nul_terminate_output)
        xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

      /* Print the environment and exit.  */
      साठी (अक्षर *स्थिरांक *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      जर (env_vector_active)
        {
          environ = initial_environ;
          मुक्त (env_vector.variable);
          मुक्त (env0_from_buffer);
        }
#endif
      परत निकास_सफल;
    }

  reset_signal_handlers ();
  जर (sig_mask_changed)
    set_signal_proc_mask ();

  जर (report_signal_handling)
    list_signal_handling ();

  जर (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      जर (स_बदलो (newdir) != 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  अक्षर *program = argv[optind];
  जर (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  जर (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      साठी (पूर्णांक i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, "%s", quote (program));

  जर (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
