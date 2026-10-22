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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "env"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **usvars;
ಸ್ಥಿರ idx_t usvars_alloc;
ಸ್ಥಿರ idx_t usvars_ಬಳಸಿದ;

/* Storage for entries read by --env0-from.  */
ಸ್ಥಿರ ಅಕ್ಷರ *env0_from_ಬಫರ್;

/* An environment vector managed directly to preserve raw entries.  */
ರಚನೆ env_ಸದಿಶ
{
  ಅಕ್ಷರ **variable;
  idx_t ಬಳಸಿದ;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
ಸ್ಥಿರ bool dev_debug;
#ವ್ಯಾಖ್ಯಾನಿಸು vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
ಸ್ಥಿರ ಅಕ್ಷರ *varname;
ಸ್ಥಿರ idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು IGNORE

/* Possible actions on each signal.  */
ಎಣಿಕೆ SIGNAL_ಬಹುಲಕ {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
ಸ್ಥಿರ ಎಣಿಕೆ SIGNAL_ಬಹುಲಕ *ಸಂಕೇತಗಳು;

/* Set of signals to block.  */
ಸ್ಥಿರ sigset_t ಖಂಡ_ಸಂಕೇತಗಳು;

/* Set of signals to unblock.  */
ಸ್ಥಿರ sigset_t unblock_ಸಂಕೇತಗಳು;

/* Whether signal mask adjustment requested.  */
ಸ್ಥಿರ bool sig_ಮುಖವಾಡ_ಬದಲಾಯಿತು;

/* Whether to list non default handling.  */
ಸ್ಥಿರ bool report_signal_handling;

/* The isspace characters in the C locale.  */
#ವ್ಯಾಖ್ಯಾನಿಸು C_ISSPACE_CHARS " \t\n\v\f\r"

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  ಖಂಡ_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"argv0", required_ವಾದ, ಖಾಲಿ, 'a'},
  {"ignore-environment", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"env0-from", required_ವಾದ, ಖಾಲಿ, ENV0_FROM_OPTION},
  {"null", no_ವಾದ, ಖಾಲಿ, '0'},
  {"unset", required_ವಾದ, ಖಾಲಿ, 'u'},
  {"chdir", required_ವಾದ, ಖಾಲಿ, 'C'},
  {"default-signal", optional_ವಾದ, ಖಾಲಿ, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_ವಾದ, ಖಾಲಿ, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_ವಾದ, ಖಾಲಿ, ಖಂಡ_SIGNAL_OPTION},
  {"list-signal-handling", no_ವಾದ, ಖಾಲಿ,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"quoting-style", required_ವಾದ, ಖಾಲಿ, QUOTING_STYLE_OPTION},
  {"split-string", required_ವಾದ, ಖಾಲಿ, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [-] [NAME=ಮೌಲ್ಯ_2]... [COMMAND [ARG]...]\n"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
ಗಣ_3 each NAME ಗಮ್ಯ ಮೌಲ್ಯ_2 ಒಳ the environment and ಸತತ_2 COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth ವಾದ ದ COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         ಆರಂಭ with an ಖಾಲಿಗಳು environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         ಪಠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         remove variable from the environment\n\
"));
      oputs (_("\
  -C, --chdir=DIR\n\
         change working directory ಗಮ್ಯ DIR\n\
"));
      oputs (_("\
      --quoting-style=ಶಬ್ದ\n\
         use quoting style ಶಬ್ದ ಗಾಗಿ ಹೆಸರುಗಳು and ಮೌಲ್ಯಗಳು:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --ವಿಭಜಿಸು-string=S\n\
         process and ವಿಭಜಿಸು S into separate arguments;\n\
         ಬಳಸಿದ ಗಮ್ಯ pass ಅಪವರ್ತ್ಯ arguments on shebang ಸಾಲುಗಳು\n\
"));
      oputs (_("\
      --ಖಂಡ-ಸಂಕೇತ[=SIG]\n\
         ಖಂಡ delivery ದ SIG ಸಂಕೇತ(s) ಗಮ್ಯ COMMAND\n\
"));
      oputs (_("\
      --ಪೂರ್ವನಿಯೋಜಿತ-ಸಂಕೇತ[=SIG]\n\
         reset handling ದ SIG ಸಂಕೇತ(s) ಗಮ್ಯ the ಪೂರ್ವನಿಯೋಜಿತ\n\
"));
      oputs (_("\
      --ignore-ಸಂಕೇತ[=SIG]\n\
         ಗಣ_2 handling ದ SIG ಸಂಕೇತ(s) ಗಮ್ಯ ಮಾಡು nothing\n\
"));
      oputs (_("\
      --list-ಸಂಕೇತ-handling\n\
         list non ಪೂರ್ವನಿಯೋಜಿತ ಸಂಕೇತ handling ಗಮ್ಯ standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information ಗಾಗಿ each processing ಹೆಜ್ಜೆ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - ಸೂಚಿಸುತ್ತದೆ -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a ಸಂಕೇತ name like 'PIPE', or a ಸಂಕೇತ ಸಂಖ್ಯೆ like '13'.\n\
Without SIG, all ತಿಳಿದ ಸಂಕೇತಗಳು are included.  ಅಪವರ್ತ್ಯ_2 ಸಂಕೇತಗಳು can be\n\
comma-separated.  An ಖಾಲಿಗಳು SIG ವಾದ is a no-op.\n\
"), stdout);
      emit_exec_ಸ್ಥಿತಿ (ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_unset_var (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *var)
{
  ಆದರೆ (usvars_ಬಳಸಿದ == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, ಗಾತ್ರ *usvars);
  usvars[usvars_ಬಳಸಿದ++] = var;
}

ಸ್ಥಿರ ಶೂನ್ಯ
unset_envvars (ಶೂನ್ಯ)
{
  ಗಾಗಿ (idx_t i = 0; i < usvars_ಬಳಸಿದ; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      ಆದರೆ (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
ಸ್ಥಿರ bool
ನಮೂದು_has_name (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂದು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, idx_t name_ಉದ್ದಳತೆ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *eq = strchr (ನಮೂದು, '=');
  ಹಿಂತಿರುಗಿ (eq && eq - ನಮೂದು == name_ಉದ್ದಳತೆ
          && memeq (ನಮೂದು, name, name_ಉದ್ದಳತೆ));
}

#ಆದರೆ ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
ಸ್ಥಿರ size_t
env_ಸದಿಶ_ಸ್ಥಾನ_hash (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, size_t ಕೋಷ್ಟಕ_ಗಾತ್ರ)
{
  ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *ಸ್ಥಾನ = x;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ನಮೂದು = *ಸ್ಥಾನ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *eq = strchr (ನಮೂದು, '=');
  ಹಿಂತಿರುಗಿ hash_pjw_bare (ನಮೂದು, eq - ನಮೂದು) % ಕೋಷ್ಟಕ_ಗಾತ್ರ;
}

ಸ್ಥಿರ bool
env_ಸದಿಶ_ಸ್ಥಾನ_compare (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *y)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a = * (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) x;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b = * (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) y;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b_eq = strchr (b, '=');
  ಹಿಂತಿರುಗಿ ನಮೂದು_has_name (a, b, b_eq - b);
}
#ಆದರೆ_ಅಂತ್ಯ

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
ಸ್ಥಿರ ಶೂನ್ಯ
unset_envvars_ಒಳ_ಸದಿಶ (ರಚನೆ env_ಸದಿಶ *env)
{
  ಗಾಗಿ (idx_t i = 0; i < usvars_ಬಳಸಿದ; ++i)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      ಆದರೆ (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);
      idx_t dest = 0;
      ಗಾಗಿ (idx_t ಮೂಲ_2 = 0; ಮೂಲ_2 < env->ಬಳಸಿದ; ++ಮೂಲ_2)
        ಆದರೆ (! ನಮೂದು_has_name (env->variable[ಮೂಲ_2], name, name_ಉದ್ದಳತೆ))
          env->variable[dest++] = env->variable[ಮೂಲ_2];

      env->ಬಳಸಿದ = dest;
      env->variable[dest] = ಖಾಲಿ;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_envvar (ಅಕ್ಷರ *assignment, ಅಕ್ಷರ *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  ಆದರೆ (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_envvar_ಒಳ_ಸದಿಶ (ರಚನೆ env_ಸದಿಶ *env, ಅಕ್ಷರ *assignment, ಅಕ್ಷರ *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_ಉದ್ದಳತೆ = eq - assignment;
  ಗಾಗಿ (idx_t i = 0; i < env->ಬಳಸಿದ; ++i)
    ಆದರೆ (ನಮೂದು_has_name (env->variable[i], assignment, name_ಉದ್ದಳತೆ))
      {
        env->variable[i] = assignment;
        ಹಿಂತಿರುಗಿ;
      }

  ಆದರೆ (env->ಬಳಸಿದ + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               ಗಾತ್ರ *env->variable);
      environ = env->variable;
    }

  env->variable[env->ಬಳಸಿದ++] = assignment;
  env->variable[env->ಬಳಸಿದ] = ಖಾಲಿ;
}

/* Read the NUL-delimited environment entries in FILE.  */
ಸ್ಥಿರ size_t
read_env_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  size_t ಗಾತ್ರ_2;
  ಆದರೆ (streq (file, "-"))
    {
      xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDIN_FILENO, O_ದ್ವಿಮಾನ);
      env0_from_ಬಫರ್ = fread_file (stdin, RF_ದ್ವಿಮಾನ, &ಗಾತ್ರ_2);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    env0_from_ಬಫರ್ = read_file (file, RF_ದ್ವಿಮಾನ, &ಗಾತ್ರ_2);

  ಆದರೆ (! env0_from_ಬಫರ್)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  ಆದರೆ (ಗಾತ್ರ_2 && env0_from_ಬಫರ್[ಗಾತ್ರ_2 - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
ಸ್ಥಿರ bool
ಗಣ_envvars_from_file (ರಚನೆ env_ಸದಿಶ *env, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  size_t ಗಾತ್ರ_2 = read_env_file (file);

#ಆದರೆ defined _WIN32 && ! defined __CYGWIN__
  (ಶೂನ್ಯ) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  ಅಕ್ಷರ *assignment = ಖಾಲಿ;
  ಆಗ ((assignment = argz_ಮುಂದಿನ (env0_from_ಬಫರ್, ಗಾತ್ರ_2, assignment)))
    {
      ಅಕ್ಷರ *eq = strchr (assignment, '=');
      ಆದರೆ (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      ಗಣ_envvar (assignment, eq);
    }
  ಹಿಂತಿರುಗಿ false;
#ಇಲ್ಲದಿದ್ದರೆ
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_ಎಣಿಕೆ = argz_ಎಣಿಕೆ (env0_from_ಬಫರ್, ಗಾತ್ರ_2);

  ಆದರೆ (file_ಎಣಿಕೆ == 0)
    ಹಿಂತಿರುಗಿ false;

  idx_t inherited_ಎಣಿಕೆ = 0;
  ಆಗ (environ[inherited_ಎಣಿಕೆ])
    ++inherited_ಎಣಿಕೆ;

  ಆದರೆ (IDX_MAX - inherited_ಎಣಿಕೆ <= file_ಎಣಿಕೆ)
    xalloc_die ();
  idx_t max_ಎಣಿಕೆ = inherited_ಎಣಿಕೆ + file_ಎಣಿಕೆ;
  ಅಕ್ಷರ **merged = xnmalloc (max_ಎಣಿಕೆ + 1, ಗಾತ್ರ *merged);
  ಸ್ಮರಣೆ_ನಕಲು (merged, environ, inherited_ಎಣಿಕೆ * ಗಾತ್ರ *merged);
  merged[inherited_ಎಣಿಕೆ] = ಖಾಲಿ;

  Hash_ಕೋಷ್ಟಕ *ಸ್ಥಾನ_ಕೋಷ್ಟಕ
    = hash_initialize (max_ಎಣಿಕೆ, ಖಾಲಿ, env_ಸದಿಶ_ಸ್ಥಾನ_hash,
                       env_ಸದಿಶ_ಸ್ಥಾನ_compare, ಖಾಲಿ);
  ಆದರೆ (! ಸ್ಥಾನ_ಕೋಷ್ಟಕ)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  ಗಾಗಿ (idx_t i = 0; i < inherited_ಎಣಿಕೆ; ++i)
    {
      ಅಕ್ಷರ *eq = strchr (merged[i], '=');
      ಆದರೆ (! eq)
        ಮುಂದುವರಿ;

      ಆದರೆ (! hash_ಸೇರಿಸು (ಸ್ಥಾನ_ಕೋಷ್ಟಕ, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_ಎಣಿಕೆ = inherited_ಎಣಿಕೆ;
  ಅಕ್ಷರ *assignment = ಖಾಲಿ;
  ಆಗ ((assignment = argz_ಮುಂದಿನ (env0_from_ಬಫರ್, ಗಾತ್ರ_2, assignment)))
    {
      ಅಕ್ಷರ *eq = strchr (assignment, '=');
      ಆದರೆ (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_ಎಣಿಕೆ++] = assignment;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          ಅಕ್ಷರ **ಸ್ಥಾನ = hash_lookup (ಸ್ಥಾನ_ಕೋಷ್ಟಕ, &assignment);
          ಆದರೆ (ಸ್ಥಾನ)
            *ಸ್ಥಾನ = assignment;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              merged[merged_ಎಣಿಕೆ] = assignment;
              ಆದರೆ (! hash_ಸೇರಿಸು (ಸ್ಥಾನ_ಕೋಷ್ಟಕ, &merged[merged_ಎಣಿಕೆ]))
                xalloc_die ();
              ++merged_ಎಣಿಕೆ;
            }
        }
    }

  merged[merged_ಎಣಿಕೆ] = ಖಾಲಿ;
  env->variable = merged;
  env->ಬಳಸಿದ = merged_ಎಣಿಕೆ;
  env->allocated = max_ಎಣಿಕೆ + 1;
  environ = merged;
  hash_free (ಸ್ಥಾನ_ಕೋಷ್ಟಕ);
  ಹಿಂತಿರುಗಿ true;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_raw_environment_from_file (ರಚನೆ env_ಸದಿಶ *env,
                               ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  size_t ಗಾತ್ರ_2 = read_env_file (file);
  idx_t ನಮೂದು_ಎಣಿಕೆ = argz_ಎಣಿಕೆ (env0_from_ಬಫರ್, ಗಾತ್ರ_2);

  env->allocated = ನಮೂದು_ಎಣಿಕೆ + 1;
  env->variable = xnmalloc (env->allocated, ಗಾತ್ರ *env->variable);
  argz_ಹೊರತೆಗೆ (env0_from_ಬಫರ್, ಗಾತ್ರ_2, env->variable);
  env->ಬಳಸಿದ = ನಮೂದು_ಎಣಿಕೆ;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
ಪರಿಶೀಲನೆ_varname (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಆದರೆ (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೊನೆ = str + 3;
      ಆಗ (c_isalnum (*ಕೊನೆ) || *ಕೊನೆ == '_')
        ++ಕೊನೆ;
      ಆದರೆ (*ಕೊನೆ == '}')
        ಹಿಂತಿರುಗಿ ಕೊನೆ;
    }

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
ಹೊರತೆಗೆ_varname (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = ಪರಿಶೀಲನೆ_varname (str);

  ಆದರೆ (!p)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  ಆದರೆ (i >= vnlen)
    {
      ಬಿಡುಗಡೆ (varname);
      varname = xpalloc (ಖಾಲಿ, &vnlen, i + 1 - vnlen, -1, ಗಾತ್ರ *varname);
    }

  ಸ್ಮರಣೆ_ನಕಲು (varname, str + 2, i);
  varname[i] = 0;

  ಹಿಂತಿರುಗಿ varname;
}

/* Temporary buffer used by --split-string processing.  */
ರಚನೆ splitbuf
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
  ಅಕ್ಷರ **argv;
  ಪೂರ್ಣಾಂಕ argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  ಪೂರ್ಣಾಂಕ extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
ಸ್ಥಿರ ಶೂನ್ಯ
splitbuf_grow (ರಚನೆ splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_ಬೈಟುಗಳು = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, IDX_MAX), 2 * ಗಾತ್ರ *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_ಬೈಟುಗಳು);
}

/* In SS, append C to the last string.  */
ಸ್ಥಿರ ಶೂನ್ಯ
splitbuf_append_byte (ರಚನೆ splitbuf *ss, ಅಕ್ಷರ c)
{
  idx_t string_ಬೈಟುಗಳು = (intptr_t) ss->argv[ss->argc];
  ಆದರೆ (ss->half_alloc * ಗಾತ್ರ *ss->argv <= string_ಬೈಟುಗಳು)
    splitbuf_grow (ss);
  ((ಅಕ್ಷರ *) (ss->argv + ss->half_alloc))[string_ಬೈಟುಗಳು] = c;
  ss->argv[ss->argc] = (ಅಕ್ಷರ *) (intptr_t) {string_ಬೈಟುಗಳು + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
ಸ್ಥಿರ ಶೂನ್ಯ
check_ಆರಂಭ_new_arg (ರಚನೆ splitbuf *ss)
{
  ಆದರೆ (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      ಪೂರ್ಣಾಂಕ argc = ss->argc;
      ಆದರೆ (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
ಸ್ಥಿರ ಅಕ್ಷರ **
splitbuf_finishup (ರಚನೆ splitbuf *ss)
{
  ಪೂರ್ಣಾಂಕ argc = ss->argc;
  ಅಕ್ಷರ **argv = ss->argv;
  ಅಕ್ಷರ *stringbase = (ಅಕ್ಷರ *) (ss->argv + ss->half_alloc);
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  ಹಿಂತಿರುಗಿ argv;
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
ಸ್ಥಿರ ಅಕ್ಷರ **
build_argv (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ಪೂರ್ಣಾಂಕ extra_argc, ಪೂರ್ಣಾಂಕ *argc)
{
  bool dq = false, sq = false;
  ರಚನೆ splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * ಗಾತ್ರ *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ಖಾಲಿ;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  ಆಗ (*str)
    {
      ಅಕ್ಷರ newc = *str; /* Default: add the next character.  */

      ಆಯ್ಕೆ (*str)
        {
        ಸ್ಥಿತಿ '\'':
          ಆದರೆ (dq)
            ನಿಲ್ಲಿಸು;
          sq = !sq;
          check_ಆರಂಭ_new_arg (&ss);
          ++str;
          ಮುಂದುವರಿ;

        ಸ್ಥಿತಿ '"':
          ಆದರೆ (sq)
            ನಿಲ್ಲಿಸು;
          dq = !dq;
          check_ಆರಂಭ_new_arg (&ss);
          ++str;
          ಮುಂದುವರಿ;

        ಸ್ಥಿತಿ ' ': ಸ್ಥಿತಿ '\t': ಸ್ಥಿತಿ '\n': ಸ್ಥಿತಿ '\v': ಸ್ಥಿತಿ '\f': ಸ್ಥಿತಿ '\r':
          /* Start a new argument if outside quotes.  */
          ಆದರೆ (sq || dq)
            ನಿಲ್ಲಿಸು;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          ಮುಂದುವರಿ;

        ಸ್ಥಿತಿ '#':
          ಆದರೆ (!ss.sep)
            ನಿಲ್ಲಿಸು;
          ಹೋಗು eos; /* '#' as first char terminates the string.  */

        ಸ್ಥಿತಿ '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          ಆದರೆ (sq && str[1] != '\\' && str[1] != '\'')
            ನಿಲ್ಲಿಸು;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          ಆಯ್ಕೆ (newc)
            {
            ಸ್ಥಿತಿ '"': ಸ್ಥಿತಿ '#': ಸ್ಥಿತಿ '$': ಸ್ಥಿತಿ '\'': ಸ್ಥಿತಿ '\\':
              /* Pass escaped character as-is.  */
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '_':
              ಆದರೆ (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  ಮುಂದುವರಿ;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'c':
              ಆದರೆ (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              ಹೋಗು eos; /* '\c' terminates the string.  */

            ಸ್ಥಿತಿ 'f': newc = '\f'; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'n': newc = '\n'; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'r': newc = '\r'; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 't': newc = '\t'; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 'v': newc = '\v'; ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            ಪೂರ್ವನಿಯೋಜಿತ:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          ಆದರೆ (sq)
            ನಿಲ್ಲಿಸು;

          /* Store the ${VARNAME} value.  */
          {
            ಅಕ್ಷರ *n = ಹೊರತೆಗೆ_varname (str);
            ಆದರೆ (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            ಅಕ್ಷರ *v = getenv (n);
            ಆದರೆ (v)
              {
                check_ಆರಂಭ_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                ಗಾಗಿ (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            ಇಲ್ಲದಿದ್ದರೆ
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            ಮುಂದುವರಿ;
          }
        }

      check_ಆರಂಭ_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  ಆದರೆ (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  ಹಿಂತಿರುಗಿ splitbuf_finishup (&ss);
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
parse_ವಿಭಜಿಸು_string (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ಪೂರ್ಣಾಂಕ *orig_optind,
                    ಪೂರ್ಣಾಂಕ *orig_argc, ಅಕ್ಷರ ***orig_argv)
{
  ಪೂರ್ಣಾಂಕ extra_argc = *orig_argc - *orig_optind, newargc;
  ಅಕ್ಷರ **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  ಆದರೆ (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  ಸ್ಮರಣೆ_ನಕಲು (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * ಗಾತ್ರ *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_signal_action_params (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, bool ಗಣ_default)
{
  ಆದರೆ (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1 ; i <= SIGNUM_BOUND; i++)
        ಸಂಕೇತಗಳು[i] = ಗಣ_default ? DEFAULT_NOERR : IGNORE_NOERR;
      ಹಿಂತಿರುಗಿ;
    }

  ಅಕ್ಷರ *optarg_writable = xstrdup (arg);

  ಅಕ್ಷರ *opt_sig = strtok (optarg_writable, ",");
  ಆಗ (opt_sig)
    {
      ಪೂರ್ಣಾಂಕ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ಆದರೆ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ಆದರೆ (signum <= 0)
        usage (exit_failure);

      ಸಂಕೇತಗಳು[signum] = ಗಣ_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ಖಾಲಿ, ",");
    }

  ಬಿಡುಗಡೆ (optarg_writable);
}

ಸ್ಥಿರ ಶೂನ್ಯ
reset_signal_handlers (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ರಚನೆ sigaction act;

      ಆದರೆ (ಸಂಕೇತಗಳು[i] == UNCHANGED)
        ಮುಂದುವರಿ;

      bool ignore_errors = (ಸಂಕೇತಗಳು[i] == DEFAULT_NOERR
                            || ಸಂಕೇತಗಳು[i] == IGNORE_NOERR);

      bool ಗಣ_ಗಮ್ಯ_default = (ಸಂಕೇತಗಳು[i] == DEFAULT
                             || ಸಂಕೇತಗಳು[i] == DEFAULT_NOERR);

      ಪೂರ್ಣಾಂಕ sig_err = sigaction (i, ಖಾಲಿ, &act);

      ಆದರೆ (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      ಆದರೆ (! sig_err)
        {
          act.sa_handler = ಗಣ_ಗಮ್ಯ_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ಖಾಲಿ);
          ಆದರೆ (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      ಆದರೆ (dev_debug)
        {
          ಅಕ್ಷರ signame[SIG2STR_MAX];
          ಆದರೆ (sig2str (i, signame) != 0)
            snprintf (signame, ಗಾತ್ರ signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  ಗಣ_ಗಮ್ಯ_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ
parse_ಖಂಡ_signal_params (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, bool ಖಂಡ)
{
  ಆದರೆ (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (ಖಂಡ ? &ಖಂಡ_ಸಂಕೇತಗಳು : &unblock_ಸಂಕೇತಗಳು);
      sigemptyset (ಖಂಡ ? &unblock_ಸಂಕೇತಗಳು : &ಖಂಡ_ಸಂಕೇತಗಳು);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! sig_ಮುಖವಾಡ_ಬದಲಾಯಿತು)
    {
      /* Initialize the sets.  */
      sigemptyset (&ಖಂಡ_ಸಂಕೇತಗಳು);
      sigemptyset (&unblock_ಸಂಕೇತಗಳು);
    }

  sig_ಮುಖವಾಡ_ಬದಲಾಯಿತು = true;

  ಆದರೆ (! arg)
    ಹಿಂತಿರುಗಿ;

  ಅಕ್ಷರ *optarg_writable = xstrdup (arg);

  ಅಕ್ಷರ *opt_sig = strtok (optarg_writable, ",");
  ಆಗ (opt_sig)
    {
      ಪೂರ್ಣಾಂಕ signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ಆದರೆ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ಆದರೆ (signum <= 0)
        usage (exit_failure);

      ಆದರೆ (sigaddset (ಖಂಡ ? &ಖಂಡ_ಸಂಕೇತಗಳು : &unblock_ಸಂಕೇತಗಳು, signum) == -1)
        {
          ಆದರೆ (ಖಂಡ)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        sigdelset (ಖಂಡ ? &unblock_ಸಂಕೇತಗಳು : &ಖಂಡ_ಸಂಕೇತಗಳು, signum);

      opt_sig = strtok (ಖಾಲಿ, ",");
    }

  ಬಿಡುಗಡೆ (optarg_writable);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_signal_proc_ಮುಖವಾಡ (ಶೂನ್ಯ)
{
  /* Get the existing signal mask */
  sigset_t ಗಣ_2;

  sigemptyset (&ಗಣ_2);

  ಆದರೆ (sigprocmask (0, ಖಾಲಿ, &ಗಣ_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *debug_act = ಖಾಲಿ;

      ಆದರೆ (sigismember (&ಖಂಡ_ಸಂಕೇತಗಳು, i))
        {
          sigaddset (&ಗಣ_2, i);
          debug_act = "BLOCK";
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sigismember (&unblock_ಸಂಕೇತಗಳು, i))
        {
          sigdelset (&ಗಣ_2, i);
          debug_act = "UNBLOCK";
        }

      ಆದರೆ (dev_debug && debug_act)
        {
          ಅಕ್ಷರ signame[SIG2STR_MAX];
          ಆದರೆ (sig2str (i, signame) != 0)
            snprintf (signame, ಗಾತ್ರ signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  ಆದರೆ (sigprocmask (SIG_SETMASK, &ಗಣ_2, ಖಾಲಿ))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

ಸ್ಥಿರ ಶೂನ್ಯ
list_signal_handling (ಶೂನ್ಯ)
{
  sigset_t ಗಣ_2;

  sigemptyset (&ಗಣ_2);
  ಆದರೆ (sigprocmask (0, ಖಾಲಿ, &ಗಣ_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i <= SIGNUM_BOUND; i++)
    {
      ರಚನೆ sigaction act;
      ಆದರೆ (sigaction (i, ಖಾಲಿ, &act))
        ಮುಂದುವರಿ;

      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *blocked = sigismember (&ಗಣ_2, i) ? "BLOCK" : "";
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *connect = *ignored && *blocked ? "," : "";

      ಆದರೆ (! *ignored && ! *blocked)
        ಮುಂದುವರಿ;

      ಅಕ್ಷರ signame[SIG2STR_MAX];
      ಆದರೆ (sig2str (i, signame) != 0)
        snprintf (signame, ಗಾತ್ರ signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
initialize_ಸಂಕೇತಗಳು (ಶೂನ್ಯ)
{
  ಸಂಕೇತಗಳು = xmalloc ((ಗಾತ್ರ *ಸಂಕೇತಗಳು) * (SIGNUM_BOUND + 1));

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0 ; i <= SIGNUM_BOUND; i++)
    ಸಂಕೇತಗಳು[i] = UNCHANGED;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  ಪೂರ್ಣಾಂಕ quoting_style_opt = -1;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *newdir = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *env0_from_file = ಖಾಲಿ;
  ಅಕ್ಷರ *argv0 = ಖಾಲಿ;
  ರಚನೆ env_ಸದಿಶ env_ಸದಿಶ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ lint
  memset (&env_ಸದಿಶ, 0, ಗಾತ್ರ env_ಸದಿಶ);
  ಅಕ್ಷರ **initial_environ = environ;
#ಆದರೆ_ಅಂತ್ಯ

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_ಸಂಕೇತಗಳು ();

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, shortopts, longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'a':
          argv0 = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          ignore_environment = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'u':
          append_unset_var (optarg);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          dev_debug = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ENV0_FROM_OPTION:
          env0_from_file = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_ಖಂಡ_signal_params (optarg, false);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ಖಂಡ_SIGNAL_OPTION:
          parse_ಖಂಡ_signal_params (optarg, true);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_ವಾದಗಳು,
                                         quoting_style_vals);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'C':
          newdir = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'S':
          parse_ವಿಭಜಿಸು_string (optarg, &optind, &argc, &argv);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ' ': ಸ್ಥಿತಿ '\t': ಸ್ಥಿತಿ '\n': ಸ್ಥಿತಿ '\v': ಸ್ಥಿತಿ '\f': ಸ್ಥಿತಿ '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            ಅಕ್ಷರ c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_CANCELED);
        }
    }

  ಆದರೆ (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  ಪೂರ್ಣಾಂಕ ಕಾರ್ಯಕ್ರಮ_ಸೂಚ್ಯಂಕ = optind;
  ಆಗ (ಕಾರ್ಯಕ್ರಮ_ಸೂಚ್ಯಂಕ < argc && strchr (argv[ಕಾರ್ಯಕ್ರಮ_ಸೂಚ್ಯಂಕ], '='))
    ಕಾರ್ಯಕ್ರಮ_ಸೂಚ್ಯಂಕ++;
  bool ಕಾರ್ಯಕ್ರಮ_specified = ಕಾರ್ಯಕ್ರಮ_ಸೂಚ್ಯಂಕ < argc;

  bool quote_output = false;
  ಪೂರ್ಣಾಂಕ qs = quoting_style_opt;
  ಆದರೆ (! ಕಾರ್ಯಕ್ರಮ_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      ಆದರೆ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ಆದರೆ (0 <= qs && qs != literal_quoting_style)
    {
      ಗಣ_quoting_style (ಖಾಲಿ, qs);
      quote_output = true;
    }

  bool env_ಸದಿಶ_ಸಕ್ರಿಯ = false;
  ಆದರೆ (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      ಗಣ_raw_environment_from_file (&env_ಸದಿಶ, env0_from_file);
      env_ಸದಿಶ_ಸಕ್ರಿಯ = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#ಆದರೆ defined _WIN32 && ! defined __CYGWIN__
      ಸ್ಥಿರ ಅಕ್ಷರ *dummy_environ[] = { ಖಾಲಿ };
      environ = dummy_environ;
#ಇಲ್ಲದಿದ್ದರೆ
      env_ಸದಿಶ.variable = xnmalloc (1, ಗಾತ್ರ *env_ಸದಿಶ.variable);
      env_ಸದಿಶ.variable[0] = ಖಾಲಿ;
      env_ಸದಿಶ.ಬಳಸಿದ = 0;
      env_ಸದಿಶ.allocated = 1;
      environ = env_ಸದಿಶ.variable;
      env_ಸದಿಶ_ಸಕ್ರಿಯ = true;
#ಆದರೆ_ಅಂತ್ಯ
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (env0_from_file)
    env_ಸದಿಶ_ಸಕ್ರಿಯ = ಗಣ_envvars_from_file (&env_ಸದಿಶ, env0_from_file);

  ಆದರೆ (! ignore_environment || env0_from_file)
    {
      ಆದರೆ (env_ಸದಿಶ_ಸಕ್ರಿಯ)
        unset_envvars_ಒಳ_ಸದಿಶ (&env_ಸದಿಶ);
      ಇಲ್ಲದಿದ್ದರೆ
        unset_envvars ();
    }

  ಅಕ್ಷರ *eq;
  ಆಗ (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      ಆದರೆ (env_ಸದಿಶ_ಸಕ್ರಿಯ)
        ಗಣ_envvar_ಒಳ_ಸದಿಶ (&env_ಸದಿಶ, argv[optind], eq);
      ಇಲ್ಲದಿದ್ದರೆ
        ಗಣ_envvar (argv[optind], eq);
      optind++;
    }

  ಆದರೆ (opt_nul_terminate_output && ಕಾರ್ಯಕ್ರಮ_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  ಆದರೆ (newdir && ! ಕಾರ್ಯಕ್ರಮ_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  ಆದರೆ (argv0 && ! ಕಾರ್ಯಕ್ರಮ_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  ಆದರೆ (! ಕಾರ್ಯಕ್ರಮ_specified)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ terminator = opt_nul_terminate_output ? '\0' : '\n';

      ಆದರೆ (opt_nul_terminate_output)
        xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

      /* Print the environment and exit.  */
      ಗಾಗಿ (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ lint
      ಆದರೆ (env_ಸದಿಶ_ಸಕ್ರಿಯ)
        {
          environ = initial_environ;
          ಬಿಡುಗಡೆ (env_ಸದಿಶ.variable);
          ಬಿಡುಗಡೆ (env0_from_ಬಫರ್);
        }
#ಆದರೆ_ಅಂತ್ಯ
      ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  ಆದರೆ (sig_ಮುಖವಾಡ_ಬದಲಾಯಿತು)
    ಗಣ_signal_proc_ಮುಖವಾಡ ();

  ಆದರೆ (report_signal_handling)
    list_signal_handling ();

  ಆದರೆ (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      ಆದರೆ (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  ಅಕ್ಷರ *ಕಾರ್ಯಕ್ರಮ = argv[optind];
  ಆದರೆ (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  ಆದರೆ (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (ಕಾರ್ಯಕ್ರಮ));
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (ಕಾರ್ಯಕ್ರಮ, &argv[optind]);

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (ಕಾರ್ಯಕ್ರಮ));

  ಆದರೆ (exit_ಸ್ಥಿತಿ == EXIT_ENOENT && strpbrk (ಕಾರ್ಯಕ್ರಮ, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_ಸ್ಥಿತಿ);
}
