/* chcon -- change security context of files
   Copyright (C) 2005-2026 Free Software Foundation, Inc.

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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <selinux/selinux.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "dev-ino.h"
#అన్తర్భావ "fts-missing.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "selinux-at.h"
#అన్తర్భావ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "chcon"

#నిర్వచించు AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
స్థిర bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
స్థిర bool recurse;

/* Level of verbosity. */
స్థిర bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
స్థిర నిర్మాణం dev_ino *మూలం_dev_ino;

/* The name of the context file is being given. */
స్థిర అక్షరం స్థిరరాశి *specified_context;

/* Specific components of the context */
స్థిర అక్షరం స్థిరరాశి *specified_user;
స్థిర అక్షరం స్థిరరాశి *specified_role;
స్థిర అక్షరం స్థిరరాశి *specified_range;
స్థిర అక్షరం స్థిరరాశి *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_మూలం,
  PRESERVE_మూలం,
  REFERENCE_FILE_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"recursive", no_వాదం, ఖాళీ, 'R'},
  {"dereference", no_వాదం, ఖాళీ, DEREFERENCE_OPTION},
  {"no-dereference", no_వాదం, ఖాళీ, 'h'},
  {"no-preserve-root", no_వాదం, ఖాళీ, NO_PRESERVE_మూలం},
  {"preserve-root", no_వాదం, ఖాళీ, PRESERVE_మూలం},
  {"reference", required_వాదం, ఖాళీ, REFERENCE_FILE_OPTION},
  {"user", required_వాదం, ఖాళీ, 'u'},
  {"role", required_వాదం, ఖాళీ, 'r'},
  {"type", required_వాదం, ఖాళీ, 't'},
  {"range", required_వాదం, ఖాళీ, 'l'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
స్థిర పూర్ణాంకం
compute_context_from_ముసుగు (అక్షరం స్థిరరాశి *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  ఒకవేళ (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      తిరిగి 1;
    }

#నిర్వచించు సమితి_COMPONENT(C, comp)						\
   చేయి									\
     {									\
       ఒకవేళ (specified_ ## comp						\
           && context_ ## comp ## సమితి ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   అయితే (0)

  సమితి_COMPONENT (new_context, user);
  సమితి_COMPONENT (new_context, range);
  సమితి_COMPONENT (new_context, role);
  సమితి_COMPONENT (new_context, type);

  ఒకవేళ (!ok)
    {
      పూర్ణాంకం saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      తిరిగి 1;
    }

  *ret = new_context;
  తిరిగి 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

స్థిర పూర్ణాంకం
change_file_context (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *file, bool ignore_missing)
{
  అక్షరం *file_context = ఖాళీ;
  context_t context IF_LINT (= ఖాళీ);
  అక్షరం స్థిరరాశి * context_string;
  పూర్ణాంకం errors = 0;

  ఒకవేళ (specified_context == ఖాళీ)
    {
      పూర్ణాంకం స్థితి_2 = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      ఒకవేళ (స్థితి_2 < 0 && errno != ENODATA)
        {
          ఒకవేళ (ignore_missing && ignorable_traversal_errno (errno))
            తిరిగి 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          తిరిగి 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      ఒకవేళ (file_context == ఖాళీ)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          తిరిగి 1;
        }

      ఒకవేళ (compute_context_from_ముసుగు (file_context, &context))
        {
          freecon (file_context);
          తిరిగి 1;
        }

      context_string = context_str (context);
    }
  లేకపోతే
    {
      context_string = specified_context;
    }

  ఒకవేళ (file_context == ఖాళీ || ! streq (context_string, file_context))
    {
      పూర్ణాంకం fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      ఒకవేళ (fail)
        {
          ఒకవేళ (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  ఒకవేళ (specified_context == ఖాళీ)
    {
      context_free (context);
      freecon (file_context);
    }

  తిరిగి errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

స్థిర bool
process_file (FTS *fts, FTSENT *ent)
{
  అక్షరం స్థిరరాశి *file_నిండిన_name = ent->fts_మార్గం;
  అక్షరం స్థిరరాశి *file = ent->fts_accpath;
  స్థిరరాశి నిర్మాణం stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_నమోదు (ent);

  ఒకవేళ (ignore_missing && ignorable_fts_error (ent))
    తిరిగి true;

  ఎంపిక (ent->fts_info)
    {
    స్థితి FTS_D:
      ఒకవేళ (recurse)
        {
          ఒకవేళ (మూలం_DEV_INO_CHECK (మూలం_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              మూలం_DEV_INO_WARN (file_నిండిన_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_సమితి (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_విలువ (fts_read (fts));
              తిరిగి false;
            }
          తిరిగి true;
        }
      ఆపు;

    స్థితి FTS_DP:
      ఒకవేళ (! recurse)
        తిరిగి true;
      ఆపు;

    స్థితి FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ఒకవేళ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_సంఖ్య == 0)
        {
          ent->fts_సంఖ్య = 1;
          fts_సమితి (fts, ent, FTS_AGAIN);
          తిరిగి true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_నిండిన_name));
      ok = false;
      ఆపు;

    స్థితి FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_నిండిన_name));
      ok = false;
      ఆపు;

    స్థితి FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_నిండిన_name));
      ok = false;
      ఆపు;

    స్థితి FTS_DC:		/* directory that causes cycles */
      ఒకవేళ (చక్రం_warning_required (fts, ent))
        {
          emit_చక్రం_warning (file_నిండిన_name);
          తిరిగి false;
        }
      ఆపు;

    అప్రమేయ:
      ఆపు;
    }

  ఒకవేళ (ent->fts_info == FTS_DP
      && ok && మూలం_DEV_INO_CHECK (మూలం_dev_ino, file_stats))
    {
      మూలం_DEV_INO_WARN (file_నిండిన_name);
      ok = false;
    }

  ఒకవేళ (ok)
    {
      ఒకవేళ (verbose)
        రాయి (_("changing security context of %s\n"),
                quoteaf (file_నిండిన_name));

      ఒకవేళ (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  ఒకవేళ ( ! recurse)
    fts_సమితి (fts, ent, FTS_SKIP);

  తిరిగి ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

స్థిర bool
process_files (అక్షరం **files, పూర్ణాంకం బిట్టు_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, బిట్టు_flags, ఖాళీ);

  అయితే (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ఒకవేళ (ent == ఖాళీ)
        {
          ఒకవేళ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ఆపు;
        }

      ok &= process_file (fts, ent);
    }

  ఒకవేళ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  తిరిగి ok;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... CONTEXT FILE...\n\
  or:  %s [OPTION]... [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
        కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Change the SELinux security context యొక్క each FILE గమ్యం CONTEXT.\n\
With --reference, change the security context యొక్క each FILE గమ్యం that యొక్క RFILE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --dereference\n\
         affect the referent యొక్క each symbolic link (this is\n\
         the అప్రమేయ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead యొక్క any referenced file\n\
"));
      oputs (_("\
  -u, --user=USER\n\
         సమితి_2 user USER లోపల the లక్ష్యం security context\n\
"));
      oputs (_("\
  -r, --role=ROLE\n\
         సమితి_2 role ROLE లోపల the లక్ష్యం security context\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         సమితి_2 type TYPE లోపల the లక్ష్యం security context\n\
"));
      oputs (_("\
  -l, --range=RANGE\n\
         సమితి_2 range RANGE లోపల the లక్ష్యం security context\n\
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
         use RFILE's security context rather than specifying a CONTEXT విలువ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         నిర్వహించు on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic కొరకు every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* Bit flags that control how fts works.  */
  పూర్ణాంకం బిట్టు_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  పూర్ణాంకం dereference = -1;

  bool ok;
  bool preserve_మూలం = false;
  bool component_specified = false;
  అక్షరం *reference_file = ఖాళీ;
  పూర్ణాంకం optc;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ఖాళీ))
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

        స్థితి 'R':
          recurse = true;
          ఆపు;

        స్థితి 'f':
          /* ignore */
          ఆపు;

        స్థితి 'v':
          verbose = true;
          ఆపు;

        స్థితి 'u':
          specified_user = optarg;
          component_specified = true;
          ఆపు;

        స్థితి 'r':
          specified_role = optarg;
          component_specified = true;
          ఆపు;

        స్థితి 't':
          specified_type = optarg;
          component_specified = true;
          ఆపు;

        స్థితి 'l':
          specified_range = optarg;
          component_specified = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (recurse)
    {
      ఒకవేళ (బిట్టు_flags == FTS_PHYSICAL)
        {
          ఒకవేళ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      లేకపోతే
        {
          ఒకవేళ (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  లేకపోతే
    {
      బిట్టు_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  ఒకవేళ (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      ఒకవేళ (argc <= optind)
        error (0, 0, _("missing operand"));
      లేకపోతే
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (reference_file)
    {
      అక్షరం *ref_context = ఖాళీ;

      ఒకవేళ (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  లేకపోతే ఒకవేళ (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ఖాళీ;
    }
  లేకపోతే
    {
      specified_context = argv[optind++];
      ఒకవేళ (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  ఒకవేళ (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (recurse && preserve_మూలం)
    {
      స్థిర నిర్మాణం dev_ino dev_ino_buf;
      మూలం_dev_ino = get_మూలం_dev_ino (&dev_ino_buf);
      ఒకవేళ (మూలం_dev_ino == ఖాళీ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  లేకపోతే
    {
      మూలం_dev_ino = ఖాళీ;
    }

  ok = process_files (argv + optind, బిట్టు_flags | FTS_NOSTAT);

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
