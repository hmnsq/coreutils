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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "remove.h"
#অন্তর্ভাৱ "root-dev-ino.h"
#অন্তর্ভাৱ "yesno.h"
#অন্তর্ভাৱ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_মূল,
  PRESERVE_মূল,
  PRESUME_প্ৰবেশ_TTY_OPTION
};

গণনা interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

স্থির সংরচনা option ধ্রুৱ long_opts[] =
{
  {"force", no_argument, রিক্ত, 'f'},
  {"interactive", optional_argument, রিক্ত, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, রিক্ত, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, রিক্ত, NO_PRESERVE_মূল},
  {"preserve-root", optional_argument, রিক্ত, PRESERVE_মূল},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, রিক্ত, PRESUME_প্ৰবেশ_TTY_OPTION},

  {"recursive", no_argument, রিক্ত, 'r'},
  {"dir", no_argument, রিক্ত, 'd'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", রিক্ত
};
স্থির গণনা interactive_type ধ্রুৱ interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

স্থির শূন্য
diagnose_leading_hyphen (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  বাবে (পূর্ণাঙ্ক i = 1; i < argc; i++)
    {
      ৱর্ণ ধ্রুৱ *arg = argv[i];
      সংরচনা stat st;

      যদি (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          ভঙ্গ;
        }
    }
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         যেতিয়ালৈ still giving protection against most mistakes\n\
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
         from that of the corresponding command সাৰি argument\n\
"));
      oputs (_("\
      --no-preserve-মূল\n\
         কৰা not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-মূল[=all]\n\
         কৰা not remove '/' (স্ৱভাৱ);\n\
         with 'all', reject any command সাৰি argument\n\
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
By স্ৱভাৱ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', বাবে example '-foo',\n\
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

স্থির শূন্য
rm_option_init (সংরচনা rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->মূল_dev_ino = রিক্ত;
  x->preserve_all_মূল = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  bool preserve_মূল = true;
  সংরচনা rm_options x;
  bool prompt_once = false;
  পূর্ণাঙ্ক c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  যেতিয়ালৈ ((c = getopt_long (argc, argv, "dfirvIR", long_opts, রিক্ত)) != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ 'd':
          x.remove_empty_directories = true;
          ভঙ্গ;

        পক্ষ 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          ভঙ্গ;

        পক্ষ 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          ভঙ্গ;

        পক্ষ 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          ভঙ্গ;

        পক্ষ 'r':
        পক্ষ 'R':
          x.recursive = true;
          ভঙ্গ;

        পক্ষ INTERACTIVE_OPTION:
          {
            পূর্ণাঙ্ক i;
            যদি (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            অন্যথা
              i = interactive_always;
            ৱিকল্প (i)
              {
              পক্ষ interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                ভঙ্গ;

              পক্ষ interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                ভঙ্গ;

              পক্ষ interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                ভঙ্গ;
              }
            ভঙ্গ;
          }

        পক্ষ ONE_FILE_SYSTEM:
          x.one_file_system = true;
          ভঙ্গ;

        পক্ষ NO_PRESERVE_মূল:
          যদি (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_মূল = false;
          ভঙ্গ;

        পক্ষ PRESERVE_মূল:
          যদি (optarg)
            {
              যদি (streq (optarg, "all"))
                x.preserve_all_মূল = true;
              অন্যথা
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_মূল = true;
          ভঙ্গ;

        পক্ষ PRESUME_প্ৰবেশ_TTY_OPTION:
          x.stdin_tty = true;
          ভঙ্গ;

        পক্ষ 'v':
          x.verbose = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc <= optind)
    {
      যদি (x.ignore_missing_files)
        ঘূৰাই EXIT_SUCCESS;
      অন্যথা
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  যদি (x.recursive && preserve_মূল)
    {
      স্থির সংরচনা dev_ino dev_ino_buf;
      x.মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
      যদি (x.মূল_dev_ino == রিক্ত)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  ৱর্ণ **file =  argv + optind;

  যদি (prompt_once && (x.recursive || 3 < n_files))
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
      যদি (!yesno ())
        ঘূৰাই EXIT_SUCCESS;
    }

  গণনা RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  ঘূৰাই status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
