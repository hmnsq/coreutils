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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "chown.h"
#அந்தர்பாவ "chown-core.h"
#அந்தர்பாவ "fts_.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "root-dev-ino.h"
#அந்தர்பாவ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
நிலையான எழுத்து *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_வேர்,
  PRESERVE_வேர்,
  REFERENCE_FILE_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"recursive", no_argument, வெற்று, 'R'},
  {"changes", no_argument, வெற்று, 'c'},
  {"dereference", no_argument, வெற்று, DEREFERENCE_OPTION},
  {"from", required_argument, வெற்று, FROM_OPTION},
  {"no-dereference", no_argument, வெற்று, 'h'},
  {"no-preserve-root", no_argument, வெற்று, NO_PRESERVE_வேர்},
  {"preserve-root", no_argument, வெற்று, PRESERVE_வேர்},
  {"quiet", no_argument, வெற்று, 'f'},
  {"silent", no_argument, வெற்று, 'f'},
  {"reference", required_argument, வெற்று, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      எனில் (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      இல்லையெனில்
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
         output a diagnostic க்கு every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the இயல்பு), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only எனில் its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which நிலை a match\n\
         is not required க்கு the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-வேர்\n\
         செய் not treat '/' specially (the இயல்பு)\n\
"));
      oputs (_("\
      --preserve-வேர்\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying மதிப்புகள்.\n\
         RFILE is always dereferenced எனில் a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      எனில் (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged எனில் missing.  Group is unchanged எனில் missing, but changed\n\
to login group எனில் implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      எனில் (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s வேர் /u        Change the owner of /u to \"வேர்\".\n\
  %s வேர்:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR வேர் /u    Change the owner of /u and subfiles to \"வேர்\".\n\
"),
              program_name, program_name, program_name);
      இல்லையெனில்
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool preserve_வேர் = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  முழு bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  முழு dereference = -1;

  கட்டமைப்பு Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, வெற்று))
         != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          நிறுத்து;

        நிலை 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          நிறுத்து;

        நிலை DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          நிறுத்து;

        நிலை NO_PRESERVE_வேர்:
          preserve_வேர் = false;
          நிறுத்து;

        நிலை PRESERVE_வேர்:
          preserve_வேர் = true;
          நிறுத்து;

        நிலை REFERENCE_FILE_OPTION:
          reference_file = optarg;
          நிறுத்து;

        நிலை FROM_OPTION:
          {
            bool warn;
            எழுத்து மாறிலி *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  வெற்று, வெற்று, &warn);
            எனில் (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            நிறுத்து;
          }

        நிலை 'R':
          chopt.recurse = true;
          நிறுத்து;

        நிலை 'c':
          chopt.verbosity = V_changes_only;
          நிறுத்து;

        நிலை 'f':
          chopt.force_silent = true;
          நிறுத்து;

        நிலை 'v':
          chopt.verbosity = V_உயர்வு;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (chopt.recurse)
    {
      எனில் (bit_flags == FTS_PHYSICAL)
        {
          எனில் (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  இல்லையெனில்
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  எனில் (argc - optind < (reference_file ? 1 : 2))
    {
      எனில் (argc <= optind)
        error (0, 0, _("missing operand"));
      இல்லையெனில்
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  எனில் (reference_file)
    {
      கட்டமைப்பு stat ref_stats;
      எனில் (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      எனில் (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  இல்லையெனில்
    {
      எழுத்து *ug = argv[optind];
      எனில் (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      எழுத்து மாறிலி *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      எனில் (ug != argv[optind])
        விடுவி (ug);

      எனில் (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      எனில் (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  எனில் (chopt.recurse && preserve_வேர்)
    {
      நிலையான கட்டமைப்பு dev_ino dev_ino_buf;
      chopt.வேர்_dev_ino = get_வேர்_dev_ino (&dev_ino_buf);
      எனில் (chopt.வேர்_dev_ino == வெற்று)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
