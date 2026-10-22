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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "chown.h"
#అన్తర్భావ "chown-core.h"
#అన్తర్భావ "fts_.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME (chown_బహుళకం == CHOWN_CHOWN ? "chown" : "chgrp")

#నిర్వచించు AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
స్థిర అక్షరం *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_మూలం,
  PRESERVE_మూలం,
  REFERENCE_FILE_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"recursive", no_వాదం, ఖాళీ, 'R'},
  {"changes", no_వాదం, ఖాళీ, 'c'},
  {"dereference", no_వాదం, ఖాళీ, DEREFERENCE_OPTION},
  {"from", required_వాదం, ఖాళీ, FROM_OPTION},
  {"no-dereference", no_వాదం, ఖాళీ, 'h'},
  {"no-preserve-root", no_వాదం, ఖాళీ, NO_PRESERVE_మూలం},
  {"preserve-root", no_వాదం, ఖాళీ, PRESERVE_మూలం},
  {"quiet", no_వాదం, ఖాళీ, 'f'},
  {"silent", no_వాదం, ఖాళీ, 'f'},
  {"reference", required_వాదం, ఖాళీ, REFERENCE_FILE_OPTION},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              కార్యక్రమం_name,
              chown_బహుళకం == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              కార్యక్రమం_name);
      ఒకవేళ (chown_బహుళకం == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or సమూహం యొక్క each FILE గమ్యం OWNER and/or సమూహం_2.\n\
With --reference, change the owner and సమూహం యొక్క each FILE గమ్యం those యొక్క RFILE.\n\
\n\
"), stdout);
      లేకపోతే
        fputs (_("\
Change the సమూహం యొక్క each FILE గమ్యం సమూహం_2.\n\
With --reference, change the సమూహం యొక్క each FILE గమ్యం that యొక్క RFILE.\n\
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
         output a diagnostic కొరకు every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent యొక్క each symbolic link (this is\n\
         the అప్రమేయ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead యొక్క any referenced file;\n\
         useful only on systems that can change the ownership యొక్క a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_సమూహం\n\
         change the ownership యొక్క each file only ఒకవేళ its\n\
         current owner and/or సమూహం సరిపోలిక those specified ఇక్కడ.\n\
         Either may be omitted, లోపల which స్థితి a సరిపోలిక\n\
         is not required కొరకు the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-మూలం\n\
         చేయి not treat '/' specially (the అప్రమేయ)\n\
"));
      oputs (_("\
      --preserve-మూలం\n\
         fail గమ్యం నిర్వహించు recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying విలువలు.\n\
         RFILE is always dereferenced ఒకవేళ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         నిర్వహించు on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      ఒకవేళ (chown_బహుళకం == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged ఒకవేళ missing.  సమూహం_3 is unchanged ఒకవేళ missing, but మారింది\n\
గమ్యం login సమూహం ఒకవేళ implied by a ':' following a symbolic OWNER.\n\
OWNER and సమూహం_2 may be numeric as well as symbolic.\n\
"), stdout);

      ఒకవేళ (chown_బహుళకం == CHOWN_CHOWN)
        రాయి (_("\
\n\
Examples:\n\
  %s మూలం /u        Change the owner యొక్క /u గమ్యం \"మూలం\".\n\
  %s మూలం:staff /u  Likewise, but also change its సమూహం గమ్యం \"staff\".\n\
  %s -hR మూలం /u    Change the owner యొక్క /u and subfiles గమ్యం \"మూలం\".\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      లేకపోతే
        రాయి (_("\
\n\
Examples:\n\
  %s staff /u      Change the సమూహం యొక్క /u గమ్యం \"staff\".\n\
  %s -hR staff /u  Change the సమూహం యొక్క /u and subfiles గమ్యం \"staff\".\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool preserve_మూలం = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  పూర్ణాంకం బిట్టు_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  పూర్ణాంకం dereference = -1;

  నిర్మాణం Chown_option chopt;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'H': /* Traverse command-line symlinks-to-directories.  */
          బిట్టు_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ఆపు;

        స్థితి 'L': /* Traverse all symlinks-to-directories.  */
          బిట్టు_flags = FTS_LOGICAL;
          ఆపు;

        స్థితి 'P': /* Traverse no symlinks-to-directories.  */
          బిట్టు_flags = FTS_PHYSICAL;
          ఆపు;

        స్థితి 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ఆపు;

        స్థితి DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ఆపు;

        స్థితి NO_PRESERVE_మూలం:
          preserve_మూలం = false;
          ఆపు;

        స్థితి PRESERVE_మూలం:
          preserve_మూలం = true;
          ఆపు;

        స్థితి REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ఆపు;

        స్థితి FROM_OPTION:
          {
            bool warn;
            అక్షరం స్థిరరాశి *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ఖాళీ, ఖాళీ, &warn);
            ఒకవేళ (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            ఆపు;
          }

        స్థితి 'R':
          chopt.recurse = true;
          ఆపు;

        స్థితి 'c':
          chopt.verbosity = V_changes_only;
          ఆపు;

        స్థితి 'f':
          chopt.force_silent = true;
          ఆపు;

        స్థితి 'v':
          chopt.verbosity = V_ఎగువ;
          ఆపు;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (chopt.recurse)
    {
      ఒకవేళ (బిట్టు_flags == FTS_PHYSICAL)
        {
          ఒకవేళ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  లేకపోతే
    {
      బిట్టు_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  ఒకవేళ (argc - optind < (reference_file ? 1 : 2))
    {
      ఒకవేళ (argc <= optind)
        error (0, 0, _("missing operand"));
      లేకపోతే
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  ఒకవేళ (reference_file)
    {
      నిర్మాణం stat ref_stats;
      ఒకవేళ (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      ఒకవేళ (chown_బహుళకం == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_గమ్యం_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.సమూహం_name = gid_గమ్యం_name (ref_stats.st_gid);
    }
  లేకపోతే
    {
      అక్షరం *ug = argv[optind];
      ఒకవేళ (chown_బహుళకం == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + అక్షరశ్రేణి_పొడవు (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      అక్షరం స్థిరరాశి *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.సమూహం_name, &warn);

      ఒకవేళ (ug != argv[optind])
        విడుదల (ug);

      ఒకవేళ (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      ఒకవేళ (chown_బహుళకం == CHOWN_CHOWN && !chopt.user_name && chopt.సమూహం_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  ఒకవేళ (chopt.recurse && preserve_మూలం)
    {
      స్థిర నిర్మాణం dev_ino dev_ino_buf;
      chopt.మూలం_dev_ino = get_మూలం_dev_ino (&dev_ino_buf);
      ఒకవేళ (chopt.మూలం_dev_ino == ఖాళీ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  బిట్టు_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, బిట్టు_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
