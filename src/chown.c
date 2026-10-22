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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "chown.h"
#ଅନ୍ତର୍ଭାଵ "chown-core.h"
#ଅନ୍ତର୍ଭାଵ "fts_.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "root-dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ସ୍ଥିର ଅକ୍ଷର *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_ମୂଳ,
  PRESERVE_ମୂଳ,
  REFERENCE_FILE_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"recursive", no_argument, ଖାଲି, 'R'},
  {"changes", no_argument, ଖାଲି, 'c'},
  {"dereference", no_argument, ଖାଲି, DEREFERENCE_OPTION},
  {"from", required_argument, ଖାଲି, FROM_OPTION},
  {"no-dereference", no_argument, ଖାଲି, 'h'},
  {"no-preserve-root", no_argument, ଖାଲି, NO_PRESERVE_ମୂଳ},
  {"preserve-root", no_argument, ଖାଲି, PRESERVE_ମୂଳ},
  {"quiet", no_argument, ଖାଲି, 'f'},
  {"silent", no_argument, ଖାଲି, 'f'},
  {"reference", required_argument, ଖାଲି, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      ଯଦି (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      ଅନ୍ୟଥା
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
         output a diagnostic ପାଇଁ every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the ପୂର୍ବନିର୍ଦ୍ଧାରିତ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only ଯଦି its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which ସ୍ଥିତି a match\n\
         is not required ପାଇଁ the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-ମୂଳ\n\
         କର not treat '/' specially (the ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
      oputs (_("\
      --preserve-ମୂଳ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying ମୂଲ୍ୟଗୁଡ଼ିକ.\n\
         RFILE is always dereferenced ଯଦି a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ଯଦି (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged ଯଦି missing.  Group is unchanged ଯଦି missing, but changed\n\
to login group ଯଦି implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      ଯଦି (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s ମୂଳ /u        Change the owner of /u to \"ମୂଳ\".\n\
  %s ମୂଳ:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR ମୂଳ /u    Change the owner of /u and subfiles to \"ମୂଳ\".\n\
"),
              program_name, program_name, program_name);
      ଅନ୍ୟଥା
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool preserve_ମୂଳ = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dereference = -1;

  ସଂରଚନା Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି NO_PRESERVE_ମୂଳ:
          preserve_ମୂଳ = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PRESERVE_ମୂଳ:
          preserve_ମୂଳ = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FROM_OPTION:
          {
            bool warn;
            ଅକ୍ଷର ଅଚଳ *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ଖାଲି, ଖାଲି, &warn);
            ଯଦି (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି 'R':
          chopt.recurse = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':
          chopt.verbosity = V_changes_only;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          chopt.force_silent = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':
          chopt.verbosity = V_ଉପର;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (chopt.recurse)
    {
      ଯଦି (bit_flags == FTS_PHYSICAL)
        {
          ଯଦି (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  ଅନ୍ୟଥା
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  ଯଦି (argc - optind < (reference_file ? 1 : 2))
    {
      ଯଦି (argc <= optind)
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  ଯଦି (reference_file)
    {
      ସଂରଚନା stat ref_stats;
      ଯଦି (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      ଯଦି (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *ug = argv[optind];
      ଯଦି (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      ଅକ୍ଷର ଅଚଳ *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      ଯଦି (ug != argv[optind])
        ମୁକ୍ତ (ug);

      ଯଦି (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      ଯଦି (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  ଯଦି (chopt.recurse && preserve_ମୂଳ)
    {
      ସ୍ଥିର ସଂରଚନା dev_ino dev_ino_buf;
      chopt.ମୂଳ_dev_ino = get_ମୂଳ_dev_ino (&dev_ino_buf);
      ଯଦି (chopt.ମୂଳ_dev_ino == ଖାଲି)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
