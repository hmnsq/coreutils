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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "remove.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "yesno.h"
#ಅನ್ತರ್ಭಾವ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "rm"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  PRESUME_ಒಳಹರಿವು_TTY_OPTION
};

ಎಣಿಕೆ interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_opts[] =
{
  {"force", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"interactive", optional_ವಾದ, ಖಾಲಿ, INTERACTIVE_OPTION},

  {"one-file-system", no_ವಾದ, ಖಾಲಿ, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_ವಾದ, ಖಾಲಿ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", optional_ವಾದ, ಖಾಲಿ, PRESERVE_ಬೇರು},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_ವಾದ, ಖಾಲಿ, PRESUME_ಒಳಹರಿವು_TTY_OPTION},

  {"recursive", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"dir", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ interactive_ವಾದಗಳು[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ interactive_type ಸ್ಥಿರಾಂಕ interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_ವಾದಗಳು, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
diagnose_leading_hyphen (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < argc; i++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg = argv[i];
      ರಚನೆ stat st;

      ಆದರೆ (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          ನಿಲ್ಲಿಸು;
        }
    }
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
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
         prompt once before removing ಇನ್ನಷ್ಟು than three files,\n\
         or when removing recursively; less intrusive than -i,\n\
         ಆಗ still giving protection against most mistakes\n\
"));
      oputs (_("\
      --interactive[=WHEN]\n\
         prompt according ಗಮ್ಯ WHEN: never, once (-I), or always (-i);\n\
         without WHEN, prompt always\n\
"));
      oputs (_("\
      --one-file-system\n\
         when removing a hierarchy recursively,\n\
         skip any directory that is on a file system different\n\
         from that ದ the corresponding command ಸಾಲು ವಾದ\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಮಾಡು not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-ಬೇರು[=all]\n\
         ಮಾಡು not remove '/' (ಪೂರ್ವನಿಯೋಜಿತ);\n\
         with 'all', reject any command ಸಾಲು ವಾದ\n\
         on a separate device from its ಪೋಷಕ\n\
"));
      oputs (_("\
  -r, -R, --recursive\n\
         remove directories and their contents recursively\n\
"));
      oputs (_("\
  -d, --dir\n\
         remove ಖಾಲಿಗಳು directories\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being ಮುಗಿದ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
By ಪೂರ್ವನಿಯೋಜಿತ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option ಗಮ್ಯ remove each listed directory, too, along with all ದ its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt ಗಮ್ಯ remove a file whose ಕೊನೆಯ file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      ಬರೆ (_("\
\n\
ಗಮ್ಯ_2 remove a file whose name starts with a '-', ಗಾಗಿ example '-foo',\n\
use one ದ these commands:\n\
  %s -- -foo\n\
\n\
  %s ./-foo\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
\n\
If you use rm ಗಮ್ಯ remove a file, it might be possible ಗಮ್ಯ recover\n\
some ದ its contents, given sufficient expertise and/or ಸಮಯ.  For greater\n\
assurance that the contents are unrecoverable, consider using shred(1).\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
rm_option_init (ರಚನೆ rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_ಖಾಲಿಗಳು_directories = false;
  x->recursive = false;
  x->ಬೇರು_dev_ino = ಖಾಲಿ;
  x->preserve_all_ಬೇರು = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool preserve_ಬೇರು = true;
  ರಚನೆ rm_options x;
  bool prompt_once = false;
  ಪೂರ್ಣಾಂಕ c;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_ಗಣ_remove_linkdir ();

  ಆಗ ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'd':
          x.remove_ಖಾಲಿಗಳು_directories = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
        ಸ್ಥಿತಿ 'R':
          x.recursive = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ INTERACTIVE_OPTION:
          {
            ಪೂರ್ಣಾಂಕ i;
            ಆದರೆ (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_ವಾದಗಳು,
                             interactive_types);
            ಇಲ್ಲದಿದ್ದರೆ
              i = interactive_always;
            ಆಯ್ಕೆ (i)
              {
              ಸ್ಥಿತಿ interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                ನಿಲ್ಲಿಸು;

              ಸ್ಥಿತಿ interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                ನಿಲ್ಲಿಸು;

              ಸ್ಥಿತಿ interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                ನಿಲ್ಲಿಸು;
              }
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ ONE_FILE_SYSTEM:
          x.one_file_system = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NO_PRESERVE_ಬೇರು:
          ಆದರೆ (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_ಬೇರು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_ಬೇರು:
          ಆದರೆ (optarg)
            {
              ಆದರೆ (streq (optarg, "all"))
                x.preserve_all_ಬೇರು = true;
              ಇಲ್ಲದಿದ್ದರೆ
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_ಬೇರು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESUME_ಒಳಹರಿವು_TTY_OPTION:
          x.stdin_tty = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          x.verbose = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (argc <= optind)
    {
      ಆದರೆ (x.ignore_missing_files)
        ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (x.recursive && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ರಚನೆ dev_ino dev_ino_buf;
      x.ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಆದರೆ (x.ಬೇರು_dev_ino == ಖಾಲಿ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  ಅಕ್ಷರ **file =  argv + optind;

  ಆದರೆ (prompt_once && (x.recursive || 3 < n_files))
    {
      fprintf (stderr,
               (x.recursive
                ? ngettext ("%s: remove %ju argument recursively? ",
                            "%s: remove %ju arguments recursively? ",
                            select_plural (n_files))
                : ngettext ("%s: remove %ju argument? ",
                            "%s: remove %ju arguments? ",
                            select_plural (n_files))),
               ಕಾರ್ಯಕ್ರಮ_name, n_files);
      ಆದರೆ (!yesno ())
        ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
    }

  ಎಣಿಕೆ RM_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = rm (file, &x);
  affirm (VALID_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2));
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
