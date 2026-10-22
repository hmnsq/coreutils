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
#परिभाषय क्रमादेशः_NAME "env"

#परिभाषय AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
स्थिर वर्ण ध्रुव **usvars;
स्थिर idx_t usvars_alloc;
स्थिर idx_t usvars_प्रयुक्तम्;

/* Storage for entries read by --env0-from.  */
स्थिर वर्ण *env0_from_बफर;

/* An environment vector managed directly to preserve raw entries.  */
संरचना env_सदिशम्
{
  वर्ण **variable;
  idx_t प्रयुक्तम्;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
स्थिर bool dev_debug;
#परिभाषय vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
स्थिर वर्ण *varname;
स्थिर idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#अपरिभाषय IGNORE

/* Possible actions on each signal.  */
गणना SIGNAL_बहुलकम् {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
स्थिर गणना SIGNAL_बहुलकम् *सङ्केताः;

/* Set of signals to block.  */
स्थिर sigset_t खण्डम्_सङ्केताः;

/* Set of signals to unblock.  */
स्थिर sigset_t unblock_सङ्केताः;

/* Whether signal mask adjustment requested.  */
स्थिर bool sig_आवरणम्_परिवर्तितम्;

/* Whether to list non default handling.  */
स्थिर bool report_signal_handling;

/* The isspace characters in the C locale.  */
#परिभाषय C_ISSPACE_CHARS " \t\n\v\f\r"

स्थिर वर्ण ध्रुव shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  खण्डम्_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"argv0", required_तर्कः, रिक्त, 'a'},
  {"ignore-environment", no_तर्कः, रिक्त, 'i'},
  {"env0-from", required_तर्कः, रिक्त, ENV0_FROM_OPTION},
  {"null", no_तर्कः, रिक्त, '0'},
  {"unset", required_तर्कः, रिक्त, 'u'},
  {"chdir", required_तर्कः, रिक्त, 'C'},
  {"default-signal", optional_तर्कः, रिक्त, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_तर्कः, रिक्त, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_तर्कः, रिक्त, खण्डम्_SIGNAL_OPTION},
  {"list-signal-handling", no_तर्कः, रिक्त,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_तर्कः, रिक्त, 'v'},
  {"quoting-style", required_तर्कः, रिक्त, QUOTING_STYLE_OPTION},
  {"split-string", required_तर्कः, रिक्त, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [-] [NAME=मानम्_2]... [COMMAND [ARG]...]\n"),
              क्रमादेशः_name);
      fputs (_("\
समुच्चयः_3 each NAME गन्तव्यम् मानम्_2 अन्तर् the environment and सततम्_2 COMMAND.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --argv0=ARG\n\
         pass ARG as the zeroth तर्कः सम्बन्धि COMMAND\n\
"));
      oputs (_("\
  -i, --ignore-environment\n\
         start with an रिक्तानि environment\n\
"));
      oputs (_("\
      --env0-from=FILE\n\
         पठ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         अन्तः each output पङ्क्तिः with NUL, not newline\n\
"));
      oputs (_("\
  -u, --unset=NAME\n\
         remove variable from the environment\n\
"));
      oputs (_("\
  -C, --chdir=DIR\n\
         change working directory गन्तव्यम् DIR\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD प्रति नामानि and मानानि:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --विभज-string=S\n\
         process and विभज S into separate arguments;\n\
         प्रयुक्तम् गन्तव्यम् pass गुणजः arguments on shebang पङ्क्तयः\n\
"));
      oputs (_("\
      --खण्डम्-सङ्केत[=SIG]\n\
         खण्डम् delivery सम्बन्धि SIG सङ्केत(s) गन्तव्यम् COMMAND\n\
"));
      oputs (_("\
      --स्वभाव-सङ्केत[=SIG]\n\
         reset handling सम्बन्धि SIG सङ्केत(s) गन्तव्यम् the स्वभाव\n\
"));
      oputs (_("\
      --ignore-सङ्केत[=SIG]\n\
         समुच्चयः_2 handling सम्बन्धि SIG सङ्केत(s) गन्तव्यम् कुरु nothing\n\
"));
      oputs (_("\
      --list-सङ्केत-handling\n\
         list non स्वभाव सङ्केत handling गन्तव्यम् standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information प्रति each processing चरणः\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - उपपादयति -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a सङ्केत name like 'PIPE', or a सङ्केत सङ्ख्या like '13'.\n\
Without SIG, all ज्ञातम् सङ्केताः are included.  गुणजः_2 सङ्केताः can be\n\
comma-separated.  An रिक्तानि SIG तर्कः is a no-op.\n\
"), stdout);
      emit_exec_स्थितिः (क्रमादेशः_NAME);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

स्थिर शून्य
append_unset_var (वर्ण ध्रुव *var)
{
  यदि (usvars_प्रयुक्तम् == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, परिमाण *usvars);
  usvars[usvars_प्रयुक्तम्++] = var;
}

स्थिर शून्य
unset_envvars (शून्य)
{
  प्रति (idx_t i = 0; i < usvars_प्रयुक्तम्; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      यदि (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
स्थिर bool
प्रविष्टिः_has_name (वर्ण ध्रुव *प्रविष्टिः, वर्ण ध्रुव *name, idx_t name_दीर्घता)
{
  वर्ण ध्रुव *eq = strchr (प्रविष्टिः, '=');
  प्रत्यागम (eq && eq - प्रविष्टिः == name_दीर्घता
          && memeq (प्रविष्टिः, name, name_दीर्घता));
}

#यदि ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
स्थिर size_t
env_सदिशम्_slot_hash (शून्य ध्रुव *x, size_t सारणी_आकार)
{
  वर्ण *ध्रुव *slot = x;
  वर्ण ध्रुव *प्रविष्टिः = *slot;
  वर्ण ध्रुव *eq = strchr (प्रविष्टिः, '=');
  प्रत्यागम hash_pjw_bare (प्रविष्टिः, eq - प्रविष्टिः) % सारणी_आकार;
}

स्थिर bool
env_सदिशम्_slot_compare (शून्य ध्रुव *x, शून्य ध्रुव *y)
{
  वर्ण ध्रुव *a = * (वर्ण *ध्रुव *) x;
  वर्ण ध्रुव *b = * (वर्ण *ध्रुव *) y;
  वर्ण ध्रुव *b_eq = strchr (b, '=');
  प्रत्यागम प्रविष्टिः_has_name (a, b, b_eq - b);
}
#यदि_अन्तः

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
स्थिर शून्य
unset_envvars_अन्तर्_सदिशम् (संरचना env_सदिशम् *env)
{
  प्रति (idx_t i = 0; i < usvars_प्रयुक्तम्; ++i)
    {
      वर्ण ध्रुव *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      यदि (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_दीर्घता = वाक्यदैर्घ्यम् (name);
      idx_t dest = 0;
      प्रति (idx_t source = 0; source < env->प्रयुक्तम्; ++source)
        यदि (! प्रविष्टिः_has_name (env->variable[source], name, name_दीर्घता))
          env->variable[dest++] = env->variable[source];

      env->प्रयुक्तम् = dest;
      env->variable[dest] = रिक्त;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
स्थिर शून्य
समुच्चयः_envvar (वर्ण *assignment, वर्ण *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  यदि (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
स्थिर शून्य
समुच्चयः_envvar_अन्तर्_सदिशम् (संरचना env_सदिशम् *env, वर्ण *assignment, वर्ण *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_दीर्घता = eq - assignment;
  प्रति (idx_t i = 0; i < env->प्रयुक्तम्; ++i)
    यदि (प्रविष्टिः_has_name (env->variable[i], assignment, name_दीर्घता))
      {
        env->variable[i] = assignment;
        प्रत्यागम;
      }

  यदि (env->प्रयुक्तम् + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               परिमाण *env->variable);
      environ = env->variable;
    }

  env->variable[env->प्रयुक्तम्++] = assignment;
  env->variable[env->प्रयुक्तम्] = रिक्त;
}

/* Read the NUL-delimited environment entries in FILE.  */
स्थिर size_t
read_env_file (वर्ण ध्रुव *file)
{
  size_t आकार;
  यदि (streq (file, "-"))
    {
      xset_द्विमान_बहुलकम् (STDIN_FILENO, O_द्विमान);
      env0_from_बफर = fread_file (stdin, RF_द्विमान, &आकार);
    }
  अन्यथा
    env0_from_बफर = read_file (file, RF_द्विमान, &आकार);

  यदि (! env0_from_बफर)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  यदि (आकार && env0_from_बफर[आकार - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  प्रत्यागम आकार;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
स्थिर bool
समुच्चयः_envvars_from_file (संरचना env_सदिशम् *env, वर्ण ध्रुव *file)
{
  size_t आकार = read_env_file (file);

#यदि defined _WIN32 && ! defined __CYGWIN__
  (शून्य) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  वर्ण *assignment = रिक्त;
  यावत् ((assignment = argz_अग्र (env0_from_बफर, आकार, assignment)))
    {
      वर्ण *eq = strchr (assignment, '=');
      यदि (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      समुच्चयः_envvar (assignment, eq);
    }
  प्रत्यागम false;
#अन्यथा
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_गणनम् = argz_गणनम् (env0_from_बफर, आकार);

  यदि (file_गणनम् == 0)
    प्रत्यागम false;

  idx_t inherited_गणनम् = 0;
  यावत् (environ[inherited_गणनम्])
    ++inherited_गणनम्;

  यदि (IDX_MAX - inherited_गणनम् <= file_गणनम्)
    xalloc_die ();
  idx_t max_गणनम् = inherited_गणनम् + file_गणनम्;
  वर्ण **merged = xnmalloc (max_गणनम् + 1, परिमाण *merged);
  स्मृतिं_प्रतिलिख (merged, environ, inherited_गणनम् * परिमाण *merged);
  merged[inherited_गणनम्] = रिक्त;

  Hash_सारणी *slot_सारणी
    = hash_initialize (max_गणनम्, रिक्त, env_सदिशम्_slot_hash,
                       env_सदिशम्_slot_compare, रिक्त);
  यदि (! slot_सारणी)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  प्रति (idx_t i = 0; i < inherited_गणनम्; ++i)
    {
      वर्ण *eq = strchr (merged[i], '=');
      यदि (! eq)
        अनुवर्त;

      यदि (! hash_निवेशय (slot_सारणी, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_गणनम् = inherited_गणनम्;
  वर्ण *assignment = रिक्त;
  यावत् ((assignment = argz_अग्र (env0_from_बफर, आकार, assignment)))
    {
      वर्ण *eq = strchr (assignment, '=');
      यदि (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_गणनम्++] = assignment;
        }
      अन्यथा
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          वर्ण **slot = hash_lookup (slot_सारणी, &assignment);
          यदि (slot)
            *slot = assignment;
          अन्यथा
            {
              merged[merged_गणनम्] = assignment;
              यदि (! hash_निवेशय (slot_सारणी, &merged[merged_गणनम्]))
                xalloc_die ();
              ++merged_गणनम्;
            }
        }
    }

  merged[merged_गणनम्] = रिक्त;
  env->variable = merged;
  env->प्रयुक्तम् = merged_गणनम्;
  env->allocated = max_गणनम् + 1;
  environ = merged;
  hash_free (slot_सारणी);
  प्रत्यागम true;
#यदि_अन्तः
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
स्थिर शून्य
समुच्चयः_raw_environment_from_file (संरचना env_सदिशम् *env,
                               वर्ण ध्रुव *file)
{
  size_t आकार = read_env_file (file);
  idx_t प्रविष्टिः_गणनम् = argz_गणनम् (env0_from_बफर, आकार);

  env->allocated = प्रविष्टिः_गणनम् + 1;
  env->variable = xnmalloc (env->allocated, परिमाण *env->variable);
  argz_extract (env0_from_बफर, आकार, env->variable);
  env->प्रयुक्तम् = प्रविष्टिः_गणनम्;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_विशुद्धम्
स्थिर वर्ण ध्रुव *
अवलोकनम्_varname (वर्ण ध्रुव *str)
{
  यदि (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      वर्ण ध्रुव *अन्तः = str + 3;
      यावत् (c_isalnum (*अन्तः) || *अन्तः == '_')
        ++अन्तः;
      यदि (*अन्तः == '}')
        प्रत्यागम अन्तः;
    }

  प्रत्यागम रिक्त;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
स्थिर वर्ण *
extract_varname (वर्ण ध्रुव *str)
{
  वर्ण ध्रुव *p = अवलोकनम्_varname (str);

  यदि (!p)
    प्रत्यागम रिक्त;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  यदि (i >= vnlen)
    {
      मुक्त (varname);
      varname = xpalloc (रिक्त, &vnlen, i + 1 - vnlen, -1, परिमाण *varname);
    }

  स्मृतिं_प्रतिलिख (varname, str + 2, i);
  varname[i] = 0;

  प्रत्यागम varname;
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
  वर्ण **argv;
  पूर्णाङ्क argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  पूर्णाङ्क extra_argc;

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
  idx_t string_अष्टकानि = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (अधिकतम_पूर्णाङ्कः, IDX_MAX), 2 * परिमाण *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_अष्टकानि);
}

/* In SS, append C to the last string.  */
स्थिर शून्य
splitbuf_append_byte (संरचना splitbuf *ss, वर्ण c)
{
  idx_t string_अष्टकानि = (intptr_t) ss->argv[ss->argc];
  यदि (ss->half_alloc * परिमाण *ss->argv <= string_अष्टकानि)
    splitbuf_grow (ss);
  ((वर्ण *) (ss->argv + ss->half_alloc))[string_अष्टकानि] = c;
  ss->argv[ss->argc] = (वर्ण *) (intptr_t) {string_अष्टकानि + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
स्थिर शून्य
check_start_new_arg (संरचना splitbuf *ss)
{
  यदि (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      पूर्णाङ्क argc = ss->argc;
      यदि (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
स्थिर वर्ण **
splitbuf_finishup (संरचना splitbuf *ss)
{
  पूर्णाङ्क argc = ss->argc;
  वर्ण **argv = ss->argv;
  वर्ण *stringbase = (वर्ण *) (ss->argv + ss->half_alloc);
  प्रति (पूर्णाङ्क i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  प्रत्यागम argv;
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
स्थिर वर्ण **
build_argv (वर्ण ध्रुव *str, पूर्णाङ्क extra_argc, पूर्णाङ्क *argc)
{
  bool dq = false, sq = false;
  संरचना splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * परिमाण *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = रिक्त;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  यावत् (*str)
    {
      वर्ण newc = *str; /* Default: add the next character.  */

      विकल्प (*str)
        {
        पक्ष '\'':
          यदि (dq)
            भङ्ग;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          अनुवर्त;

        पक्ष '"':
          यदि (sq)
            भङ्ग;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          अनुवर्त;

        पक्ष ' ': पक्ष '\t': पक्ष '\n': पक्ष '\v': पक्ष '\f': पक्ष '\r':
          /* Start a new argument if outside quotes.  */
          यदि (sq || dq)
            भङ्ग;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          अनुवर्त;

        पक्ष '#':
          यदि (!ss.sep)
            भङ्ग;
          गच्छ eos; /* '#' as first char terminates the string.  */

        पक्ष '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          यदि (sq && str[1] != '\\' && str[1] != '\'')
            भङ्ग;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          विकल्प (newc)
            {
            पक्ष '"': पक्ष '#': पक्ष '$': पक्ष '\'': पक्ष '\\':
              /* Pass escaped character as-is.  */
              भङ्ग;

            पक्ष '_':
              यदि (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  अनुवर्त;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              भङ्ग;

            पक्ष 'c':
              यदि (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              गच्छ eos; /* '\c' terminates the string.  */

            पक्ष 'f': newc = '\f'; भङ्ग;
            पक्ष 'n': newc = '\n'; भङ्ग;
            पक्ष 'r': newc = '\r'; भङ्ग;
            पक्ष 't': newc = '\t'; भङ्ग;
            पक्ष 'v': newc = '\v'; भङ्ग;

            पक्ष '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            स्वभाव:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          भङ्ग;

        पक्ष '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          यदि (sq)
            भङ्ग;

          /* Store the ${VARNAME} value.  */
          {
            वर्ण *n = extract_varname (str);
            यदि (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            वर्ण *v = getenv (n);
            यदि (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                प्रति (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            अन्यथा
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            अनुवर्त;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  यदि (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  प्रत्यागम splitbuf_finishup (&ss);
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
parse_विभज_string (वर्ण ध्रुव *str, पूर्णाङ्क *orig_optind,
                    पूर्णाङ्क *orig_argc, वर्ण ***orig_argv)
{
  पूर्णाङ्क extra_argc = *orig_argc - *orig_optind, newargc;
  वर्ण **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  यदि (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      प्रति (पूर्णाङ्क i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  स्मृतिं_प्रतिलिख (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * परिमाण *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

स्थिर शून्य
parse_signal_action_params (वर्ण ध्रुव *arg, bool समुच्चयः_default)
{
  यदि (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      प्रति (पूर्णाङ्क i = 1 ; i <= SIGNUM_BOUND; i++)
        सङ्केताः[i] = समुच्चयः_default ? DEFAULT_NOERR : IGNORE_NOERR;
      प्रत्यागम;
    }

  वर्ण *optarg_writable = xstrdup (arg);

  वर्ण *opt_sig = strtok (optarg_writable, ",");
  यावत् (opt_sig)
    {
      पूर्णाङ्क signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      यदि (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      यदि (signum <= 0)
        usage (exit_failure);

      सङ्केताः[signum] = समुच्चयः_default ? DEFAULT : IGNORE;

      opt_sig = strtok (रिक्त, ",");
    }

  मुक्त (optarg_writable);
}

स्थिर शून्य
reset_signal_handlers (शून्य)
{
  प्रति (पूर्णाङ्क i = 1; i <= SIGNUM_BOUND; i++)
    {
      संरचना sigaction act;

      यदि (सङ्केताः[i] == UNCHANGED)
        अनुवर्त;

      bool ignore_errors = (सङ्केताः[i] == DEFAULT_NOERR
                            || सङ्केताः[i] == IGNORE_NOERR);

      bool समुच्चयः_गन्तव्यम्_default = (सङ्केताः[i] == DEFAULT
                             || सङ्केताः[i] == DEFAULT_NOERR);

      पूर्णाङ्क sig_err = sigaction (i, रिक्त, &act);

      यदि (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      यदि (! sig_err)
        {
          act.sa_handler = समुच्चयः_गन्तव्यम्_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, रिक्त);
          यदि (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      यदि (dev_debug)
        {
          वर्ण signame[SIG2STR_MAX];
          यदि (sig2str (i, signame) != 0)
            snprintf (signame, परिमाण signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  समुच्चयः_गन्तव्यम्_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


स्थिर शून्य
parse_खण्डम्_signal_params (वर्ण ध्रुव *arg, bool खण्डम्)
{
  यदि (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (खण्डम् ? &खण्डम्_सङ्केताः : &unblock_सङ्केताः);
      sigemptyset (खण्डम् ? &unblock_सङ्केताः : &खण्डम्_सङ्केताः);
    }
  अन्यथा यदि (! sig_आवरणम्_परिवर्तितम्)
    {
      /* Initialize the sets.  */
      sigemptyset (&खण्डम्_सङ्केताः);
      sigemptyset (&unblock_सङ्केताः);
    }

  sig_आवरणम्_परिवर्तितम् = true;

  यदि (! arg)
    प्रत्यागम;

  वर्ण *optarg_writable = xstrdup (arg);

  वर्ण *opt_sig = strtok (optarg_writable, ",");
  यावत् (opt_sig)
    {
      पूर्णाङ्क signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      यदि (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      यदि (signum <= 0)
        usage (exit_failure);

      यदि (sigaddset (खण्डम् ? &खण्डम्_सङ्केताः : &unblock_सङ्केताः, signum) == -1)
        {
          यदि (खण्डम्)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      अन्यथा
        sigdelset (खण्डम् ? &unblock_सङ्केताः : &खण्डम्_सङ्केताः, signum);

      opt_sig = strtok (रिक्त, ",");
    }

  मुक्त (optarg_writable);
}

स्थिर शून्य
समुच्चयः_signal_proc_आवरणम् (शून्य)
{
  /* Get the existing signal mask */
  sigset_t समुच्चयः_2;

  sigemptyset (&समुच्चयः_2);

  यदि (sigprocmask (0, रिक्त, &समुच्चयः_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  प्रति (पूर्णाङ्क i = 1; i <= SIGNUM_BOUND; i++)
    {
      वर्ण ध्रुव *debug_act = रिक्त;

      यदि (sigismember (&खण्डम्_सङ्केताः, i))
        {
          sigaddset (&समुच्चयः_2, i);
          debug_act = "BLOCK";
        }
      अन्यथा यदि (sigismember (&unblock_सङ्केताः, i))
        {
          sigdelset (&समुच्चयः_2, i);
          debug_act = "UNBLOCK";
        }

      यदि (dev_debug && debug_act)
        {
          वर्ण signame[SIG2STR_MAX];
          यदि (sig2str (i, signame) != 0)
            snprintf (signame, परिमाण signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  यदि (sigprocmask (SIG_SETMASK, &समुच्चयः_2, रिक्त))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

स्थिर शून्य
list_signal_handling (शून्य)
{
  sigset_t समुच्चयः_2;

  sigemptyset (&समुच्चयः_2);
  यदि (sigprocmask (0, रिक्त, &समुच्चयः_2))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  प्रति (पूर्णाङ्क i = 1; i <= SIGNUM_BOUND; i++)
    {
      संरचना sigaction act;
      यदि (sigaction (i, रिक्त, &act))
        अनुवर्त;

      वर्ण ध्रुव *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      वर्ण ध्रुव *blocked = sigismember (&समुच्चयः_2, i) ? "BLOCK" : "";
      वर्ण ध्रुव *connect = *ignored && *blocked ? "," : "";

      यदि (! *ignored && ! *blocked)
        अनुवर्त;

      वर्ण signame[SIG2STR_MAX];
      यदि (sig2str (i, signame) != 0)
        snprintf (signame, परिमाण signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

स्थिर शून्य
initialize_सङ्केताः (शून्य)
{
  सङ्केताः = xmalloc ((परिमाण *सङ्केताः) * (SIGNUM_BOUND + 1));

  प्रति (पूर्णाङ्क i = 0 ; i <= SIGNUM_BOUND; i++)
    सङ्केताः[i] = UNCHANGED;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  पूर्णाङ्क quoting_style_opt = -1;
  वर्ण ध्रुव *newdir = रिक्त;
  वर्ण ध्रुव *env0_from_file = रिक्त;
  वर्ण *argv0 = रिक्त;
  संरचना env_सदिशम् env_सदिशम्;
#यदि_परिभाषितम् lint
  स्मृतिं_पूरय (&env_सदिशम्, 0, परिमाण env_सदिशम्);
  वर्ण **initial_environ = environ;
#यदि_अन्तः

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_सङ्केताः ();

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
          argv0 = optarg;
          भङ्ग;
        पक्ष 'i':
          ignore_environment = true;
          भङ्ग;
        पक्ष 'u':
          append_unset_var (optarg);
          भङ्ग;
        पक्ष 'v':
          dev_debug = true;
          भङ्ग;
        पक्ष '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          भङ्ग;
        पक्ष ENV0_FROM_OPTION:
          env0_from_file = optarg;
          भङ्ग;
        पक्ष DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_खण्डम्_signal_params (optarg, false);
          भङ्ग;
        पक्ष IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          भङ्ग;
        पक्ष खण्डम्_SIGNAL_OPTION:
          parse_खण्डम्_signal_params (optarg, true);
          भङ्ग;
        पक्ष LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          भङ्ग;
        पक्ष QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_तर्काः,
                                         quoting_style_vals);
          भङ्ग;
        पक्ष 'C':
          newdir = optarg;
          भङ्ग;
        पक्ष 'S':
          parse_विभज_string (optarg, &optind, &argc, &argv);
          भङ्ग;
        पक्ष ' ': पक्ष '\t': पक्ष '\n': पक्ष '\v': पक्ष '\f': पक्ष '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            वर्ण c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_CANCELED);
        }
    }

  यदि (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  पूर्णाङ्क क्रमादेशः_सूचकांक = optind;
  यावत् (क्रमादेशः_सूचकांक < argc && strchr (argv[क्रमादेशः_सूचकांक], '='))
    क्रमादेशः_सूचकांक++;
  bool क्रमादेशः_specified = क्रमादेशः_सूचकांक < argc;

  bool quote_output = false;
  पूर्णाङ्क qs = quoting_style_opt;
  यदि (! क्रमादेशः_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
    }
  यदि (0 <= qs && qs != literal_quoting_style)
    {
      समुच्चयः_quoting_style (रिक्त, qs);
      quote_output = true;
    }

  bool env_सदिशम्_सक्रिय = false;
  यदि (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      समुच्चयः_raw_environment_from_file (&env_सदिशम्, env0_from_file);
      env_सदिशम्_सक्रिय = true;
    }
  अन्यथा यदि (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#यदि defined _WIN32 && ! defined __CYGWIN__
      स्थिर वर्ण *dummy_environ[] = { रिक्त };
      environ = dummy_environ;
#अन्यथा
      env_सदिशम्.variable = xnmalloc (1, परिमाण *env_सदिशम्.variable);
      env_सदिशम्.variable[0] = रिक्त;
      env_सदिशम्.प्रयुक्तम् = 0;
      env_सदिशम्.allocated = 1;
      environ = env_सदिशम्.variable;
      env_सदिशम्_सक्रिय = true;
#यदि_अन्तः
    }
  अन्यथा यदि (env0_from_file)
    env_सदिशम्_सक्रिय = समुच्चयः_envvars_from_file (&env_सदिशम्, env0_from_file);

  यदि (! ignore_environment || env0_from_file)
    {
      यदि (env_सदिशम्_सक्रिय)
        unset_envvars_अन्तर्_सदिशम् (&env_सदिशम्);
      अन्यथा
        unset_envvars ();
    }

  वर्ण *eq;
  यावत् (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      यदि (env_सदिशम्_सक्रिय)
        समुच्चयः_envvar_अन्तर्_सदिशम् (&env_सदिशम्, argv[optind], eq);
      अन्यथा
        समुच्चयः_envvar (argv[optind], eq);
      optind++;
    }

  यदि (opt_nul_terminate_output && क्रमादेशः_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  यदि (newdir && ! क्रमादेशः_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  यदि (argv0 && ! क्रमादेशः_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  यदि (! क्रमादेशः_specified)
    {
      वर्ण ध्रुव terminator = opt_nul_terminate_output ? '\0' : '\n';

      यदि (opt_nul_terminate_output)
        xset_द्विमान_बहुलकम् (STDOUT_FILENO, O_द्विमान);

      /* Print the environment and exit.  */
      प्रति (वर्ण *ध्रुव *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#यदि_परिभाषितम् lint
      यदि (env_सदिशम्_सक्रिय)
        {
          environ = initial_environ;
          मुक्त (env_सदिशम्.variable);
          मुक्त (env0_from_बफर);
        }
#यदि_अन्तः
      प्रत्यागम EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  यदि (sig_आवरणम्_परिवर्तितम्)
    समुच्चयः_signal_proc_आवरणम् ();

  यदि (report_signal_handling)
    list_signal_handling ();

  यदि (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      यदि (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  वर्ण *क्रमादेशः = argv[optind];
  यदि (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  यदि (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (क्रमादेशः));
      प्रति (पूर्णाङ्क i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (क्रमादेशः, &argv[optind]);

  पूर्णाङ्क exit_स्थितिः = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (क्रमादेशः));

  यदि (exit_स्थितिः == EXIT_ENOENT && strpbrk (क्रमादेशः, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_स्थितिः);
}
