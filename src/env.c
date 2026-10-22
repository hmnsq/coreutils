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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argz.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <c-ctype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hashcode-mem.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "operand2sig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "printenv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sig2str.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "env"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ **usvars;
ਸਥਿਰ idx_t usvars_alloc;
ਸਥਿਰ idx_t usvars_ਵਰਤਿਆ;

/* Storage for entries read by --env0-from.  */
ਸਥਿਰ ਅੱਖਰ *env0_from_ਬਫਰ;

/* An environment vector managed directly to preserve raw entries.  */
ਬਣਤਰ env_ਸਦਿਸ਼
{
  ਅੱਖਰ **variable;
  idx_t ਵਰਤਿਆ;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ਸਥਿਰ bool dev_debug;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ਸਥਿਰ ਅੱਖਰ *varname;
ਸਥਿਰ idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ IGNORE

/* Possible actions on each signal.  */
ਗਿਣਤੀ SIGNAL_ਬਹੁਲਕ {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ਸਥਿਰ ਗਿਣਤੀ SIGNAL_ਬਹੁਲਕ *ਸੰਕੇਤ_2;

/* Set of signals to block.  */
ਸਥਿਰ sigset_t ਖੰਡ_ਸੰਕੇਤ;

/* Set of signals to unblock.  */
ਸਥਿਰ sigset_t unblock_ਸੰਕੇਤ;

/* Whether signal mask adjustment requested.  */
ਸਥਿਰ bool sig_ਨਕਾਬ_ਬਦਲਿਆ;

/* Whether to list non default handling.  */
ਸਥਿਰ bool report_signal_handling;

/* The isspace characters in the C locale.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ C_ISSPACE_CHARS " \t\n\v\f\r"

ਸਥਿਰ ਅੱਖਰ ਅਚਲ shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  ਖੰਡ_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"argv0", required_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"ignore-environment", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"env0-from", required_ਦਲੀਲ, ਨਿਰਾ, ENV0_FROM_OPTION},
  {"null", no_ਦਲੀਲ, ਨਿਰਾ, '0'},
  {"unset", required_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"chdir", required_ਦਲੀਲ, ਨਿਰਾ, 'C'},
  {"default-signal", optional_ਦਲੀਲ, ਨਿਰਾ, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_ਦਲੀਲ, ਨਿਰਾ, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_ਦਲੀਲ, ਨਿਰਾ, ਖੰਡ_SIGNAL_OPTION},
  {"list-signal-handling", no_ਦਲੀਲ, ਨਿਰਾ,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"quoting-style", required_ਦਲੀਲ, ਨਿਰਾ, QUOTING_STYLE_OPTION},
  {"split-string", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [-] [NAME=ਮੁੱਲ_2]... [COMMAND [ARG]...]\n"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
ਸੈੱਟ_3 each NAME ਮੰਜ਼ਿਲ ਮੁੱਲ_2 ਵਿੱਚ the environment and ਲੜੀਵਾਰ_2 COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth ਦਲੀਲ ਦਾ COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         ਸ਼ੁਰੂ with an ਖਾਲੀ_ਥਾਵਾਂ environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         ਪਠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         remove variable from the environment\n\
"));
      oputs (_("\
  -C, --chdir=DIR\n\
         change working directory ਮੰਜ਼ਿਲ DIR\n\
"));
      oputs (_("\
      --quoting-style=ਸ਼ਬਦ\n\
         use quoting style ਸ਼ਬਦ ਲਈ ਨਾਂ and ਮੁੱਲਾਂ:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --ਵੰਡੋ-string=S\n\
         process and ਵੰਡੋ S into separate arguments;\n\
         ਵਰਤਿਆ ਮੰਜ਼ਿਲ pass ਗੁਣਜ arguments on shebang ਸਤਰਾਂ\n\
"));
      oputs (_("\
      --ਖੰਡ-ਸੰਕੇਤ[=SIG]\n\
         ਖੰਡ delivery ਦਾ SIG ਸੰਕੇਤ(s) ਮੰਜ਼ਿਲ COMMAND\n\
"));
      oputs (_("\
      --ਮੂਲ-ਸੰਕੇਤ[=SIG]\n\
         reset handling ਦਾ SIG ਸੰਕੇਤ(s) ਮੰਜ਼ਿਲ the ਮੂਲ\n\
"));
      oputs (_("\
      --ignore-ਸੰਕੇਤ[=SIG]\n\
         ਸੈੱਟ_2 handling ਦਾ SIG ਸੰਕੇਤ(s) ਮੰਜ਼ਿਲ ਕਰੋ nothing\n\
"));
      oputs (_("\
      --list-ਸੰਕੇਤ-handling\n\
         list non ਮੂਲ ਸੰਕੇਤ handling ਮੰਜ਼ਿਲ standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information ਲਈ each processing ਕਦਮ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - ਸਿੱਟਾ_ਦਿੰਦਾ -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a ਸੰਕੇਤ name like 'PIPE', or a ਸੰਕੇਤ ਸੰਖਿਆ like '13'.\n\
Without SIG, all ਗਿਆਤ ਸੰਕੇਤ_2 are included.  ਗੁਣਜ_2 ਸੰਕੇਤ_2 can be\n\
comma-separated.  An ਖਾਲੀ_ਥਾਵਾਂ SIG ਦਲੀਲ is a no-op.\n\
"), stdout);
      emit_exec_ਸਥਿਤੀ (ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ ਖਾਲੀ
append_unset_var (ਅੱਖਰ ਅਚਲ *var)
{
  ਜੇ (usvars_ਵਰਤਿਆ == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, ਆਕਾਰ *usvars);
  usvars[usvars_ਵਰਤਿਆ++] = var;
}

ਸਥਿਰ ਖਾਲੀ
unset_envvars (ਖਾਲੀ)
{
  ਲਈ (idx_t i = 0; i < usvars_ਵਰਤਿਆ; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      ਜੇ (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ਸਥਿਰ bool
ਇੰਦਰਾਜ_has_name (ਅੱਖਰ ਅਚਲ *ਇੰਦਰਾਜ, ਅੱਖਰ ਅਚਲ *name, idx_t name_ਲੰਮਾਈ)
{
  ਅੱਖਰ ਅਚਲ *eq = strchr (ਇੰਦਰਾਜ, '=');
  ਵਾਪਸ (eq && eq - ਇੰਦਰਾਜ == name_ਲੰਮਾਈ
          && memeq (ਇੰਦਰਾਜ, name, name_ਲੰਮਾਈ));
}

#ਜੇ ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ਸਥਿਰ size_t
env_ਸਦਿਸ਼_ਥਾਂ_hash (ਖਾਲੀ ਅਚਲ *x, size_t ਸਾਰਣੀ_ਮਾਪ)
{
  ਅੱਖਰ *ਅਚਲ *ਥਾਂ = x;
  ਅੱਖਰ ਅਚਲ *ਇੰਦਰਾਜ = *ਥਾਂ;
  ਅੱਖਰ ਅਚਲ *eq = strchr (ਇੰਦਰਾਜ, '=');
  ਵਾਪਸ hash_pjw_bare (ਇੰਦਰਾਜ, eq - ਇੰਦਰਾਜ) % ਸਾਰਣੀ_ਮਾਪ;
}

ਸਥਿਰ bool
env_ਸਦਿਸ਼_ਥਾਂ_compare (ਖਾਲੀ ਅਚਲ *x, ਖਾਲੀ ਅਚਲ *y)
{
  ਅੱਖਰ ਅਚਲ *a = * (ਅੱਖਰ *ਅਚਲ *) x;
  ਅੱਖਰ ਅਚਲ *b = * (ਅੱਖਰ *ਅਚਲ *) y;
  ਅੱਖਰ ਅਚਲ *b_eq = strchr (b, '=');
  ਵਾਪਸ ਇੰਦਰਾਜ_has_name (a, b, b_eq - b);
}
#ਜੇ_ਅੰਤ

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ਸਥਿਰ ਖਾਲੀ
unset_envvars_ਵਿੱਚ_ਸਦਿਸ਼ (ਬਣਤਰ env_ਸਦਿਸ਼ *env)
{
  ਲਈ (idx_t i = 0; i < usvars_ਵਰਤਿਆ; ++i)
    {
      ਅੱਖਰ ਅਚਲ *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      ਜੇ (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ (name);
      idx_t dest = 0;
      ਲਈ (idx_t ਸਰੋਤ_2 = 0; ਸਰੋਤ_2 < env->ਵਰਤਿਆ; ++ਸਰੋਤ_2)
        ਜੇ (! ਇੰਦਰਾਜ_has_name (env->variable[ਸਰੋਤ_2], name, name_ਲੰਮਾਈ))
          env->variable[dest++] = env->variable[ਸਰੋਤ_2];

      env->ਵਰਤਿਆ = dest;
      env->variable[dest] = ਨਿਰਾ;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_envvar (ਅੱਖਰ *assignment, ਅੱਖਰ *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  ਜੇ (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_envvar_ਵਿੱਚ_ਸਦਿਸ਼ (ਬਣਤਰ env_ਸਦਿਸ਼ *env, ਅੱਖਰ *assignment, ਅੱਖਰ *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_ਲੰਮਾਈ = eq - assignment;
  ਲਈ (idx_t i = 0; i < env->ਵਰਤਿਆ; ++i)
    ਜੇ (ਇੰਦਰਾਜ_has_name (env->variable[i], assignment, name_ਲੰਮਾਈ))
      {
        env->variable[i] = assignment;
        ਵਾਪਸ;
      }

  ਜੇ (env->ਵਰਤਿਆ + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               ਆਕਾਰ *env->variable);
      environ = env->variable;
    }

  env->variable[env->ਵਰਤਿਆ++] = assignment;
  env->variable[env->ਵਰਤਿਆ] = ਨਿਰਾ;
}

/* Read the NUL-delimited environment entries in FILE.  */
ਸਥਿਰ size_t
read_env_file (ਅੱਖਰ ਅਚਲ *file)
{
  size_t ਮਾਪ;
  ਜੇ (streq (file, "-"))
    {
      xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
      env0_from_ਬਫਰ = fread_file (stdin, RF_ਦੁਆਧਾਰੀ, &ਮਾਪ);
    }
  ਨਹੀਂਤਾਂ
    env0_from_ਬਫਰ = read_file (file, RF_ਦੁਆਧਾਰੀ, &ਮਾਪ);

  ਜੇ (! env0_from_ਬਫਰ)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  ਜੇ (ਮਾਪ && env0_from_ਬਫਰ[ਮਾਪ - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  ਵਾਪਸ ਮਾਪ;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ਸਥਿਰ bool
ਸੈੱਟ_envvars_from_file (ਬਣਤਰ env_ਸਦਿਸ਼ *env, ਅੱਖਰ ਅਚਲ *file)
{
  size_t ਮਾਪ = read_env_file (file);

#ਜੇ defined _WIN32 && ! defined __CYGWIN__
  (ਖਾਲੀ) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  ਅੱਖਰ *assignment = ਨਿਰਾ;
  ਜਦੋਂ ((assignment = argz_ਅਗਲਾ (env0_from_ਬਫਰ, ਮਾਪ, assignment)))
    {
      ਅੱਖਰ *eq = strchr (assignment, '=');
      ਜੇ (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      ਸੈੱਟ_envvar (assignment, eq);
    }
  ਵਾਪਸ false;
#ਨਹੀਂਤਾਂ
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_ਗਿਣਤੀ = argz_ਗਿਣਤੀ (env0_from_ਬਫਰ, ਮਾਪ);

  ਜੇ (file_ਗਿਣਤੀ == 0)
    ਵਾਪਸ false;

  idx_t inherited_ਗਿਣਤੀ = 0;
  ਜਦੋਂ (environ[inherited_ਗਿਣਤੀ])
    ++inherited_ਗਿਣਤੀ;

  ਜੇ (IDX_MAX - inherited_ਗਿਣਤੀ <= file_ਗਿਣਤੀ)
    xalloc_die ();
  idx_t max_ਗਿਣਤੀ = inherited_ਗਿਣਤੀ + file_ਗਿਣਤੀ;
  ਅੱਖਰ **merged = xnmalloc (max_ਗਿਣਤੀ + 1, ਆਕਾਰ *merged);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (merged, environ, inherited_ਗਿਣਤੀ * ਆਕਾਰ *merged);
  merged[inherited_ਗਿਣਤੀ] = ਨਿਰਾ;

  Hash_ਸਾਰਣੀ *ਥਾਂ_ਸਾਰਣੀ
    = hash_initialize (max_ਗਿਣਤੀ, ਨਿਰਾ, env_ਸਦਿਸ਼_ਥਾਂ_hash,
                       env_ਸਦਿਸ਼_ਥਾਂ_compare, ਨਿਰਾ);
  ਜੇ (! ਥਾਂ_ਸਾਰਣੀ)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  ਲਈ (idx_t i = 0; i < inherited_ਗਿਣਤੀ; ++i)
    {
      ਅੱਖਰ *eq = strchr (merged[i], '=');
      ਜੇ (! eq)
        ਜਾਰੀ;

      ਜੇ (! hash_ਪਾਓ (ਥਾਂ_ਸਾਰਣੀ, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_ਗਿਣਤੀ = inherited_ਗਿਣਤੀ;
  ਅੱਖਰ *assignment = ਨਿਰਾ;
  ਜਦੋਂ ((assignment = argz_ਅਗਲਾ (env0_from_ਬਫਰ, ਮਾਪ, assignment)))
    {
      ਅੱਖਰ *eq = strchr (assignment, '=');
      ਜੇ (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_ਗਿਣਤੀ++] = assignment;
        }
      ਨਹੀਂਤਾਂ
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          ਅੱਖਰ **ਥਾਂ = hash_lookup (ਥਾਂ_ਸਾਰਣੀ, &assignment);
          ਜੇ (ਥਾਂ)
            *ਥਾਂ = assignment;
          ਨਹੀਂਤਾਂ
            {
              merged[merged_ਗਿਣਤੀ] = assignment;
              ਜੇ (! hash_ਪਾਓ (ਥਾਂ_ਸਾਰਣੀ, &merged[merged_ਗਿਣਤੀ]))
                xalloc_die ();
              ++merged_ਗਿਣਤੀ;
            }
        }
    }

  merged[merged_ਗਿਣਤੀ] = ਨਿਰਾ;
  env->variable = merged;
  env->ਵਰਤਿਆ = merged_ਗਿਣਤੀ;
  env->allocated = max_ਗਿਣਤੀ + 1;
  environ = merged;
  hash_free (ਥਾਂ_ਸਾਰਣੀ);
  ਵਾਪਸ true;
#ਜੇ_ਅੰਤ
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_raw_environment_from_file (ਬਣਤਰ env_ਸਦਿਸ਼ *env,
                               ਅੱਖਰ ਅਚਲ *file)
{
  size_t ਮਾਪ = read_env_file (file);
  idx_t ਇੰਦਰਾਜ_ਗਿਣਤੀ = argz_ਗਿਣਤੀ (env0_from_ਬਫਰ, ਮਾਪ);

  env->allocated = ਇੰਦਰਾਜ_ਗਿਣਤੀ + 1;
  env->variable = xnmalloc (env->allocated, ਆਕਾਰ *env->variable);
  argz_ਕੱਢੋ (env0_from_ਬਫਰ, ਮਾਪ, env->variable);
  env->ਵਰਤਿਆ = ਇੰਦਰਾਜ_ਗਿਣਤੀ;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
ਜਾਂਚ_varname (ਅੱਖਰ ਅਚਲ *str)
{
  ਜੇ (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      ਅੱਖਰ ਅਚਲ *ਸਿਰਾ = str + 3;
      ਜਦੋਂ (c_isalnum (*ਸਿਰਾ) || *ਸਿਰਾ == '_')
        ++ਸਿਰਾ;
      ਜੇ (*ਸਿਰਾ == '}')
        ਵਾਪਸ ਸਿਰਾ;
    }

  ਵਾਪਸ ਨਿਰਾ;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ਸਥਿਰ ਅੱਖਰ *
ਕੱਢੋ_varname (ਅੱਖਰ ਅਚਲ *str)
{
  ਅੱਖਰ ਅਚਲ *p = ਜਾਂਚ_varname (str);

  ਜੇ (!p)
    ਵਾਪਸ ਨਿਰਾ;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  ਜੇ (i >= vnlen)
    {
      ਮੁਕਤ (varname);
      varname = xpalloc (ਨਿਰਾ, &vnlen, i + 1 - vnlen, -1, ਆਕਾਰ *varname);
    }

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (varname, str + 2, i);
  varname[i] = 0;

  ਵਾਪਸ varname;
}

/* Temporary buffer used by --split-string processing.  */
ਬਣਤਰ splitbuf
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
  ਅੱਖਰ **argv;
  ਪੂਰਨ argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  ਪੂਰਨ extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
ਸਥਿਰ ਖਾਲੀ
splitbuf_grow (ਬਣਤਰ splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_ਬਾਈਟਾਂ = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, IDX_MAX), 2 * ਆਕਾਰ *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_ਬਾਈਟਾਂ);
}

/* In SS, append C to the last string.  */
ਸਥਿਰ ਖਾਲੀ
splitbuf_append_byte (ਬਣਤਰ splitbuf *ss, ਅੱਖਰ c)
{
  idx_t string_ਬਾਈਟਾਂ = (intptr_t) ss->argv[ss->argc];
  ਜੇ (ss->half_alloc * ਆਕਾਰ *ss->argv <= string_ਬਾਈਟਾਂ)
    splitbuf_grow (ss);
  ((ਅੱਖਰ *) (ss->argv + ss->half_alloc))[string_ਬਾਈਟਾਂ] = c;
  ss->argv[ss->argc] = (ਅੱਖਰ *) (intptr_t) {string_ਬਾਈਟਾਂ + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ਸਥਿਰ ਖਾਲੀ
check_ਸ਼ੁਰੂ_new_arg (ਬਣਤਰ splitbuf *ss)
{
  ਜੇ (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      ਪੂਰਨ argc = ss->argc;
      ਜੇ (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
ਸਥਿਰ ਅੱਖਰ **
splitbuf_finishup (ਬਣਤਰ splitbuf *ss)
{
  ਪੂਰਨ argc = ss->argc;
  ਅੱਖਰ **argv = ss->argv;
  ਅੱਖਰ *stringbase = (ਅੱਖਰ *) (ss->argv + ss->half_alloc);
  ਲਈ (ਪੂਰਨ i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  ਵਾਪਸ argv;
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
ਸਥਿਰ ਅੱਖਰ **
build_argv (ਅੱਖਰ ਅਚਲ *str, ਪੂਰਨ extra_argc, ਪੂਰਨ *argc)
{
  bool dq = false, sq = false;
  ਬਣਤਰ splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * ਆਕਾਰ *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ਨਿਰਾ;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  ਜਦੋਂ (*str)
    {
      ਅੱਖਰ newc = *str; /* Default: add the next character.  */

      ਚੋਣ (*str)
        {
        ਹਾਲਤ '\'':
          ਜੇ (dq)
            ਰੋਕੋ;
          sq = !sq;
          check_ਸ਼ੁਰੂ_new_arg (&ss);
          ++str;
          ਜਾਰੀ;

        ਹਾਲਤ '"':
          ਜੇ (sq)
            ਰੋਕੋ;
          dq = !dq;
          check_ਸ਼ੁਰੂ_new_arg (&ss);
          ++str;
          ਜਾਰੀ;

        ਹਾਲਤ ' ': ਹਾਲਤ '\t': ਹਾਲਤ '\n': ਹਾਲਤ '\v': ਹਾਲਤ '\f': ਹਾਲਤ '\r':
          /* Start a new argument if outside quotes.  */
          ਜੇ (sq || dq)
            ਰੋਕੋ;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          ਜਾਰੀ;

        ਹਾਲਤ '#':
          ਜੇ (!ss.sep)
            ਰੋਕੋ;
          ਜਾਓ eos; /* '#' as first char terminates the string.  */

        ਹਾਲਤ '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          ਜੇ (sq && str[1] != '\\' && str[1] != '\'')
            ਰੋਕੋ;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          ਚੋਣ (newc)
            {
            ਹਾਲਤ '"': ਹਾਲਤ '#': ਹਾਲਤ '$': ਹਾਲਤ '\'': ਹਾਲਤ '\\':
              /* Pass escaped character as-is.  */
              ਰੋਕੋ;

            ਹਾਲਤ '_':
              ਜੇ (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  ਜਾਰੀ;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              ਰੋਕੋ;

            ਹਾਲਤ 'c':
              ਜੇ (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              ਜਾਓ eos; /* '\c' terminates the string.  */

            ਹਾਲਤ 'f': newc = '\f'; ਰੋਕੋ;
            ਹਾਲਤ 'n': newc = '\n'; ਰੋਕੋ;
            ਹਾਲਤ 'r': newc = '\r'; ਰੋਕੋ;
            ਹਾਲਤ 't': newc = '\t'; ਰੋਕੋ;
            ਹਾਲਤ 'v': newc = '\v'; ਰੋਕੋ;

            ਹਾਲਤ '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            ਮੂਲ:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          ਰੋਕੋ;

        ਹਾਲਤ '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          ਜੇ (sq)
            ਰੋਕੋ;

          /* Store the ${VARNAME} value.  */
          {
            ਅੱਖਰ *n = ਕੱਢੋ_varname (str);
            ਜੇ (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            ਅੱਖਰ *v = getenv (n);
            ਜੇ (v)
              {
                check_ਸ਼ੁਰੂ_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                ਲਈ (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            ਨਹੀਂਤਾਂ
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            ਜਾਰੀ;
          }
        }

      check_ਸ਼ੁਰੂ_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  ਜੇ (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  ਵਾਪਸ splitbuf_finishup (&ss);
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
ਸਥਿਰ ਖਾਲੀ
parse_ਵੰਡੋ_string (ਅੱਖਰ ਅਚਲ *str, ਪੂਰਨ *orig_optind,
                    ਪੂਰਨ *orig_argc, ਅੱਖਰ ***orig_argv)
{
  ਪੂਰਨ extra_argc = *orig_argc - *orig_optind, newargc;
  ਅੱਖਰ **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  ਜੇ (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      ਲਈ (ਪੂਰਨ i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * ਆਕਾਰ *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ਸਥਿਰ ਖਾਲੀ
parse_signal_action_params (ਅੱਖਰ ਅਚਲ *arg, bool ਸੈੱਟ_default)
{
  ਜੇ (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      ਲਈ (ਪੂਰਨ i = 1 ; i <= SIGNUM_BOUND; i++)
        ਸੰਕੇਤ_2[i] = ਸੈੱਟ_default ? DEFAULT_NOERR : IGNORE_NOERR;
      ਵਾਪਸ;
    }

  ਅੱਖਰ *optarg_writable = xstrdup (arg);

  ਅੱਖਰ *opt_sig = strtok (optarg_writable, ",");
  ਜਦੋਂ (opt_sig)
    {
      ਪੂਰਨ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ਜੇ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ਜੇ (signum <= 0)
        usage (exit_failure);

      ਸੰਕੇਤ_2[signum] = ਸੈੱਟ_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ਨਿਰਾ, ",");
    }

  ਮੁਕਤ (optarg_writable);
}

ਸਥਿਰ ਖਾਲੀ
reset_signal_handlers (ਖਾਲੀ)
{
  ਲਈ (ਪੂਰਨ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ਬਣਤਰ sigaction act;

      ਜੇ (ਸੰਕੇਤ_2[i] == UNCHANGED)
        ਜਾਰੀ;

      bool ignore_errors = (ਸੰਕੇਤ_2[i] == DEFAULT_NOERR
                            || ਸੰਕੇਤ_2[i] == IGNORE_NOERR);

      bool ਸੈੱਟ_ਮੰਜ਼ਿਲ_default = (ਸੰਕੇਤ_2[i] == DEFAULT
                             || ਸੰਕੇਤ_2[i] == DEFAULT_NOERR);

      ਪੂਰਨ sig_err = sigaction (i, ਨਿਰਾ, &act);

      ਜੇ (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      ਜੇ (! sig_err)
        {
          act.sa_handler = ਸੈੱਟ_ਮੰਜ਼ਿਲ_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ਨਿਰਾ);
          ਜੇ (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      ਜੇ (dev_debug)
        {
          ਅੱਖਰ signame[SIG2STR_MAX];
          ਜੇ (sig2str (i, signame) != 0)
            snprintf (signame, ਆਕਾਰ signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  ਸੈੱਟ_ਮੰਜ਼ਿਲ_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ਸਥਿਰ ਖਾਲੀ
parse_ਖੰਡ_signal_params (ਅੱਖਰ ਅਚਲ *arg, bool ਖੰਡ)
{
  ਜੇ (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (ਖੰਡ ? &ਖੰਡ_ਸੰਕੇਤ : &unblock_ਸੰਕੇਤ);
      sigemptyset (ਖੰਡ ? &unblock_ਸੰਕੇਤ : &ਖੰਡ_ਸੰਕੇਤ);
    }
  ਨਹੀਂਤਾਂ ਜੇ (! sig_ਨਕਾਬ_ਬਦਲਿਆ)
    {
      /* Initialize the sets.  */
      sigemptyset (&ਖੰਡ_ਸੰਕੇਤ);
      sigemptyset (&unblock_ਸੰਕੇਤ);
    }

  sig_ਨਕਾਬ_ਬਦਲਿਆ = true;

  ਜੇ (! arg)
    ਵਾਪਸ;

  ਅੱਖਰ *optarg_writable = xstrdup (arg);

  ਅੱਖਰ *opt_sig = strtok (optarg_writable, ",");
  ਜਦੋਂ (opt_sig)
    {
      ਪੂਰਨ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ਜੇ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ਜੇ (signum <= 0)
        usage (exit_failure);

      ਜੇ (sigaddset (ਖੰਡ ? &ਖੰਡ_ਸੰਕੇਤ : &unblock_ਸੰਕੇਤ, signum) == -1)
        {
          ਜੇ (ਖੰਡ)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      ਨਹੀਂਤਾਂ
        sigdelset (ਖੰਡ ? &unblock_ਸੰਕੇਤ : &ਖੰਡ_ਸੰਕੇਤ, signum);

      opt_sig = strtok (ਨਿਰਾ, ",");
    }

  ਮੁਕਤ (optarg_writable);
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_signal_proc_ਨਕਾਬ (ਖਾਲੀ)
{
  /* Get the existing signal mask */
  sigset_t ਸੈੱਟ_2;

  sigemptyset (&ਸੈੱਟ_2);

  ਜੇ (sigprocmask (0, ਨਿਰਾ, &ਸੈੱਟ_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ਲਈ (ਪੂਰਨ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ਅੱਖਰ ਅਚਲ *debug_act = ਨਿਰਾ;

      ਜੇ (sigismember (&ਖੰਡ_ਸੰਕੇਤ, i))
        {
          sigaddset (&ਸੈੱਟ_2, i);
          debug_act = "BLOCK";
        }
      ਨਹੀਂਤਾਂ ਜੇ (sigismember (&unblock_ਸੰਕੇਤ, i))
        {
          sigdelset (&ਸੈੱਟ_2, i);
          debug_act = "UNBLOCK";
        }

      ਜੇ (dev_debug && debug_act)
        {
          ਅੱਖਰ signame[SIG2STR_MAX];
          ਜੇ (sig2str (i, signame) != 0)
            snprintf (signame, ਆਕਾਰ signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  ਜੇ (sigprocmask (SIG_SETMASK, &ਸੈੱਟ_2, ਨਿਰਾ))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ਸਥਿਰ ਖਾਲੀ
list_signal_handling (ਖਾਲੀ)
{
  sigset_t ਸੈੱਟ_2;

  sigemptyset (&ਸੈੱਟ_2);
  ਜੇ (sigprocmask (0, ਨਿਰਾ, &ਸੈੱਟ_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ਲਈ (ਪੂਰਨ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ਬਣਤਰ sigaction act;
      ਜੇ (sigaction (i, ਨਿਰਾ, &act))
        ਜਾਰੀ;

      ਅੱਖਰ ਅਚਲ *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      ਅੱਖਰ ਅਚਲ *blocked = sigismember (&ਸੈੱਟ_2, i) ? "BLOCK" : "";
      ਅੱਖਰ ਅਚਲ *connect = *ignored && *blocked ? "," : "";

      ਜੇ (! *ignored && ! *blocked)
        ਜਾਰੀ;

      ਅੱਖਰ signame[SIG2STR_MAX];
      ਜੇ (sig2str (i, signame) != 0)
        snprintf (signame, ਆਕਾਰ signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ਸਥਿਰ ਖਾਲੀ
initialize_ਸੰਕੇਤ (ਖਾਲੀ)
{
  ਸੰਕੇਤ_2 = xmalloc ((ਆਕਾਰ *ਸੰਕੇਤ_2) * (SIGNUM_BOUND + 1));

  ਲਈ (ਪੂਰਨ i = 0 ; i <= SIGNUM_BOUND; i++)
    ਸੰਕੇਤ_2[i] = UNCHANGED;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  ਪੂਰਨ quoting_style_opt = -1;
  ਅੱਖਰ ਅਚਲ *newdir = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *env0_from_file = ਨਿਰਾ;
  ਅੱਖਰ *argv0 = ਨਿਰਾ;
  ਬਣਤਰ env_ਸਦਿਸ਼ env_ਸਦਿਸ਼;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ lint
  memset (&env_ਸਦਿਸ਼, 0, ਆਕਾਰ env_ਸਦਿਸ਼);
  ਅੱਖਰ **initial_environ = environ;
#ਜੇ_ਅੰਤ

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_ਸੰਕੇਤ ();

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, shortopts, longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'a':
          argv0 = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          ignore_environment = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'u':
          append_unset_var (optarg);
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          dev_debug = true;
          ਰੋਕੋ;
        ਹਾਲਤ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ਰੋਕੋ;
        ਹਾਲਤ ENV0_FROM_OPTION:
          env0_from_file = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_ਖੰਡ_signal_params (optarg, false);
          ਰੋਕੋ;
        ਹਾਲਤ IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          ਰੋਕੋ;
        ਹਾਲਤ ਖੰਡ_SIGNAL_OPTION:
          parse_ਖੰਡ_signal_params (optarg, true);
          ਰੋਕੋ;
        ਹਾਲਤ LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          ਰੋਕੋ;
        ਹਾਲਤ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_ਦਲੀਲਾਂ,
                                         quoting_style_vals);
          ਰੋਕੋ;
        ਹਾਲਤ 'C':
          newdir = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'S':
          parse_ਵੰਡੋ_string (optarg, &optind, &argc, &argv);
          ਰੋਕੋ;
        ਹਾਲਤ ' ': ਹਾਲਤ '\t': ਹਾਲਤ '\n': ਹਾਲਤ '\v': ਹਾਲਤ '\f': ਹਾਲਤ '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            ਅੱਖਰ c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_CANCELED);
        }
    }

  ਜੇ (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  ਪੂਰਨ ਪ੍ਰੋਗਰਾਮ_ਸੂਚਕਾਂਕ = optind;
  ਜਦੋਂ (ਪ੍ਰੋਗਰਾਮ_ਸੂਚਕਾਂਕ < argc && strchr (argv[ਪ੍ਰੋਗਰਾਮ_ਸੂਚਕਾਂਕ], '='))
    ਪ੍ਰੋਗਰਾਮ_ਸੂਚਕਾਂਕ++;
  bool ਪ੍ਰੋਗਰਾਮ_specified = ਪ੍ਰੋਗਰਾਮ_ਸੂਚਕਾਂਕ < argc;

  bool quote_output = false;
  ਪੂਰਨ qs = quoting_style_opt;
  ਜੇ (! ਪ੍ਰੋਗਰਾਮ_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      ਜੇ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ਜੇ (0 <= qs && qs != literal_quoting_style)
    {
      ਸੈੱਟ_quoting_style (ਨਿਰਾ, qs);
      quote_output = true;
    }

  bool env_ਸਦਿਸ਼_ਸਰਗਰਮ = false;
  ਜੇ (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      ਸੈੱਟ_raw_environment_from_file (&env_ਸਦਿਸ਼, env0_from_file);
      env_ਸਦਿਸ਼_ਸਰਗਰਮ = true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#ਜੇ defined _WIN32 && ! defined __CYGWIN__
      ਸਥਿਰ ਅੱਖਰ *dummy_environ[] = { ਨਿਰਾ };
      environ = dummy_environ;
#ਨਹੀਂਤਾਂ
      env_ਸਦਿਸ਼.variable = xnmalloc (1, ਆਕਾਰ *env_ਸਦਿਸ਼.variable);
      env_ਸਦਿਸ਼.variable[0] = ਨਿਰਾ;
      env_ਸਦਿਸ਼.ਵਰਤਿਆ = 0;
      env_ਸਦਿਸ਼.allocated = 1;
      environ = env_ਸਦਿਸ਼.variable;
      env_ਸਦਿਸ਼_ਸਰਗਰਮ = true;
#ਜੇ_ਅੰਤ
    }
  ਨਹੀਂਤਾਂ ਜੇ (env0_from_file)
    env_ਸਦਿਸ਼_ਸਰਗਰਮ = ਸੈੱਟ_envvars_from_file (&env_ਸਦਿਸ਼, env0_from_file);

  ਜੇ (! ignore_environment || env0_from_file)
    {
      ਜੇ (env_ਸਦਿਸ਼_ਸਰਗਰਮ)
        unset_envvars_ਵਿੱਚ_ਸਦਿਸ਼ (&env_ਸਦਿਸ਼);
      ਨਹੀਂਤਾਂ
        unset_envvars ();
    }

  ਅੱਖਰ *eq;
  ਜਦੋਂ (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      ਜੇ (env_ਸਦਿਸ਼_ਸਰਗਰਮ)
        ਸੈੱਟ_envvar_ਵਿੱਚ_ਸਦਿਸ਼ (&env_ਸਦਿਸ਼, argv[optind], eq);
      ਨਹੀਂਤਾਂ
        ਸੈੱਟ_envvar (argv[optind], eq);
      optind++;
    }

  ਜੇ (opt_nul_terminate_output && ਪ੍ਰੋਗਰਾਮ_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  ਜੇ (newdir && ! ਪ੍ਰੋਗਰਾਮ_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  ਜੇ (argv0 && ! ਪ੍ਰੋਗਰਾਮ_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  ਜੇ (! ਪ੍ਰੋਗਰਾਮ_specified)
    {
      ਅੱਖਰ ਅਚਲ terminator = opt_nul_terminate_output ? '\0' : '\n';

      ਜੇ (opt_nul_terminate_output)
        xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);

      /* Print the environment and exit.  */
      ਲਈ (ਅੱਖਰ *ਅਚਲ *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ lint
      ਜੇ (env_ਸਦਿਸ਼_ਸਰਗਰਮ)
        {
          environ = initial_environ;
          ਮੁਕਤ (env_ਸਦਿਸ਼.variable);
          ਮੁਕਤ (env0_from_ਬਫਰ);
        }
#ਜੇ_ਅੰਤ
      ਵਾਪਸ EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  ਜੇ (sig_ਨਕਾਬ_ਬਦਲਿਆ)
    ਸੈੱਟ_signal_proc_ਨਕਾਬ ();

  ਜੇ (report_signal_handling)
    list_signal_handling ();

  ਜੇ (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      ਜੇ (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  ਅੱਖਰ *ਪ੍ਰੋਗਰਾਮ = argv[optind];
  ਜੇ (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  ਜੇ (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (ਪ੍ਰੋਗਰਾਮ));
      ਲਈ (ਪੂਰਨ i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (ਪ੍ਰੋਗਰਾਮ, &argv[optind]);

  ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (ਪ੍ਰੋਗਰਾਮ));

  ਜੇ (exit_ਸਥਿਤੀ == EXIT_ENOENT && strpbrk (ਪ੍ਰੋਗਰਾਮ, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_ਸਥਿਤੀ);
}
