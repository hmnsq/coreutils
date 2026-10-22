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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include <selinux/selinux.h>

#include "system.h"
#include "dev-ino.h"
#include "fts-missing.h"
#include "ignore-value.h"
#include "quote.h"
#include "root-dev-ino.h"
#include "selinux-at.h"
#include "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
static bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
static bool recurse;

/* Level of verbosity. */
static bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
static sa_nracanaa dev_ino *root_dev_ino;

/* The name of the context file is being given. */
static var_na const *specified_context;

/* Specific components of the context */
static var_na const *specified_user;
static var_na const *specified_role;
static var_na const *specified_range;
static var_na const *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ROOT,
  PRESERVE_ROOT,
  REFERENCE_FILE_OPTION
};

static sa_nracanaa option const long_options[] =
{
  {"recursive", no_argument, NULL, 'R'},
  {"dereference", no_argument, NULL, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, NULL, 'h'},
  {"no-preserve-root", no_argument, NULL, NO_PRESERVE_ROOT},
  {"preserve-root", no_argument, NULL, PRESERVE_ROOT},
  {"reference", required_argument, NULL, REFERENCE_FILE_OPTION},
  {"user", required_argument, NULL, 'u'},
  {"role", required_argument, NULL, 'r'},
  {"type", required_argument, NULL, 't'},
  {"range", required_argument, NULL, 'l'},
  {"verbose", no_argument, NULL, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
static ຈຳນວນເຕັມ
compute_context_from_mask (var_na const *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  ຖ້າ (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ສົ່ງຄືນ 1;
    }

#define SET_COMPONENT(C, comp)						\
   do									\
     {									\
       ຖ້າ (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   ໃນຂະນະທີ່ (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  ຖ້າ (!ok)
    {
      ຈຳນວນເຕັມ saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ສົ່ງຄືນ 1;
    }

  *ret = new_context;
  ສົ່ງຄືນ 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

static ຈຳນວນເຕັມ
change_file_context (ຈຳນວນເຕັມ fd, var_na const *file, bool ignore_missing)
{
  var_na *file_context = NULL;
  context_t context IF_LINT (= NULL);
  var_na const * context_string;
  ຈຳນວນເຕັມ errors = 0;

  ຖ້າ (specified_context == NULL)
    {
      ຈຳນວນເຕັມ status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      ຖ້າ (status < 0 && errno != ENODATA)
        {
          ຖ້າ (ignore_missing && ignorable_traversal_errno (errno))
            ສົ່ງຄືນ 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ສົ່ງຄືນ 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      ຖ້າ (file_context == NULL)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ສົ່ງຄືນ 1;
        }

      ຖ້າ (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          ສົ່ງຄືນ 1;
        }

      context_string = context_str (context);
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      context_string = specified_context;
    }

  ຖ້າ (file_context == NULL || ! streq (context_string, file_context))
    {
      ຈຳນວນເຕັມ fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      ຖ້າ (fail)
        {
          ຖ້າ (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  ຖ້າ (specified_context == NULL)
    {
      context_free (context);
      freecon (file_context);
    }

  ສົ່ງຄືນ errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

static bool
process_file (FTS *fts, FTSENT *ent)
{
  var_na const *file_full_name = ent->fts_path;
  var_na const *file = ent->fts_accpath;
  const sa_nracanaa stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ຖ້າ (ignore_missing && ignorable_fts_error (ent))
    ສົ່ງຄືນ true;

  switch (ent->fts_info)
    {
    pak_sa FTS_D:
      ຖ້າ (recurse)
        {
          ຖ້າ (ROOT_DEV_INO_CHECK (root_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ROOT_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_value (fts_read (fts));
              ສົ່ງຄືນ false;
            }
          ສົ່ງຄືນ true;
        }
      bha_nga;

    pak_sa FTS_DP:
      ຖ້າ (! recurse)
        ສົ່ງຄືນ true;
      bha_nga;

    pak_sa FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ຖ້າ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_number == 0)
        {
          ent->fts_number = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ສົ່ງຄືນ true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_DC:		/* directory that causes cycles */
      ຖ້າ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ສົ່ງຄືນ false;
        }
      bha_nga;

    default:
      bha_nga;
    }

  ຖ້າ (ent->fts_info == FTS_DP
      && ok && ROOT_DEV_INO_CHECK (root_dev_ino, file_stats))
    {
      ROOT_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  ຖ້າ (ok)
    {
      ຖ້າ (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      ຖ້າ (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  ຖ້າ ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ສົ່ງຄືນ ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

static bool
process_files (var_na **files, ຈຳນວນເຕັມ bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, NULL);

  ໃນຂະນະທີ່ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ຖ້າ (ent == NULL)
        {
          ຖ້າ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          bha_nga;
        }

      ok &= process_file (fts, ent);
    }

  ຖ້າ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ສົ່ງຄືນ ok;
}

void
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
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
         the default), rather than the symbolic link itself\n\
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
      --no-preserve-root\n\
         do not treat '/' specially (the default)\n\
"));
      oputs (_("\
      --preserve-root\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT value\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ສຳລັບ every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  /* Bit flags that control how fts works.  */
  ຈຳນວນເຕັມ bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ຈຳນວນເຕັມ dereference = -1;

  bool ok;
  bool preserve_root = false;
  bool component_specified = false;
  var_na *reference_file = NULL;
  ຈຳນວນເຕັມ optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ໃນຂະນະທີ່ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, NULL))
         != -1)
    {
      switch (optc)
        {
        pak_sa 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          bha_nga;

        pak_sa 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          bha_nga;

        pak_sa 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          bha_nga;

        pak_sa 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          bha_nga;

        pak_sa DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          bha_nga;

        pak_sa NO_PRESERVE_ROOT:
          preserve_root = false;
          bha_nga;

        pak_sa PRESERVE_ROOT:
          preserve_root = true;
          bha_nga;

        pak_sa REFERENCE_FILE_OPTION:
          reference_file = optarg;
          bha_nga;

        pak_sa 'R':
          recurse = true;
          bha_nga;

        pak_sa 'f':
          /* ignore */
          bha_nga;

        pak_sa 'v':
          verbose = true;
          bha_nga;

        pak_sa 'u':
          specified_user = optarg;
          component_specified = true;
          bha_nga;

        pak_sa 'r':
          specified_role = optarg;
          component_specified = true;
          bha_nga;

        pak_sa 't':
          specified_type = optarg;
          component_specified = true;
          bha_nga;

        pak_sa 'l':
          specified_range = optarg;
          component_specified = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  ຖ້າ (recurse)
    {
      ຖ້າ (bit_flags == FTS_PHYSICAL)
        {
          ຖ້າ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          ຖ້າ (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  ຖ້າ (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      ຖ້າ (argc <= optind)
        error (0, 0, _("missing operand"));
      ບໍ່ດັ່ງນັ້ນ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (reference_file)
    {
      var_na *ref_context = NULL;

      ຖ້າ (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = NULL;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      specified_context = argv[optind++];
      ຖ້າ (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  ຖ້າ (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  ຖ້າ (recurse && preserve_root)
    {
      static sa_nracanaa dev_ino dev_ino_buf;
      root_dev_ino = get_root_dev_ino (&dev_ino_buf);
      ຖ້າ (root_dev_ino == NULL)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      root_dev_ino = NULL;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  ສົ່ງຄືນ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
