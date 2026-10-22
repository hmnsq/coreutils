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
স্থির বর্ণ *reference_file;

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

স্থির সংরচনা option ধ্রুব long_options[] =
{
  {"recursive", no_argument, রিক্ত, 'R'},
  {"changes", no_argument, রিক্ত, 'c'},
  {"dereference", no_argument, রিক্ত, DEREFERENCE_OPTION},
  {"from", required_argument, রিক্ত, FROM_OPTION},
  {"no-dereference", no_argument, রিক্ত, 'h'},
  {"no-preserve-root", no_argument, রিক্ত, NO_PRESERVE_মূল},
  {"preserve-root", no_argument, রিক্ত, PRESERVE_মূল},
  {"quiet", no_argument, রিক্ত, 'f'},
  {"silent", no_argument, রিক্ত, 'f'},
  {"reference", required_argument, রিক্ত, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
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
         output a diagnostic প্রতি every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the স্বভাব), rather than the symbolic link itself\n\
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
         Either may be omitted, in which পক্ষ a match\n\
         is not required প্রতি the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-মূল\n\
         কুরু not treat '/' specially (the স্বভাব)\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  bool preserve_মূল = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  পূর্ণাঙ্ক bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  পূর্ণাঙ্ক dereference = -1;

  সংরচনা Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  পূর্ণাঙ্ক optc;
  যাবত্ ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, রিক্ত))
         != -1)
    {
      বিকল্প (optc)
        {
        পক্ষ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ভঙ্গ;

        পক্ষ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ভঙ্গ;

        পক্ষ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ভঙ্গ;

        পক্ষ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ভঙ্গ;

        পক্ষ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ভঙ্গ;

        পক্ষ NO_PRESERVE_মূল:
          preserve_মূল = false;
          ভঙ্গ;

        পক্ষ PRESERVE_মূল:
          preserve_মূল = true;
          ভঙ্গ;

        পক্ষ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ভঙ্গ;

        পক্ষ FROM_OPTION:
          {
            bool warn;
            বর্ণ ধ্রুব *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  রিক্ত, রিক্ত, &warn);
            যদি (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            ভঙ্গ;
          }

        পক্ষ 'R':
          chopt.recurse = true;
          ভঙ্গ;

        পক্ষ 'c':
          chopt.verbosity = V_changes_only;
          ভঙ্গ;

        পক্ষ 'f':
          chopt.force_silent = true;
          ভঙ্গ;

        পক্ষ 'v':
          chopt.verbosity = V_উপরের;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্বভাব:
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
      সংরচনা stat ref_stats;
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
      বর্ণ *ug = argv[optind];
      যদি (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      বর্ণ ধ্রুব *e = parse_user_spec_warn (ug, &uid, &gid,
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
      স্থির সংরচনা dev_ino dev_ino_buf;
      chopt.মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
      যদি (chopt.মূল_dev_ino == রিক্ত)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
