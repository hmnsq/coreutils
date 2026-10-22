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
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  PRESUME_ಒಳಹರಿವು_TTY_OPTION
};

ಗಣನಾ interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_opts[] =
{
  {"force", no_argument, ರಿಕ್ತ, 'f'},
  {"interactive", optional_argument, ರಿಕ್ತ, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, ರಿಕ್ತ, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, ರಿಕ್ತ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", optional_argument, ರಿಕ್ತ, PRESERVE_ಬೇರು},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, ರಿಕ್ತ, PRESUME_ಒಳಹರಿವು_TTY_OPTION},

  {"recursive", no_argument, ರಿಕ್ತ, 'r'},
  {"dir", no_argument, ರಿಕ್ತ, 'd'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ interactive_type ಧ್ರುವ interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
diagnose_leading_hyphen (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < argc; i++)
    {
      ವರ್ಣ ಧ್ರುವ *arg = argv[i];
      ಸಂರಚನಾ stat st;

      ಯದಿ (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          ಭಙ್ಗ;
        }
    }
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
         ಯಾವತ್ still giving protection against most mistakes\n\
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
         from that of the corresponding command ಸಾಲು argument\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಕುರು not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-ಬೇರು[=all]\n\
         ಕುರು not remove '/' (ಸ್ವಭಾವ);\n\
         with 'all', reject any command ಸಾಲು argument\n\
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
By ಸ್ವಭಾವ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', ಪ್ರತಿ example '-foo',\n\
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

ಸ್ಥಿರ ಶೂನ್ಯ
rm_option_init (ಸಂರಚನಾ rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->ಬೇರು_dev_ino = ರಿಕ್ತ;
  x->preserve_all_ಬೇರು = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool preserve_ಬೇರು = true;
  ಸಂರಚನಾ rm_options x;
  bool prompt_once = false;
  ಪೂರ್ಣಾಙ್ಕ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  ಯಾವತ್ ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'd':
          x.remove_empty_directories = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
        ಪಕ್ಷ 'R':
          x.recursive = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ INTERACTIVE_OPTION:
          {
            ಪೂರ್ಣಾಙ್ಕ i;
            ಯದಿ (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            ಅನ್ಯಥಾ
              i = interactive_always;
            ವಿಕಲ್ಪ (i)
              {
              ಪಕ್ಷ interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                ಭಙ್ಗ;

              ಪಕ್ಷ interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                ಭಙ್ಗ;

              ಪಕ್ಷ interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                ಭಙ್ಗ;
              }
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ ONE_FILE_SYSTEM:
          x.one_file_system = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ NO_PRESERVE_ಬೇರು:
          ಯದಿ (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_ಬೇರು = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_ಬೇರು:
          ಯದಿ (optarg)
            {
              ಯದಿ (streq (optarg, "all"))
                x.preserve_all_ಬೇರು = true;
              ಅನ್ಯಥಾ
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_ಬೇರು = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESUME_ಒಳಹರಿವು_TTY_OPTION:
          x.stdin_tty = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          x.verbose = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (argc <= optind)
    {
      ಯದಿ (x.ignore_missing_files)
        ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
      ಅನ್ಯಥಾ
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (x.recursive && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ಸಂರಚನಾ dev_ino dev_ino_buf;
      x.ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಯದಿ (x.ಬೇರು_dev_ino == ರಿಕ್ತ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  ವರ್ಣ **file =  argv + optind;

  ಯದಿ (prompt_once && (x.recursive || 3 < n_files))
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
      ಯದಿ (!yesno ())
        ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
    }

  ಗಣನಾ RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  ಪ್ರತ್ಯಾಗಮ status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
