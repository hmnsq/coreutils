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
#وصف_ڪريو پروگرام_NAME (chown_گهڻائي == CHOWN_CHOWN ? "chown" : "chgrp")

#وصف_ڪريو AUTHORS \
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
  NO_PRESERVE_پاڙ,
  PRESERVE_پاڙ,
  REFERENCE_FILE_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"recursive", no_دليل, عدم, 'R'},
  {"changes", no_دليل, عدم, 'c'},
  {"dereference", no_دليل, عدم, DEREFERENCE_OPTION},
  {"from", required_دليل, عدم, FROM_OPTION},
  {"no-dereference", no_دليل, عدم, 'h'},
  {"no-preserve-root", no_دليل, عدم, NO_PRESERVE_پاڙ},
  {"preserve-root", no_دليل, عدم, PRESERVE_پاڙ},
  {"quiet", no_دليل, عدم, 'f'},
  {"silent", no_دليل, عدم, 'f'},
  {"reference", required_دليل, عدم, REFERENCE_FILE_OPTION},
  {"verbose", no_دليل, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              پروگرام_name,
              chown_گهڻائي == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              پروگرام_name);
      إذا (chown_گهڻائي == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or گروهه جو each FILE منزل OWNER and/or گروهه_2.\n\
With --reference, change the owner and گروهه جو each FILE منزل those جو RFILE.\n\
\n\
"), stdout);
      وإلا
        fputs (_("\
Change the گروهه جو each FILE منزل گروهه_2.\n\
With --reference, change the گروهه جو each FILE منزل that جو RFILE.\n\
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
         affect the referent جو each symbolic link (this is\n\
         the افتراضي), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead جو any referenced file;\n\
         useful only on systems that can change the ownership جو a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_گروهه\n\
         change the ownership جو each file only إذا its\n\
         current owner and/or گروهه ميلاپ those specified هتي.\n\
         Either may be omitted, اندريون which حالة a ميلاپ\n\
         is not required لكل the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-پاڙ\n\
         افعل not treat '/' specially (the افتراضي)\n\
"));
      oputs (_("\
      --preserve-پاڙ\n\
         fail منزل هلايو recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying قدرون.\n\
         RFILE is always dereferenced إذا a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         هلايو on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      إذا (chown_گهڻائي == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged إذا missing.  گروهه_3 is unchanged إذا missing, but بدليو\n\
منزل login گروهه إذا implied by a ':' following a symbolic OWNER.\n\
OWNER and گروهه_2 may be numeric as well as symbolic.\n\
"), stdout);

      إذا (chown_گهڻائي == CHOWN_CHOWN)
        لکو (_("\
\n\
Examples:\n\
  %s پاڙ /u        Change the owner جو /u منزل \"پاڙ\".\n\
  %s پاڙ:staff /u  Likewise, but also change its گروهه منزل \"staff\".\n\
  %s -hR پاڙ /u    Change the owner جو /u and subfiles منزل \"پاڙ\".\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      وإلا
        لکو (_("\
\n\
Examples:\n\
  %s staff /u      Change the گروهه جو /u منزل \"staff\".\n\
  %s -hR staff /u  Change the گروهه جو /u and subfiles منزل \"staff\".\n\
"),
              پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  bool preserve_پاڙ = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  صحيح ٻٽو_انگ_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحيح dereference = -1;

  بنية Chown_option chopt;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          ٻٽو_انگ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          اكسر;

        حالة 'L': /* Traverse all symlinks-to-directories.  */
          ٻٽو_انگ_flags = FTS_LOGICAL;
          اكسر;

        حالة 'P': /* Traverse no symlinks-to-directories.  */
          ٻٽو_انگ_flags = FTS_PHYSICAL;
          اكسر;

        حالة 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          اكسر;

        حالة DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          اكسر;

        حالة NO_PRESERVE_پاڙ:
          preserve_پاڙ = false;
          اكسر;

        حالة PRESERVE_پاڙ:
          preserve_پاڙ = true;
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
          chopt.verbosity = V_مٿاهون;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (chopt.recurse)
    {
      إذا (ٻٽو_انگ_flags == FTS_PHYSICAL)
        {
          إذا (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  وإلا
    {
      ٻٽو_انگ_flags = FTS_PHYSICAL;
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

      إذا (chown_گهڻائي == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_منزل_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.گروهه_name = gid_منزل_name (ref_stats.st_gid);
    }
  وإلا
    {
      حرف *ug = argv[optind];
      إذا (chown_گهڻائي == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + لڙي_ڊيگهه (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      حرف ثابت *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.گروهه_name, &warn);

      إذا (ug != argv[optind])
        حرر (ug);

      إذا (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      إذا (chown_گهڻائي == CHOWN_CHOWN && !chopt.user_name && chopt.گروهه_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  إذا (chopt.recurse && preserve_پاڙ)
    {
      ساكن بنية dev_ino dev_ino_buf;
      chopt.پاڙ_dev_ino = get_پاڙ_dev_ino (&dev_ino_buf);
      إذا (chopt.پاڙ_dev_ino == عدم)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  ٻٽو_انگ_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, ٻٽو_انگ_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
