/* chown, chgrp -- change user and group ownership of files
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu>. */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "chown.h"
#ಅನ್ತರ್ಭಾವ "chown-core.h"
#ಅನ್ತರ್ಭಾವ "fts_.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME (chown_ಬಹುಲಕ == CHOWN_CHOWN ? "chown" : "chgrp")

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ಸ್ಥಿರ ಅಕ್ಷರ *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  REFERENCE_FILE_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"recursive", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"changes", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"dereference", no_ವಾದ, ಖಾಲಿ, DEREFERENCE_OPTION},
  {"from", required_ವಾದ, ಖಾಲಿ, FROM_OPTION},
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"no-preserve-root", no_ವಾದ, ಖಾಲಿ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", no_ವಾದ, ಖಾಲಿ, PRESERVE_ಬೇರು},
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"silent", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"reference", required_ವಾದ, ಖಾಲಿ, REFERENCE_FILE_OPTION},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name,
              chown_ಬಹುಲಕ == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              ಕಾರ್ಯಕ್ರಮ_name);
      ಆದರೆ (chown_ಬಹುಲಕ == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or ಗುಂಪು ದ each FILE ಗಮ್ಯ OWNER and/or ಗುಂಪು_2.\n\
With --reference, change the owner and ಗುಂಪು ದ each FILE ಗಮ್ಯ those ದ RFILE.\n\
\n\
"), stdout);
      ಇಲ್ಲದಿದ್ದರೆ
        fputs (_("\
Change the ಗುಂಪು ದ each FILE ಗಮ್ಯ ಗುಂಪು_2.\n\
With --reference, change the ಗುಂಪು ದ each FILE ಗಮ್ಯ that ದ RFILE.\n\
\n\
"), stdout);
      oputs (_("\
  -c, --changes\n\
         like verbose but report only when a change is made\n\
"));
      oputs (_("\
  -f, --silent, --quiet\n\
         suppress most error messages\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ಗಾಗಿ every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent ದ each symbolic link (this is\n\
         the ಪೂರ್ವನಿಯೋಜಿತ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead ದ any referenced file;\n\
         useful only on systems that can change the ownership ದ a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_ಗುಂಪು\n\
         change the ownership ದ each file only ಆದರೆ its\n\
         current owner and/or ಗುಂಪು ಹೊಂದಿಕೆ those specified ಇಲ್ಲಿ.\n\
         Either may be omitted, ಒಳ which ಸ್ಥಿತಿ a ಹೊಂದಿಕೆ\n\
         is not required ಗಾಗಿ the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಮಾಡು not treat '/' specially (the ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
      --preserve-ಬೇರು\n\
         fail ಗಮ್ಯ ನಿರ್ವಹಿಸು recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying ಮೌಲ್ಯಗಳು.\n\
         RFILE is always dereferenced ಆದರೆ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         ನಿರ್ವಹಿಸು on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಆದರೆ (chown_ಬಹುಲಕ == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged ಆದರೆ missing.  ಗುಂಪು_3 is unchanged ಆದರೆ missing, but ಬದಲಾಯಿತು\n\
ಗಮ್ಯ login ಗುಂಪು ಆದರೆ implied by a ':' following a symbolic OWNER.\n\
OWNER and ಗುಂಪು_2 may be numeric as well as symbolic.\n\
"), stdout);

      ಆದರೆ (chown_ಬಹುಲಕ == CHOWN_CHOWN)
        ಬರೆ (_("\
\n\
Examples:\n\
  %s ಬೇರು /u        Change the owner ದ /u ಗಮ್ಯ \"ಬೇರು\".\n\
  %s ಬೇರು:staff /u  Likewise, but also change its ಗುಂಪು ಗಮ್ಯ \"staff\".\n\
  %s -hR ಬೇರು /u    Change the owner ದ /u and subfiles ಗಮ್ಯ \"ಬೇರು\".\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      ಇಲ್ಲದಿದ್ದರೆ
        ಬರೆ (_("\
\n\
Examples:\n\
  %s staff /u      Change the ಗುಂಪು ದ /u ಗಮ್ಯ \"staff\".\n\
  %s -hR staff /u  Change the ಗುಂಪು ದ /u and subfiles ಗಮ್ಯ \"staff\".\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool preserve_ಬೇರು = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ಪೂರ್ಣಾಂಕ dereference = -1;

  ರಚನೆ Chown_option chopt;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'H': /* Traverse command-line symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L': /* Traverse all symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_LOGICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'P': /* Traverse no symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NO_PRESERVE_ಬೇರು:
          preserve_ಬೇರು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_ಬೇರು:
          preserve_ಬೇರು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FROM_OPTION:
          {
            bool warn;
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ಖಾಲಿ, ಖಾಲಿ, &warn);
            ಆದರೆ (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ 'R':
          chopt.recurse = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          chopt.verbosity = V_changes_only;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          chopt.force_silent = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          chopt.verbosity = V_ಮೇಲಿನ;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (chopt.recurse)
    {
      ಆದರೆ (ಬಿಟ್_flags == FTS_PHYSICAL)
        {
          ಆದರೆ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬಿಟ್_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  ಆದರೆ (argc - optind < (reference_file ? 1 : 2))
    {
      ಆದರೆ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  ಆದರೆ (reference_file)
    {
      ರಚನೆ stat ref_stats;
      ಆದರೆ (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      ಆದರೆ (chown_ಬಹುಲಕ == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_ಗಮ್ಯ_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.ಗುಂಪು_name = gid_ಗಮ್ಯ_name (ref_stats.st_gid);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *ug = argv[optind];
      ಆದರೆ (chown_ಬಹುಲಕ == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.ಗುಂಪು_name, &warn);

      ಆದರೆ (ug != argv[optind])
        ಬಿಡುಗಡೆ (ug);

      ಆದರೆ (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      ಆದರೆ (chown_ಬಹುಲಕ == CHOWN_CHOWN && !chopt.user_name && chopt.ಗುಂಪು_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  ಆದರೆ (chopt.recurse && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ರಚನೆ dev_ino dev_ino_buf;
      chopt.ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಆದರೆ (chopt.ಬೇರು_dev_ino == ಖಾಲಿ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  ಬಿಟ್_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, ಬಿಟ್_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
