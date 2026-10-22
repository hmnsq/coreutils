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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <selinux/selinux.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "dev-ino.h"
#অন্তর্ভাৱ "fts-missing.h"
#অন্তর্ভাৱ "ignore-value.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "root-dev-ino.h"
#অন্তর্ভাৱ "selinux-at.h"
#অন্তর্ভাৱ "xfts.h"

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
স্থির সংরচনা dev_ino *মূল_dev_ino;

/* The name of the context file is being given. */
স্থির ৱর্ণ ধ্রুৱ *specified_context;

/* Specific components of the context */
স্থির ৱর্ণ ধ্রুৱ *specified_user;
স্থির ৱর্ণ ধ্রুৱ *specified_role;
স্থির ৱর্ণ ধ্রুৱ *specified_range;
স্থির ৱর্ণ ধ্রুৱ *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_মূল,
  PRESERVE_মূল,
  REFERENCE_FILE_OPTION
};

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"recursive", no_argument, রিক্ত, 'R'},
  {"dereference", no_argument, রিক্ত, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, রিক্ত, 'h'},
  {"no-preserve-root", no_argument, রিক্ত, NO_PRESERVE_মূল},
  {"preserve-root", no_argument, রিক্ত, PRESERVE_মূল},
  {"reference", required_argument, রিক্ত, REFERENCE_FILE_OPTION},
  {"user", required_argument, রিক্ত, 'u'},
  {"role", required_argument, রিক্ত, 'r'},
  {"type", required_argument, রিক্ত, 't'},
  {"range", required_argument, রিক্ত, 'l'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
স্থির পূর্ণাঙ্ক
compute_context_from_mask (ৱর্ণ ধ্রুৱ *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  যদি (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ঘূৰাই 1;
    }

#define SET_COMPONENT(C, comp)						\
   কৰা									\
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
   যেতিয়ালৈ (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  যদি (!ok)
    {
      পূর্ণাঙ্ক saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ঘূৰাই 1;
    }

  *ret = new_context;
  ঘূৰাই 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

স্থির পূর্ণাঙ্ক
change_file_context (পূর্ণাঙ্ক fd, ৱর্ণ ধ্রুৱ *file, bool ignore_missing)
{
  ৱর্ণ *file_context = রিক্ত;
  context_t context IF_LINT (= রিক্ত);
  ৱর্ণ ধ্রুৱ * context_string;
  পূর্ণাঙ্ক errors = 0;

  যদি (specified_context == রিক্ত)
    {
      পূর্ণাঙ্ক status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      যদি (status < 0 && errno != ENODATA)
        {
          যদি (ignore_missing && ignorable_traversal_errno (errno))
            ঘূৰাই 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ঘূৰাই 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      যদি (file_context == রিক্ত)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ঘূৰাই 1;
        }

      যদি (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          ঘূৰাই 1;
        }

      context_string = context_str (context);
    }
  অন্যথা
    {
      context_string = specified_context;
    }

  যদি (file_context == রিক্ত || ! streq (context_string, file_context))
    {
      পূর্ণাঙ্ক fail = (affect_symlink_referent
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

  যদি (specified_context == রিক্ত)
    {
      context_free (context);
      freecon (file_context);
    }

  ঘূৰাই errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

স্থির bool
process_file (FTS *fts, FTSENT *ent)
{
  ৱর্ণ ধ্রুৱ *file_full_name = ent->fts_path;
  ৱর্ণ ধ্রুৱ *file = ent->fts_accpath;
  ধ্রুৱ সংরচনা stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    ঘূৰাই true;

  ৱিকল্প (ent->fts_info)
    {
    পক্ষ FTS_D:
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
              ঘূৰাই false;
            }
          ঘূৰাই true;
        }
      ভঙ্গ;

    পক্ষ FTS_DP:
      যদি (! recurse)
        ঘূৰাই true;
      ভঙ্গ;

    পক্ষ FTS_NS:
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
          ঘূৰাই true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      ভঙ্গ;

    পক্ষ FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      ভঙ্গ;

    পক্ষ FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      ভঙ্গ;

    পক্ষ FTS_DC:		/* directory that causes cycles */
      যদি (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ঘূৰাই false;
        }
      ভঙ্গ;

    স্ৱভাৱ:
      ভঙ্গ;
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

  ঘূৰাই ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

স্থির bool
process_files (ৱর্ণ **files, পূর্ণাঙ্ক bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, রিক্ত);

  যেতিয়ালৈ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      যদি (ent == রিক্ত)
        {
          যদি (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ভঙ্গ;
        }

      ok &= process_file (fts, ent);
    }

  যদি (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ঘূৰাই ok;
}

শূন্য
usage (পূর্ণাঙ্ক status)
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
         the স্ৱভাৱ), rather than the symbolic link itself\n\
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
         কৰা not treat '/' specially (the স্ৱভাৱ)\n\
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
         output a diagnostic বাবে every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  /* Bit flags that control how fts works.  */
  পূর্ণাঙ্ক bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  পূর্ণাঙ্ক dereference = -1;

  bool ok;
  bool preserve_মূল = false;
  bool component_specified = false;
  ৱর্ণ *reference_file = রিক্ত;
  পূর্ণাঙ্ক optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, রিক্ত))
         != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ভঙ্গ;

        পক্ষ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ভঙ্গ;

        পক্ষ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ভঙ্গ;

        পক্ষ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ভঙ্গ;

        পক্ষ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ভঙ্গ;

        পক্ষ NO_PRESERVE_মূল:
          preserve_মূল = false;
          ভঙ্গ;

        পক্ষ PRESERVE_মূল:
          preserve_মূল = true;
          ভঙ্গ;

        পক্ষ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ভঙ্গ;

        পক্ষ 'R':
          recurse = true;
          ভঙ্গ;

        পক্ষ 'f':
          /* ignore */
          ভঙ্গ;

        পক্ষ 'v':
          verbose = true;
          ভঙ্গ;

        পক্ষ 'u':
          specified_user = optarg;
          component_specified = true;
          ভঙ্গ;

        পক্ষ 'r':
          specified_role = optarg;
          component_specified = true;
          ভঙ্গ;

        পক্ষ 't':
          specified_type = optarg;
          component_specified = true;
          ভঙ্গ;

        পক্ষ 'l':
          specified_range = optarg;
          component_specified = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
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
      ৱর্ণ *ref_context = রিক্ত;

      যদি (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  অন্যথা যদি (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = রিক্ত;
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
      স্থির সংরচনা dev_ino dev_ino_buf;
      মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);
      যদি (মূল_dev_ino == রিক্ত)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  অন্যথা
    {
      মূল_dev_ino = রিক্ত;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
