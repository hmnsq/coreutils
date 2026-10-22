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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "remove.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "yesno.h"
#అన్తర్భావ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "rm"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_మూలం,
  PRESERVE_మూలం,
  PRESUME_ఆదానం_TTY_OPTION
};

గణన interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

స్థిర నిర్మాణం option స్థిరరాశి long_opts[] =
{
  {"force", no_వాదం, ఖాళీ, 'f'},
  {"interactive", optional_వాదం, ఖాళీ, INTERACTIVE_OPTION},

  {"one-file-system", no_వాదం, ఖాళీ, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_వాదం, ఖాళీ, NO_PRESERVE_మూలం},
  {"preserve-root", optional_వాదం, ఖాళీ, PRESERVE_మూలం},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_వాదం, ఖాళీ, PRESUME_ఆదానం_TTY_OPTION},

  {"recursive", no_వాదం, ఖాళీ, 'r'},
  {"dir", no_వాదం, ఖాళీ, 'd'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి interactive_వాదనలు[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ఖాళీ
};
స్థిర గణన interactive_type స్థిరరాశి interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_వాదనలు, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

స్థిర శూన్య
diagnose_leading_hyphen (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  కొరకు (పూర్ణాంకం i = 1; i < argc; i++)
    {
      అక్షరం స్థిరరాశి *arg = argv[i];
      నిర్మాణం stat st;

      ఒకవేళ (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          ఆపు;
        }
    }
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [FILE]...\n"), కార్యక్రమం_name);
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
         prompt once before removing మరిన్ని than three files,\n\
         or when removing recursively; less intrusive than -i,\n\
         అయితే still giving protection against most mistakes\n\
"));
      oputs (_("\
      --interactive[=WHEN]\n\
         prompt according గమ్యం WHEN: never, once (-I), or always (-i);\n\
         without WHEN, prompt always\n\
"));
      oputs (_("\
      --one-file-system\n\
         when removing a hierarchy recursively,\n\
         skip any directory that is on a file system different\n\
         from that యొక్క the corresponding command పంక్తి వాదం\n\
"));
      oputs (_("\
      --no-preserve-మూలం\n\
         చేయి not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-మూలం[=all]\n\
         చేయి not remove '/' (అప్రమేయ);\n\
         with 'all', reject any command పంక్తి వాదం\n\
         on a separate device from its మాతృ\n\
"));
      oputs (_("\
  -r, -R, --recursive\n\
         remove directories and their contents recursively\n\
"));
      oputs (_("\
  -d, --dir\n\
         remove ఖాళీలు directories\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being పూర్తి\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
By అప్రమేయ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option గమ్యం remove each listed directory, too, along with all యొక్క its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt గమ్యం remove a file whose చివరి file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      రాయి (_("\
\n\
గమ్యం_2 remove a file whose name starts with a '-', కొరకు example '-foo',\n\
use one యొక్క these commands:\n\
  %s -- -foo\n\
\n\
  %s ./-foo\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
\n\
If you use rm గమ్యం remove a file, it might be possible గమ్యం recover\n\
some యొక్క its contents, given sufficient expertise and/or సమయం.  For greater\n\
assurance that the contents are unrecoverable, consider using shred(1).\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర శూన్య
rm_option_init (నిర్మాణం rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_ఖాళీలు_directories = false;
  x->recursive = false;
  x->మూలం_dev_ino = ఖాళీ;
  x->preserve_all_మూలం = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool preserve_మూలం = true;
  నిర్మాణం rm_options x;
  bool prompt_once = false;
  పూర్ణాంకం c;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_సమితి_remove_linkdir ();

  అయితే ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'd':
          x.remove_ఖాళీలు_directories = true;
          ఆపు;

        స్థితి 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          ఆపు;

        స్థితి 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          ఆపు;

        స్థితి 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          ఆపు;

        స్థితి 'r':
        స్థితి 'R':
          x.recursive = true;
          ఆపు;

        స్థితి INTERACTIVE_OPTION:
          {
            పూర్ణాంకం i;
            ఒకవేళ (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_వాదనలు,
                             interactive_types);
            లేకపోతే
              i = interactive_always;
            ఎంపిక (i)
              {
              స్థితి interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                ఆపు;

              స్థితి interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                ఆపు;

              స్థితి interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                ఆపు;
              }
            ఆపు;
          }

        స్థితి ONE_FILE_SYSTEM:
          x.one_file_system = true;
          ఆపు;

        స్థితి NO_PRESERVE_మూలం:
          ఒకవేళ (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_మూలం = false;
          ఆపు;

        స్థితి PRESERVE_మూలం:
          ఒకవేళ (optarg)
            {
              ఒకవేళ (streq (optarg, "all"))
                x.preserve_all_మూలం = true;
              లేకపోతే
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_మూలం = true;
          ఆపు;

        స్థితి PRESUME_ఆదానం_TTY_OPTION:
          x.stdin_tty = true;
          ఆపు;

        స్థితి 'v':
          x.verbose = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (argc <= optind)
    {
      ఒకవేళ (x.ignore_missing_files)
        తిరిగి EXIT_SUCCESS;
      లేకపోతే
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (x.recursive && preserve_మూలం)
    {
      స్థిర నిర్మాణం dev_ino dev_ino_buf;
      x.మూలం_dev_ino = get_మూలం_dev_ino (&dev_ino_buf);
      ఒకవేళ (x.మూలం_dev_ino == ఖాళీ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  అక్షరం **file =  argv + optind;

  ఒకవేళ (prompt_once && (x.recursive || 3 < n_files))
    {
      fprintf (stderr,
               (x.recursive
                ? ngettext ("%s: remove %ju argument recursively? ",
                            "%s: remove %ju arguments recursively? ",
                            select_plural (n_files))
                : ngettext ("%s: remove %ju argument? ",
                            "%s: remove %ju arguments? ",
                            select_plural (n_files))),
               కార్యక్రమం_name, n_files);
      ఒకవేళ (!yesno ())
        తిరిగి EXIT_SUCCESS;
    }

  గణన RM_స్థితి స్థితి_2 = rm (file, &x);
  affirm (VALID_స్థితి (స్థితి_2));
  తిరిగి స్థితి_2 == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
