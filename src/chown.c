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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "chown.h"
#അന്തര്ഭാവ "chown-core.h"
#അന്തര്ഭാവ "fts_.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "root-dev-ino.h"
#അന്തര്ഭാവ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
സ്ഥിതം അക്ഷരം *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_വേര്,
  PRESERVE_വേര്,
  REFERENCE_FILE_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"recursive", no_argument, ശൂന്യ, 'R'},
  {"changes", no_argument, ശൂന്യ, 'c'},
  {"dereference", no_argument, ശൂന്യ, DEREFERENCE_OPTION},
  {"from", required_argument, ശൂന്യ, FROM_OPTION},
  {"no-dereference", no_argument, ശൂന്യ, 'h'},
  {"no-preserve-root", no_argument, ശൂന്യ, NO_PRESERVE_വേര്},
  {"preserve-root", no_argument, ശൂന്യ, PRESERVE_വേര്},
  {"quiet", no_argument, ശൂന്യ, 'f'},
  {"silent", no_argument, ശൂന്യ, 'f'},
  {"reference", required_argument, ശൂന്യ, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      എങ്കിൽ (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      അല്ലെങ്കിൽ
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
         output a diagnostic വേണ്ടി every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the സ്ഥിരസ്ഥിതി), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only എങ്കിൽ its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which അവസ്ഥ a match\n\
         is not required വേണ്ടി the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-വേര്\n\
         ചെയ്യുക not treat '/' specially (the സ്ഥിരസ്ഥിതി)\n\
"));
      oputs (_("\
      --preserve-വേര്\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying മൂല്യങ്ങൾ.\n\
         RFILE is always dereferenced എങ്കിൽ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      എങ്കിൽ (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged എങ്കിൽ missing.  Group is unchanged എങ്കിൽ missing, but changed\n\
to login group എങ്കിൽ implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      എങ്കിൽ (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s വേര് /u        Change the owner of /u to \"വേര്\".\n\
  %s വേര്:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR വേര് /u    Change the owner of /u and subfiles to \"വേര്\".\n\
"),
              program_name, program_name, program_name);
      അല്ലെങ്കിൽ
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool preserve_വേര് = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  പൂർണ്ണസംഖ്യ bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  പൂർണ്ണസംഖ്യ dereference = -1;

  ഘടന Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          നിർത്തുക;

        അവസ്ഥ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          നിർത്തുക;

        അവസ്ഥ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          നിർത്തുക;

        അവസ്ഥ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          നിർത്തുക;

        അവസ്ഥ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          നിർത്തുക;

        അവസ്ഥ NO_PRESERVE_വേര്:
          preserve_വേര് = false;
          നിർത്തുക;

        അവസ്ഥ PRESERVE_വേര്:
          preserve_വേര് = true;
          നിർത്തുക;

        അവസ്ഥ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          നിർത്തുക;

        അവസ്ഥ FROM_OPTION:
          {
            bool warn;
            അക്ഷരം സ്ഥിരം *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ശൂന്യ, ശൂന്യ, &warn);
            എങ്കിൽ (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            നിർത്തുക;
          }

        അവസ്ഥ 'R':
          chopt.recurse = true;
          നിർത്തുക;

        അവസ്ഥ 'c':
          chopt.verbosity = V_changes_only;
          നിർത്തുക;

        അവസ്ഥ 'f':
          chopt.force_silent = true;
          നിർത്തുക;

        അവസ്ഥ 'v':
          chopt.verbosity = V_മുകളിലെ;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (chopt.recurse)
    {
      എങ്കിൽ (bit_flags == FTS_PHYSICAL)
        {
          എങ്കിൽ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  അല്ലെങ്കിൽ
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  എങ്കിൽ (argc - optind < (reference_file ? 1 : 2))
    {
      എങ്കിൽ (argc <= optind)
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  എങ്കിൽ (reference_file)
    {
      ഘടന stat ref_stats;
      എങ്കിൽ (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      എങ്കിൽ (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *ug = argv[optind];
      എങ്കിൽ (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      അക്ഷരം സ്ഥിരം *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      എങ്കിൽ (ug != argv[optind])
        സ്വതന്ത്രം (ug);

      എങ്കിൽ (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      എങ്കിൽ (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  എങ്കിൽ (chopt.recurse && preserve_വേര്)
    {
      സ്ഥിതം ഘടന dev_ino dev_ino_buf;
      chopt.വേര്_dev_ino = get_വേര്_dev_ino (&dev_ino_buf);
      എങ്കിൽ (chopt.വേര്_dev_ino == ശൂന്യ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
