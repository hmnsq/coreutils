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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <argz.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <c-ctype.h>
#అన్తర్భావ <signal.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hashcode-mem.h"
#అన్తర్భావ "system.h"
#అన్తర్భావ "operand2sig.h"
#అన్తర్భావ "printenv.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "read-file.h"
#అన్తర్భావ "sig2str.h"
#అన్తర్భావ "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "env"

#నిర్వచించు AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
స్థిర అక్షరం స్థిరరాశి **usvars;
స్థిర idx_t usvars_alloc;
స్థిర idx_t usvars_వాడిన;

/* Storage for entries read by --env0-from.  */
స్థిర అక్షరం *env0_from_బఫరు;

/* An environment vector managed directly to preserve raw entries.  */
నిర్మాణం env_సదిశ
{
  అక్షరం **variable;
  idx_t వాడిన;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
స్థిర bool dev_debug;
#నిర్వచించు vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
స్థిర అక్షరం *varname;
స్థిర idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#నిర్వచనం_రద్దు IGNORE

/* Possible actions on each signal.  */
గణన SIGNAL_బహుళకం {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
స్థిర గణన SIGNAL_బహుళకం *సంకేతాలు;

/* Set of signals to block.  */
స్థిర sigset_t ఖండం_సంకేతాలు;

/* Set of signals to unblock.  */
స్థిర sigset_t unblock_సంకేతాలు;

/* Whether signal mask adjustment requested.  */
స్థిర bool sig_ముసుగు_మారింది;

/* Whether to list non default handling.  */
స్థిర bool report_signal_handling;

/* The isspace characters in the C locale.  */
#నిర్వచించు C_ISSPACE_CHARS " \t\n\v\f\r"

స్థిర అక్షరం స్థిరరాశి shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  ఖండం_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"argv0", required_వాదం, ఖాళీ, 'a'},
  {"ignore-environment", no_వాదం, ఖాళీ, 'i'},
  {"env0-from", required_వాదం, ఖాళీ, ENV0_FROM_OPTION},
  {"null", no_వాదం, ఖాళీ, '0'},
  {"unset", required_వాదం, ఖాళీ, 'u'},
  {"chdir", required_వాదం, ఖాళీ, 'C'},
  {"default-signal", optional_వాదం, ఖాళీ, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_వాదం, ఖాళీ, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_వాదం, ఖాళీ, ఖండం_SIGNAL_OPTION},
  {"list-signal-handling", no_వాదం, ఖాళీ,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_వాదం, ఖాళీ, 'v'},
  {"quoting-style", required_వాదం, ఖాళీ, QUOTING_STYLE_OPTION},
  {"split-string", required_వాదం, ఖాళీ, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [-] [NAME=విలువ_2]... [COMMAND [ARG]...]\n"),
              కార్యక్రమం_name);
      fputs (_("\
సమితి_3 each NAME గమ్యం విలువ_2 లోపల the environment and నిరంతరం_2 COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth వాదం యొక్క COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         ప్రారంభం with an ఖాళీలు environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         పఠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         remove variable from the environment\n\
"));
      oputs (_("\
  -C, --chdir=DIR\n\
         change working directory గమ్యం DIR\n\
"));
      oputs (_("\
      --quoting-style=మాట\n\
         use quoting style మాట కొరకు పేర్లు and విలువలు:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --విభజించు-string=S\n\
         process and విభజించు S into separate arguments;\n\
         వాడిన గమ్యం pass గుణిజం arguments on shebang పంక్తులు\n\
"));
      oputs (_("\
      --ఖండం-సంకేతం[=SIG]\n\
         ఖండం delivery యొక్క SIG సంకేతం(s) గమ్యం COMMAND\n\
"));
      oputs (_("\
      --అప్రమేయ-సంకేతం[=SIG]\n\
         reset handling యొక్క SIG సంకేతం(s) గమ్యం the అప్రమేయ\n\
"));
      oputs (_("\
      --ignore-సంకేతం[=SIG]\n\
         సమితి_2 handling యొక్క SIG సంకేతం(s) గమ్యం చేయి nothing\n\
"));
      oputs (_("\
      --list-సంకేతం-handling\n\
         list non అప్రమేయ సంకేతం handling గమ్యం standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information కొరకు each processing అడుగు\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - సూచిస్తుంది -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a సంకేతం name like 'PIPE', or a సంకేతం సంఖ్య like '13'.\n\
Without SIG, all తెలిసిన సంకేతాలు are included.  గుణిజం_2 సంకేతాలు can be\n\
comma-separated.  An ఖాళీలు SIG వాదం is a no-op.\n\
"), stdout);
      emit_exec_స్థితి (కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర శూన్య
append_unset_var (అక్షరం స్థిరరాశి *var)
{
  ఒకవేళ (usvars_వాడిన == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, పరిమాణం *usvars);
  usvars[usvars_వాడిన++] = var;
}

స్థిర శూన్య
unset_envvars (శూన్య)
{
  కొరకు (idx_t i = 0; i < usvars_వాడిన; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      ఒకవేళ (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
స్థిర bool
నమోదు_has_name (అక్షరం స్థిరరాశి *నమోదు_2, అక్షరం స్థిరరాశి *name, idx_t name_నిడివి)
{
  అక్షరం స్థిరరాశి *eq = strchr (నమోదు_2, '=');
  తిరిగి (eq && eq - నమోదు_2 == name_నిడివి
          && memeq (నమోదు_2, name, name_నిడివి));
}

#ఒకవేళ ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
స్థిర size_t
env_సదిశ_స్థానం_hash (శూన్య స్థిరరాశి *x, size_t పట్టిక_కొలత)
{
  అక్షరం *స్థిరరాశి *స్థానం = x;
  అక్షరం స్థిరరాశి *నమోదు_2 = *స్థానం;
  అక్షరం స్థిరరాశి *eq = strchr (నమోదు_2, '=');
  తిరిగి hash_pjw_bare (నమోదు_2, eq - నమోదు_2) % పట్టిక_కొలత;
}

స్థిర bool
env_సదిశ_స్థానం_compare (శూన్య స్థిరరాశి *x, శూన్య స్థిరరాశి *y)
{
  అక్షరం స్థిరరాశి *a = * (అక్షరం *స్థిరరాశి *) x;
  అక్షరం స్థిరరాశి *b = * (అక్షరం *స్థిరరాశి *) y;
  అక్షరం స్థిరరాశి *b_eq = strchr (b, '=');
  తిరిగి నమోదు_has_name (a, b, b_eq - b);
}
#అయితే_ముగింపు

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
స్థిర శూన్య
unset_envvars_లోపల_సదిశ (నిర్మాణం env_సదిశ *env)
{
  కొరకు (idx_t i = 0; i < usvars_వాడిన; ++i)
    {
      అక్షరం స్థిరరాశి *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      ఒకవేళ (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_నిడివి = అక్షరశ్రేణి_పొడవు (name);
      idx_t dest = 0;
      కొరకు (idx_t ఆధారం_2 = 0; ఆధారం_2 < env->వాడిన; ++ఆధారం_2)
        ఒకవేళ (! నమోదు_has_name (env->variable[ఆధారం_2], name, name_నిడివి))
          env->variable[dest++] = env->variable[ఆధారం_2];

      env->వాడిన = dest;
      env->variable[dest] = ఖాళీ;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
స్థిర శూన్య
సమితి_envvar (అక్షరం *assignment, అక్షరం *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  ఒకవేళ (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
స్థిర శూన్య
సమితి_envvar_లోపల_సదిశ (నిర్మాణం env_సదిశ *env, అక్షరం *assignment, అక్షరం *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_నిడివి = eq - assignment;
  కొరకు (idx_t i = 0; i < env->వాడిన; ++i)
    ఒకవేళ (నమోదు_has_name (env->variable[i], assignment, name_నిడివి))
      {
        env->variable[i] = assignment;
        తిరిగి;
      }

  ఒకవేళ (env->వాడిన + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               పరిమాణం *env->variable);
      environ = env->variable;
    }

  env->variable[env->వాడిన++] = assignment;
  env->variable[env->వాడిన] = ఖాళీ;
}

/* Read the NUL-delimited environment entries in FILE.  */
స్థిర size_t
read_env_file (అక్షరం స్థిరరాశి *file)
{
  size_t కొలత;
  ఒకవేళ (streq (file, "-"))
    {
      xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
      env0_from_బఫరు = fread_file (stdin, RF_ద్విమాన, &కొలత);
    }
  లేకపోతే
    env0_from_బఫరు = read_file (file, RF_ద్విమాన, &కొలత);

  ఒకవేళ (! env0_from_బఫరు)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  ఒకవేళ (కొలత && env0_from_బఫరు[కొలత - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  తిరిగి కొలత;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
స్థిర bool
సమితి_envvars_from_file (నిర్మాణం env_సదిశ *env, అక్షరం స్థిరరాశి *file)
{
  size_t కొలత = read_env_file (file);

#ఒకవేళ defined _WIN32 && ! defined __CYGWIN__
  (శూన్య) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  అక్షరం *assignment = ఖాళీ;
  అయితే ((assignment = argz_తదుపరి (env0_from_బఫరు, కొలత, assignment)))
    {
      అక్షరం *eq = strchr (assignment, '=');
      ఒకవేళ (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      సమితి_envvar (assignment, eq);
    }
  తిరిగి false;
#లేకపోతే
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_లెక్క = argz_లెక్క (env0_from_బఫరు, కొలత);

  ఒకవేళ (file_లెక్క == 0)
    తిరిగి false;

  idx_t inherited_లెక్క = 0;
  అయితే (environ[inherited_లెక్క])
    ++inherited_లెక్క;

  ఒకవేళ (IDX_MAX - inherited_లెక్క <= file_లెక్క)
    xalloc_die ();
  idx_t max_లెక్క = inherited_లెక్క + file_లెక్క;
  అక్షరం **merged = xnmalloc (max_లెక్క + 1, పరిమాణం *merged);
  జ్ఞాపకం_నకలు (merged, environ, inherited_లెక్క * పరిమాణం *merged);
  merged[inherited_లెక్క] = ఖాళీ;

  Hash_పట్టిక *స్థానం_పట్టిక
    = hash_initialize (max_లెక్క, ఖాళీ, env_సదిశ_స్థానం_hash,
                       env_సదిశ_స్థానం_compare, ఖాళీ);
  ఒకవేళ (! స్థానం_పట్టిక)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  కొరకు (idx_t i = 0; i < inherited_లెక్క; ++i)
    {
      అక్షరం *eq = strchr (merged[i], '=');
      ఒకవేళ (! eq)
        కొనసాగు;

      ఒకవేళ (! hash_చొప్పించు (స్థానం_పట్టిక, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_లెక్క = inherited_లెక్క;
  అక్షరం *assignment = ఖాళీ;
  అయితే ((assignment = argz_తదుపరి (env0_from_బఫరు, కొలత, assignment)))
    {
      అక్షరం *eq = strchr (assignment, '=');
      ఒకవేళ (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_లెక్క++] = assignment;
        }
      లేకపోతే
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          అక్షరం **స్థానం = hash_lookup (స్థానం_పట్టిక, &assignment);
          ఒకవేళ (స్థానం)
            *స్థానం = assignment;
          లేకపోతే
            {
              merged[merged_లెక్క] = assignment;
              ఒకవేళ (! hash_చొప్పించు (స్థానం_పట్టిక, &merged[merged_లెక్క]))
                xalloc_die ();
              ++merged_లెక్క;
            }
        }
    }

  merged[merged_లెక్క] = ఖాళీ;
  env->variable = merged;
  env->వాడిన = merged_లెక్క;
  env->allocated = max_లెక్క + 1;
  environ = merged;
  hash_free (స్థానం_పట్టిక);
  తిరిగి true;
#అయితే_ముగింపు
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
స్థిర శూన్య
సమితి_raw_environment_from_file (నిర్మాణం env_సదిశ *env,
                               అక్షరం స్థిరరాశి *file)
{
  size_t కొలత = read_env_file (file);
  idx_t నమోదు_లెక్క = argz_లెక్క (env0_from_బఫరు, కొలత);

  env->allocated = నమోదు_లెక్క + 1;
  env->variable = xnmalloc (env->allocated, పరిమాణం *env->variable);
  argz_వెలికితీయి (env0_from_బఫరు, కొలత, env->variable);
  env->వాడిన = నమోదు_లెక్క;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_శుద్ధ
స్థిర అక్షరం స్థిరరాశి *
పరిశీలన_varname (అక్షరం స్థిరరాశి *str)
{
  ఒకవేళ (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      అక్షరం స్థిరరాశి *చివర = str + 3;
      అయితే (c_isalnum (*చివర) || *చివర == '_')
        ++చివర;
      ఒకవేళ (*చివర == '}')
        తిరిగి చివర;
    }

  తిరిగి ఖాళీ;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
స్థిర అక్షరం *
వెలికితీయి_varname (అక్షరం స్థిరరాశి *str)
{
  అక్షరం స్థిరరాశి *p = పరిశీలన_varname (str);

  ఒకవేళ (!p)
    తిరిగి ఖాళీ;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  ఒకవేళ (i >= vnlen)
    {
      విడుదల (varname);
      varname = xpalloc (ఖాళీ, &vnlen, i + 1 - vnlen, -1, పరిమాణం *varname);
    }

  జ్ఞాపకం_నకలు (varname, str + 2, i);
  varname[i] = 0;

  తిరిగి varname;
}

/* Temporary buffer used by --split-string processing.  */
నిర్మాణం splitbuf
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
  అక్షరం **argv;
  పూర్ణాంకం argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  పూర్ణాంకం extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
స్థిర శూన్య
splitbuf_grow (నిర్మాణం splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_బైట్లు = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (గరిష్ఠ_పూర్ణాంకం, IDX_MAX), 2 * పరిమాణం *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_బైట్లు);
}

/* In SS, append C to the last string.  */
స్థిర శూన్య
splitbuf_append_byte (నిర్మాణం splitbuf *ss, అక్షరం c)
{
  idx_t string_బైట్లు = (intptr_t) ss->argv[ss->argc];
  ఒకవేళ (ss->half_alloc * పరిమాణం *ss->argv <= string_బైట్లు)
    splitbuf_grow (ss);
  ((అక్షరం *) (ss->argv + ss->half_alloc))[string_బైట్లు] = c;
  ss->argv[ss->argc] = (అక్షరం *) (intptr_t) {string_బైట్లు + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
స్థిర శూన్య
check_ప్రారంభం_new_arg (నిర్మాణం splitbuf *ss)
{
  ఒకవేళ (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      పూర్ణాంకం argc = ss->argc;
      ఒకవేళ (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
స్థిర అక్షరం **
splitbuf_finishup (నిర్మాణం splitbuf *ss)
{
  పూర్ణాంకం argc = ss->argc;
  అక్షరం **argv = ss->argv;
  అక్షరం *stringbase = (అక్షరం *) (ss->argv + ss->half_alloc);
  కొరకు (పూర్ణాంకం i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  తిరిగి argv;
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
స్థిర అక్షరం **
build_argv (అక్షరం స్థిరరాశి *str, పూర్ణాంకం extra_argc, పూర్ణాంకం *argc)
{
  bool dq = false, sq = false;
  నిర్మాణం splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * పరిమాణం *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ఖాళీ;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  అయితే (*str)
    {
      అక్షరం newc = *str; /* Default: add the next character.  */

      ఎంపిక (*str)
        {
        స్థితి '\'':
          ఒకవేళ (dq)
            ఆపు;
          sq = !sq;
          check_ప్రారంభం_new_arg (&ss);
          ++str;
          కొనసాగు;

        స్థితి '"':
          ఒకవేళ (sq)
            ఆపు;
          dq = !dq;
          check_ప్రారంభం_new_arg (&ss);
          ++str;
          కొనసాగు;

        స్థితి ' ': స్థితి '\t': స్థితి '\n': స్థితి '\v': స్థితి '\f': స్థితి '\r':
          /* Start a new argument if outside quotes.  */
          ఒకవేళ (sq || dq)
            ఆపు;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          కొనసాగు;

        స్థితి '#':
          ఒకవేళ (!ss.sep)
            ఆపు;
          వెళ్ళు eos; /* '#' as first char terminates the string.  */

        స్థితి '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          ఒకవేళ (sq && str[1] != '\\' && str[1] != '\'')
            ఆపు;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          ఎంపిక (newc)
            {
            స్థితి '"': స్థితి '#': స్థితి '$': స్థితి '\'': స్థితి '\\':
              /* Pass escaped character as-is.  */
              ఆపు;

            స్థితి '_':
              ఒకవేళ (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  కొనసాగు;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              ఆపు;

            స్థితి 'c':
              ఒకవేళ (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              వెళ్ళు eos; /* '\c' terminates the string.  */

            స్థితి 'f': newc = '\f'; ఆపు;
            స్థితి 'n': newc = '\n'; ఆపు;
            స్థితి 'r': newc = '\r'; ఆపు;
            స్థితి 't': newc = '\t'; ఆపు;
            స్థితి 'v': newc = '\v'; ఆపు;

            స్థితి '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            అప్రమేయ:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          ఆపు;

        స్థితి '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          ఒకవేళ (sq)
            ఆపు;

          /* Store the ${VARNAME} value.  */
          {
            అక్షరం *n = వెలికితీయి_varname (str);
            ఒకవేళ (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            అక్షరం *v = getenv (n);
            ఒకవేళ (v)
              {
                check_ప్రారంభం_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                కొరకు (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            లేకపోతే
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            కొనసాగు;
          }
        }

      check_ప్రారంభం_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  ఒకవేళ (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  తిరిగి splitbuf_finishup (&ss);
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
స్థిర శూన్య
parse_విభజించు_string (అక్షరం స్థిరరాశి *str, పూర్ణాంకం *orig_optind,
                    పూర్ణాంకం *orig_argc, అక్షరం ***orig_argv)
{
  పూర్ణాంకం extra_argc = *orig_argc - *orig_optind, newargc;
  అక్షరం **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  ఒకవేళ (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      కొరకు (పూర్ణాంకం i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  జ్ఞాపకం_నకలు (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * పరిమాణం *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

స్థిర శూన్య
parse_signal_action_params (అక్షరం స్థిరరాశి *arg, bool సమితి_default)
{
  ఒకవేళ (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      కొరకు (పూర్ణాంకం i = 1 ; i <= SIGNUM_BOUND; i++)
        సంకేతాలు[i] = సమితి_default ? DEFAULT_NOERR : IGNORE_NOERR;
      తిరిగి;
    }

  అక్షరం *optarg_writable = xstrdup (arg);

  అక్షరం *opt_sig = strtok (optarg_writable, ",");
  అయితే (opt_sig)
    {
      పూర్ణాంకం signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ఒకవేళ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ఒకవేళ (signum <= 0)
        usage (exit_failure);

      సంకేతాలు[signum] = సమితి_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ఖాళీ, ",");
    }

  విడుదల (optarg_writable);
}

స్థిర శూన్య
reset_signal_handlers (శూన్య)
{
  కొరకు (పూర్ణాంకం i = 1; i <= SIGNUM_BOUND; i++)
    {
      నిర్మాణం sigaction act;

      ఒకవేళ (సంకేతాలు[i] == UNCHANGED)
        కొనసాగు;

      bool ignore_errors = (సంకేతాలు[i] == DEFAULT_NOERR
                            || సంకేతాలు[i] == IGNORE_NOERR);

      bool సమితి_గమ్యం_default = (సంకేతాలు[i] == DEFAULT
                             || సంకేతాలు[i] == DEFAULT_NOERR);

      పూర్ణాంకం sig_err = sigaction (i, ఖాళీ, &act);

      ఒకవేళ (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      ఒకవేళ (! sig_err)
        {
          act.sa_handler = సమితి_గమ్యం_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ఖాళీ);
          ఒకవేళ (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      ఒకవేళ (dev_debug)
        {
          అక్షరం signame[SIG2STR_MAX];
          ఒకవేళ (sig2str (i, signame) != 0)
            snprintf (signame, పరిమాణం signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  సమితి_గమ్యం_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


స్థిర శూన్య
parse_ఖండం_signal_params (అక్షరం స్థిరరాశి *arg, bool ఖండం)
{
  ఒకవేళ (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (ఖండం ? &ఖండం_సంకేతాలు : &unblock_సంకేతాలు);
      sigemptyset (ఖండం ? &unblock_సంకేతాలు : &ఖండం_సంకేతాలు);
    }
  లేకపోతే ఒకవేళ (! sig_ముసుగు_మారింది)
    {
      /* Initialize the sets.  */
      sigemptyset (&ఖండం_సంకేతాలు);
      sigemptyset (&unblock_సంకేతాలు);
    }

  sig_ముసుగు_మారింది = true;

  ఒకవేళ (! arg)
    తిరిగి;

  అక్షరం *optarg_writable = xstrdup (arg);

  అక్షరం *opt_sig = strtok (optarg_writable, ",");
  అయితే (opt_sig)
    {
      పూర్ణాంకం signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      ఒకవేళ (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      ఒకవేళ (signum <= 0)
        usage (exit_failure);

      ఒకవేళ (sigaddset (ఖండం ? &ఖండం_సంకేతాలు : &unblock_సంకేతాలు, signum) == -1)
        {
          ఒకవేళ (ఖండం)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      లేకపోతే
        sigdelset (ఖండం ? &unblock_సంకేతాలు : &ఖండం_సంకేతాలు, signum);

      opt_sig = strtok (ఖాళీ, ",");
    }

  విడుదల (optarg_writable);
}

స్థిర శూన్య
సమితి_signal_proc_ముసుగు (శూన్య)
{
  /* Get the existing signal mask */
  sigset_t సమితి_2;

  sigemptyset (&సమితి_2);

  ఒకవేళ (sigprocmask (0, ఖాళీ, &సమితి_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  కొరకు (పూర్ణాంకం i = 1; i <= SIGNUM_BOUND; i++)
    {
      అక్షరం స్థిరరాశి *debug_act = ఖాళీ;

      ఒకవేళ (sigismember (&ఖండం_సంకేతాలు, i))
        {
          sigaddset (&సమితి_2, i);
          debug_act = "BLOCK";
        }
      లేకపోతే ఒకవేళ (sigismember (&unblock_సంకేతాలు, i))
        {
          sigdelset (&సమితి_2, i);
          debug_act = "UNBLOCK";
        }

      ఒకవేళ (dev_debug && debug_act)
        {
          అక్షరం signame[SIG2STR_MAX];
          ఒకవేళ (sig2str (i, signame) != 0)
            snprintf (signame, పరిమాణం signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  ఒకవేళ (sigprocmask (SIG_SETMASK, &సమితి_2, ఖాళీ))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

స్థిర శూన్య
list_signal_handling (శూన్య)
{
  sigset_t సమితి_2;

  sigemptyset (&సమితి_2);
  ఒకవేళ (sigprocmask (0, ఖాళీ, &సమితి_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  కొరకు (పూర్ణాంకం i = 1; i <= SIGNUM_BOUND; i++)
    {
      నిర్మాణం sigaction act;
      ఒకవేళ (sigaction (i, ఖాళీ, &act))
        కొనసాగు;

      అక్షరం స్థిరరాశి *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      అక్షరం స్థిరరాశి *blocked = sigismember (&సమితి_2, i) ? "BLOCK" : "";
      అక్షరం స్థిరరాశి *connect = *ignored && *blocked ? "," : "";

      ఒకవేళ (! *ignored && ! *blocked)
        కొనసాగు;

      అక్షరం signame[SIG2STR_MAX];
      ఒకవేళ (sig2str (i, signame) != 0)
        snprintf (signame, పరిమాణం signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

స్థిర శూన్య
initialize_సంకేతాలు (శూన్య)
{
  సంకేతాలు = xmalloc ((పరిమాణం *సంకేతాలు) * (SIGNUM_BOUND + 1));

  కొరకు (పూర్ణాంకం i = 0 ; i <= SIGNUM_BOUND; i++)
    సంకేతాలు[i] = UNCHANGED;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  పూర్ణాంకం quoting_style_opt = -1;
  అక్షరం స్థిరరాశి *newdir = ఖాళీ;
  అక్షరం స్థిరరాశి *env0_from_file = ఖాళీ;
  అక్షరం *argv0 = ఖాళీ;
  నిర్మాణం env_సదిశ env_సదిశ;
#నిర్వచితమైతే lint
  memset (&env_సదిశ, 0, పరిమాణం env_సదిశ);
  అక్షరం **initial_environ = environ;
#అయితే_ముగింపు

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_సంకేతాలు ();

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, shortopts, longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'a':
          argv0 = optarg;
          ఆపు;
        స్థితి 'i':
          ignore_environment = true;
          ఆపు;
        స్థితి 'u':
          append_unset_var (optarg);
          ఆపు;
        స్థితి 'v':
          dev_debug = true;
          ఆపు;
        స్థితి '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ఆపు;
        స్థితి ENV0_FROM_OPTION:
          env0_from_file = optarg;
          ఆపు;
        స్థితి DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_ఖండం_signal_params (optarg, false);
          ఆపు;
        స్థితి IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          ఆపు;
        స్థితి ఖండం_SIGNAL_OPTION:
          parse_ఖండం_signal_params (optarg, true);
          ఆపు;
        స్థితి LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          ఆపు;
        స్థితి QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_వాదనలు,
                                         quoting_style_vals);
          ఆపు;
        స్థితి 'C':
          newdir = optarg;
          ఆపు;
        స్థితి 'S':
          parse_విభజించు_string (optarg, &optind, &argc, &argv);
          ఆపు;
        స్థితి ' ': స్థితి '\t': స్థితి '\n': స్థితి '\v': స్థితి '\f': స్థితి '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            అక్షరం c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_CANCELED);
        }
    }

  ఒకవేళ (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  పూర్ణాంకం కార్యక్రమం_సూచీ = optind;
  అయితే (కార్యక్రమం_సూచీ < argc && strchr (argv[కార్యక్రమం_సూచీ], '='))
    కార్యక్రమం_సూచీ++;
  bool కార్యక్రమం_specified = కార్యక్రమం_సూచీ < argc;

  bool quote_output = false;
  పూర్ణాంకం qs = quoting_style_opt;
  ఒకవేళ (! కార్యక్రమం_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      ఒకవేళ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ఒకవేళ (0 <= qs && qs != literal_quoting_style)
    {
      సమితి_quoting_style (ఖాళీ, qs);
      quote_output = true;
    }

  bool env_సదిశ_చురుకైన = false;
  ఒకవేళ (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      సమితి_raw_environment_from_file (&env_సదిశ, env0_from_file);
      env_సదిశ_చురుకైన = true;
    }
  లేకపోతే ఒకవేళ (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#ఒకవేళ defined _WIN32 && ! defined __CYGWIN__
      స్థిర అక్షరం *dummy_environ[] = { ఖాళీ };
      environ = dummy_environ;
#లేకపోతే
      env_సదిశ.variable = xnmalloc (1, పరిమాణం *env_సదిశ.variable);
      env_సదిశ.variable[0] = ఖాళీ;
      env_సదిశ.వాడిన = 0;
      env_సదిశ.allocated = 1;
      environ = env_సదిశ.variable;
      env_సదిశ_చురుకైన = true;
#అయితే_ముగింపు
    }
  లేకపోతే ఒకవేళ (env0_from_file)
    env_సదిశ_చురుకైన = సమితి_envvars_from_file (&env_సదిశ, env0_from_file);

  ఒకవేళ (! ignore_environment || env0_from_file)
    {
      ఒకవేళ (env_సదిశ_చురుకైన)
        unset_envvars_లోపల_సదిశ (&env_సదిశ);
      లేకపోతే
        unset_envvars ();
    }

  అక్షరం *eq;
  అయితే (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      ఒకవేళ (env_సదిశ_చురుకైన)
        సమితి_envvar_లోపల_సదిశ (&env_సదిశ, argv[optind], eq);
      లేకపోతే
        సమితి_envvar (argv[optind], eq);
      optind++;
    }

  ఒకవేళ (opt_nul_terminate_output && కార్యక్రమం_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  ఒకవేళ (newdir && ! కార్యక్రమం_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  ఒకవేళ (argv0 && ! కార్యక్రమం_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  ఒకవేళ (! కార్యక్రమం_specified)
    {
      అక్షరం స్థిరరాశి terminator = opt_nul_terminate_output ? '\0' : '\n';

      ఒకవేళ (opt_nul_terminate_output)
        xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);

      /* Print the environment and exit.  */
      కొరకు (అక్షరం *స్థిరరాశి *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#నిర్వచితమైతే lint
      ఒకవేళ (env_సదిశ_చురుకైన)
        {
          environ = initial_environ;
          విడుదల (env_సదిశ.variable);
          విడుదల (env0_from_బఫరు);
        }
#అయితే_ముగింపు
      తిరిగి EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  ఒకవేళ (sig_ముసుగు_మారింది)
    సమితి_signal_proc_ముసుగు ();

  ఒకవేళ (report_signal_handling)
    list_signal_handling ();

  ఒకవేళ (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      ఒకవేళ (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  అక్షరం *కార్యక్రమం = argv[optind];
  ఒకవేళ (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  ఒకవేళ (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (కార్యక్రమం));
      కొరకు (పూర్ణాంకం i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (కార్యక్రమం, &argv[optind]);

  పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (కార్యక్రమం));

  ఒకవేళ (exit_స్థితి == EXIT_ENOENT && strpbrk (కార్యక్రమం, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_స్థితి);
}
