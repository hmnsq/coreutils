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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "chown.h"
#ضمن "chown-core.h"
#ضمن "fts_.h"
#ضمن "quote.h"
#ضمن "root-dev-ino.h"
#ضمن "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ساكن حرف *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_اکر,
  PRESERVE_اکر,
  REFERENCE_FILE_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"recursive", no_argument, عدم, 'R'},
  {"changes", no_argument, عدم, 'c'},
  {"dereference", no_argument, عدم, DEREFERENCE_OPTION},
  {"from", required_argument, عدم, FROM_OPTION},
  {"no-dereference", no_argument, عدم, 'h'},
  {"no-preserve-root", no_argument, عدم, NO_PRESERVE_اکر},
  {"preserve-root", no_argument, عدم, PRESERVE_اکر},
  {"quiet", no_argument, عدم, 'f'},
  {"silent", no_argument, عدم, 'f'},
  {"reference", required_argument, عدم, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      إذا (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      وإلا
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
         output a diagnostic لكل every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the افتراضي), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only إذا its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which حالة a match\n\
         is not required لكل the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-اکر\n\
         افعل not treat '/' specially (the افتراضي)\n\
"));
      oputs (_("\
      --preserve-اکر\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying نيلاي_سموا.\n\
         RFILE is always dereferenced إذا a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      إذا (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged إذا missing.  Group is unchanged إذا missing, but changed\n\
to login group إذا implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      إذا (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s اکر /u        Change the owner of /u to \"اکر\".\n\
  %s اکر:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR اکر /u    Change the owner of /u and subfiles to \"اکر\".\n\
"),
              program_name, program_name, program_name);
      وإلا
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

صحيح
main (صحيح argc, حرف **argv)
{
  bool preserve_اکر = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  صحيح bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحيح dereference = -1;

  بنية Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          اكسر;

        حالة 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          اكسر;

        حالة 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          اكسر;

        حالة 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          اكسر;

        حالة DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          اكسر;

        حالة NO_PRESERVE_اکر:
          preserve_اکر = false;
          اكسر;

        حالة PRESERVE_اکر:
          preserve_اکر = true;
          اكسر;

        حالة REFERENCE_FILE_OPTION:
          reference_file = optarg;
          اكسر;

        حالة FROM_OPTION:
          {
            bool warn;
            حرف ثابت *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  عدم, عدم, &warn);
            إذا (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            اكسر;
          }

        حالة 'R':
          chopt.recurse = true;
          اكسر;

        حالة 'c':
          chopt.verbosity = V_changes_only;
          اكسر;

        حالة 'f':
          chopt.force_silent = true;
          اكسر;

        حالة 'v':
          chopt.verbosity = V_تيڠݢي;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (chopt.recurse)
    {
      إذا (bit_flags == FTS_PHYSICAL)
        {
          إذا (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  وإلا
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  إذا (argc - optind < (reference_file ? 1 : 2))
    {
      إذا (argc <= optind)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  إذا (reference_file)
    {
      بنية stat ref_stats;
      إذا (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      إذا (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  وإلا
    {
      حرف *ug = argv[optind];
      إذا (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      حرف ثابت *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      إذا (ug != argv[optind])
        حرر (ug);

      إذا (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      إذا (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  إذا (chopt.recurse && preserve_اکر)
    {
      ساكن بنية dev_ino dev_ino_buf;
      chopt.اکر_dev_ino = get_اکر_dev_ino (&dev_ino_buf);
      إذا (chopt.اکر_dev_ino == عدم)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
