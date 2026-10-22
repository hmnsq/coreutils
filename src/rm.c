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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "remove.h"
#ଅନ୍ତର୍ଭାଵ "root-dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "yesno.h"
#ଅନ୍ତର୍ଭାଵ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_ମୂଳ,
  PRESERVE_ମୂଳ,
  PRESUME_ନିବେଶ_TTY_OPTION
};

ଗଣନା interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_opts[] =
{
  {"force", no_argument, ଖାଲି, 'f'},
  {"interactive", optional_argument, ଖାଲି, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, ଖାଲି, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, ଖାଲି, NO_PRESERVE_ମୂଳ},
  {"preserve-root", optional_argument, ଖାଲି, PRESERVE_ମୂଳ},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, ଖାଲି, PRESUME_ନିବେଶ_TTY_OPTION},

  {"recursive", no_argument, ଖାଲି, 'r'},
  {"dir", no_argument, ଖାଲି, 'd'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", ଖାଲି
};
ସ୍ଥିର ଗଣନା interactive_type ଅଚଳ interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

ସ୍ଥିର ଶୂନ୍ୟ
diagnose_leading_hyphen (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < argc; i++)
    {
      ଅକ୍ଷର ଅଚଳ *arg = argv[i];
      ସଂରଚନା stat st;

      ଯଦି (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          ଭାଙ୍ଗ;
        }
    }
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         ଯେତେବେଳେ still giving protection against most mistakes\n\
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
         from that of the corresponding command ଧାଡ଼ି argument\n\
"));
      oputs (_("\
      --no-preserve-ମୂଳ\n\
         କର not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-ମୂଳ[=all]\n\
         କର not remove '/' (ପୂର୍ବନିର୍ଦ୍ଧାରିତ);\n\
         with 'all', reject any command ଧାଡ଼ି argument\n\
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
By ପୂର୍ବନିର୍ଦ୍ଧାରିତ, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', ପାଇଁ example '-foo',\n\
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

ସ୍ଥିର ଶୂନ୍ୟ
rm_option_init (ସଂରଚନା rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->ମୂଳ_dev_ino = ଖାଲି;
  x->preserve_all_ମୂଳ = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool preserve_ମୂଳ = true;
  ସଂରଚନା rm_options x;
  bool prompt_once = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "dfirvIR", long_opts, ଖାଲି)) != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'd':
          x.remove_empty_directories = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'r':
        ସ୍ଥିତି 'R':
          x.recursive = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି INTERACTIVE_OPTION:
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
            ଯଦି (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            ଅନ୍ୟଥା
              i = interactive_always;
            ବାଛ (i)
              {
              ସ୍ଥିତି interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                ଭାଙ୍ଗ;

              ସ୍ଥିତି interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                ଭାଙ୍ଗ;

              ସ୍ଥିତି interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                ଭାଙ୍ଗ;
              }
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି ONE_FILE_SYSTEM:
          x.one_file_system = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି NO_PRESERVE_ମୂଳ:
          ଯଦି (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_ମୂଳ = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PRESERVE_ମୂଳ:
          ଯଦି (optarg)
            {
              ଯଦି (streq (optarg, "all"))
                x.preserve_all_ମୂଳ = true;
              ଅନ୍ୟଥା
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_ମୂଳ = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PRESUME_ନିବେଶ_TTY_OPTION:
          x.stdin_tty = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':
          x.verbose = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (argc <= optind)
    {
      ଯଦି (x.ignore_missing_files)
        ଫେରନ୍ତୁ EXIT_SUCCESS;
      ଅନ୍ୟଥା
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (x.recursive && preserve_ମୂଳ)
    {
      ସ୍ଥିର ସଂରଚନା dev_ino dev_ino_buf;
      x.ମୂଳ_dev_ino = get_ମୂଳ_dev_ino (&dev_ino_buf);
      ଯଦି (x.ମୂଳ_dev_ino == ଖାଲି)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  ଅକ୍ଷର **file =  argv + optind;

  ଯଦି (prompt_once && (x.recursive || 3 < n_files))
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
      ଯଦି (!yesno ())
        ଫେରନ୍ତୁ EXIT_SUCCESS;
    }

  ଗଣନା RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  ଫେରନ୍ତୁ status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
