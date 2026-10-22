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
#تعریف_کرو پروگرام_NAME (chown_منوال == CHOWN_CHOWN ? "chown" : "chgrp")

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ساکن حرف *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_جڑ,
  PRESERVE_جڑ,
  REFERENCE_FILE_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"recursive", no_دلیل, تہی, 'R'},
  {"changes", no_دلیل, تہی, 'c'},
  {"dereference", no_دلیل, تہی, DEREFERENCE_OPTION},
  {"from", required_دلیل, تہی, FROM_OPTION},
  {"no-dereference", no_دلیل, تہی, 'h'},
  {"no-preserve-root", no_دلیل, تہی, NO_PRESERVE_جڑ},
  {"preserve-root", no_دلیل, تہی, PRESERVE_جڑ},
  {"quiet", no_دلیل, تہی, 'f'},
  {"silent", no_دلیل, تہی, 'f'},
  {"reference", required_دلیل, تہی, REFERENCE_FILE_OPTION},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              پروگرام_name,
              chown_منوال == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              پروگرام_name);
      اگر (chown_منوال == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or گروہ کا each FILE مقصد OWNER and/or گروہ_2.\n\
With --reference, change the owner and گروہ کا each FILE مقصد those کا RFILE.\n\
\n\
"), stdout);
      ورنہ
        fputs (_("\
Change the گروہ کا each FILE مقصد گروہ_2.\n\
With --reference, change the گروہ کا each FILE مقصد that کا RFILE.\n\
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
         output a diagnostic برائے every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent کا each symbolic link (this is\n\
         the طےشدہ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead کا any referenced file;\n\
         useful only on systems that can change the ownership کا a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_گروہ\n\
         change the ownership کا each file only اگر its\n\
         current owner and/or گروہ مطابقت those specified یہاں.\n\
         Either may be omitted, اندر which حالت a مطابقت\n\
         is not required برائے the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-جڑ\n\
         کرو not treat '/' specially (the طےشدہ)\n\
"));
      oputs (_("\
      --preserve-جڑ\n\
         fail مقصد عمل_کرو recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying اقدار.\n\
         RFILE is always dereferenced اگر a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         عمل_کرو on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      اگر (chown_منوال == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged اگر missing.  گروہ_3 is unchanged اگر missing, but بدلا\n\
مقصد login گروہ اگر implied by a ':' following a symbolic OWNER.\n\
OWNER and گروہ_2 may be numeric as well as symbolic.\n\
"), stdout);

      اگر (chown_منوال == CHOWN_CHOWN)
        لکھو (_("\
\n\
Examples:\n\
  %s جڑ /u        Change the owner کا /u مقصد \"جڑ\".\n\
  %s جڑ:staff /u  Likewise, but also change its گروہ مقصد \"staff\".\n\
  %s -hR جڑ /u    Change the owner کا /u and subfiles مقصد \"جڑ\".\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      ورنہ
        لکھو (_("\
\n\
Examples:\n\
  %s staff /u      Change the گروہ کا /u مقصد \"staff\".\n\
  %s -hR staff /u  Change the گروہ کا /u and subfiles مقصد \"staff\".\n\
"),
              پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool preserve_جڑ = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  صحیح ثنائی_ہندسہ_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحیح dereference = -1;

  ساخت Chown_option chopt;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, تہی))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'H': /* Traverse command-line symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          توڑو;

        حالت 'L': /* Traverse all symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_LOGICAL;
          توڑو;

        حالت 'P': /* Traverse no symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_PHYSICAL;
          توڑو;

        حالت 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          توڑو;

        حالت DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          توڑو;

        حالت NO_PRESERVE_جڑ:
          preserve_جڑ = false;
          توڑو;

        حالت PRESERVE_جڑ:
          preserve_جڑ = true;
          توڑو;

        حالت REFERENCE_FILE_OPTION:
          reference_file = optarg;
          توڑو;

        حالت FROM_OPTION:
          {
            bool warn;
            حرف ثابت *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  تہی, تہی, &warn);
            اگر (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            توڑو;
          }

        حالت 'R':
          chopt.recurse = true;
          توڑو;

        حالت 'c':
          chopt.verbosity = V_changes_only;
          توڑو;

        حالت 'f':
          chopt.force_silent = true;
          توڑو;

        حالت 'v':
          chopt.verbosity = V_بلند;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (chopt.recurse)
    {
      اگر (ثنائی_ہندسہ_flags == FTS_PHYSICAL)
        {
          اگر (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  ورنہ
    {
      ثنائی_ہندسہ_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  اگر (argc - optind < (reference_file ? 1 : 2))
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      ورنہ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  اگر (reference_file)
    {
      ساخت stat ref_stats;
      اگر (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      اگر (chown_منوال == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_مقصد_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.گروہ_name = gid_مقصد_name (ref_stats.st_gid);
    }
  ورنہ
    {
      حرف *ug = argv[optind];
      اگر (chown_منوال == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + لڑی_لمبائی (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      حرف ثابت *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.گروہ_name, &warn);

      اگر (ug != argv[optind])
        آزاد (ug);

      اگر (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      اگر (chown_منوال == CHOWN_CHOWN && !chopt.user_name && chopt.گروہ_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  اگر (chopt.recurse && preserve_جڑ)
    {
      ساکن ساخت dev_ino dev_ino_buf;
      chopt.جڑ_dev_ino = get_جڑ_dev_ino (&dev_ino_buf);
      اگر (chopt.جڑ_dev_ino == تہی)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  ثنائی_ہندسہ_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, ثنائی_ہندسہ_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
