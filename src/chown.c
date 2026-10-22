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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "chown.h"
#অন্তর্ভাব "chown-core.h"
#অন্তর্ভাব "fts_.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "root-dev-ino.h"
#অন্তর্ভাব "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
স্থির অক্ষর *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_মূল,
  PRESERVE_মূল,
  REFERENCE_FILE_OPTION
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"recursive", no_argument, ফাঁকা, 'R'},
  {"changes", no_argument, ফাঁকা, 'c'},
  {"dereference", no_argument, ফাঁকা, DEREFERENCE_OPTION},
  {"from", required_argument, ফাঁকা, FROM_OPTION},
  {"no-dereference", no_argument, ফাঁকা, 'h'},
  {"no-preserve-root", no_argument, ফাঁকা, NO_PRESERVE_মূল},
  {"preserve-root", no_argument, ফাঁকা, PRESERVE_মূল},
  {"quiet", no_argument, ফাঁকা, 'f'},
  {"silent", no_argument, ফাঁকা, 'f'},
  {"reference", required_argument, ফাঁকা, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      যদি (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      অন্যথা
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
         output a diagnostic জন্য every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the পূর্বনির্ধারিত), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only যদি its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which ক্ষেত্র a match\n\
         is not required জন্য the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-মূল\n\
         করো not treat '/' specially (the পূর্বনির্ধারিত)\n\
"));
      oputs (_("\
      --preserve-মূল\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying মানসমূহ.\n\
         RFILE is always dereferenced যদি a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      যদি (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged যদি missing.  Group is unchanged যদি missing, but changed\n\
to login group যদি implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      যদি (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s মূল /u        Change the owner of /u to \"মূল\".\n\
  %s মূল:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR মূল /u    Change the owner of /u and subfiles to \"মূল\".\n\
"),
              program_name, program_name, program_name);
      অন্যথা
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

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool preserve_মূল = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  পূর্ণসংখ্যা bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  পূর্ণসংখ্যা dereference = -1;

  কাঠামো Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  পূর্ণসংখ্যা optc;
  যখন ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ফাঁকা))
         != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          বিরতি;

        ক্ষেত্র 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          বিরতি;

        ক্ষেত্র 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          বিরতি;

        ক্ষেত্র 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          বিরতি;

        ক্ষেত্র DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          বিরতি;

        ক্ষেত্র NO_PRESERVE_মূল:
          preserve_মূল = false;
          বিরতি;

        ক্ষেত্র PRESERVE_মূল:
          preserve_মূল = true;
          বিরতি;

        ক্ষেত্র REFERENCE_FILE_OPTION:
          reference_file = optarg;
          বিরতি;

        ক্ষেত্র FROM_OPTION:
          {
            bool warn;
            অক্ষর ধ্রুবক *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ফাঁকা, ফাঁকা, &warn);
            যদি (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            বিরতি;
          }

        ক্ষেত্র 'R':
          chopt.recurse = true;
          বিরতি;

        ক্ষেত্র 'c':
          chopt.verbosity = V_changes_only;
          বিরতি;

        ক্ষেত্র 'f':
          chopt.force_silent = true;
          বিরতি;

        ক্ষেত্র 'v':
          chopt.verbosity = V_উপরের;
          বিরতি;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (chopt.recurse)
    {
      যদি (bit_flags == FTS_PHYSICAL)
        {
          যদি (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  অন্যথা
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  যদি (argc - optind < (reference_file ? 1 : 2))
    {
      যদি (argc <= optind)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  যদি (reference_file)
    {
      কাঠামো stat ref_stats;
      যদি (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      যদি (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  অন্যথা
    {
      অক্ষর *ug = argv[optind];
      যদি (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      অক্ষর ধ্রুবক *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      যদি (ug != argv[optind])
        মুক্ত (ug);

      যদি (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      যদি (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  যদি (chopt.recurse && preserve_মূল)
    {
      স্থির কাঠামো dev_ino dev_ino_buf;
      chopt.মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
      যদি (chopt.মূল_dev_ino == ফাঁকা)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
