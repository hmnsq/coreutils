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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "remove.h"
#ضمن "root-dev-ino.h"
#ضمن "yesno.h"
#ضمن "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "rm"

#وصف_ڪريو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_پاڙ,
  PRESERVE_پاڙ,
  PRESUME_داخلا_TTY_OPTION
};

تعداد interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ساكن بنية option ثابت long_opts[] =
{
  {"force", no_دليل, عدم, 'f'},
  {"interactive", optional_دليل, عدم, INTERACTIVE_OPTION},

  {"one-file-system", no_دليل, عدم, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_دليل, عدم, NO_PRESERVE_پاڙ},
  {"preserve-root", optional_دليل, عدم, PRESERVE_پاڙ},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_دليل, عدم, PRESUME_داخلا_TTY_OPTION},

  {"recursive", no_دليل, عدم, 'r'},
  {"dir", no_دليل, عدم, 'd'},
  {"verbose", no_دليل, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت interactive_دليلون[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", عدم
};
ساكن تعداد interactive_type ثابت interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_دليلون, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ساكن فراغ
diagnose_leading_hyphen (صحيح argc, حرف **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  لكل (صحيح i = 1; i < argc; i++)
    {
      حرف ثابت *arg = argv[i];
      بنية stat st;

      إذا (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          اكسر;
        }
    }
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
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
         prompt once before removing وڌيڪ than three files,\n\
         or when removing recursively; less intrusive than -i,\n\
         طالما still giving protection against most mistakes\n\
"));
      oputs (_("\
      --interactive[=WHEN]\n\
         prompt according منزل WHEN: never, once (-I), or always (-i);\n\
         without WHEN, prompt always\n\
"));
      oputs (_("\
      --one-file-system\n\
         when removing a hierarchy recursively,\n\
         skip any directory that is on a file system different\n\
         from that جو the corresponding command سٽ دليل\n\
"));
      oputs (_("\
      --no-preserve-پاڙ\n\
         افعل not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-پاڙ[=all]\n\
         افعل not remove '/' (افتراضي);\n\
         with 'all', reject any command سٽ دليل\n\
         on a separate device from its والد\n\
"));
      oputs (_("\
  -r, -R, --recursive\n\
         remove directories and their contents recursively\n\
"));
      oputs (_("\
  -d, --dir\n\
         remove خالي_جايون directories\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being پورو\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
By افتراضي, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option منزل remove each listed directory, too, along with all جو its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt منزل remove a file whose آخري file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      لکو (_("\
\n\
منزل_2 remove a file whose name starts with a '-', لكل example '-foo',\n\
use one جو these commands:\n\
  %s -- -foo\n\
\n\
  %s ./-foo\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
\n\
If you use rm منزل remove a file, it might be possible منزل recover\n\
some جو its contents, given sufficient expertise and/or وقت.  For greater\n\
assurance that the contents are unrecoverable, consider using shred(1).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

ساكن فراغ
rm_option_init (بنية rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_خالي_جايون_directories = false;
  x->recursive = false;
  x->پاڙ_dev_ino = عدم;
  x->preserve_all_پاڙ = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  bool preserve_پاڙ = true;
  بنية rm_options x;
  bool prompt_once = false;
  صحيح c;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_سيٽ_remove_linkdir ();

  طالما ((c = getopt_long (argc, argv, "dfirvIR", long_opts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'd':
          x.remove_خالي_جايون_directories = true;
          اكسر;

        حالة 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          اكسر;

        حالة 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          اكسر;

        حالة 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          اكسر;

        حالة 'r':
        حالة 'R':
          x.recursive = true;
          اكسر;

        حالة INTERACTIVE_OPTION:
          {
            صحيح i;
            إذا (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_دليلون,
                             interactive_types);
            وإلا
              i = interactive_always;
            بدل (i)
              {
              حالة interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                اكسر;

              حالة interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                اكسر;

              حالة interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                اكسر;
              }
            اكسر;
          }

        حالة ONE_FILE_SYSTEM:
          x.one_file_system = true;
          اكسر;

        حالة NO_PRESERVE_پاڙ:
          إذا (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_پاڙ = false;
          اكسر;

        حالة PRESERVE_پاڙ:
          إذا (optarg)
            {
              إذا (streq (optarg, "all"))
                x.preserve_all_پاڙ = true;
              وإلا
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_پاڙ = true;
          اكسر;

        حالة PRESUME_داخلا_TTY_OPTION:
          x.stdin_tty = true;
          اكسر;

        حالة 'v':
          x.verbose = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc <= optind)
    {
      إذا (x.ignore_missing_files)
        أرجع EXIT_SUCCESS;
      وإلا
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  إذا (x.recursive && preserve_پاڙ)
    {
      ساكن بنية dev_ino dev_ino_buf;
      x.پاڙ_dev_ino = get_پاڙ_dev_ino (&dev_ino_buf);
      إذا (x.پاڙ_dev_ino == عدم)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  حرف **file =  argv + optind;

  إذا (prompt_once && (x.recursive || 3 < n_files))
    {
      fprintf (stderr,
               (x.recursive
                ? ngettext ("%s: remove %ju argument recursively? ",
                            "%s: remove %ju arguments recursively? ",
                            select_plural (n_files))
                : ngettext ("%s: remove %ju argument? ",
                            "%s: remove %ju arguments? ",
                            select_plural (n_files))),
               پروگرام_name, n_files);
      إذا (!yesno ())
        أرجع EXIT_SUCCESS;
    }

  تعداد RM_حالت حالت = rm (file, &x);
  affirm (VALID_حالت (حالت));
  أرجع حالت == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
