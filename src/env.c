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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <argz.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <c-ctype.h>
#অন্তর্ভাব <signal.h>

#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "hashcode-mem.h"
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "operand2sig.h"
#অন্তর্ভাব "printenv.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "read-file.h"
#অন্তর্ভাব "sig2str.h"
#অন্তর্ভাব "xbinary-io.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "env"

#define AUTHORS \
  proper_name ("Richard Mlynarik"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Assaf Gordon")

/* Array of envvars to unset.  */
স্থির অক্ষর ধ্রুবক **usvars;
স্থির idx_t usvars_alloc;
স্থির idx_t usvars_used;

/* Storage for entries read by --env0-from.  */
স্থির অক্ষর *env0_from_বাফার;

/* An environment vector managed directly to preserve raw entries.  */
কাঠামো env_vector
{
  অক্ষর **variable;
  idx_t used;
  idx_t allocated;
};

/* Annotate the output with extra info to aid the user.  */
স্থির bool dev_debug;
#define vmsg devmsg

/* Buffer and length of extracted envvars in -S strings.  */
স্থির অক্ষর *varname;
স্থির idx_t vnlen;

/* On mingw, winbase.h defines this to zero.  Undefine it so we can use it
   in the enum.  */
#undef IGNORE

/* Possible actions on each signal.  */
গণনা SIGNAL_MODE {
  UNCHANGED = 0,
  DEFAULT,       /* Set to default handler (SIG_DFL).  */
  DEFAULT_NOERR, /* Ditto, but ignore sigaction(2) errors.  */
  IGNORE,        /* Set to ignore (SIG_IGN).  */
  IGNORE_NOERR   /* Ditto, but ignore sigaction(2) errors.  */
};
স্থির গণনা SIGNAL_MODE *signals;

/* Set of signals to block.  */
স্থির sigset_t block_signals;

/* Set of signals to unblock.  */
স্থির sigset_t unblock_signals;

/* Whether signal mask adjustment requested.  */
স্থির bool sig_mask_changed;

/* Whether to list non default handling.  */
স্থির bool report_signal_handling;

/* The isspace characters in the C locale.  */
#define C_ISSPACE_CHARS " \t\n\v\f\r"

স্থির অক্ষর ধ্রুবক shortopts[] = "+a:C:iS:u:v0" C_ISSPACE_CHARS;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  ENV0_FROM_OPTION = CHAR_MAX + 1,
  DEFAULT_SIGNAL_OPTION,
  IGNORE_SIGNAL_OPTION,
  BLOCK_SIGNAL_OPTION,
  LIST_SIGNAL_HANDLING_OPTION,
  QUOTING_STYLE_OPTION,
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"argv0", required_argument, ফাঁকা, 'a'},
  {"ignore-environment", no_argument, ফাঁকা, 'i'},
  {"env0-from", required_argument, ফাঁকা, ENV0_FROM_OPTION},
  {"null", no_argument, ফাঁকা, '0'},
  {"unset", required_argument, ফাঁকা, 'u'},
  {"chdir", required_argument, ফাঁকা, 'C'},
  {"default-signal", optional_argument, ফাঁকা, DEFAULT_SIGNAL_OPTION},
  {"ignore-signal",  optional_argument, ফাঁকা, IGNORE_SIGNAL_OPTION},
  {"block-signal",   optional_argument, ফাঁকা, BLOCK_SIGNAL_OPTION},
  {"list-signal-handling", no_argument, ফাঁকা,  LIST_SIGNAL_HANDLING_OPTION},
  {"debug", no_argument, ফাঁকা, 'v'},
  {"quoting-style", required_argument, ফাঁকা, QUOTING_STYLE_OPTION},
  {"split-string", required_argument, ফাঁকা, 'S'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [-] [NAME=মান_2]... [COMMAND [ARG]...]\n"),
              program_name);
      fputs (_("\
Set each NAME to মান_2 in the environment and run COMMAND.\n\
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
         পঠ NUL-delimited environment entries from FILE\n\
"));
      oputs (_("\
  -0, --null\n\
         end each output সারি with NUL, not newline\n\
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
         use quoting style WORD জন্য names and মানসমূহ:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (_("\
  -S, --split-string=S\n\
         process and split S into separate arguments;\n\
         used to pass multiple arguments on shebang সারিগুলি\n\
"));
      oputs (_("\
      --block-সংকেত[=SIG]\n\
         block delivery of SIG সংকেত(s) to COMMAND\n\
"));
      oputs (_("\
      --পূর্বনির্ধারিত-সংকেত[=SIG]\n\
         reset handling of SIG সংকেত(s) to the পূর্বনির্ধারিত\n\
"));
      oputs (_("\
      --ignore-সংকেত[=SIG]\n\
         set handling of SIG সংকেত(s) to করো nothing\n\
"));
      oputs (_("\
      --list-সংকেত-handling\n\
         list non পূর্বনির্ধারিত সংকেত handling to standard error\n\
"));
      oputs (_("\
  -v, --debug\n\
         print verbose information জন্য each processing step\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
A mere - implies -i.  If no COMMAND, print the resulting environment.\n\
"), stdout);
      fputs (_("\
\n\
SIG may be a সংকেত name like 'PIPE', or a সংকেত সংখ্যা like '13'.\n\
Without SIG, all known signals are included.  Multiple signals can be\n\
comma-separated.  An empty SIG argument is a no-op.\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

স্থির শূন্য
append_unset_var (অক্ষর ধ্রুবক *var)
{
  যদি (usvars_used == usvars_alloc)
    usvars = xpalloc (usvars, &usvars_alloc, 1, -1, আকার *usvars);
  usvars[usvars_used++] = var;
}

স্থির শূন্য
unset_envvars (শূন্য)
{
  জন্য (idx_t i = 0; i < usvars_used; ++i)
    {
      vmsg (_("unsetting: %s\n"), quote (usvars[i]));

      যদি (unsetenv (usvars[i]))
        error (EXIT_CANCELED, errno, _("cannot unset %s"),
               quote (usvars[i]));
    }
}

/* Return true if ENTRY is an assignment whose name is NAME.  */
স্থির bool
entry_has_name (অক্ষর ধ্রুবক *entry, অক্ষর ধ্রুবক *name, idx_t name_দৈর্ঘ্য)
{
  অক্ষর ধ্রুবক *eq = strchr (entry, '=');
  ফেরত (eq && eq - entry == name_দৈর্ঘ্য
          && memeq (entry, name, name_দৈর্ঘ্য));
}

#যদি ! defined _WIN32 || defined __CYGWIN__
/* Hash an environment vector slot by the name in its assignment.  */
স্থির size_t
env_vector_slot_hash (শূন্য ধ্রুবক *x, size_t table_আকার)
{
  অক্ষর *ধ্রুবক *slot = x;
  অক্ষর ধ্রুবক *entry = *slot;
  অক্ষর ধ্রুবক *eq = strchr (entry, '=');
  ফেরত hash_pjw_bare (entry, eq - entry) % table_আকার;
}

স্থির bool
env_vector_slot_compare (শূন্য ধ্রুবক *x, শূন্য ধ্রুবক *y)
{
  অক্ষর ধ্রুবক *a = * (অক্ষর *ধ্রুবক *) x;
  অক্ষর ধ্রুবক *b = * (অক্ষর *ধ্রুবক *) y;
  অক্ষর ধ্রুবক *b_eq = strchr (b, '=');
  ফেরত entry_has_name (a, b, b_eq - b);
}
#endif

/* Remove the requested variables from ENV,
   directly, preserving other raw entries.  */
স্থির শূন্য
unset_envvars_in_vector (কাঠামো env_vector *env)
{
  জন্য (idx_t i = 0; i < usvars_used; ++i)
    {
      অক্ষর ধ্রুবক *name = usvars[i];
      vmsg (_("unsetting: %s\n"), quote (name));

      যদি (! *name || strchr (name, '='))
        error (EXIT_CANCELED, EINVAL, _("cannot unset %s"), quoteaf (name));

      idx_t name_দৈর্ঘ্য = strlen (name);
      idx_t dest = 0;
      জন্য (idx_t source = 0; source < env->used; ++source)
        যদি (! entry_has_name (env->variable[source], name, name_দৈর্ঘ্য))
          env->variable[dest++] = env->variable[source];

      env->used = dest;
      env->variable[dest] = ফাঁকা;
    }
}

/* Add ASSIGNMENT to the environment.  EQ points to its '=' byte.  */
স্থির শূন্য
set_envvar (অক্ষর *assignment, অক্ষর *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  যদি (putenv (assignment))
    {
      *eq = '\0';
      error (EXIT_CANCELED, errno, _("cannot set %s"), quote (assignment));
    }
}

/* Add ASSIGNMENT to ENV directly, preserving other raw entries.
   EQ points to its '=' byte.  Replace the first existing assignment
   with the same name, as putenv does, or append ASSIGNMENT if none.  */
স্থির শূন্য
set_envvar_in_vector (কাঠামো env_vector *env, অক্ষর *assignment, অক্ষর *eq)
{
  vmsg (_("setting:   %s\n"), quote (assignment));

  idx_t name_দৈর্ঘ্য = eq - assignment;
  জন্য (idx_t i = 0; i < env->used; ++i)
    যদি (entry_has_name (env->variable[i], assignment, name_দৈর্ঘ্য))
      {
        env->variable[i] = assignment;
        ফেরত;
      }

  যদি (env->used + 1 == env->allocated)
    {
      env->variable = xpalloc (env->variable, &env->allocated, 1, -1,
                               আকার *env->variable);
      environ = env->variable;
    }

  env->variable[env->used++] = assignment;
  env->variable[env->used] = ফাঁকা;
}

/* Read the NUL-delimited environment entries in FILE.  */
স্থির size_t
read_env_file (অক্ষর ধ্রুবক *file)
{
  size_t আকার_2;
  যদি (streq (file, "-"))
    {
      xset_দ্বিমিক_mode (STDIN_FILENO, O_দ্বিমিক);
      env0_from_বাফার = fread_file (stdin, RF_দ্বিমিক, &আকার_2);
    }
  অন্যথা
    env0_from_বাফার = read_file (file, RF_দ্বিমিক, &আকার_2);

  যদি (! env0_from_বাফার)
    error (EXIT_CANCELED, errno, _("cannot read %s"), quoteaf (file));
  যদি (আকার_2 && env0_from_বাফার[আকার_2 - 1] != '\0')
    error (EXIT_CANCELED, 0, _("%s: file must end with a NUL byte"),
           quotef (file));

  ফেরত আকার_2;
}

/* Merge the NUL-delimited entries in FILE into the environment.
   Return true if ENV now owns the environment vector.  */
স্থির bool
set_envvars_from_file (কাঠামো env_vector *env, অক্ষর ধ্রুবক *file)
{
  size_t আকার_2 = read_env_file (file);

#যদি defined _WIN32 && ! defined __CYGWIN__
  (শূন্য) env;
  /* On native Windows, _putenv keeps _environ, _wenviron, and the process
     environment block synchronized.  Replacing environ directly would
     conflict with its ownership of those data structures.  */
  অক্ষর *assignment = ফাঁকা;
  যখন ((assignment = argz_পরবর্তী (env0_from_বাফার, আকার_2, assignment)))
    {
      অক্ষর *eq = strchr (assignment, '=');
      যদি (! eq)
        error (EXIT_CANCELED, 0,
               _("invalid variable specification %s in %s"),
               quoteaf_n (0, assignment), quoteaf_n (1, file));

      set_envvar (assignment, eq);
    }
  ফেরত false;
#অন্যথা
  /* Construct and index the merged vector, rather than making putenv
     repeatedly scan the growing environment for every assignment.  */
  idx_t file_গুনতি = argz_গুনতি (env0_from_বাফার, আকার_2);

  যদি (file_গুনতি == 0)
    ফেরত false;

  idx_t inherited_গুনতি = 0;
  যখন (environ[inherited_গুনতি])
    ++inherited_গুনতি;

  যদি (IDX_MAX - inherited_গুনতি <= file_গুনতি)
    xalloc_die ();
  idx_t max_গুনতি = inherited_গুনতি + file_গুনতি;
  অক্ষর **merged = xnmalloc (max_গুনতি + 1, আকার *merged);
  memcpy (merged, environ, inherited_গুনতি * আকার *merged);
  merged[inherited_গুনতি] = ফাঁকা;

  Hash_table *slot_table
    = hash_initialize (max_গুনতি, ফাঁকা, env_vector_slot_hash,
                       env_vector_slot_compare, ফাঁকা);
  যদি (! slot_table)
    xalloc_die ();

  /* Index only the first occurrence of each inherited name, matching
     putenv's replacement of the first such entry.  */
  জন্য (idx_t i = 0; i < inherited_গুনতি; ++i)
    {
      অক্ষর *eq = strchr (merged[i], '=');
      যদি (! eq)
        চালু;

      যদি (! hash_ঢোকাও (slot_table, &merged[i]))
        xalloc_die ();
    }

  idx_t merged_গুনতি = inherited_গুনতি;
  অক্ষর *assignment = ফাঁকা;
  যখন ((assignment = argz_পরবর্তী (env0_from_বাফার, আকার_2, assignment)))
    {
      অক্ষর *eq = strchr (assignment, '=');
      যদি (! eq)
        {
          vmsg (_("adding:    %s\n"), quote (assignment));
          merged[merged_গুনতি++] = assignment;
        }
      অন্যথা
        {
          vmsg (_("setting:   %s\n"), quote (assignment));

          অক্ষর **slot = hash_lookup (slot_table, &assignment);
          যদি (slot)
            *slot = assignment;
          অন্যথা
            {
              merged[merged_গুনতি] = assignment;
              যদি (! hash_ঢোকাও (slot_table, &merged[merged_গুনতি]))
                xalloc_die ();
              ++merged_গুনতি;
            }
        }
    }

  merged[merged_গুনতি] = ফাঁকা;
  env->variable = merged;
  env->used = merged_গুনতি;
  env->allocated = max_গুনতি + 1;
  environ = merged;
  hash_free (slot_table);
  ফেরত true;
#endif
}

/* Replace the environment with the NUL-delimited entries in FILE,
   preserving the entries byte-for-byte and in their original order.  */
স্থির শূন্য
set_raw_environment_from_file (কাঠামো env_vector *env,
                               অক্ষর ধ্রুবক *file)
{
  size_t আকার_2 = read_env_file (file);
  idx_t entry_গুনতি = argz_গুনতি (env0_from_বাফার, আকার_2);

  env->allocated = entry_গুনতি + 1;
  env->variable = xnmalloc (env->allocated, আকার *env->variable);
  argz_extract (env0_from_বাফার, আকার_2, env->variable);
  env->used = entry_গুনতি;
  environ = env->variable;
}

/* Return a pointer to the end of a valid ${VARNAME} string, or NULL.
   'str' should point to the '$' character.
   First letter in VARNAME must be alpha or underscore,
   rest of letters are alnum or underscore.
   Any other character is an error.  */
ATTRIBUTE_PURE
স্থির অক্ষর ধ্রুবক *
scan_varname (অক্ষর ধ্রুবক *str)
{
  যদি (str[1] == '{' && (c_isalpha (str[2]) || str[2] == '_'))
    {
      অক্ষর ধ্রুবক *end = str + 3;
      যখন (c_isalnum (*end) || *end == '_')
        ++end;
      যদি (*end == '}')
        ফেরত end;
    }

  ফেরত ফাঁকা;
}

/* Return a pointer to a static buffer containing the VARNAME as
   extracted from a '${VARNAME}' string.
   The returned string will be NUL terminated.
   The returned pointer should not be freed.
   Return NULL if not a valid ${VARNAME} syntax.  */
স্থির অক্ষর *
extract_varname (অক্ষর ধ্রুবক *str)
{
  অক্ষর ধ্রুবক *p = scan_varname (str);

  যদি (!p)
    ফেরত ফাঁকা;

  /* -2 and +2 (below) account for the '${' prefix.  */
  idx_t i = p - str - 2;

  যদি (i >= vnlen)
    {
      মুক্ত (varname);
      varname = xpalloc (ফাঁকা, &vnlen, i + 1 - vnlen, -1, আকার *varname);
    }

  memcpy (varname, str + 2, i);
  varname[i] = 0;

  ফেরত varname;
}

/* Temporary buffer used by --split-string processing.  */
কাঠামো splitbuf
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
  অক্ষর **argv;
  পূর্ণসংখ্যা argc;
  idx_t half_alloc;

  /* The number of extra argv slots to keep room for.  */
  পূর্ণসংখ্যা extra_argc;

  /* Whether processing should act as if the most recent character
     seen was a separator.  */
  bool sep;
};

/* Expand SS so that it has at least one more argv slot and at least
   one more string byte.  */
স্থির শূন্য
splitbuf_grow (কাঠামো splitbuf *ss)
{
  idx_t old_half_alloc = ss->half_alloc;
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  ss->argv = xpalloc (ss->argv, &ss->half_alloc, 1,
                      MIN (INT_MAX, IDX_MAX), 2 * আকার *ss->argv);
  memmove (ss->argv + ss->half_alloc, ss->argv + old_half_alloc, string_bytes);
}

/* In SS, append C to the last string.  */
স্থির শূন্য
splitbuf_append_byte (কাঠামো splitbuf *ss, অক্ষর c)
{
  idx_t string_bytes = (intptr_t) ss->argv[ss->argc];
  যদি (ss->half_alloc * আকার *ss->argv <= string_bytes)
    splitbuf_grow (ss);
  ((অক্ষর *) (ss->argv + ss->half_alloc))[string_bytes] = c;
  ss->argv[ss->argc] = (অক্ষর *) (intptr_t) {string_bytes + 1};
}

/* If SS's most recent character was a separator, finish off its
   previous argument and start a new one.  */
স্থির শূন্য
check_start_new_arg (কাঠামো splitbuf *ss)
{
  যদি (ss->sep)
    {
      splitbuf_append_byte (ss, '\0');
      পূর্ণসংখ্যা argc = ss->argc;
      যদি (ss->half_alloc <= argc + ss->extra_argc + 1)
        splitbuf_grow (ss);
      ss->argv[argc + 1] = ss->argv[argc];
      ss->argc = argc + 1;
      ss->sep = false;
    }
}

/* All additions to SS have been made.  Convert its offsets to pointers,
   and return the resulting argument vector.  */
স্থির অক্ষর **
splitbuf_finishup (কাঠামো splitbuf *ss)
{
  পূর্ণসংখ্যা argc = ss->argc;
  অক্ষর **argv = ss->argv;
  অক্ষর *stringbase = (অক্ষর *) (ss->argv + ss->half_alloc);
  জন্য (পূর্ণসংখ্যা i = 1; i < argc; i++)
    argv[i] = stringbase + (intptr_t) argv[i];
  ফেরত argv;
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
স্থির অক্ষর **
build_argv (অক্ষর ধ্রুবক *str, পূর্ণসংখ্যা extra_argc, পূর্ণসংখ্যা *argc)
{
  bool dq = false, sq = false;
  কাঠামো splitbuf ss;
  ss.argv = xnmalloc (extra_argc + 2, 2 * আকার *ss.argv);
  ss.argc = 1;
  ss.half_alloc = extra_argc + 2;
  ss.extra_argc = extra_argc;
  ss.sep = true;
  ss.argv[ss.argc] = ফাঁকা;

  /* In the following loop,
     'break' causes the character 'newc' to be added to *dest,
     'continue' skips the character.  */
  যখন (*str)
    {
      অক্ষর newc = *str; /* Default: add the next character.  */

      নির্বাচন (*str)
        {
        ক্ষেত্র '\'':
          যদি (dq)
            বিরতি;
          sq = !sq;
          check_start_new_arg (&ss);
          ++str;
          চালু;

        ক্ষেত্র '"':
          যদি (sq)
            বিরতি;
          dq = !dq;
          check_start_new_arg (&ss);
          ++str;
          চালু;

        ক্ষেত্র ' ': ক্ষেত্র '\t': ক্ষেত্র '\n': ক্ষেত্র '\v': ক্ষেত্র '\f': ক্ষেত্র '\r':
          /* Start a new argument if outside quotes.  */
          যদি (sq || dq)
            বিরতি;
          ss.sep = true;
          str += strspn (str, C_ISSPACE_CHARS);
          চালু;

        ক্ষেত্র '#':
          যদি (!ss.sep)
            বিরতি;
          যাও eos; /* '#' as first char terminates the string.  */

        ক্ষেত্র '\\':
          /* Backslash inside single-quotes is not special, except \\
             and \'.  */
          যদি (sq && str[1] != '\\' && str[1] != '\'')
            বিরতি;

          /* Skip the backslash and examine the next character.  */
          newc = *++str;
          নির্বাচন (newc)
            {
            ক্ষেত্র '"': ক্ষেত্র '#': ক্ষেত্র '$': ক্ষেত্র '\'': ক্ষেত্র '\\':
              /* Pass escaped character as-is.  */
              বিরতি;

            ক্ষেত্র '_':
              যদি (!dq)
                {
                  ++str;  /* '\_' outside double-quotes is arg separator.  */
                  ss.sep = true;
                  চালু;
                }
              newc = ' ';  /* '\_' inside double-quotes is space.  */
              বিরতি;

            ক্ষেত্র 'c':
              যদি (dq)
                error (EXIT_CANCELED, 0,
                       _("'\\c' must not appear in double-quoted -S string"));
              যাও eos; /* '\c' terminates the string.  */

            ক্ষেত্র 'f': newc = '\f'; বিরতি;
            ক্ষেত্র 'n': newc = '\n'; বিরতি;
            ক্ষেত্র 'r': newc = '\r'; বিরতি;
            ক্ষেত্র 't': newc = '\t'; বিরতি;
            ক্ষেত্র 'v': newc = '\v'; বিরতি;

            ক্ষেত্র '\0':
              error (EXIT_CANCELED, 0,
                     _("invalid backslash at end of string in -S"));

            পূর্বনির্ধারিত:
              error (EXIT_CANCELED, 0,
                     _("invalid sequence '\\%c' in -S"), newc);
            }
          বিরতি;

        ক্ষেত্র '$':
          /* ${VARNAME} are not expanded inside single-quotes.  */
          যদি (sq)
            বিরতি;

          /* Store the ${VARNAME} value.  */
          {
            অক্ষর *n = extract_varname (str);
            যদি (!n)
              error (EXIT_CANCELED, 0,
                     _("only ${VARNAME} expansion is supported, error at: %s"),
                     str);

            অক্ষর *v = getenv (n);
            যদি (v)
              {
                check_start_new_arg (&ss);
                vmsg (_("expanding ${%s} into %s\n"), n, quote (v));
                জন্য (; *v; v++)
                  splitbuf_append_byte (&ss, *v);
              }
            অন্যথা
              vmsg (_("replacing ${%s} with empty string\n"), n);

            str = strchr (str, '}') + 1;
            চালু;
          }
        }

      check_start_new_arg (&ss);
      splitbuf_append_byte (&ss, newc);
      ++str;
    }

  যদি (dq || sq)
    error (EXIT_CANCELED, 0, _("no terminating quote in -S string"));

 eos:
  splitbuf_append_byte (&ss, '\0');
  *argc = ss.argc;
  ফেরত splitbuf_finishup (&ss);
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
স্থির শূন্য
parse_split_string (অক্ষর ধ্রুবক *str, পূর্ণসংখ্যা *orig_optind,
                    পূর্ণসংখ্যা *orig_argc, অক্ষর ***orig_argv)
{
  পূর্ণসংখ্যা extra_argc = *orig_argc - *orig_optind, newargc;
  অক্ষর **newargv = build_argv (str, extra_argc, &newargc);

  /* Restore argv[0] - the 'env' executable name.  */
  *newargv = (*orig_argv)[0];

  /* Print parsed arguments.  */
  যদি (dev_debug && 1 < newargc)
    {
      vmsg (_("split -S:  %s\n"), quote (str));
      vmsg (_(" into:     %s\n"), quote (newargv[1]));
      জন্য (পূর্ণসংখ্যা i = 2; i < newargc; i++)
        vmsg (_("     &     %s\n"), quote (newargv[i]));
    }

  /* Add remaining arguments and terminating null from the original
     command line.  */
  memcpy (newargv + newargc, *orig_argv + *orig_optind,
          (extra_argc + 1) * আকার *newargv);

  /* Set new values for original getopt variables.  */
  *orig_argc = newargc + extra_argc;
  *orig_argv = newargv;
  *orig_optind = 0; /* Tell getopt to restart from first argument.  */
}

স্থির শূন্য
parse_signal_action_params (অক্ষর ধ্রুবক *arg, bool set_default)
{
  যদি (! arg)
    {
      /* Without an argument, reset all signals.
         Some signals cannot be set to ignore or default (e.g., SIGKILL,
         SIGSTOP on most OSes, and SIGCONT on AIX.) - so ignore errors.  */
      জন্য (পূর্ণসংখ্যা i = 1 ; i <= SIGNUM_BOUND; i++)
        signals[i] = set_default ? DEFAULT_NOERR : IGNORE_NOERR;
      ফেরত;
    }

  অক্ষর *optarg_writable = xstrdup (arg);

  অক্ষর *opt_sig = strtok (optarg_writable, ",");
  যখন (opt_sig)
    {
      পূর্ণসংখ্যা signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      যদি (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      যদি (signum <= 0)
        usage (exit_failure);

      signals[signum] = set_default ? DEFAULT : IGNORE;

      opt_sig = strtok (ফাঁকা, ",");
    }

  মুক্ত (optarg_writable);
}

স্থির শূন্য
reset_signal_handlers (শূন্য)
{
  জন্য (পূর্ণসংখ্যা i = 1; i <= SIGNUM_BOUND; i++)
    {
      কাঠামো sigaction act;

      যদি (signals[i] == UNCHANGED)
        চালু;

      bool ignore_errors = (signals[i] == DEFAULT_NOERR
                            || signals[i] == IGNORE_NOERR);

      bool set_to_default = (signals[i] == DEFAULT
                             || signals[i] == DEFAULT_NOERR);

      পূর্ণসংখ্যা sig_err = sigaction (i, ফাঁকা, &act);

      যদি (sig_err && !ignore_errors)
        error (EXIT_CANCELED, errno,
               _("failed to get signal action for signal %d"), i);

      যদি (! sig_err)
        {
          act.sa_handler = set_to_default ? SIG_DFL : SIG_IGN;
          sig_err = sigaction (i, &act, ফাঁকা);
          যদি (sig_err && !ignore_errors)
            error (EXIT_CANCELED, errno,
                   _("failed to set signal action for signal %d"), i);
        }

      যদি (dev_debug)
        {
          অক্ষর signame[SIG2STR_MAX];
          যদি (sig2str (i, signame) != 0)
            snprintf (signame, আকার signame, "SIG%d", i);
          vmsg (_("Reset signal %s (%d) to %s%s\n"),
                  signame, i,
                  set_to_default ? "DEFAULT" : "IGNORE",
                  sig_err ? _(" (failure ignored)") : "");
        }
    }
}


স্থির শূন্য
parse_block_signal_params (অক্ষর ধ্রুবক *arg, bool block)
{
  যদি (! arg)
    {
      /* Without an argument, reset all signals.  */
      sigfillset (block ? &block_signals : &unblock_signals);
      sigemptyset (block ? &unblock_signals : &block_signals);
    }
  অন্যথা যদি (! sig_mask_changed)
    {
      /* Initialize the sets.  */
      sigemptyset (&block_signals);
      sigemptyset (&unblock_signals);
    }

  sig_mask_changed = true;

  যদি (! arg)
    ফেরত;

  অক্ষর *optarg_writable = xstrdup (arg);

  অক্ষর *opt_sig = strtok (optarg_writable, ",");
  যখন (opt_sig)
    {
      পূর্ণসংখ্যা signum = operand2sig (opt_sig);
      /* operand2sig accepts signal 0 (EXIT) - but we reject it.  */
      যদি (signum == 0)
        error (0, 0, _("%s: invalid signal"), quote (opt_sig));
      যদি (signum <= 0)
        usage (exit_failure);

      যদি (sigaddset (block ? &block_signals : &unblock_signals, signum) == -1)
        {
          যদি (block)
            error (EXIT_CANCELED, errno,
                   _("failed to block signal %d"), signum);
          /* else diagnosed in parse_signal_action_params().  */
        }
      অন্যথা
        sigdelset (block ? &unblock_signals : &block_signals, signum);

      opt_sig = strtok (ফাঁকা, ",");
    }

  মুক্ত (optarg_writable);
}

স্থির শূন্য
set_signal_proc_mask (শূন্য)
{
  /* Get the existing signal mask */
  sigset_t set;

  sigemptyset (&set);

  যদি (sigprocmask (0, ফাঁকা, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  জন্য (পূর্ণসংখ্যা i = 1; i <= SIGNUM_BOUND; i++)
    {
      অক্ষর ধ্রুবক *debug_act = ফাঁকা;

      যদি (sigismember (&block_signals, i))
        {
          sigaddset (&set, i);
          debug_act = "BLOCK";
        }
      অন্যথা যদি (sigismember (&unblock_signals, i))
        {
          sigdelset (&set, i);
          debug_act = "UNBLOCK";
        }

      যদি (dev_debug && debug_act)
        {
          অক্ষর signame[SIG2STR_MAX];
          যদি (sig2str (i, signame) != 0)
            snprintf (signame, আকার signame, "SIG%d", i);
          vmsg (_("signal %s (%d) mask set to %s\n"),
                  signame, i, debug_act);
        }
    }

  যদি (sigprocmask (SIG_SETMASK, &set, ফাঁকা))
    error (EXIT_CANCELED, errno, _("failed to set signal process mask"));
}

স্থির শূন্য
list_signal_handling (শূন্য)
{
  sigset_t set;

  sigemptyset (&set);
  যদি (sigprocmask (0, ফাঁকা, &set))
    error (EXIT_CANCELED, errno, _("failed to get signal process mask"));

  জন্য (পূর্ণসংখ্যা i = 1; i <= SIGNUM_BOUND; i++)
    {
      কাঠামো sigaction act;
      যদি (sigaction (i, ফাঁকা, &act))
        চালু;

      অক্ষর ধ্রুবক *ignored = act.sa_handler == SIG_IGN ? "IGNORE" : "";
      অক্ষর ধ্রুবক *blocked = sigismember (&set, i) ? "BLOCK" : "";
      অক্ষর ধ্রুবক *connect = *ignored && *blocked ? "," : "";

      যদি (! *ignored && ! *blocked)
        চালু;

      অক্ষর signame[SIG2STR_MAX];
      যদি (sig2str (i, signame) != 0)
        snprintf (signame, আকার signame, "SIG%d", i);
      fprintf (stderr, "%-10s (%2d): %s%s%s\n", signame, i,
               blocked, connect, ignored);
    }
}

স্থির শূন্য
initialize_signals (শূন্য)
{
  signals = xmalloc ((আকার *signals) * (SIGNUM_BOUND + 1));

  জন্য (পূর্ণসংখ্যা i = 0 ; i <= SIGNUM_BOUND; i++)
    signals[i] = UNCHANGED;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool ignore_environment = false;
  bool opt_nul_terminate_output = false;
  পূর্ণসংখ্যা quoting_style_opt = -1;
  অক্ষর ধ্রুবক *newdir = ফাঁকা;
  অক্ষর ধ্রুবক *env0_from_file = ফাঁকা;
  অক্ষর *argv0 = ফাঁকা;
  কাঠামো env_vector env_vector;
#ifdef lint
  memset (&env_vector, 0, আকার env_vector);
  অক্ষর **initial_environ = environ;
#endif

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  initialize_signals ();

  পূর্ণসংখ্যা optc;
  যখন ((optc = getopt_long (argc, argv, shortopts, longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'a':
          argv0 = optarg;
          বিরতি;
        ক্ষেত্র 'i':
          ignore_environment = true;
          বিরতি;
        ক্ষেত্র 'u':
          append_unset_var (optarg);
          বিরতি;
        ক্ষেত্র 'v':
          dev_debug = true;
          বিরতি;
        ক্ষেত্র '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          বিরতি;
        ক্ষেত্র ENV0_FROM_OPTION:
          env0_from_file = optarg;
          বিরতি;
        ক্ষেত্র DEFAULT_SIGNAL_OPTION:
          parse_signal_action_params (optarg, true);
          parse_block_signal_params (optarg, false);
          বিরতি;
        ক্ষেত্র IGNORE_SIGNAL_OPTION:
          parse_signal_action_params (optarg, false);
          বিরতি;
        ক্ষেত্র BLOCK_SIGNAL_OPTION:
          parse_block_signal_params (optarg, true);
          বিরতি;
        ক্ষেত্র LIST_SIGNAL_HANDLING_OPTION:
          report_signal_handling = true;
          বিরতি;
        ক্ষেত্র QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          বিরতি;
        ক্ষেত্র 'C':
          newdir = optarg;
          বিরতি;
        ক্ষেত্র 'S':
          parse_split_string (optarg, &optind, &argc, &argv);
          বিরতি;
        ক্ষেত্র ' ': ক্ষেত্র '\t': ক্ষেত্র '\n': ক্ষেত্র '\v': ক্ষেত্র '\f': ক্ষেত্র '\r':
          /* These are undocumented options.  Attempt to detect
             incorrect shebang usage with extraneous space, e.g.:
                #!/usr/bin/env -i command
             In which case argv[1] == "-i command".  */
          {
            অক্ষর c = optc;
            error (0, 0, _("invalid option -- %s"), quote_mem (&c, 1));
            error (0, 0, _("use -[v]S to pass options in shebang lines"));
            usage (EXIT_CANCELED);
          }

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_CANCELED);
        }
    }

  যদি (optind < argc && streq (argv[optind], "-"))
    {
      ignore_environment = true;
      ++optind;
    }

  পূর্ণসংখ্যা program_সূচক = optind;
  যখন (program_সূচক < argc && strchr (argv[program_সূচক], '='))
    program_সূচক++;
  bool program_specified = program_সূচক < argc;

  bool quote_output = false;
  পূর্ণসংখ্যা qs = quoting_style_opt;
  যদি (! program_specified && qs < 0 && isatty (STDOUT_FILENO))
    {
      /* Get the value from $QUOTING_STYLE before unsetting env vars.  */
      qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
    }
  যদি (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ফাঁকা, qs);
      quote_output = true;
    }

  bool env_vector_active = false;
  যদি (ignore_environment && env0_from_file)
    {
      vmsg (_("clearing environment\n"));
      set_raw_environment_from_file (&env_vector, env0_from_file);
      env_vector_active = true;
    }
  অন্যথা যদি (ignore_environment)
    {
      vmsg (_("clearing environment\n"));
#যদি defined _WIN32 && ! defined __CYGWIN__
      স্থির অক্ষর *dummy_environ[] = { ফাঁকা };
      environ = dummy_environ;
#অন্যথা
      env_vector.variable = xnmalloc (1, আকার *env_vector.variable);
      env_vector.variable[0] = ফাঁকা;
      env_vector.used = 0;
      env_vector.allocated = 1;
      environ = env_vector.variable;
      env_vector_active = true;
#endif
    }
  অন্যথা যদি (env0_from_file)
    env_vector_active = set_envvars_from_file (&env_vector, env0_from_file);

  যদি (! ignore_environment || env0_from_file)
    {
      যদি (env_vector_active)
        unset_envvars_in_vector (&env_vector);
      অন্যথা
        unset_envvars ();
    }

  অক্ষর *eq;
  যখন (optind < argc && (eq = strchr (argv[optind], '=')))
    {
      যদি (env_vector_active)
        set_envvar_in_vector (&env_vector, argv[optind], eq);
      অন্যথা
        set_envvar (argv[optind], eq);
      optind++;
    }

  যদি (opt_nul_terminate_output && program_specified)
    {
      error (0, 0, _("cannot specify --null (-0) with command"));
      usage (EXIT_CANCELED);
    }

  যদি (newdir && ! program_specified)
    {
      error (0, 0, _("must specify command with --chdir (-C)"));
      usage (EXIT_CANCELED);
    }

  যদি (argv0 && ! program_specified)
    {
      error (0, 0, _("must specify command with --argv0 (-a)"));
      usage (EXIT_CANCELED);
    }

  যদি (! program_specified)
    {
      অক্ষর ধ্রুবক terminator = opt_nul_terminate_output ? '\0' : '\n';

      যদি (opt_nul_terminate_output)
        xset_দ্বিমিক_mode (STDOUT_FILENO, O_দ্বিমিক);

      /* Print the environment and exit.  */
      জন্য (অক্ষর *ধ্রুবক *e = environ; *e; ++e)
        print_envvar (*e, terminator, quote_output);
#ifdef lint
      যদি (env_vector_active)
        {
          environ = initial_environ;
          মুক্ত (env_vector.variable);
          মুক্ত (env0_from_বাফার);
        }
#endif
      ফেরত EXIT_SUCCESS;
    }

  reset_signal_handlers ();
  যদি (sig_mask_changed)
    set_signal_proc_mask ();

  যদি (report_signal_handling)
    list_signal_handling ();

  যদি (newdir)
    {
      vmsg (_("change dir: %s\n"), quote (newdir));

      যদি (chdir (newdir) != 0)
        error (EXIT_CANCELED, errno, _("cannot change directory to %s"),
               quoteaf (newdir));
    }

  অক্ষর *program = argv[optind];
  যদি (argv0)
    {
      vmsg ("argv0:     %s\n", quote (argv0));
      argv[optind] = argv0;
    }

  যদি (dev_debug)
    {
      vmsg (_("executing: %s\n"), quote (program));
      জন্য (পূর্ণসংখ্যা i=optind; i<argc; ++i)
        vmsg ("   arg[%d]= %s\n", i-optind, quote (argv[i]));
    }

  execvp (program, &argv[optind]);

  পূর্ণসংখ্যা exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (program));

  যদি (exit_status == EXIT_ENOENT && strpbrk (program, C_ISSPACE_CHARS))
    error (0, 0, _("use -[v]S to pass options in shebang lines"));

  main_exit (exit_status);
}
