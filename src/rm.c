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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "remove.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "yesno.h"
#अन्तर्भाव "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_मूलम्,
  PRESERVE_मूलम्,
  PRESUME_निवेशः_TTY_OPTION
};

गणना interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

स्थिर संरचना option ध्रुव long_opts[] =
{
  {"force", no_argument, रिक्त, 'f'},
  {"interactive", optional_argument, रिक्त, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, रिक्त, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_मूलम्},
  {"preserve-root", optional_argument, रिक्त, PRESERVE_मूलम्},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, रिक्त, PRESUME_निवेशः_TTY_OPTION},

  {"recursive", no_argument, रिक्त, 'r'},
  {"dir", no_argument, रिक्त, 'd'},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर वर्ण ध्रुव *ध्रुव interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", रिक्त
};
स्थिर गणना interactive_type ध्रुव interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

स्थिर शून्य
diagnose_leading_hyphen (पूर्णाङ्क argc, वर्ण **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  अत्थाय (पूर्णाङ्क i = 1; i < argc; i++)
    {
      वर्ण ध्रुव *arg = argv[i];
      संरचना stat st;

      सचे (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          भङ्ग;
        }
    }
}

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
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
         याव still giving protection against most mistakes\n\
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
         from that of the corresponding command पङ्क्तिः argument\n\
"));
      oputs (_("\
      --no-preserve-मूलम्\n\
         करोहि not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-मूलम्[=all]\n\
         करोहि not remove '/' (स्वभाव);\n\
         with 'all', reject any command पङ्क्तिः argument\n\
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
By स्वभाव, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', अत्थाय example '-foo',\n\
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

स्थिर शून्य
rm_option_init (संरचना rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->मूलम्_dev_ino = रिक्त;
  x->preserve_all_मूलम् = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool preserve_मूलम् = true;
  संरचना rm_options x;
  bool prompt_once = false;
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  याव ((c = getopt_long (argc, argv, "dfirvIR", long_opts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'd':
          x.remove_empty_directories = true;
          भङ्ग;

        पक्ष 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          भङ्ग;

        पक्ष 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          भङ्ग;

        पक्ष 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          भङ्ग;

        पक्ष 'r':
        पक्ष 'R':
          x.recursive = true;
          भङ्ग;

        पक्ष INTERACTIVE_OPTION:
          {
            पूर्णाङ्क i;
            सचे (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            अञ्ञथा
              i = interactive_always;
            विकल्प (i)
              {
              पक्ष interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                भङ्ग;

              पक्ष interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                भङ्ग;

              पक्ष interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                भङ्ग;
              }
            भङ्ग;
          }

        पक्ष ONE_FILE_SYSTEM:
          x.one_file_system = true;
          भङ्ग;

        पक्ष NO_PRESERVE_मूलम्:
          सचे (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_मूलम् = false;
          भङ्ग;

        पक्ष PRESERVE_मूलम्:
          सचे (optarg)
            {
              सचे (streq (optarg, "all"))
                x.preserve_all_मूलम् = true;
              अञ्ञथा
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_मूलम् = true;
          भङ्ग;

        पक्ष PRESUME_निवेशः_TTY_OPTION:
          x.stdin_tty = true;
          भङ्ग;

        पक्ष 'v':
          x.verbose = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  सचे (argc <= optind)
    {
      सचे (x.ignore_missing_files)
        पटिनिवत्त EXIT_SUCCESS;
      अञ्ञथा
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  सचे (x.recursive && preserve_मूलम्)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      x.मूलम्_dev_ino = get_मूलम्_dev_ino (&dev_ino_buf);
      सचे (x.मूलम्_dev_ino == रिक्त)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  वर्ण **file =  argv + optind;

  सचे (prompt_once && (x.recursive || 3 < n_files))
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
      सचे (!yesno ())
        पटिनिवत्त EXIT_SUCCESS;
    }

  गणना RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  पटिनिवत्त status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
