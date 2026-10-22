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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "remove.h"
#અન્તર્ભાવ "root-dev-ino.h"
#અન્તર્ભાવ "yesno.h"
#અન્તર્ભાવ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_મૂળ,
  PRESERVE_મૂળ,
  PRESUME_આગત_TTY_OPTION
};

ગણના interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

સ્થિર સંરચના option અચલ long_opts[] =
{
  {"force", no_argument, ખાલી, 'f'},
  {"interactive", optional_argument, ખાલી, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, ખાલી, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, ખાલી, NO_PRESERVE_મૂળ},
  {"preserve-root", optional_argument, ખાલી, PRESERVE_મૂળ},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, ખાલી, PRESUME_આગત_TTY_OPTION},

  {"recursive", no_argument, ખાલી, 'r'},
  {"dir", no_argument, ખાલી, 'd'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

સ્થિર અક્ષર અચલ *અચલ interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ખાલી
};
સ્થિર ગણના interactive_type અચલ interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

સ્થિર શૂન્ય
diagnose_leading_hyphen (પૂર્ણાંક argc, અક્ષર **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  માટે (પૂર્ણાંક i = 1; i < argc; i++)
    {
      અક્ષર અચલ *arg = argv[i];
      સંરચના stat st;

      જો (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          તોડો;
        }
    }
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         જ્યારે still giving protection against most mistakes\n\
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
         from that of the corresponding command લીટી argument\n\
"));
      oputs (_("\
      --no-preserve-મૂળ\n\
         કરો not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-મૂળ[=all]\n\
         કરો not remove '/' (મૂળભૂત);\n\
         with 'all', reject any command લીટી argument\n\
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
By મૂળભૂત, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', માટે example '-foo',\n\
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

સ્થિર શૂન્ય
rm_option_init (સંરચના rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->મૂળ_dev_ino = ખાલી;
  x->preserve_all_મૂળ = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool preserve_મૂળ = true;
  સંરચના rm_options x;
  bool prompt_once = false;
  પૂર્ણાંક c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  જ્યારે ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ખાલી)) != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'd':
          x.remove_empty_directories = true;
          તોડો;

        સ્થિતિ 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          તોડો;

        સ્થિતિ 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          તોડો;

        સ્થિતિ 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          તોડો;

        સ્થિતિ 'r':
        સ્થિતિ 'R':
          x.recursive = true;
          તોડો;

        સ્થિતિ INTERACTIVE_OPTION:
          {
            પૂર્ણાંક i;
            જો (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            નહીંતર
              i = interactive_always;
            પસંદગી (i)
              {
              સ્થિતિ interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                તોડો;

              સ્થિતિ interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                તોડો;

              સ્થિતિ interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                તોડો;
              }
            તોડો;
          }

        સ્થિતિ ONE_FILE_SYSTEM:
          x.one_file_system = true;
          તોડો;

        સ્થિતિ NO_PRESERVE_મૂળ:
          જો (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_મૂળ = false;
          તોડો;

        સ્થિતિ PRESERVE_મૂળ:
          જો (optarg)
            {
              જો (streq (optarg, "all"))
                x.preserve_all_મૂળ = true;
              નહીંતર
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_મૂળ = true;
          તોડો;

        સ્થિતિ PRESUME_આગત_TTY_OPTION:
          x.stdin_tty = true;
          તોડો;

        સ્થિતિ 'v':
          x.verbose = true;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  જો (argc <= optind)
    {
      જો (x.ignore_missing_files)
        પાછા EXIT_SUCCESS;
      નહીંતર
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  જો (x.recursive && preserve_મૂળ)
    {
      સ્થિર સંરચના dev_ino dev_ino_buf;
      x.મૂળ_dev_ino = get_મૂળ_dev_ino (&dev_ino_buf);
      જો (x.મૂળ_dev_ino == ખાલી)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  અક્ષર **file =  argv + optind;

  જો (prompt_once && (x.recursive || 3 < n_files))
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
      જો (!yesno ())
        પાછા EXIT_SUCCESS;
    }

  ગણના RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  પાછા status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
