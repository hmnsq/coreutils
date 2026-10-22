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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "remove.h"
#অন্তর্ভাব "root-dev-ino.h"
#অন্তর্ভাব "yesno.h"
#অন্তর্ভাব "priv-set.h"

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
  PRESUME_প্রবেশ_TTY_OPTION
};

গণনা interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

স্থির কাঠামো option ধ্রুবক long_opts[] =
{
  {"force", no_argument, ফাঁকা, 'f'},
  {"interactive", optional_argument, ফাঁকা, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, ফাঁকা, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, ফাঁকা, NO_PRESERVE_মূল},
  {"preserve-root", optional_argument, ফাঁকা, PRESERVE_মূল},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, ফাঁকা, PRESUME_প্রবেশ_TTY_OPTION},

  {"recursive", no_argument, ফাঁকা, 'r'},
  {"dir", no_argument, ফাঁকা, 'd'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ফাঁকা
};
স্থির গণনা interactive_type ধ্রুবক interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

স্থির শূন্য
diagnose_leading_hyphen (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  জন্য (পূর্ণসংখ্যা i = 1; i < argc; i++)
    {
      অক্ষর ধ্রুবক *arg = argv[i];
      কাঠামো stat st;

      যদি (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          বিরতি;
        }
    }
}

শূন্য
usage (পূর্ণসংখ্যা status)
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
         যখন still giving protection against most mistakes\n\
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
         from that of the corresponding command সারি argument\n\
"));
      oputs (_("\
      --no-preserve-মূল\n\
         করো not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-মূল[=all]\n\
         করো not remove '/' (পূর্বনির্ধারিত);\n\
         with 'all', reject any command সারি argument\n\
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
By পূর্বনির্ধারিত, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', জন্য example '-foo',\n\
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
rm_option_init (কাঠামো rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->মূল_dev_ino = ফাঁকা;
  x->preserve_all_মূল = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool preserve_মূল = true;
  কাঠামো rm_options x;
  bool prompt_once = false;
  পূর্ণসংখ্যা c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  যখন ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ফাঁকা)) != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'd':
          x.remove_empty_directories = true;
          বিরতি;

        ক্ষেত্র 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          বিরতি;

        ক্ষেত্র 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          বিরতি;

        ক্ষেত্র 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          বিরতি;

        ক্ষেত্র 'r':
        ক্ষেত্র 'R':
          x.recursive = true;
          বিরতি;

        ক্ষেত্র INTERACTIVE_OPTION:
          {
            পূর্ণসংখ্যা i;
            যদি (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            অন্যথা
              i = interactive_always;
            নির্বাচন (i)
              {
              ক্ষেত্র interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                বিরতি;

              ক্ষেত্র interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                বিরতি;

              ক্ষেত্র interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                বিরতি;
              }
            বিরতি;
          }

        ক্ষেত্র ONE_FILE_SYSTEM:
          x.one_file_system = true;
          বিরতি;

        ক্ষেত্র NO_PRESERVE_মূল:
          যদি (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_মূল = false;
          বিরতি;

        ক্ষেত্র PRESERVE_মূল:
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
          বিরতি;

        ক্ষেত্র PRESUME_প্রবেশ_TTY_OPTION:
          x.stdin_tty = true;
          বিরতি;

        ক্ষেত্র 'v':
          x.verbose = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc <= optind)
    {
      যদি (x.ignore_missing_files)
        ফেরত EXIT_SUCCESS;
      অন্যথা
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  যদি (x.recursive && preserve_মূল)
    {
      স্থির কাঠামো dev_ino dev_ino_buf;
      x.মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
      যদি (x.মূল_dev_ino == ফাঁকা)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  অক্ষর **file =  argv + optind;

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
        ফেরত EXIT_SUCCESS;
    }

  গণনা RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  ফেরত status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
