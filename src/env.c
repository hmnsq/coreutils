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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <argz.h>
#समावेश <getopt.h>
#समावेश <c-ctype.h>
#समावेश <signal.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "hash.h"
#समावेश "hashcode-mem.h"
#समावेश "system.h"
#समावेश "operand2sig.h"
#समावेश "printenv.h"
#समावेश "quote.h"
#समावेश "read-file.h"
#समावेश "sig2str.h"
#समावेश "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "env"

#घोषणा AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
अटल अक्षर स्थिर **usvars;
अटल idx_t usvars_alloc;
अटल idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
अटल अक्षर *env0_from_बफर;

/* An environment vector managed directly to preserve raw entries.  */
काष्ठा env_vector
{
  अक्षर **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
अटल bool dev_debug;
#घोषणा vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
अटल अक्षर *varname;
अटल idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#अघोषित IGNORE

/* Possible actions on each signal.  */
क्रमागत SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
अटल क्रमागत SIGNAL_MODE *signals;

/* Set of signals to block.  */
अटल sigset_t block_signals;

/* Set of signals to unblock.  */
अटल sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
अटल bool sig_mask_changed;

/* Whether to list non default handling.  */
अटल bool report_signal_handling;

/* The isspace characters in the C locale.  */
#घोषणा C_ISSPACE_CHARS " \t\n\v\f\r"

अटल अक्षर स्थिर shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  ENV0_FROM_OPTION = अक्षर_उच्च + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"argv0", required_argument, शून्य, 'a'},
  {"ignore-environment", no_argument, शून्य, 'i'},
  {"env0-from", required_argument, शून्य, ENV0_FROM_OPTION},
  {"null", no_argument, शून्य, '0'},
  {"unset", required_argument, शून्य, 'u'},
  {"chdir", required_argument, शून्य, 'C'},
  {"default-signal", optional_argument, शून्य, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, शून्य, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, शून्य, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, शून्य,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, शून्य, 'v'},
  {"quoting-style", required_argument, शून्य, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, शून्य, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
         पढ़ो NUL-delimited environment entries from खाता\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output पंक्ति with NUL, not newline\n\
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
         use quoting style WORD क्रम names and मूल्यावली:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang पंक्तियाँ\n\
"));
      oputs (_("\
      --block-संकेत[=SIG]\n\
         block delivery of SIG संकेत(s) to COMMAND\n\
"));
      oputs (_("\
      --शेष-संकेत[=SIG]\n\
         reset handling of SIG संकेत(s) to the शेष\n\
"));
      oputs (_("\
      --ignore-संकेत[=SIG]\n\
         set handling of SIG संकेत(s) to करो nothing\n\
"));
      oputs (_("\
      --list-संकेत-handling\n\
         list non शेष संकेत handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information क्रम each processing step\n\
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

अटल व्योम
append_unset_var (अक्षर स्थिर *var)
{
  अगर (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, माप *usvars);
  usvars[usvars_used++] = var;
}

अटल व्योम
unset_envvars (व्योम)
{
  क्रम (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      अगर (unsetenv (usvars[i]))
        error (EXIT_CANCELED, त्रुटि_सं, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
अटल bool
entry_has_name (अक्षर स्थिर *entry, अक्षर स्थिर *name, idx_t name_लंबाई)
{
  अक्षर स्थिर *eq = म_अक्षर (entry, '=');
  वापस (eq && eq - entry == name_लंबाई
          && memeq (entry, name, name_लंबाई));
}

#अगर ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
अटल माप_प्रकार
env_vector_slot_hash (व्योम स्थिर *x, माप_प्रकार table_आकार)
{
  अक्षर *स्थिर *slot = x;
  अक्षर स्थिर *entry = *slot;
  अक्षर स्थिर *eq = म_अक्षर (entry, '=');
  वापस hash_pjw_bare (entry, eq - entry) % table_आकार;
}

अटल bool
env_vector_slot_compare (व्योम स्थिर *x, व्योम स्थिर *y)
{
  अक्षर स्थिर *a = * (अक्षर *स्थिर *) x;
  अक्षर स्थिर *b = * (अक्षर *स्थिर *) y;
  अक्षर स्थिर *b_eq = म_अक्षर (b, '=');
  वापस entry_has_name (a, b, b_eq - b);
}
#पूर्ण_अगर

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
अटल व्योम
unset_envvars_in_vector (काष्ठा env_vector *env)
{
  क्रम (idx_t i = 0; i < usvars_used; ++i)
    {
      अक्षर स्थिर *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      अगर (! *name || म_अक्षर (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_लंबाई = म_माप (name);
      idx_t dest = 0;
      क्रम (idx_t source = 0; source < env->used; ++source)
        अगर (! entry_has_name (env->variable[source], name, name_लंबाई))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = शून्य;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
अटल व्योम
set_envvar (अक्षर *assignment, अक्षर *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  अगर (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, त्रुटि_सं, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
अटल व्योम
set_envvar_in_vector (काष्ठा env_vector *env, अक्षर *assignment, अक्षर *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_लंबाई = eq - assignment;
  क्रम (idx_t i = 0; i < env->used; ++i)
    अगर (entry_has_name (env->variable[i], assignment, name_लंबाई))
      {
        env->variable[i] = assignment;
        वापस;
      }

  अगर (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               माप *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = शून्य;
}

/* Read the NUL-delimited environment entries in FILE.  */
अटल माप_प्रकार
read_env_file (अक्षर स्थिर *file)
{
  माप_प्रकार आकार;
  अगर (streq (file, "-"))
    {
      xset_द्विआधारी_mode (STDIN_FILENO, O_द्विआधारी);
      env0_from_बफर = fread_file (मानक_निवेश, RF_द्विआधारी, &आकार);
    }
  अन्यथा
    env0_from_बफर = read_file (file, RF_द्विआधारी, &आकार);

  अगर (! env0_from_बफर)
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot read %s"), quoteaf (file));
  अगर (आकार && env0_from_बफर[आकार - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  वापस आकार;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
अटल bool
set_envvars_from_file (काष्ठा env_vector *env, अक्षर स्थिर *file)
{
  माप_प्रकार आकार = read_env_file (file);

#अगर defined _WIN32 && ! defined __CYGWIN__
  (व्योम) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  अक्षर *assignment = शून्य;
  जबतक ((assignment = argz_अग्र (env0_from_बफर, आकार, assignment)))
    {
      अक्षर *eq = म_अक्षर (assignment, '=');
      अगर (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  वापस false;
#अन्यथा
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_गिनती = argz_गिनती (env0_from_बफर, आकार);

  अगर (file_गिनती == 0)
    वापस false;

  idx_t inherited_गिनती = 0;
  जबतक (environ[inherited_गिनती])
    ++inherited_गिनती;

  अगर (IDX_MAX - inherited_गिनती <= file_गिनती)
    xalloc_die ();
  idx_t max_गिनती = inherited_गिनती + file_गिनती;
  अक्षर **merged = xnmalloc (max_गिनती + 1, माप *merged);
  स_नकल (merged, environ, inherited_गिनती * माप *merged);
  merged[inherited_गिनती] = शून्य;

  Hash_table *slot_table
    = hash_initialize (max_गिनती, शून्य, env_vector_slot_hash,
                       env_vector_slot_compare, शून्य);
  अगर (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  क्रम (idx_t i = 0; i < inherited_गिनती; ++i)
    {
      अक्षर *eq = म_अक्षर (merged[i], '=');
      अगर (! eq)
        जारी;

      अगर (! hash_जोड़ो (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_गिनती = inherited_गिनती;
  अक्षर *assignment = शून्य;
  जबतक ((assignment = argz_अग्र (env0_from_बफर, आकार, assignment)))
    {
      अक्षर *eq = म_अक्षर (assignment, '=');
      अगर (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_गिनती++] = assignment;
        }
      अन्यथा
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          अक्षर **slot = hash_lookup (slot_table, &assignment);
          अगर (slot)
            *slot = assignment;
          अन्यथा
            {
              merged[merged_गिनती] = assignment;
              अगर (! hash_जोड़ो (slot_table, &merged[merged_गिनती]))
                xalloc_die ();
              ++merged_गिनती;
            }
        }
    }

  merged[merged_गिनती] = शून्य;
  env->variable = merged;
  env->used = merged_गिनती;
  env->allocated = max_गिनती + 1;
  environ = merged;
  hash_free (slot_table);
  वापस true;
#पूर्ण_अगर
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
अटल व्योम
set_raw_environment_from_file (काष्ठा env_vector *env,
                               अक्षर स्थिर *file)
{
  माप_प्रकार आकार = read_env_file (file);
  idx_t entry_गिनती = argz_गिनती (env0_from_बफर, आकार);

  env->allocated = entry_गिनती + 1;
  env->variable = xnmalloc (env->allocated, माप *env->variable);
  argz_extract (env0_from_बफर, आकार, env->variable);
  env->used = entry_गिनती;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
अटल अक्षर स्थिर *
scan_varname (अक्षर स्थिर *str)
{
  अगर (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      अक्षर स्थिर *end = str + 3;
      जबतक (c_isalnum (*end) || *end == '_')
        ++end;
      अगर (*end == '}')
        वापस end;
    }

  वापस शून्य;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
अटल अक्षर *
extract_varname (अक्षर स्थिर *str)
{
  अक्षर स्थिर *p = scan_varname (str);

  अगर (!p)
    वापस शून्य;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  अगर (i >= vnlen)
    {
      मुक्त (varname);
      varname = xpalloc (शून्य, &vnlen, i + 1 - vnlen, -1, माप *varname);
    }

  स_नकल (varname, str + 2, i);
  varname[i] = 0;

  वापस varname;
}

/* Temporary buffer used by --split-string processing.  */
काष्ठा splitbuf
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
अटल व्योम
splitbuf_grow (काष्ठा splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (पूर्णांक_उच्च, IDX_MAX), 2 * माप *ss->argv);
  स_हटाओ (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
अटल व्योम
splitbuf_append_byte (काष्ठा splitbuf *ss, अक्षर c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  अगर (ss->half_alloc * माप *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((अक्षर *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (अक्षर *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
अटल व्योम
check_start_new_arg (काष्ठा splitbuf *ss)
{
  अगर (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      पूर्णांक argc = ss->argc;
      अगर (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
अटल अक्षर **
splitbuf_finishup (काष्ठा splitbuf *ss)
{
  पूर्णांक argc = ss->argc;
  अक्षर **argv = ss->argv;
  अक्षर *stringbase = (अक्षर *) (ss->argv + ss->half_alloc);
  क्रम (पूर्णांक i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  वापस argv;
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
अटल अक्षर **
build_argv (अक्षर स्थिर *str, पूर्णांक extra_argc, पूर्णांक *argc)
{
  bool dq = false, sq = false;
  काष्ठा splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * माप *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = शून्य;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  जबतक (*str)
    {
      अक्षर newc = *str; /* Default: add the next character.  */

      चयन (*str)
        {
        हाल '\'':
          अगर (dq)
            अवरोध;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          जारी;

        हाल '"':
          अगर (sq)
            अवरोध;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          जारी;

        हाल ' ': हाल '\t': हाल '\n': हाल '\v': हाल '\f': हाल '\r':
          /* Start a new argument if outside quotes.  */
          अगर (sq || dq)
            अवरोध;
          ss.sep = true;
          str += म_अखोज (str, C_ISSPACE_CHARS);
          जारी;

        हाल '#':
          अगर (!ss.sep)
            अवरोध;
          जाओ eos; /* '#' as first char terminates the string.  */

        हाल '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          अगर (sq && str[1] != '\\' && str[1] != '\'')
            अवरोध;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          चयन (newc)
            {
            हाल '"': हाल '#': हाल '$': हाल '\'': हाल '\\':
              /* Pass escaped character as-is.  */
              अवरोध;

            हाल '_':
              अगर (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  जारी;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              अवरोध;

            हाल 'c':
              अगर (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              जाओ eos; /* '\c' terminates the string.  */

            हाल 'f': newc = '\f'; अवरोध;
            हाल 'n': newc = '\n'; अवरोध;
            हाल 'r': newc = '\r'; अवरोध;
            हाल 't': newc = '\t'; अवरोध;
            हाल 'v': newc = '\v'; अवरोध;

            हाल '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            शेष:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          अवरोध;

        हाल '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          अगर (sq)
            अवरोध;

          /* Store the ${VARNAME} value.  */
          {
            अक्षर *n = extract_varname (str);
            अगर (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            अक्षर *v = दो_पर्या (n);
            अगर (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                क्रम (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            अन्यथा
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = म_अक्षर (str, '}') + 1;
            जारी;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  अगर (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  वापस splitbuf_finishup (&ss);
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
अटल व्योम
parse_split_string (अक्षर स्थिर *str, पूर्णांक *orig_optind,
                    पूर्णांक *orig_argc, अक्षर ***orig_argv)
{
  पूर्णांक extra_argc = *orig_argc - *orig_optind, newargc;
  अक्षर **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  अगर (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      क्रम (पूर्णांक i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  स_नकल (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * माप *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

अटल व्योम
parse_signal_action_params (अक्षर स्थिर *arg, bool set_default)
{
  अगर (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      क्रम (पूर्णांक i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      वापस;
    }

  अक्षर *optarg_writable = xstrdup (arg);

  अक्षर *opt_sig = म_मोहर (optarg_writable, ",");
  जबतक (opt_sig)
    {
      पूर्णांक signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      अगर (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      अगर (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = म_मोहर (शून्य, ",");
    }

  मुक्त (optarg_writable);
}

अटल व्योम
reset_signal_handlers (व्योम)
{
  क्रम (पूर्णांक i = 1; i <= SIGNUM_BOUND; i++)
    {
      काष्ठा sigaction act;

      अगर (signals[i] == UNCHANGED)
        जारी;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      पूर्णांक sig_err = sigaction (i, शून्य, &act);

      अगर (sig_err && !ignore_errors)
        error (EXIT_CANCELED, त्रुटि_सं,
               _("failed to get signal action for signal %d"), i);

      अगर (! sig_err)
        {
          act.sa_handler = set_to_default ? संक_पूर्व : संक_छोड़ो;
          sig_err = sigaction (i, &act, शून्य);
          अगर (sig_err && !ignore_errors)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to set signal action for signal %d"), i);
        }

      अगर (dev_debug)
        {
          अक्षर signame[SIG2STR_MAX];
          अगर (sig2str (i, signame) != 0)
            snprintf (signame, माप signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


अटल व्योम
parse_block_signal_params (अक्षर स्थिर *arg, bool block)
{
  अगर (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  अन्यथा अगर (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  अगर (! arg)
    वापस;

  अक्षर *optarg_writable = xstrdup (arg);

  अक्षर *opt_sig = म_मोहर (optarg_writable, ",");
  जबतक (opt_sig)
    {
      पूर्णांक signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      अगर (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      अगर (signum <= 0)
        usage (exit_failure);

      अगर (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          अगर (block)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      अन्यथा
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = म_मोहर (शून्य, ",");
    }

  मुक्त (optarg_writable);
}

अटल व्योम
set_signal_proc_mask (व्योम)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  अगर (sigprocmask (0, शून्य, &set))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to get signal process mask"));

  क्रम (पूर्णांक i = 1; i <= SIGNUM_BOUND; i++)
    {
      अक्षर स्थिर *debug_act = शून्य;

      अगर (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      अन्यथा अगर (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      अगर (dev_debug && debug_act)
        {
          अक्षर signame[SIG2STR_MAX];
          अगर (sig2str (i, signame) != 0)
            snprintf (signame, माप signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  अगर (sigprocmask (SIG_SETMASK, &set, शून्य))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set signal process mask"));
}

अटल व्योम
list_signal_handling (व्योम)
{
  sigset_t set;

  sigemptyset (&set);
  अगर (sigprocmask (0, शून्य, &set))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to get signal process mask"));

  क्रम (पूर्णांक i = 1; i <= SIGNUM_BOUND; i++)
    {
      काष्ठा sigaction act;
      अगर (sigaction (i, शून्य, &act))
        जारी;

      अक्षर स्थिर *ignored = act.sa_handler == संक_छोड़ो ? "IGNORE" : "";
      अक्षर स्थिर *blocked = sigismember (&set, i) ? "BLOCK" : "";
      अक्षर स्थिर *connect = *ignored && *blocked ? "," : "";

      अगर (! *ignored && ! *blocked)
        जारी;

      अक्षर signame[SIG2STR_MAX];
      अगर (sig2str (i, signame) != 0)
        snprintf (signame, माप signame, "SIG%d", i);
      ख_लिखो (मानक_त्रुटि, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

अटल व्योम
initialize_signals (व्योम)
{
  signals = xmalloc ((माप *signals) * (SIGNUM_BOUND + 1));

  क्रम (पूर्णांक i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  पूर्णांक quoting_style_opt = -1;
  अक्षर स्थिर *newdir = शून्य;
  अक्षर स्थिर *env0_from_file = शून्य;
  अक्षर *argv0 = शून्य;
  काष्ठा env_vector env_vector;
#अगर_घोषित lint
  स_रखो (&env_vector, 0, माप env_vector);
  अक्षर **initial_environ = environ;
#पूर्ण_अगर

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  initialize_signals ();

  पूर्णांक optc;
  जबतक ((optc = getopt_long (argc, argv, shortopts, longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'a':
          argv0 = optarg;
          अवरोध;
        हाल 'i':
          ignore_environment = true;
          अवरोध;
        हाल 'u':
          append_unset_var (optarg);
          अवरोध;
        हाल 'v':
          dev_debug = true;
          अवरोध;
        हाल '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          अवरोध;
        हाल ENV0_FROM_OPTION:
          env0_from_file = optarg;
          अवरोध;
        हाल DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          अवरोध;
        हाल IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          अवरोध;
        हाल BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          अवरोध;
        हाल LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          अवरोध;
        हाल QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          अवरोध;
        हाल 'C':
          newdir = optarg;
          अवरोध;
        हाल 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          अवरोध;
        हाल ' ': हाल '\t': हाल '\n': हाल '\v': हाल '\f': हाल '\r':
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
        शेष:
          usage (EXIT_CANCELED);
        }
    }

  अगर (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  पूर्णांक program_सूचकांक = optind;
  जबतक (program_सूचकांक < argc && म_अक्षर (argv[program_सूचकांक], '='))
    program_सूचकांक++;
  bool program_specified = program_सूचकांक < argc;

  bool quote_output = false;
  पूर्णांक qs = quoting_style_opt;
  अगर (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      अगर (qs < 0)
        qs = shell_escape_quoting_style;
    }
  अगर (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (शून्य, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  अगर (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  अन्यथा अगर (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#अगर defined _WIN32 && ! defined __CYGWIN__
      अटल अक्षर *dummy_environ[] = { शून्य };
      environ = dummy_environ;
#अन्यथा
      env_vector.variable = xnmalloc (1, माप *env_vector.variable);
      env_vector.variable[0] = शून्य;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#पूर्ण_अगर
    }
  अन्यथा अगर (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  अगर (! ignore_environment || env0_from_file)
    {
      अगर (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      अन्यथा
        unset_envvars ();
    }

  अक्षर *eq;
  जबतक (optind < argc && (eq = म_अक्षर (argv[optind], '=')))
    {
      अगर (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      अन्यथा
        set_envvar (argv[optind], eq);
      optind++;
    }

  अगर (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  अगर (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  अगर (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  अगर (! program_specified)
    {
      अक्षर स्थिर terminator = opt_nul_terminate_output ? '\0' : '\n';

      अगर (opt_nul_terminate_output)
        xset_द्विआधारी_mode (STDOUT_FILENO, O_द्विआधारी);

      /* Print the environment and exit.  */
      क्रम (अक्षर *स्थिर *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#अगर_घोषित lint
      अगर (env_vector_active)
        {
          environ = initial_environ;
          मुक्त (env_vector.variable);
          मुक्त (env0_from_बफर);
        }
#पूर्ण_अगर
      वापस निकास_सफल;
    }

  reset_signal_handlers ();
  अगर (sig_mask_changed)
    set_signal_proc_mask ();

  अगर (report_signal_handling)
    list_signal_handling ();

  अगर (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      अगर (स_बदलो (newdir) != 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  अक्षर *program = argv[optind];
  अगर (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  अगर (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      क्रम (पूर्णांक i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, "%s", quote (program));

  अगर (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
