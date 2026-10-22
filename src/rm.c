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
#تعریف_کرو پروگرام_NAME "rm"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_جڑ,
  PRESERVE_جڑ,
  PRESUME_داخلہ_TTY_OPTION
};

شمار interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ساکن ساخت option ثابت long_opts[] =
{
  {"force", no_دلیل, تہی, 'f'},
  {"interactive", optional_دلیل, تہی, INTERACTIVE_OPTION},

  {"one-file-system", no_دلیل, تہی, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_دلیل, تہی, NO_PRESERVE_جڑ},
  {"preserve-root", optional_دلیل, تہی, PRESERVE_جڑ},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_دلیل, تہی, PRESUME_داخلہ_TTY_OPTION},

  {"recursive", no_دلیل, تہی, 'r'},
  {"dir", no_دلیل, تہی, 'd'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

ساکن حرف ثابت *ثابت interactive_دلائل[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", تہی
};
ساکن شمار interactive_type ثابت interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_دلائل, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ساکن خالی
diagnose_leading_hyphen (صحیح argc, حرف **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  برائے (صحیح i = 1; i < argc; i++)
    {
      حرف ثابت *arg = argv[i];
      ساخت stat st;

      اگر (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          توڑو;
        }
    }
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
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
         prompt once before removing مزید than three files,\n\
         or when removing recursively; less intrusive than -i,\n\
         جب still giving protection against most mistakes\n\
"));
      oputs (_("\
      --interactive[=WHEN]\n\
         prompt according مقصد WHEN: never, once (-I), or always (-i);\n\
         without WHEN, prompt always\n\
"));
      oputs (_("\
      --one-file-system\n\
         when removing a hierarchy recursively,\n\
         skip any directory that is on a file system different\n\
         from that کا the corresponding command سطر دلیل\n\
"));
      oputs (_("\
      --no-preserve-جڑ\n\
         کرو not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-جڑ[=all]\n\
         کرو not remove '/' (طےشدہ);\n\
         with 'all', reject any command سطر دلیل\n\
         on a separate device from its والد\n\
"));
      oputs (_("\
  -r, -R, --recursive\n\
         remove directories and their contents recursively\n\
"));
      oputs (_("\
  -d, --dir\n\
         remove خالی_خانے directories\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being مکمل\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
By طےشدہ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option مقصد remove each listed directory, too, along with all کا its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt مقصد remove a file whose آخری file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      لکھو (_("\
\n\
مقصد_2 remove a file whose name starts with a '-', برائے example '-foo',\n\
use one کا these commands:\n\
  %s -- -foo\n\
\n\
  %s ./-foo\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
\n\
If you use rm مقصد remove a file, it might be possible مقصد recover\n\
some کا its contents, given sufficient expertise and/or وقت.  For greater\n\
assurance that the contents are unrecoverable, consider using shred(1).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن خالی
rm_option_init (ساخت rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_خالی_خانے_directories = false;
  x->recursive = false;
  x->جڑ_dev_ino = تہی;
  x->preserve_all_جڑ = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool preserve_جڑ = true;
  ساخت rm_options x;
  bool prompt_once = false;
  صحیح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_سیٹ_remove_linkdir ();

  جب ((c = getopt_long (argc, argv, "dfirvIR", long_opts, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت 'd':
          x.remove_خالی_خانے_directories = true;
          توڑو;

        حالت 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          توڑو;

        حالت 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          توڑو;

        حالت 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          توڑو;

        حالت 'r':
        حالت 'R':
          x.recursive = true;
          توڑو;

        حالت INTERACTIVE_OPTION:
          {
            صحیح i;
            اگر (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_دلائل,
                             interactive_types);
            ورنہ
              i = interactive_always;
            انتخاب (i)
              {
              حالت interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                توڑو;

              حالت interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                توڑو;

              حالت interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                توڑو;
              }
            توڑو;
          }

        حالت ONE_FILE_SYSTEM:
          x.one_file_system = true;
          توڑو;

        حالت NO_PRESERVE_جڑ:
          اگر (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_جڑ = false;
          توڑو;

        حالت PRESERVE_جڑ:
          اگر (optarg)
            {
              اگر (streq (optarg, "all"))
                x.preserve_all_جڑ = true;
              ورنہ
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_جڑ = true;
          توڑو;

        حالت PRESUME_داخلہ_TTY_OPTION:
          x.stdin_tty = true;
          توڑو;

        حالت 'v':
          x.verbose = true;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc <= optind)
    {
      اگر (x.ignore_missing_files)
        واپس EXIT_SUCCESS;
      ورنہ
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  اگر (x.recursive && preserve_جڑ)
    {
      ساکن ساخت dev_ino dev_ino_buf;
      x.جڑ_dev_ino = get_جڑ_dev_ino (&dev_ino_buf);
      اگر (x.جڑ_dev_ino == تہی)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  حرف **file =  argv + optind;

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
               پروگرام_name, n_files);
      اگر (!yesno ())
        واپس EXIT_SUCCESS;
    }

  شمار RM_حالت حالت_2 = rm (file, &x);
  affirm (VALID_حالت (حالت_2));
  واپس حالت_2 == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
