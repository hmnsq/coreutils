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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "system.h"
#شامل "chown.h"
#شامل "chown-core.h"
#شامل "fts_.h"
#شامل "quote.h"
#شامل "root-dev-ino.h"
#شامل "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ایستا نویسه *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_ریشه,
  PRESERVE_ریشه,
  REFERENCE_FILE_OPTION
};

ایستا ساختار option ثابت long_options[] =
{
  {"recursive", no_argument, پوچ, 'R'},
  {"changes", no_argument, پوچ, 'c'},
  {"dereference", no_argument, پوچ, DEREFERENCE_OPTION},
  {"from", required_argument, پوچ, FROM_OPTION},
  {"no-dereference", no_argument, پوچ, 'h'},
  {"no-preserve-root", no_argument, پوچ, NO_PRESERVE_ریشه},
  {"preserve-root", no_argument, پوچ, PRESERVE_ریشه},
  {"quiet", no_argument, پوچ, 'f'},
  {"silent", no_argument, پوچ, 'f'},
  {"reference", required_argument, پوچ, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      اگر (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      وگرنه
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
         output a diagnostic برای every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the پیشفرض), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only اگر its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which حالت a match\n\
         is not required برای the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-ریشه\n\
         انجامبده not treat '/' specially (the پیشفرض)\n\
"));
      oputs (_("\
      --preserve-ریشه\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying مقادیر.\n\
         RFILE is always dereferenced اگر a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      اگر (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged اگر missing.  Group is unchanged اگر missing, but changed\n\
to login group اگر implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      اگر (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s ریشه /u        Change the owner of /u to \"ریشه\".\n\
  %s ریشه:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR ریشه /u    Change the owner of /u and subfiles to \"ریشه\".\n\
"),
              program_name, program_name, program_name);
      وگرنه
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

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool preserve_ریشه = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  صحیح bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحیح dereference = -1;

  ساختار Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, پوچ))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          بشکن;

        حالت 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          بشکن;

        حالت 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          بشکن;

        حالت 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          بشکن;

        حالت DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          بشکن;

        حالت NO_PRESERVE_ریشه:
          preserve_ریشه = false;
          بشکن;

        حالت PRESERVE_ریشه:
          preserve_ریشه = true;
          بشکن;

        حالت REFERENCE_FILE_OPTION:
          reference_file = optarg;
          بشکن;

        حالت FROM_OPTION:
          {
            bool warn;
            نویسه ثابت *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  پوچ, پوچ, &warn);
            اگر (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            بشکن;
          }

        حالت 'R':
          chopt.recurse = true;
          بشکن;

        حالت 'c':
          chopt.verbosity = V_changes_only;
          بشکن;

        حالت 'f':
          chopt.force_silent = true;
          بشکن;

        حالت 'v':
          chopt.verbosity = V_بالا;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (chopt.recurse)
    {
      اگر (bit_flags == FTS_PHYSICAL)
        {
          اگر (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  وگرنه
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  اگر (argc - optind < (reference_file ? 1 : 2))
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      وگرنه
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  اگر (reference_file)
    {
      ساختار stat ref_stats;
      اگر (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      اگر (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  وگرنه
    {
      نویسه *ug = argv[optind];
      اگر (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      نویسه ثابت *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      اگر (ug != argv[optind])
        آزاد (ug);

      اگر (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      اگر (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  اگر (chopt.recurse && preserve_ریشه)
    {
      ایستا ساختار dev_ino dev_ino_buf;
      chopt.ریشه_dev_ino = get_ریشه_dev_ino (&dev_ino_buf);
      اگر (chopt.ریشه_dev_ino == پوچ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
