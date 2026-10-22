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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <selinux/selinux.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "dev-ino.h"
#ಅನ್ತರ್ಭಾವ "fts-missing.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "selinux-at.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
ಸ್ಥಿರ bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
ಸ್ಥಿರ bool recurse;

/* Level of verbosity. */
ಸ್ಥಿರ bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ಸ್ಥಿರ ಸಂರಚನಾ dev_ino *ಬೇರು_dev_ino;

/* The name of the context file is being given. */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *specified_context;

/* Specific components of the context */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *specified_user;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *specified_role;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *specified_range;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  REFERENCE_FILE_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"recursive", no_argument, ರಿಕ್ತ, 'R'},
  {"dereference", no_argument, ರಿಕ್ತ, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, ರಿಕ್ತ, 'h'},
  {"no-preserve-root", no_argument, ರಿಕ್ತ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", no_argument, ರಿಕ್ತ, PRESERVE_ಬೇರು},
  {"reference", required_argument, ರಿಕ್ತ, REFERENCE_FILE_OPTION},
  {"user", required_argument, ರಿಕ್ತ, 'u'},
  {"role", required_argument, ರಿಕ್ತ, 'r'},
  {"type", required_argument, ರಿಕ್ತ, 't'},
  {"range", required_argument, ರಿಕ್ತ, 'l'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compute_context_from_mask (ವರ್ಣ ಧ್ರುವ *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  ಯದಿ (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ಪ್ರತ್ಯಾಗಮ 1;
    }

#define SET_COMPONENT(C, comp)						\
   ಕುರು									\
     {									\
       ಯದಿ (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   ಯಾವತ್ (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  ಯದಿ (!ok)
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ಪ್ರತ್ಯಾಗಮ 1;
    }

  *ret = new_context;
  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
change_file_context (ಪೂರ್ಣಾಙ್ಕ fd, ವರ್ಣ ಧ್ರುವ *file, bool ignore_missing)
{
  ವರ್ಣ *file_context = ರಿಕ್ತ;
  context_t context IF_LINT (= ರಿಕ್ತ);
  ವರ್ಣ ಧ್ರುವ * context_string;
  ಪೂರ್ಣಾಙ್ಕ errors = 0;

  ಯದಿ (specified_context == ರಿಕ್ತ)
    {
      ಪೂರ್ಣಾಙ್ಕ status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      ಯದಿ (status < 0 && errno != ENODATA)
        {
          ಯದಿ (ignore_missing && ignorable_traversal_errno (errno))
            ಪ್ರತ್ಯಾಗಮ 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ಪ್ರತ್ಯಾಗಮ 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      ಯದಿ (file_context == ರಿಕ್ತ)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ಪ್ರತ್ಯಾಗಮ 1;
        }

      ಯದಿ (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          ಪ್ರತ್ಯಾಗಮ 1;
        }

      context_string = context_str (context);
    }
  ಅನ್ಯಥಾ
    {
      context_string = specified_context;
    }

  ಯದಿ (file_context == ರಿಕ್ತ || ! streq (context_string, file_context))
    {
      ಪೂರ್ಣಾಙ್ಕ fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      ಯದಿ (fail)
        {
          ಯದಿ (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  ಯದಿ (specified_context == ರಿಕ್ತ)
    {
      context_free (context);
      freecon (file_context);
    }

  ಪ್ರತ್ಯಾಗಮ errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ಸ್ಥಿರ bool
process_file (FTS *fts, FTSENT *ent)
{
  ವರ್ಣ ಧ್ರುವ *file_full_name = ent->fts_path;
  ವರ್ಣ ಧ್ರುವ *file = ent->fts_accpath;
  ಧ್ರುವ ಸಂರಚನಾ stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ಯದಿ (ignore_missing && ignorable_fts_error (ent))
    ಪ್ರತ್ಯಾಗಮ true;

  ವಿಕಲ್ಪ (ent->fts_info)
    {
    ಪಕ್ಷ FTS_D:
      ಯದಿ (recurse)
        {
          ಯದಿ (ಬೇರು_DEV_INO_CHECK (ಬೇರು_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ಬೇರು_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ಮೌಲ್ಯ (fts_read (fts));
              ಪ್ರತ್ಯಾಗಮ false;
            }
          ಪ್ರತ್ಯಾಗಮ true;
        }
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DP:
      ಯದಿ (! recurse)
        ಪ್ರತ್ಯಾಗಮ true;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ಯದಿ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ಸಂಖ್ಯೆ == 0)
        {
          ent->fts_ಸಂಖ್ಯೆ = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ಪ್ರತ್ಯಾಗಮ true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DC:		/* directory that causes cycles */
      ಯದಿ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ಪ್ರತ್ಯಾಗಮ false;
        }
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಭಙ್ಗ;
    }

  ಯದಿ (ent->fts_info == FTS_DP
      && ok && ಬೇರು_DEV_INO_CHECK (ಬೇರು_dev_ino, file_stats))
    {
      ಬೇರು_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  ಯದಿ (ok)
    {
      ಯದಿ (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      ಯದಿ (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  ಯದಿ ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ಸ್ಥಿರ bool
process_files (ವರ್ಣ **files, ಪೂರ್ಣಾಙ್ಕ bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ರಿಕ್ತ);

  ಯಾವತ್ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ಯದಿ (ent == ರಿಕ್ತ)
        {
          ಯದಿ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ಭಙ್ಗ;
        }

      ok &= process_file (fts, ent);
    }

  ಯದಿ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... CONTEXT FILE...\n\
  or:  %s [OPTION]... [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
        program_name, program_name, program_name);
      fputs (_("\
Change the SELinux security context of each FILE to CONTEXT.\n\
With --reference, change the security context of each FILE to that of RFILE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the ಸ್ವಭಾವ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file\n\
"));
      oputs (_("\
  -u, --user=USER\n\
         set user USER in the target security context\n\
"));
      oputs (_("\
  -r, --role=ROLE\n\
         set role ROLE in the target security context\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         set type TYPE in the target security context\n\
"));
      oputs (_("\
  -l, --range=RANGE\n\
         set range RANGE in the target security context\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಕುರು not treat '/' specially (the ಸ್ವಭಾವ)\n\
"));
      oputs (_("\
      --preserve-ಬೇರು\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ಪ್ರತಿ every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  /* Bit flags that control how fts works.  */
  ಪೂರ್ಣಾಙ್ಕ bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ಪೂರ್ಣಾಙ್ಕ dereference = -1;

  bool ok;
  bool preserve_ಬೇರು = false;
  bool component_specified = false;
  ವರ್ಣ *reference_file = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ಭಙ್ಗ;

        ಪಕ್ಷ NO_PRESERVE_ಬೇರು:
          preserve_ಬೇರು = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_ಬೇರು:
          preserve_ಬೇರು = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'R':
          recurse = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          /* ignore */
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          verbose = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          specified_user = optarg;
          component_specified = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
          specified_role = optarg;
          component_specified = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          specified_type = optarg;
          component_specified = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'l':
          specified_range = optarg;
          component_specified = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (recurse)
    {
      ಯದಿ (bit_flags == FTS_PHYSICAL)
        {
          ಯದಿ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  ಅನ್ಯಥಾ
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  ಯದಿ (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      ಯದಿ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (reference_file)
    {
      ವರ್ಣ *ref_context = ರಿಕ್ತ;

      ಯದಿ (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  ಅನ್ಯಥಾ ಯದಿ (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ
    {
      specified_context = argv[optind++];
      ಯದಿ (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  ಯದಿ (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (recurse && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ಸಂರಚನಾ dev_ino dev_ino_buf;
      ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಯದಿ (ಬೇರು_dev_ino == ರಿಕ್ತ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ಅನ್ಯಥಾ
    {
      ಬೇರು_dev_ino = ರಿಕ್ತ;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
