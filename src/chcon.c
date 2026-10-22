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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>
#شامل <selinux/selinux.h>

#شامل "system.h"
#شامل "dev-ino.h"
#شامل "fts-missing.h"
#شامل "ignore-value.h"
#شامل "quote.h"
#شامل "root-dev-ino.h"
#شامل "selinux-at.h"
#شامل "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
ایستا bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
ایستا bool recurse;

/* Level of verbosity. */
ایستا bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ایستا ساختار dev_ino *ریشه_dev_ino;

/* The name of the context file is being given. */
ایستا نویسه ثابت *specified_context;

/* Specific components of the context */
ایستا نویسه ثابت *specified_user;
ایستا نویسه ثابت *specified_role;
ایستا نویسه ثابت *specified_range;
ایستا نویسه ثابت *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ریشه,
  PRESERVE_ریشه,
  REFERENCE_FILE_OPTION
};

ایستا ساختار option ثابت long_options[] =
{
  {"recursive", no_argument, پوچ, 'R'},
  {"dereference", no_argument, پوچ, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, پوچ, 'h'},
  {"no-preserve-root", no_argument, پوچ, NO_PRESERVE_ریشه},
  {"preserve-root", no_argument, پوچ, PRESERVE_ریشه},
  {"reference", required_argument, پوچ, REFERENCE_FILE_OPTION},
  {"user", required_argument, پوچ, 'u'},
  {"role", required_argument, پوچ, 'r'},
  {"type", required_argument, پوچ, 't'},
  {"range", required_argument, پوچ, 'l'},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ایستا صحیح
compute_context_from_mask (نویسه ثابت *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  اگر (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      بازگشت 1;
    }

#define SET_COMPONENT(C, comp)						\
   انجامبده									\
     {									\
       اگر (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   وقتی (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  اگر (!ok)
    {
      صحیح saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      بازگشت 1;
    }

  *ret = new_context;
  بازگشت 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ایستا صحیح
change_file_context (صحیح fd, نویسه ثابت *file, bool ignore_missing)
{
  نویسه *file_context = پوچ;
  context_t context IF_LINT (= پوچ);
  نویسه ثابت * context_string;
  صحیح errors = 0;

  اگر (specified_context == پوچ)
    {
      صحیح status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      اگر (status < 0 && errno != ENODATA)
        {
          اگر (ignore_missing && ignorable_traversal_errno (errno))
            بازگشت 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          بازگشت 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      اگر (file_context == پوچ)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          بازگشت 1;
        }

      اگر (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          بازگشت 1;
        }

      context_string = context_str (context);
    }
  وگرنه
    {
      context_string = specified_context;
    }

  اگر (file_context == پوچ || ! streq (context_string, file_context))
    {
      صحیح fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      اگر (fail)
        {
          اگر (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  اگر (specified_context == پوچ)
    {
      context_free (context);
      freecon (file_context);
    }

  بازگشت errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ایستا bool
process_file (FTS *fts, FTSENT *ent)
{
  نویسه ثابت *file_full_name = ent->fts_path;
  نویسه ثابت *file = ent->fts_accpath;
  ثابت ساختار stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  اگر (ignore_missing && ignorable_fts_error (ent))
    بازگشت true;

  انتخاب (ent->fts_info)
    {
    حالت FTS_D:
      اگر (recurse)
        {
          اگر (ریشه_DEV_INO_CHECK (ریشه_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ریشه_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_مقدار (fts_read (fts));
              بازگشت false;
            }
          بازگشت true;
        }
      بشکن;

    حالت FTS_DP:
      اگر (! recurse)
        بازگشت true;
      بشکن;

    حالت FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      اگر (ent->fts_level == FTS_ROOTLEVEL && ent->fts_عدد == 0)
        {
          ent->fts_عدد = 1;
          fts_set (fts, ent, FTS_AGAIN);
          بازگشت true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      بشکن;

    حالت FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      بشکن;

    حالت FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      بشکن;

    حالت FTS_DC:		/* directory that causes cycles */
      اگر (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          بازگشت false;
        }
      بشکن;

    پیشفرض:
      بشکن;
    }

  اگر (ent->fts_info == FTS_DP
      && ok && ریشه_DEV_INO_CHECK (ریشه_dev_ino, file_stats))
    {
      ریشه_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  اگر (ok)
    {
      اگر (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      اگر (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  اگر ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  بازگشت ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ایستا bool
process_files (نویسه **files, صحیح bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, پوچ);

  وقتی (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      اگر (ent == پوچ)
        {
          اگر (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          بشکن;
        }

      ok &= process_file (fts, ent);
    }

  اگر (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  بازگشت ok;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         the پیشفرض), rather than the symbolic link itself\n\
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
      --no-preserve-ریشه\n\
         انجامبده not treat '/' specially (the پیشفرض)\n\
"));
      oputs (_("\
      --preserve-ریشه\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT مقدار\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic برای every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  /* Bit flags that control how fts works.  */
  صحیح bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحیح dereference = -1;

  bool ok;
  bool preserve_ریشه = false;
  bool component_specified = false;
  نویسه *reference_file = پوچ;
  صحیح optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, پوچ))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          بشکن;

        حالت 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          بشکن;

        حالت 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          بشکن;

        حالت 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          بشکن;

        حالت DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          بشکن;

        حالت NO_PRESERVE_ریشه:
          preserve_ریشه = false;
          بشکن;

        حالت PRESERVE_ریشه:
          preserve_ریشه = true;
          بشکن;

        حالت REFERENCE_FILE_OPTION:
          reference_file = optarg;
          بشکن;

        حالت 'R':
          recurse = true;
          بشکن;

        حالت 'f':
          /* ignore */
          بشکن;

        حالت 'v':
          verbose = true;
          بشکن;

        حالت 'u':
          specified_user = optarg;
          component_specified = true;
          بشکن;

        حالت 'r':
          specified_role = optarg;
          component_specified = true;
          بشکن;

        حالت 't':
          specified_type = optarg;
          component_specified = true;
          بشکن;

        حالت 'l':
          specified_range = optarg;
          component_specified = true;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (recurse)
    {
      اگر (bit_flags == FTS_PHYSICAL)
        {
          اگر (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      وگرنه
        {
          اگر (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  وگرنه
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  اگر (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      وگرنه
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  اگر (reference_file)
    {
      نویسه *ref_context = پوچ;

      اگر (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  وگرنه اگر (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = پوچ;
    }
  وگرنه
    {
      specified_context = argv[optind++];
      اگر (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  اگر (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  اگر (recurse && preserve_ریشه)
    {
      ایستا ساختار dev_ino dev_ino_buf;
      ریشه_dev_ino = get_ریشه_dev_ino (&dev_ino_buf);
      اگر (ریشه_dev_ino == پوچ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  وگرنه
    {
      ریشه_dev_ino = پوچ;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
