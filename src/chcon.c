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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "chcon"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
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
ಸ್ಥಿರ ರಚನೆ dev_ino *ಬೇರು_dev_ino;

/* The name of the context file is being given. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_context;

/* Specific components of the context */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_user;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_role;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_range;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  REFERENCE_FILE_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"recursive", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"dereference", no_ವಾದ, ಖಾಲಿ, DEREFERENCE_OPTION},
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"no-preserve-root", no_ವಾದ, ಖಾಲಿ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", no_ವಾದ, ಖಾಲಿ, PRESERVE_ಬೇರು},
  {"reference", required_ವಾದ, ಖಾಲಿ, REFERENCE_FILE_OPTION},
  {"user", required_ವಾದ, ಖಾಲಿ, 'u'},
  {"role", required_ವಾದ, ಖಾಲಿ, 'r'},
  {"type", required_ವಾದ, ಖಾಲಿ, 't'},
  {"range", required_ವಾದ, ಖಾಲಿ, 'l'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compute_context_from_ಮುಖವಾಡ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  ಆದರೆ (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ಹಿಂತಿರುಗಿ 1;
    }

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_COMPONENT(C, comp)						\
   ಮಾಡು									\
     {									\
       ಆದರೆ (specified_ ## comp						\
           && context_ ## comp ## ಗಣ ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   ಆಗ (0)

  ಗಣ_COMPONENT (new_context, user);
  ಗಣ_COMPONENT (new_context, range);
  ಗಣ_COMPONENT (new_context, role);
  ಗಣ_COMPONENT (new_context, type);

  ಆದರೆ (!ok)
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ಹಿಂತಿರುಗಿ 1;
    }

  *ret = new_context;
  ಹಿಂತಿರುಗಿ 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
change_file_context (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, bool ignore_missing)
{
  ಅಕ್ಷರ *file_context = ಖಾಲಿ;
  context_t context IF_LINT (= ಖಾಲಿ);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * context_string;
  ಪೂರ್ಣಾಂಕ errors = 0;

  ಆದರೆ (specified_context == ಖಾಲಿ)
    {
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      ಆದರೆ (ಸ್ಥಿತಿ_2 < 0 && errno != ENODATA)
        {
          ಆದರೆ (ignore_missing && ignorable_traversal_errno (errno))
            ಹಿಂತಿರುಗಿ 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ಹಿಂತಿರುಗಿ 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      ಆದರೆ (file_context == ಖಾಲಿ)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ಹಿಂತಿರುಗಿ 1;
        }

      ಆದರೆ (compute_context_from_ಮುಖವಾಡ (file_context, &context))
        {
          freecon (file_context);
          ಹಿಂತಿರುಗಿ 1;
        }

      context_string = context_str (context);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      context_string = specified_context;
    }

  ಆದರೆ (file_context == ಖಾಲಿ || ! streq (context_string, file_context))
    {
      ಪೂರ್ಣಾಂಕ fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      ಆದರೆ (fail)
        {
          ಆದರೆ (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  ಆದರೆ (specified_context == ಖಾಲಿ)
    {
      context_free (context);
      freecon (file_context);
    }

  ಹಿಂತಿರುಗಿ errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ಸ್ಥಿರ bool
process_file (FTS *fts, FTSENT *ent)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_ತುಂಬಿದ_name = ent->fts_ಮಾರ್ಗ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = ent->fts_accpath;
  ಸ್ಥಿರಾಂಕ ರಚನೆ stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_ನಮೂದು (ent);

  ಆದರೆ (ignore_missing && ignorable_fts_error (ent))
    ಹಿಂತಿರುಗಿ true;

  ಆಯ್ಕೆ (ent->fts_info)
    {
    ಸ್ಥಿತಿ FTS_D:
      ಆದರೆ (recurse)
        {
          ಆದರೆ (ಬೇರು_DEV_INO_CHECK (ಬೇರು_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ಬೇರು_DEV_INO_WARN (file_ತುಂಬಿದ_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_ಗಣ (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ಮೌಲ್ಯ (fts_read (fts));
              ಹಿಂತಿರುಗಿ false;
            }
          ಹಿಂತಿರುಗಿ true;
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DP:
      ಆದರೆ (! recurse)
        ಹಿಂತಿರುಗಿ true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ಆದರೆ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ಸಂಖ್ಯೆ == 0)
        {
          ent->fts_ಸಂಖ್ಯೆ = 1;
          fts_ಗಣ (fts, ent, FTS_AGAIN);
          ಹಿಂತಿರುಗಿ true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_ತುಂಬಿದ_name));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_ತುಂಬಿದ_name));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_ತುಂಬಿದ_name));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DC:		/* directory that causes cycles */
      ಆದರೆ (ಚಕ್ರ_warning_required (fts, ent))
        {
          emit_ಚಕ್ರ_warning (file_ತುಂಬಿದ_name);
          ಹಿಂತಿರುಗಿ false;
        }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ನಿಲ್ಲಿಸು;
    }

  ಆದರೆ (ent->fts_info == FTS_DP
      && ok && ಬೇರು_DEV_INO_CHECK (ಬೇರು_dev_ino, file_stats))
    {
      ಬೇರು_DEV_INO_WARN (file_ತುಂಬಿದ_name);
      ok = false;
    }

  ಆದರೆ (ok)
    {
      ಆದರೆ (verbose)
        ಬರೆ (_("changing security context of %s\n"),
                quoteaf (file_ತುಂಬಿದ_name));

      ಆದರೆ (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  ಆದರೆ ( ! recurse)
    fts_ಗಣ (fts, ent, FTS_SKIP);

  ಹಿಂತಿರುಗಿ ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ಸ್ಥಿರ bool
process_files (ಅಕ್ಷರ **files, ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, ಬಿಟ್_flags, ಖಾಲಿ);

  ಆಗ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ಆದರೆ (ent == ಖಾಲಿ)
        {
          ಆದರೆ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        }

      ok &= process_file (fts, ent);
    }

  ಆದರೆ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... CONTEXT FILE...\n\
  or:  %s [OPTION]... [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
        ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Change the SELinux security context ದ each FILE ಗಮ್ಯ CONTEXT.\n\
With --reference, change the security context ದ each FILE ಗಮ್ಯ that ದ RFILE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --dereference\n\
         affect the referent ದ each symbolic link (this is\n\
         the ಪೂರ್ವನಿಯೋಜಿತ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead ದ any referenced file\n\
"));
      oputs (_("\
  -u, --user=USER\n\
         ಗಣ_2 user USER ಒಳ the ಗುರಿ security context\n\
"));
      oputs (_("\
  -r, --role=ROLE\n\
         ಗಣ_2 role ROLE ಒಳ the ಗುರಿ security context\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         ಗಣ_2 type TYPE ಒಳ the ಗುರಿ security context\n\
"));
      oputs (_("\
  -l, --range=RANGE\n\
         ಗಣ_2 range RANGE ಒಳ the ಗುರಿ security context\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಮಾಡು not treat '/' specially (the ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
      --preserve-ಬೇರು\n\
         fail ಗಮ್ಯ ನಿರ್ವಹಿಸು recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         ನಿರ್ವಹಿಸು on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ಗಾಗಿ every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* Bit flags that control how fts works.  */
  ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ಪೂರ್ಣಾಂಕ dereference = -1;

  bool ok;
  bool preserve_ಬೇರು = false;
  bool component_specified = false;
  ಅಕ್ಷರ *reference_file = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ optc;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'H': /* Traverse command-line symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L': /* Traverse all symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_LOGICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'P': /* Traverse no symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NO_PRESERVE_ಬೇರು:
          preserve_ಬೇರು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_ಬೇರು:
          preserve_ಬೇರು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'R':
          recurse = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          /* ignore */
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          verbose = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          specified_user = optarg;
          component_specified = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
          specified_role = optarg;
          component_specified = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          specified_type = optarg;
          component_specified = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          specified_range = optarg;
          component_specified = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (recurse)
    {
      ಆದರೆ (ಬಿಟ್_flags == FTS_PHYSICAL)
        {
          ಆದರೆ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬಿಟ್_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  ಆದರೆ (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      ಆದರೆ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (reference_file)
    {
      ಅಕ್ಷರ *ref_context = ಖಾಲಿ;

      ಆದರೆ (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      specified_context = argv[optind++];
      ಆದರೆ (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  ಆದರೆ (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (recurse && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ರಚನೆ dev_ino dev_ino_buf;
      ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಆದರೆ (ಬೇರು_dev_ino == ಖಾಲಿ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬೇರು_dev_ino = ಖಾಲಿ;
    }

  ok = process_files (argv + optind, ಬಿಟ್_flags | FTS_NOSTAT);

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
