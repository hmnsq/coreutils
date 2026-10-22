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
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ಸ್ಥಿರ ವರ್ಣ *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  REFERENCE_FILE_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"recursive", no_argument, ರಿಕ್ತ, 'R'},
  {"changes", no_argument, ರಿಕ್ತ, 'c'},
  {"dereference", no_argument, ರಿಕ್ತ, DEREFERENCE_OPTION},
  {"from", required_argument, ರಿಕ್ತ, FROM_OPTION},
  {"no-dereference", no_argument, ರಿಕ್ತ, 'h'},
  {"no-preserve-root", no_argument, ರಿಕ್ತ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", no_argument, ರಿಕ್ತ, PRESERVE_ಬೇರು},
  {"quiet", no_argument, ರಿಕ್ತ, 'f'},
  {"silent", no_argument, ರಿಕ್ತ, 'f'},
  {"reference", required_argument, ರಿಕ್ತ, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      ಯದಿ (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      ಅನ್ಯಥಾ
        fputs (_("\
Change the group of each FILE to GROUP.\n\
With --reference, change the group of each FILE to that of RFILE.\n\
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
         output a diagnostic ಪ್ರತಿ every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the ಸ್ವಭಾವ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only ಯದಿ its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which ಪಕ್ಷ a match\n\
         is not required ಪ್ರತಿ the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಕುರು not treat '/' specially (the ಸ್ವಭಾವ)\n\
"));
      oputs (_("\
      --preserve-ಬೇರು\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying ಮೌಲ್ಯಗಳು.\n\
         RFILE is always dereferenced ಯದಿ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ಯದಿ (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged ಯದಿ missing.  Group is unchanged ಯದಿ missing, but changed\n\
to login group ಯದಿ implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      ಯದಿ (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s ಬೇರು /u        Change the owner of /u to \"ಬೇರು\".\n\
  %s ಬೇರು:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR ಬೇರು /u    Change the owner of /u and subfiles to \"ಬೇರು\".\n\
"),
              program_name, program_name, program_name);
      ಅನ್ಯಥಾ
        printf (_("\
\n\
Examples:\n\
  %s staff /u      Change the group of /u to \"staff\".\n\
  %s -hR staff /u  Change the group of /u and subfiles to \"staff\".\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool preserve_ಬೇರು = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  ಪೂರ್ಣಾಙ್ಕ bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ಪೂರ್ಣಾಙ್ಕ dereference = -1;

  ಸಂರಚನಾ Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ಭಙ್ಗ;

        ಪಕ್ಷ NO_PRESERVE_ಬೇರು:
          preserve_ಬೇರು = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_ಬೇರು:
          preserve_ಬೇರು = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ FROM_OPTION:
          {
            bool warn;
            ವರ್ಣ ಧ್ರುವ *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ರಿಕ್ತ, ರಿಕ್ತ, &warn);
            ಯದಿ (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ 'R':
          chopt.recurse = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'c':
          chopt.verbosity = V_changes_only;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          chopt.force_silent = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          chopt.verbosity = V_ಮೇಲಿನ;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (chopt.recurse)
    {
      ಯದಿ (bit_flags == FTS_PHYSICAL)
        {
          ಯದಿ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  ಅನ್ಯಥಾ
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  ಯದಿ (argc - optind < (reference_file ? 1 : 2))
    {
      ಯದಿ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  ಯದಿ (reference_file)
    {
      ಸಂರಚನಾ stat ref_stats;
      ಯದಿ (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      ಯದಿ (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *ug = argv[optind];
      ಯದಿ (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      ವರ್ಣ ಧ್ರುವ *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      ಯದಿ (ug != argv[optind])
        ಮುಕ್ತ (ug);

      ಯದಿ (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      ಯದಿ (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  ಯದಿ (chopt.recurse && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ಸಂರಚನಾ dev_ino dev_ino_buf;
      chopt.ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಯದಿ (chopt.ಬೇರು_dev_ino == ರಿಕ್ತ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
