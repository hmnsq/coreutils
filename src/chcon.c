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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <selinux/selinux.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "dev-ino.h"
#অন্তর্ভাব "fts-missing.h"
#অন্তর্ভাব "ignore-value.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "root-dev-ino.h"
#অন্তর্ভাব "selinux-at.h"
#অন্তর্ভাব "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
স্থির bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
স্থির bool recurse;

/* Level of verbosity. */
স্থির bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
স্থির কাঠামো dev_ino *মূল_dev_ino;

/* The name of the context file is being given. */
স্থির অক্ষর ধ্রুবক *specified_context;

/* Specific components of the context */
স্থির অক্ষর ধ্রুবক *specified_user;
স্থির অক্ষর ধ্রুবক *specified_role;
স্থির অক্ষর ধ্রুবক *specified_range;
স্থির অক্ষর ধ্রুবক *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_মূল,
  PRESERVE_মূল,
  REFERENCE_FILE_OPTION
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"recursive", no_argument, ফাঁকা, 'R'},
  {"dereference", no_argument, ফাঁকা, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, ফাঁকা, 'h'},
  {"no-preserve-root", no_argument, ফাঁকা, NO_PRESERVE_মূল},
  {"preserve-root", no_argument, ফাঁকা, PRESERVE_মূল},
  {"reference", required_argument, ফাঁকা, REFERENCE_FILE_OPTION},
  {"user", required_argument, ফাঁকা, 'u'},
  {"role", required_argument, ফাঁকা, 'r'},
  {"type", required_argument, ফাঁকা, 't'},
  {"range", required_argument, ফাঁকা, 'l'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
স্থির পূর্ণসংখ্যা
compute_context_from_mask (অক্ষর ধ্রুবক *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  যদি (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ফেরত 1;
    }

#define SET_COMPONENT(C, comp)						\
   করো									\
     {									\
       যদি (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   যখন (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  যদি (!ok)
    {
      পূর্ণসংখ্যা saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ফেরত 1;
    }

  *ret = new_context;
  ফেরত 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

স্থির পূর্ণসংখ্যা
change_file_context (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *file, bool ignore_missing)
{
  অক্ষর *file_context = ফাঁকা;
  context_t context IF_LINT (= ফাঁকা);
  অক্ষর ধ্রুবক * context_string;
  পূর্ণসংখ্যা errors = 0;

  যদি (specified_context == ফাঁকা)
    {
      পূর্ণসংখ্যা status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      যদি (status < 0 && errno != ENODATA)
        {
          যদি (ignore_missing && ignorable_traversal_errno (errno))
            ফেরত 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ফেরত 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      যদি (file_context == ফাঁকা)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ফেরত 1;
        }

      যদি (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          ফেরত 1;
        }

      context_string = context_str (context);
    }
  অন্যথা
    {
      context_string = specified_context;
    }

  যদি (file_context == ফাঁকা || ! streq (context_string, file_context))
    {
      পূর্ণসংখ্যা fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      যদি (fail)
        {
          যদি (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  যদি (specified_context == ফাঁকা)
    {
      context_free (context);
      freecon (file_context);
    }

  ফেরত errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

স্থির bool
process_file (FTS *fts, FTSENT *ent)
{
  অক্ষর ধ্রুবক *file_full_name = ent->fts_path;
  অক্ষর ধ্রুবক *file = ent->fts_accpath;
  ধ্রুবক কাঠামো stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    ফেরত true;

  নির্বাচন (ent->fts_info)
    {
    ক্ষেত্র FTS_D:
      যদি (recurse)
        {
          যদি (মূল_DEV_INO_CHECK (মূল_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              মূল_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_মান (fts_read (fts));
              ফেরত false;
            }
          ফেরত true;
        }
      বিরতি;

    ক্ষেত্র FTS_DP:
      যদি (! recurse)
        ফেরত true;
      বিরতি;

    ক্ষেত্র FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      যদি (ent->fts_level == FTS_ROOTLEVEL && ent->fts_সংখ্যা == 0)
        {
          ent->fts_সংখ্যা = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ফেরত true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      বিরতি;

    ক্ষেত্র FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      বিরতি;

    ক্ষেত্র FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      বিরতি;

    ক্ষেত্র FTS_DC:		/* directory that causes cycles */
      যদি (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ফেরত false;
        }
      বিরতি;

    পূর্বনির্ধারিত:
      বিরতি;
    }

  যদি (ent->fts_info == FTS_DP
      && ok && মূল_DEV_INO_CHECK (মূল_dev_ino, file_stats))
    {
      মূল_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  যদি (ok)
    {
      যদি (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      যদি (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  যদি ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ফেরত ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

স্থির bool
process_files (অক্ষর **files, পূর্ণসংখ্যা bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ফাঁকা);

  যখন (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      যদি (ent == ফাঁকা)
        {
          যদি (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          বিরতি;
        }

      ok &= process_file (fts, ent);
    }

  যদি (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ফেরত ok;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         the পূর্বনির্ধারিত), rather than the symbolic link itself\n\
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
      --no-preserve-মূল\n\
         করো not treat '/' specially (the পূর্বনির্ধারিত)\n\
"));
      oputs (_("\
      --preserve-মূল\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT মান\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic জন্য every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  /* Bit flags that control how fts works.  */
  পূর্ণসংখ্যা bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  পূর্ণসংখ্যা dereference = -1;

  bool ok;
  bool preserve_মূল = false;
  bool component_specified = false;
  অক্ষর *reference_file = ফাঁকা;
  পূর্ণসংখ্যা optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ফাঁকা))
         != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          বিরতি;

        ক্ষেত্র 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          বিরতি;

        ক্ষেত্র 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          বিরতি;

        ক্ষেত্র 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          বিরতি;

        ক্ষেত্র DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          বিরতি;

        ক্ষেত্র NO_PRESERVE_মূল:
          preserve_মূল = false;
          বিরতি;

        ক্ষেত্র PRESERVE_মূল:
          preserve_মূল = true;
          বিরতি;

        ক্ষেত্র REFERENCE_FILE_OPTION:
          reference_file = optarg;
          বিরতি;

        ক্ষেত্র 'R':
          recurse = true;
          বিরতি;

        ক্ষেত্র 'f':
          /* ignore */
          বিরতি;

        ক্ষেত্র 'v':
          verbose = true;
          বিরতি;

        ক্ষেত্র 'u':
          specified_user = optarg;
          component_specified = true;
          বিরতি;

        ক্ষেত্র 'r':
          specified_role = optarg;
          component_specified = true;
          বিরতি;

        ক্ষেত্র 't':
          specified_type = optarg;
          component_specified = true;
          বিরতি;

        ক্ষেত্র 'l':
          specified_range = optarg;
          component_specified = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (recurse)
    {
      যদি (bit_flags == FTS_PHYSICAL)
        {
          যদি (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      অন্যথা
        {
          যদি (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  অন্যথা
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  যদি (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      যদি (argc <= optind)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  যদি (reference_file)
    {
      অক্ষর *ref_context = ফাঁকা;

      যদি (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  অন্যথা যদি (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ফাঁকা;
    }
  অন্যথা
    {
      specified_context = argv[optind++];
      যদি (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  যদি (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  যদি (recurse && preserve_মূল)
    {
      স্থির কাঠামো dev_ino dev_ino_buf;
      মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
      যদি (মূল_dev_ino == ফাঁকা)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  অন্যথা
    {
      মূল_dev_ino = ফাঁকা;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
