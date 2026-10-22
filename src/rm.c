/* 'rm' file deletion utility for GNU.
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/* Initially written by Paul Rubin, David MacKenzie, and Richard Stallman.
   Reworked to use chdir and avoid recursion, and later, rewritten
   once again, to use fts, by Jim Meyering.  */

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "remove.h"
#അന്തര്ഭാവ "root-dev-ino.h"
#അന്തര്ഭാവ "yesno.h"
#അന്തര്ഭാവ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_വേര്,
  PRESERVE_വേര്,
  PRESUME_നിവേശം_TTY_OPTION
};

എണ്ണൽ interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

സ്ഥിതം ഘടന option സ്ഥിരം long_opts[] =
{
  {"force", no_argument, ശൂന്യ, 'f'},
  {"interactive", optional_argument, ശൂന്യ, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, ശൂന്യ, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, ശൂന്യ, NO_PRESERVE_വേര്},
  {"preserve-root", optional_argument, ശൂന്യ, PRESERVE_വേര്},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, ശൂന്യ, PRESUME_നിവേശം_TTY_OPTION},

  {"recursive", no_argument, ശൂന്യ, 'r'},
  {"dir", no_argument, ശൂന്യ, 'd'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ശൂന്യ
};
സ്ഥിതം എണ്ണൽ interactive_type സ്ഥിരം interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

സ്ഥിതം ശൂന്യം
diagnose_leading_hyphen (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < argc; i++)
    {
      അക്ഷരം സ്ഥിരം *arg = argv[i];
      ഘടന stat st;

      എങ്കിൽ (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          നിർത്തുക;
        }
    }
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Remove (unlink) the FILE(s).\n\
\n\
"), stdout);
      oputs (_("\
  -f, --force\n\
         ignore nonexistent files and arguments, never prompt\n\
"));
      oputs (_("\
  -i\n\
         prompt before every removal\n\
"));
      oputs (_("\
  -I\n\
         prompt once before removing more than three files,\n\
         or when removing recursively; less intrusive than -i,\n\
         ആകുമ്പോൾ still giving protection against most mistakes\n\
"));
      oputs (_("\
      --interactive[=WHEN]\n\
         prompt according to WHEN: never, once (-I), or always (-i);\n\
         without WHEN, prompt always\n\
"));
      oputs (_("\
      --one-file-system\n\
         when removing a hierarchy recursively,\n\
         skip any directory that is on a file system different\n\
         from that of the corresponding command വരി argument\n\
"));
      oputs (_("\
      --no-preserve-വേര്\n\
         ചെയ്യുക not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-വേര്[=all]\n\
         ചെയ്യുക not remove '/' (സ്ഥിരസ്ഥിതി);\n\
         with 'all', reject any command വരി argument\n\
         on a separate device from its parent\n\
"));
      oputs (_("\
  -r, -R, --recursive\n\
         remove directories and their contents recursively\n\
"));
      oputs (_("\
  -d, --dir\n\
         remove empty directories\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being done\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
By സ്ഥിരസ്ഥിതി, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', വേണ്ടി example '-foo',\n\
use one of these commands:\n\
  %s -- -foo\n\
\n\
  %s ./-foo\n\
"),
              program_name, program_name);
      fputs (_("\
\n\
If you use rm to remove a file, it might be possible to recover\n\
some of its contents, given sufficient expertise and/or time.  For greater\n\
assurance that the contents are unrecoverable, consider using shred(1).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

സ്ഥിതം ശൂന്യം
rm_option_init (ഘടന rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->വേര്_dev_ino = ശൂന്യ;
  x->preserve_all_വേര് = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool preserve_വേര് = true;
  ഘടന rm_options x;
  bool prompt_once = false;
  പൂർണ്ണസംഖ്യ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'd':
          x.remove_empty_directories = true;
          നിർത്തുക;

        അവസ്ഥ 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          നിർത്തുക;

        അവസ്ഥ 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          നിർത്തുക;

        അവസ്ഥ 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          നിർത്തുക;

        അവസ്ഥ 'r':
        അവസ്ഥ 'R':
          x.recursive = true;
          നിർത്തുക;

        അവസ്ഥ INTERACTIVE_OPTION:
          {
            പൂർണ്ണസംഖ്യ i;
            എങ്കിൽ (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            അല്ലെങ്കിൽ
              i = interactive_always;
            തിരഞ്ഞെടുപ്പ് (i)
              {
              അവസ്ഥ interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                നിർത്തുക;

              അവസ്ഥ interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                നിർത്തുക;

              അവസ്ഥ interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                നിർത്തുക;
              }
            നിർത്തുക;
          }

        അവസ്ഥ ONE_FILE_SYSTEM:
          x.one_file_system = true;
          നിർത്തുക;

        അവസ്ഥ NO_PRESERVE_വേര്:
          എങ്കിൽ (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_വേര് = false;
          നിർത്തുക;

        അവസ്ഥ PRESERVE_വേര്:
          എങ്കിൽ (optarg)
            {
              എങ്കിൽ (streq (optarg, "all"))
                x.preserve_all_വേര് = true;
              അല്ലെങ്കിൽ
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_വേര് = true;
          നിർത്തുക;

        അവസ്ഥ PRESUME_നിവേശം_TTY_OPTION:
          x.stdin_tty = true;
          നിർത്തുക;

        അവസ്ഥ 'v':
          x.verbose = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (argc <= optind)
    {
      എങ്കിൽ (x.ignore_missing_files)
        തിരികെ EXIT_SUCCESS;
      അല്ലെങ്കിൽ
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (x.recursive && preserve_വേര്)
    {
      സ്ഥിതം ഘടന dev_ino dev_ino_buf;
      x.വേര്_dev_ino = get_വേര്_dev_ino (&dev_ino_buf);
      എങ്കിൽ (x.വേര്_dev_ino == ശൂന്യ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  അക്ഷരം **file =  argv + optind;

  എങ്കിൽ (prompt_once && (x.recursive || 3 < n_files))
    {
      fprintf (stderr,
               (x.recursive
                ? ngettext ("%s: remove %ju argument recursively? ",
                            "%s: remove %ju arguments recursively? ",
                            select_plural (n_files))
                : ngettext ("%s: remove %ju argument? ",
                            "%s: remove %ju arguments? ",
                            select_plural (n_files))),
               program_name, n_files);
      എങ്കിൽ (!yesno ())
        തിരികെ EXIT_SUCCESS;
    }

  എണ്ണൽ RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  തിരികെ status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
