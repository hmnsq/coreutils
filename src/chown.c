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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "chown.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "chown-core.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fts_.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME (chown_ਬਹੁਲਕ == CHOWN_CHOWN ? "chown" : "chgrp")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
ਸਥਿਰ ਅੱਖਰ *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_ਜੜ੍ਹ,
  PRESERVE_ਜੜ੍ਹ,
  REFERENCE_FILE_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"recursive", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"changes", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, DEREFERENCE_OPTION},
  {"from", required_ਦਲੀਲ, ਨਿਰਾ, FROM_OPTION},
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"no-preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, NO_PRESERVE_ਜੜ੍ਹ},
  {"preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, PRESERVE_ਜੜ੍ਹ},
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"silent", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"reference", required_ਦਲੀਲ, ਨਿਰਾ, REFERENCE_FILE_OPTION},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name,
              chown_ਬਹੁਲਕ == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              ਪ੍ਰੋਗਰਾਮ_name);
      ਜੇ (chown_ਬਹੁਲਕ == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or ਸਮੂਹ ਦਾ each FILE ਮੰਜ਼ਿਲ OWNER and/or ਸਮੂਹ_2.\n\
With --reference, change the owner and ਸਮੂਹ ਦਾ each FILE ਮੰਜ਼ਿਲ those ਦਾ RFILE.\n\
\n\
"), stdout);
      ਨਹੀਂਤਾਂ
        fputs (_("\
Change the ਸਮੂਹ ਦਾ each FILE ਮੰਜ਼ਿਲ ਸਮੂਹ_2.\n\
With --reference, change the ਸਮੂਹ ਦਾ each FILE ਮੰਜ਼ਿਲ that ਦਾ RFILE.\n\
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
         output a diagnostic ਲਈ every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent ਦਾ each symbolic link (this is\n\
         the ਮੂਲ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead ਦਾ any referenced file;\n\
         useful only on systems that can change the ownership ਦਾ a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_ਸਮੂਹ\n\
         change the ownership ਦਾ each file only ਜੇ its\n\
         current owner and/or ਸਮੂਹ ਮੇਲ those specified ਇੱਥੇ.\n\
         Either may be omitted, ਵਿੱਚ which ਹਾਲਤ a ਮੇਲ\n\
         is not required ਲਈ the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-ਜੜ੍ਹ\n\
         ਕਰੋ not treat '/' specially (the ਮੂਲ)\n\
"));
      oputs (_("\
      --preserve-ਜੜ੍ਹ\n\
         fail ਮੰਜ਼ਿਲ ਚਲਾਓ recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying ਮੁੱਲਾਂ.\n\
         RFILE is always dereferenced ਜੇ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         ਚਲਾਓ on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਜੇ (chown_ਬਹੁਲਕ == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged ਜੇ missing.  ਸਮੂਹ_3 is unchanged ਜੇ missing, but ਬਦਲਿਆ\n\
ਮੰਜ਼ਿਲ login ਸਮੂਹ ਜੇ implied by a ':' following a symbolic OWNER.\n\
OWNER and ਸਮੂਹ_2 may be numeric as well as symbolic.\n\
"), stdout);

      ਜੇ (chown_ਬਹੁਲਕ == CHOWN_CHOWN)
        ਲਿਖੋ (_("\
\n\
Examples:\n\
  %s ਜੜ੍ਹ /u        Change the owner ਦਾ /u ਮੰਜ਼ਿਲ \"ਜੜ੍ਹ\".\n\
  %s ਜੜ੍ਹ:staff /u  Likewise, but also change its ਸਮੂਹ ਮੰਜ਼ਿਲ \"staff\".\n\
  %s -hR ਜੜ੍ਹ /u    Change the owner ਦਾ /u and subfiles ਮੰਜ਼ਿਲ \"ਜੜ੍ਹ\".\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      ਨਹੀਂਤਾਂ
        ਲਿਖੋ (_("\
\n\
Examples:\n\
  %s staff /u      Change the ਸਮੂਹ ਦਾ /u ਮੰਜ਼ਿਲ \"staff\".\n\
  %s -hR staff /u  Change the ਸਮੂਹ ਦਾ /u and subfiles ਮੰਜ਼ਿਲ \"staff\".\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool preserve_ਜੜ੍ਹ = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ਪੂਰਨ dereference = -1;

  ਬਣਤਰ Chown_option chopt;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'H': /* Traverse command-line symlinks-to-directories.  */
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'L': /* Traverse all symlinks-to-directories.  */
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_LOGICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'P': /* Traverse no symlinks-to-directories.  */
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_PHYSICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ਰੋਕੋ;

        ਹਾਲਤ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ਰੋਕੋ;

        ਹਾਲਤ NO_PRESERVE_ਜੜ੍ਹ:
          preserve_ਜੜ੍ਹ = false;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_ਜੜ੍ਹ:
          preserve_ਜੜ੍ਹ = true;
          ਰੋਕੋ;

        ਹਾਲਤ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ FROM_OPTION:
          {
            bool warn;
            ਅੱਖਰ ਅਚਲ *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  ਨਿਰਾ, ਨਿਰਾ, &warn);
            ਜੇ (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            ਰੋਕੋ;
          }

        ਹਾਲਤ 'R':
          chopt.recurse = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          chopt.verbosity = V_changes_only;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          chopt.force_silent = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          chopt.verbosity = V_ਉੱਚਾ;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (chopt.recurse)
    {
      ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ_flags == FTS_PHYSICAL)
        {
          ਜੇ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  ਜੇ (argc - optind < (reference_file ? 1 : 2))
    {
      ਜੇ (argc <= optind)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  ਜੇ (reference_file)
    {
      ਬਣਤਰ stat ref_stats;
      ਜੇ (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      ਜੇ (chown_ਬਹੁਲਕ == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_ਮੰਜ਼ਿਲ_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.ਸਮੂਹ_name = gid_ਮੰਜ਼ਿਲ_name (ref_stats.st_gid);
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *ug = argv[optind];
      ਜੇ (chown_ਬਹੁਲਕ == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + ਲੜੀ_ਲੰਬਾਈ (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      ਅੱਖਰ ਅਚਲ *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.ਸਮੂਹ_name, &warn);

      ਜੇ (ug != argv[optind])
        ਮੁਕਤ (ug);

      ਜੇ (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      ਜੇ (chown_ਬਹੁਲਕ == CHOWN_CHOWN && !chopt.user_name && chopt.ਸਮੂਹ_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  ਜੇ (chopt.recurse && preserve_ਜੜ੍ਹ)
    {
      ਸਥਿਰ ਬਣਤਰ dev_ino dev_ino_buf;
      chopt.ਜੜ੍ਹ_dev_ino = get_ਜੜ੍ਹ_dev_ino (&dev_ino_buf);
      ਜੇ (chopt.ਜੜ੍ਹ_dev_ino == ਨਿਰਾ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  ਦੋਆਧਾਰੀ_ਅੰਕ_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, ਦੋਆਧਾਰੀ_ਅੰਕ_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
