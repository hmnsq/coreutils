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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "remove.h"
#அந்தர்பாவ "root-dev-ino.h"
#அந்தர்பாவ "yesno.h"
#அந்தர்பாவ "priv-set.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rm"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard M. Stallman"), \
  proper_name ("Jim Meyering")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  INTERACTIVE_OPTION = CHAR_MAX + 1,
  ONE_FILE_SYSTEM,
  NO_PRESERVE_வேர்,
  PRESERVE_வேர்,
  PRESUME_உள்ளீடு_TTY_OPTION
};

எண்ணிக்கை interactive_type
  {
    interactive_never,		/* 0: no option or --interactive=never */
    interactive_once,		/* 1: -I or --interactive=once */
    interactive_always		/* 2: default, -i or --interactive=always */
  };

நிலையான கட்டமைப்பு option மாறிலி long_opts[] =
{
  {"force", no_argument, வெற்று, 'f'},
  {"interactive", optional_argument, வெற்று, INTERACTIVE_OPTION},

  {"one-file-system", no_argument, வெற்று, ONE_FILE_SYSTEM},
  {"no-preserve-root", no_argument, வெற்று, NO_PRESERVE_வேர்},
  {"preserve-root", optional_argument, வெற்று, PRESERVE_வேர்},

  /* This is solely for testing.  Do not document.  */
  /* It is relatively difficult to ensure that there is a tty on stdin.
     Since rm acts differently depending on that, without this option,
     it'd be harder to test the parts of rm that depend on that setting.  */
  {"-presume-input-tty", no_argument, வெற்று, PRESUME_உள்ளீடு_TTY_OPTION},

  {"recursive", no_argument, வெற்று, 'r'},
  {"dir", no_argument, வெற்று, 'd'},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

நிலையான எழுத்து மாறிலி *மாறிலி interactive_args[] =
{
  "never", "no", "none",
  "once",
  "always", "yes", வெற்று
};
நிலையான எண்ணிக்கை interactive_type மாறிலி interactive_types[] =
{
  interactive_never, interactive_never, interactive_never,
  interactive_once,
  interactive_always, interactive_always
};
ARGMATCH_VERIFY (interactive_args, interactive_types);

/* Advise the user about invalid usages like "rm -foo" if the file
   "-foo" exists, assuming ARGC and ARGV are as with 'main'.  */

நிலையான வெறுமை
diagnose_leading_hyphen (முழு argc, எழுத்து **argv)
{
  /* OPTIND is unreliable, so iterate through the arguments looking
     for a file name that looks like an option.  */

  க்கு (முழு i = 1; i < argc; i++)
    {
      எழுத்து மாறிலி *arg = argv[i];
      கட்டமைப்பு stat st;

      எனில் (arg[0] == '-' && arg[1] && lstat (arg, &st) == 0)
        {
          fprintf (stderr,
                   _("Try '%s ./%s' to remove the file %s.\n"),
                   argv[0],
                   quotearg_n_style (1, shell_escape_quoting_style, arg),
                   quoteaf (arg));
          நிறுத்து;
        }
    }
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         போது still giving protection against most mistakes\n\
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
         from that of the corresponding command வரி argument\n\
"));
      oputs (_("\
      --no-preserve-வேர்\n\
         செய் not treat '/' specially\n\
"));
      oputs (_("\
      --preserve-வேர்[=all]\n\
         செய் not remove '/' (இயல்பு);\n\
         with 'all', reject any command வரி argument\n\
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
By இயல்பு, rm does not remove directories.  Use the --recursive (-r or -R)\n\
option to remove each listed directory, too, along with all of its contents.\n\
"), stdout);
      fputs (_("\
\n\
Any attempt to remove a file whose last file name component is '.' or '..'\n\
is rejected with a diagnostic.\n\
"), stdout);
      printf (_("\
\n\
To remove a file whose name starts with a '-', க்கு example '-foo',\n\
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

நிலையான வெறுமை
rm_option_init (கட்டமைப்பு rm_options *x)
{
  x->ignore_missing_files = false;
  x->interactive = RMI_SOMETIMES;
  x->one_file_system = false;
  x->remove_empty_directories = false;
  x->recursive = false;
  x->வேர்_dev_ino = வெற்று;
  x->preserve_all_வேர் = false;
  x->stdin_tty = isatty (STDIN_FILENO);
  x->verbose = false;

  /* Since this program exits immediately after calling 'rm', rm need not
     expend unnecessary effort to preserve the initial working directory.  */
  x->require_restore_cwd = false;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool preserve_வேர் = true;
  கட்டமைப்பு rm_options x;
  bool prompt_once = false;
  முழு c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  rm_option_init (&x);

  /* Try to disable the ability to unlink a directory.  */
  priv_set_remove_linkdir ();

  போது ((c = getopt_long (argc, argv, "dfirvIR", long_opts, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை 'd':
          x.remove_empty_directories = true;
          நிறுத்து;

        நிலை 'f':
          x.interactive = RMI_NEVER;
          x.ignore_missing_files = true;
          prompt_once = false;
          நிறுத்து;

        நிலை 'i':
          x.interactive = RMI_ALWAYS;
          x.ignore_missing_files = false;
          prompt_once = false;
          நிறுத்து;

        நிலை 'I':
          x.interactive = RMI_SOMETIMES;
          x.ignore_missing_files = false;
          prompt_once = true;
          நிறுத்து;

        நிலை 'r':
        நிலை 'R':
          x.recursive = true;
          நிறுத்து;

        நிலை INTERACTIVE_OPTION:
          {
            முழு i;
            எனில் (optarg)
              i = XARGMATCH ("--interactive", optarg, interactive_args,
                             interactive_types);
            இல்லையெனில்
              i = interactive_always;
            தேர்வு (i)
              {
              நிலை interactive_never:
                x.interactive = RMI_NEVER;
                prompt_once = false;
                நிறுத்து;

              நிலை interactive_once:
                x.interactive = RMI_SOMETIMES;
                x.ignore_missing_files = false;
                prompt_once = true;
                நிறுத்து;

              நிலை interactive_always:
                x.interactive = RMI_ALWAYS;
                x.ignore_missing_files = false;
                prompt_once = false;
                நிறுத்து;
              }
            நிறுத்து;
          }

        நிலை ONE_FILE_SYSTEM:
          x.one_file_system = true;
          நிறுத்து;

        நிலை NO_PRESERVE_வேர்:
          எனில் (! streq (argv[optind - 1], "--no-preserve-root"))
            error (EXIT_FAILURE, 0,
                   _("you may not abbreviate the --no-preserve-root option"));
          preserve_வேர் = false;
          நிறுத்து;

        நிலை PRESERVE_வேர்:
          எனில் (optarg)
            {
              எனில் (streq (optarg, "all"))
                x.preserve_all_வேர் = true;
              இல்லையெனில்
                error (EXIT_FAILURE, 0,
                       _("unrecognized --preserve-root argument: %s"),
                       quoteaf (optarg));
            }
          preserve_வேர் = true;
          நிறுத்து;

        நிலை PRESUME_உள்ளீடு_TTY_OPTION:
          x.stdin_tty = true;
          நிறுத்து;

        நிலை 'v':
          x.verbose = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          diagnose_leading_hyphen (argc, argv);
          usage (EXIT_FAILURE);
        }
    }

  எனில் (argc <= optind)
    {
      எனில் (x.ignore_missing_files)
        திரும்பு EXIT_SUCCESS;
      இல்லையெனில்
        {
          error (0, 0, _("missing operand"));
          usage (EXIT_FAILURE);
        }
    }

  எனில் (x.recursive && preserve_வேர்)
    {
      நிலையான கட்டமைப்பு dev_ino dev_ino_buf;
      x.வேர்_dev_ino = get_வேர்_dev_ino (&dev_ino_buf);
      எனில் (x.வேர்_dev_ino == வெற்று)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  uintmax_t n_files = argc - optind;
  எழுத்து **file =  argv + optind;

  எனில் (prompt_once && (x.recursive || 3 < n_files))
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
      எனில் (!yesno ())
        திரும்பு EXIT_SUCCESS;
    }

  எண்ணிக்கை RM_status status = rm (file, &x);
  affirm (VALID_STATUS (status));
  திரும்பு status == RM_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
