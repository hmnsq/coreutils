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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "chown.h"
#અન્તર્ભાવ "chown-core.h"
#અન્તર્ભાવ "fts_.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "root-dev-ino.h"
#અન્તર્ભાવ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
સ્થિર અક્ષર *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_મૂળ,
  PRESERVE_મૂળ,
  REFERENCE_FILE_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"recursive", no_argument, ખાલી, 'R'},
  {"changes", no_argument, ખાલી, 'c'},
  {"dereference", no_argument, ખાલી, DEREFERENCE_OPTION},
  {"from", required_argument, ખાલી, FROM_OPTION},
  {"no-dereference", no_argument, ખાલી, 'h'},
  {"no-preserve-root", no_argument, ખાલી, NO_PRESERVE_મૂળ},
  {"preserve-root", no_argument, ખાલી, PRESERVE_મૂળ},
  {"quiet", no_argument, ખાલી, 'f'},
  {"silent", no_argument, ખાલી, 'f'},
  {"reference", required_argument, ખાલી, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      જો (chown_mode == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or group of each FILE to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each FILE to those of RFILE.\n\
\n\
"), stdout);
      નહીંતર
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
         output a diagnostic માટે every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the મૂળભૂત), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only જો its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which સ્થિતિ a match\n\
         is not required માટે the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-મૂળ\n\
         કરો not treat '/' specially (the મૂળભૂત)\n\
"));
      oputs (_("\
      --preserve-મૂળ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying કિંમતો.\n\
         RFILE is always dereferenced જો a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      જો (chown_mode == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged જો missing.  Group is unchanged જો missing, but changed\n\
to login group જો implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), stdout);

      જો (chown_mode == CHOWN_CHOWN)
        printf (_("\
\n\
Examples:\n\
  %s મૂળ /u        Change the owner of /u to \"મૂળ\".\n\
  %s મૂળ:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR મૂળ /u    Change the owner of /u and subfiles to \"મૂળ\".\n\
"),
              program_name, program_name, program_name);
      નહીંતર
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool preserve_મૂળ = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  પૂર્ણાંક bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  પૂર્ણાંક dereference = -1;

  સંરચના Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          તોડો;

        સ્થિતિ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          તોડો;

        સ્થિતિ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          તોડો;

        સ્થિતિ NO_PRESERVE_મૂળ:
          preserve_મૂળ = false;
          તોડો;

        સ્થિતિ PRESERVE_મૂળ:
          preserve_મૂળ = true;
          તોડો;

        સ્થિતિ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          તોડો;

        સ્થિતિ FROM_OPTION:
          {
            bool warn;
            અક્ષર અચલ *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ખાલી, ખાલી, &warn);
            જો (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            તોડો;
          }

        સ્થિતિ 'R':
          chopt.recurse = true;
          તોડો;

        સ્થિતિ 'c':
          chopt.verbosity = V_changes_only;
          તોડો;

        સ્થિતિ 'f':
          chopt.force_silent = true;
          તોડો;

        સ્થિતિ 'v':
          chopt.verbosity = V_ઉપલું;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (chopt.recurse)
    {
      જો (bit_flags == FTS_PHYSICAL)
        {
          જો (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  નહીંતર
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  જો (argc - optind < (reference_file ? 1 : 2))
    {
      જો (argc <= optind)
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  જો (reference_file)
    {
      સંરચના stat ref_stats;
      જો (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      જો (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  નહીંતર
    {
      અક્ષર *ug = argv[optind];
      જો (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + strlen (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      અક્ષર અચલ *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      જો (ug != argv[optind])
        મુક્ત (ug);

      જો (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      જો (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  જો (chopt.recurse && preserve_મૂળ)
    {
      સ્થિર સંરચના dev_ino dev_ino_buf;
      chopt.મૂળ_dev_ino = get_મૂળ_dev_ino (&dev_ino_buf);
      જો (chopt.મૂળ_dev_ino == ખાલી)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
