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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "remove.h"
#شامل "root-dev-ino.h"
#شامل "yesno.h"
#شامل "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_ریشه,
  PRESERVE_ریشه,
  PRESUME_درون_داد_TTY_OPTION
};

شمارش interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ایستا ساختار option ثابت long_opts[] =
{
  {"force", no_argument, پوچ, 'f'},
  {"interactive", optional_argument, پوچ, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, پوچ, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, پوچ, NO_PRESERVE_ریشه},
  {"preserve-root", optional_argument, پوچ, PRESERVE_ریشه},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, پوچ, PRESUME_درون_داد_TTY_OPTION},

  {"recursive", no_argument, پوچ, 'r'},
  {"dir", no_argument, پوچ, 'd'},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

ایستا نویسه ثابت *ثابت interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", پوچ
};
ایستا شمارش interactive_type ثابت interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ایستا تهی
diagnose_leading_hyphen (صحیح argc, نویسه **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  برای (صحیح i = 1; i < argc; i++)
    {
      نویسه ثابت *arg = argv[i];
      ساختار stat st;

      اگر (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          بشکن;
        }
    }
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         وقتی still giving protection against most mistakes\n\
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
         from that of the corresponding command سطر argument\n\
"));
      oputs (_("\
      --no-preserve-ریشه\n\
         انجامبده not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-ریشه[=all]\n\
         انجامبده not remove '/' (پیشفرض);\n\
         with 'all', reject any command سطر argument\n\
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
By پیشفرض, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', برای example '-foo',\n\
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

ایستا تهی
rm_option_init (ساختار rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->ریشه_dev_ino = پوچ;
  x->preserve_all_ریشه = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool preserve_ریشه = true;
  ساختار rm_options x;
  bool prompt_once = false;
  صحیح c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  وقتی ((c = getopt_long (argc, argv, "dfirvIR", long_opts, پوچ)) != -1)
    {
      انتخاب (c)
        {
        حالت 'd':
          x.remove_empty_directories = true;
          بشکن;

        حالت 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          بشکن;

        حالت 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          بشکن;

        حالت 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          بشکن;

        حالت 'r':
        حالت 'R':
          x.recursive = true;
          بشکن;

        حالت INTERACTIVE_OPTION:
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            وگرنه
              i = interactive_always;
            انتخاب (i)
              {
              حالت interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                بشکن;

              حالت interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                بشکن;

              حالت interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                بشکن;
              }
            بشکن;
          }

        حالت ONE_FILE_SYSTEM:
          x.one_file_system = true;
          بشکن;

        حالت NO_PRESERVE_ریشه:
          اگر (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_ریشه = false;
          بشکن;

        حالت PRESERVE_ریشه:
          اگر (optarg)
            {
              اگر (streq (optarg, "all"))
                x.preserve_all_ریشه = true;
              وگرنه
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_ریشه = true;
          بشکن;

        حالت PRESUME_درون_داد_TTY_OPTION:
          x.stdin_tty = true;
          بشکن;

        حالت 'v':
          x.verbose = true;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc <= optind)
    {
      اگر (x.ignore_missing_files)
        بازگشت EXIT_SUCCESS;
      وگرنه
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  اگر (x.recursive && preserve_ریشه)
    {
      ایستا ساختار dev_ino dev_ino_buf;
      x.ریشه_dev_ino = get_ریشه_dev_ino (&dev_ino_buf);
      اگر (x.ریشه_dev_ino == پوچ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  نویسه **file =  argv + optind;

  اگر (prompt_once && (x.recursive || 3 < n_files))
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
      اگر (!yesno ())
        بازگشت EXIT_SUCCESS;
    }

  شمارش RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  بازگشت status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
