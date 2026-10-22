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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remove.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "yesno.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "rm"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_ਜੜ੍ਹ,
  PRESERVE_ਜੜ੍ਹ,
  PRESUME_ਨਿਵੇਸ਼_TTY_OPTION
};

ਗਿਣਤੀ interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_opts[] =
{
  {"force", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"interactive", optional_ਦਲੀਲ, ਨਿਰਾ, INTERACTIVE_OPTION},

  {"one-file-system", no_ਦਲੀਲ, ਨਿਰਾ, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, NO_PRESERVE_ਜੜ੍ਹ},
  {"preserve-root", optional_ਦਲੀਲ, ਨਿਰਾ, PRESERVE_ਜੜ੍ਹ},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_ਦਲੀਲ, ਨਿਰਾ, PRESUME_ਨਿਵੇਸ਼_TTY_OPTION},

  {"recursive", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"dir", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ interactive_ਦਲੀਲਾਂ[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ interactive_type ਅਚਲ interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_ਦਲੀਲਾਂ, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ਸਥਿਰ ਖਾਲੀ
diagnose_leading_hyphen (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  ਲਈ (ਪੂਰਨ i = 1; i < argc; i++)
    {
      ਅੱਖਰ ਅਚਲ *arg = argv[i];
      ਬਣਤਰ stat st;

      ਜੇ (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          ਰੋਕੋ;
        }
    }
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
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
         prompt once before removing ਹੋਰ than three files,\n\
         or when removing recursively; less intrusive than -i,\n\
         ਜਦੋਂ still giving protection against most mistakes\n\
"));
      oputs (_("\
      --interactive[=WHEN]\n\
         prompt according ਮੰਜ਼ਿਲ WHEN: never, once (-I), or always (-i);\n\
         without WHEN, prompt always\n\
"));
      oputs (_("\
      --one-file-system\n\
         when removing a hierarchy recursively,\n\
         skip any directory that is on a file system different\n\
         from that ਦਾ the corresponding command ਸਤਰ ਦਲੀਲ\n\
"));
      oputs (_("\
      --no-preserve-ਜੜ੍ਹ\n\
         ਕਰੋ not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-ਜੜ੍ਹ[=all]\n\
         ਕਰੋ not remove '/' (ਮੂਲ);\n\
         with 'all', reject any command ਸਤਰ ਦਲੀਲ\n\
         on a separate device from its ਮਾਪਾ\n\
"));
      oputs (_("\
  -r, -R, --recursive\n\
         remove directories and their contents recursively\n\
"));
      oputs (_("\
  -d, --dir\n\
         remove ਖਾਲੀ_ਥਾਵਾਂ directories\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being ਪੂਰਾ\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
By ਮੂਲ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option ਮੰਜ਼ਿਲ remove each listed directory, too, along with all ਦਾ its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt ਮੰਜ਼ਿਲ remove a file whose ਆਖਰੀ file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      ਲਿਖੋ (_("\
\n\
ਮੰਜ਼ਿਲ_2 remove a file whose name starts with a '-', ਲਈ example '-foo',\n\
use one ਦਾ these commands:\n\
  %s -- -foo\n\
\n\
  %s ./-foo\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
\n\
If you use rm ਮੰਜ਼ਿਲ remove a file, it might be possible ਮੰਜ਼ਿਲ recover\n\
some ਦਾ its contents, given sufficient expertise and/or ਸਮਾਂ.  For greater\n\
assurance that the contents are unrecoverable, consider using shred(1).\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ ਖਾਲੀ
rm_option_init (ਬਣਤਰ rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_ਖਾਲੀ_ਥਾਵਾਂ_directories = false;
  x->recursive = false;
  x->ਜੜ੍ਹ_dev_ino = ਨਿਰਾ;
  x->preserve_all_ਜੜ੍ਹ = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool preserve_ਜੜ੍ਹ = true;
  ਬਣਤਰ rm_options x;
  bool prompt_once = false;
  ਪੂਰਨ c;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_ਸੈੱਟ_remove_linkdir ();

  ਜਦੋਂ ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'd':
          x.remove_ਖਾਲੀ_ਥਾਵਾਂ_directories = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
        ਹਾਲਤ 'R':
          x.recursive = true;
          ਰੋਕੋ;

        ਹਾਲਤ INTERACTIVE_OPTION:
          {
            ਪੂਰਨ i;
            ਜੇ (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_ਦਲੀਲਾਂ,
                             interactive_types);
            ਨਹੀਂਤਾਂ
              i = interactive_always;
            ਚੋਣ (i)
              {
              ਹਾਲਤ interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                ਰੋਕੋ;

              ਹਾਲਤ interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                ਰੋਕੋ;

              ਹਾਲਤ interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                ਰੋਕੋ;
              }
            ਰੋਕੋ;
          }

        ਹਾਲਤ ONE_FILE_SYSTEM:
          x.one_file_system = true;
          ਰੋਕੋ;

        ਹਾਲਤ NO_PRESERVE_ਜੜ੍ਹ:
          ਜੇ (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_ਜੜ੍ਹ = false;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_ਜੜ੍ਹ:
          ਜੇ (optarg)
            {
              ਜੇ (streq (optarg, "all"))
                x.preserve_all_ਜੜ੍ਹ = true;
              ਨਹੀਂਤਾਂ
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_ਜੜ੍ਹ = true;
          ਰੋਕੋ;

        ਹਾਲਤ PRESUME_ਨਿਵੇਸ਼_TTY_OPTION:
          x.stdin_tty = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          x.verbose = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (argc <= optind)
    {
      ਜੇ (x.ignore_missing_files)
        ਵਾਪਸ EXIT_SUCCESS;
      ਨਹੀਂਤਾਂ
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (x.recursive && preserve_ਜੜ੍ਹ)
    {
      ਸਥਿਰ ਬਣਤਰ dev_ino dev_ino_buf;
      x.ਜੜ੍ਹ_dev_ino = get_ਜੜ੍ਹ_dev_ino (&dev_ino_buf);
      ਜੇ (x.ਜੜ੍ਹ_dev_ino == ਨਿਰਾ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  ਅੱਖਰ **file =  argv + optind;

  ਜੇ (prompt_once && (x.recursive || 3 < n_files))
    {
      fprintf (stderr,
               (x.recursive
                ? ngettext ("%s: remove %ju argument recursively? ",
                            "%s: remove %ju arguments recursively? ",
                            select_plural (n_files))
                : ngettext ("%s: remove %ju argument? ",
                            "%s: remove %ju arguments? ",
                            select_plural (n_files))),
               ਪ੍ਰੋਗਰਾਮ_name, n_files);
      ਜੇ (!yesno ())
        ਵਾਪਸ EXIT_SUCCESS;
    }

  ਗਿਣਤੀ RM_ਸਥਿਤੀ ਸਥਿਤੀ = rm (file, &x);
  affirm (VALID_ਸਥਿਤੀ (ਸਥਿਤੀ));
  ਵਾਪਸ ਸਥਿਤੀ == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
