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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "chown.h"
#अन्तर्भाव "chown-core.h"
#अन्तर्भाव "fts_.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
स्थिर वर्ण *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_मूलम्,
  PRESERVE_मूलम्,
  REFERENCE_FILE_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"recursive", no_argument, रिक्त, 'R'},
  {"changes", no_argument, रिक्त, 'c'},
  {"dereference", no_argument, रिक्त, DEREFERENCE_OPTION},
  {"from", required_argument, रिक्त, FROM_OPTION},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_मूलम्},
  {"preserve-root", no_argument, रिक्त, PRESERVE_मूलम्},
  {"quiet", no_argument, रिक्त, 'f'},
  {"silent", no_argument, रिक्त, 'f'},
  {"reference", required_argument, रिक्त, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      जइ (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      अण्णहा
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
         output a diagnostic प्रति every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the स्वभाव), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only जइ its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which पक्ष a match\n\
         is not required प्रति the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-मूलम्\n\
         करेहि not treat '/' specially (the स्वभाव)\n\
"));
      oputs (_("\
      --preserve-मूलम्\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying मानानि.\n\
         RFILE is always dereferenced जइ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      जइ (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged जइ missing.  Group is unchanged जइ missing, but changed\n\
to login group जइ implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      जइ (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s मूलम् /u        Change the owner of /u to \"मूलम्\".\n\
  %s मूलम्:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR मूलम् /u    Change the owner of /u and subfiles to \"मूलम्\".\n\
"),
              program_name, program_name, program_name);
      अण्णहा
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

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool preserve_मूलम् = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  पूर्णाङ्क bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  पूर्णाङ्क dereference = -1;

  संरचना Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  पूर्णाङ्क optc;
  जाव ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          भङ्ग;

        पक्ष 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          भङ्ग;

        पक्ष DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          भङ्ग;

        पक्ष NO_PRESERVE_मूलम्:
          preserve_मूलम् = false;
          भङ्ग;

        पक्ष PRESERVE_मूलम्:
          preserve_मूलम् = true;
          भङ्ग;

        पक्ष REFERENCE_FILE_OPTION:
          reference_file = optarg;
          भङ्ग;

        पक्ष FROM_OPTION:
          {
            bool warn;
            वर्ण ध्रुव *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  रिक्त, रिक्त, &warn);
            जइ (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            भङ्ग;
          }

        पक्ष 'R':
          chopt.recurse = true;
          भङ्ग;

        पक्ष 'c':
          chopt.verbosity = V_changes_only;
          भङ्ग;

        पक्ष 'f':
          chopt.force_silent = true;
          भङ्ग;

        पक्ष 'v':
          chopt.verbosity = V_उच्चः;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  जइ (chopt.recurse)
    {
      जइ (bit_flags == FTS_PHYSICAL)
        {
          जइ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  अण्णहा
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  जइ (argc - optind < (reference_file ? 1 : 2))
    {
      जइ (argc <= optind)
        error (0, 0, _("missing operand"));
      अण्णहा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  जइ (reference_file)
    {
      संरचना stat ref_stats;
      जइ (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      जइ (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  अण्णहा
    {
      वर्ण *ug = argv[optind];
      जइ (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      वर्ण ध्रुव *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      जइ (ug != argv[optind])
        मुक्त (ug);

      जइ (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      जइ (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  जइ (chopt.recurse && preserve_मूलम्)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      chopt.मूलम्_dev_ino = get_मूलम्_dev_ino (&dev_ino_buf);
      जइ (chopt.मूलम्_dev_ino == रिक्त)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
