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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>
#ضمن <selinux/selinux.h>

#ضمن "system.h"
#ضمن "dev-ino.h"
#ضمن "fts-missing.h"
#ضمن "ignore-value.h"
#ضمن "quote.h"
#ضمن "root-dev-ino.h"
#ضمن "selinux-at.h"
#ضمن "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
ساكن bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
ساكن bool recurse;

/* Level of verbosity. */
ساكن bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ساكن بنية dev_ino *ڕەگ_dev_ino;

/* The name of the context file is being given. */
ساكن حرف ثابت *specified_context;

/* Specific components of the context */
ساكن حرف ثابت *specified_user;
ساكن حرف ثابت *specified_role;
ساكن حرف ثابت *specified_range;
ساكن حرف ثابت *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ڕەگ,
  PRESERVE_ڕەگ,
  REFERENCE_FILE_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"recursive", no_argument, عدم, 'R'},
  {"dereference", no_argument, عدم, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, عدم, 'h'},
  {"no-preserve-root", no_argument, عدم, NO_PRESERVE_ڕەگ},
  {"preserve-root", no_argument, عدم, PRESERVE_ڕەگ},
  {"reference", required_argument, عدم, REFERENCE_FILE_OPTION},
  {"user", required_argument, عدم, 'u'},
  {"role", required_argument, عدم, 'r'},
  {"type", required_argument, عدم, 't'},
  {"range", required_argument, عدم, 'l'},
  {"verbose", no_argument, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ساكن صحيح
compute_context_from_mask (حرف ثابت *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  إذا (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      أرجع 1;
    }

#define SET_COMPONENT(C, comp)						\
   افعل									\
     {									\
       إذا (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   طالما (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  إذا (!ok)
    {
      صحيح saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      أرجع 1;
    }

  *ret = new_context;
  أرجع 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ساكن صحيح
change_file_context (صحيح fd, حرف ثابت *file, bool ignore_missing)
{
  حرف *file_context = عدم;
  context_t context IF_LINT (= عدم);
  حرف ثابت * context_string;
  صحيح errors = 0;

  إذا (specified_context == عدم)
    {
      صحيح status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      إذا (status < 0 && errno != ENODATA)
        {
          إذا (ignore_missing && ignorable_traversal_errno (errno))
            أرجع 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          أرجع 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      إذا (file_context == عدم)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          أرجع 1;
        }

      إذا (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          أرجع 1;
        }

      context_string = context_str (context);
    }
  وإلا
    {
      context_string = specified_context;
    }

  إذا (file_context == عدم || ! streq (context_string, file_context))
    {
      صحيح fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      إذا (fail)
        {
          إذا (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  إذا (specified_context == عدم)
    {
      context_free (context);
      freecon (file_context);
    }

  أرجع errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ساكن bool
process_file (FTS *fts, FTSENT *ent)
{
  حرف ثابت *file_full_name = ent->fts_path;
  حرف ثابت *file = ent->fts_accpath;
  ثابت بنية stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  إذا (ignore_missing && ignorable_fts_error (ent))
    أرجع true;

  بدل (ent->fts_info)
    {
    حالة FTS_D:
      إذا (recurse)
        {
          إذا (ڕەگ_DEV_INO_CHECK (ڕەگ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ڕەگ_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_بەها (fts_read (fts));
              أرجع false;
            }
          أرجع true;
        }
      اكسر;

    حالة FTS_DP:
      إذا (! recurse)
        أرجع true;
      اكسر;

    حالة FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      إذا (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ژمار == 0)
        {
          ent->fts_ژمار = 1;
          fts_set (fts, ent, FTS_AGAIN);
          أرجع true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      اكسر;

    حالة FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      اكسر;

    حالة FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      اكسر;

    حالة FTS_DC:		/* directory that causes cycles */
      إذا (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          أرجع false;
        }
      اكسر;

    افتراضي:
      اكسر;
    }

  إذا (ent->fts_info == FTS_DP
      && ok && ڕەگ_DEV_INO_CHECK (ڕەگ_dev_ino, file_stats))
    {
      ڕەگ_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  إذا (ok)
    {
      إذا (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      إذا (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  إذا ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  أرجع ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساكن bool
process_files (حرف **files, صحيح bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, عدم);

  طالما (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      إذا (ent == عدم)
        {
          إذا (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          اكسر;
        }

      ok &= process_file (fts, ent);
    }

  إذا (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  أرجع ok;
}

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
         the افتراضي), rather than the symbolic link itself\n\
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
      --no-preserve-ڕەگ\n\
         افعل not treat '/' specially (the افتراضي)\n\
"));
      oputs (_("\
      --preserve-ڕەگ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT بەها\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic لكل every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  /* Bit flags that control how fts works.  */
  صحيح bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحيح dereference = -1;

  bool ok;
  bool preserve_ڕەگ = false;
  bool component_specified = false;
  حرف *reference_file = عدم;
  صحيح optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, عدم))
         != -1)
    {
      بدل (optc)
        {
        حالة 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          اكسر;

        حالة 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          اكسر;

        حالة 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          اكسر;

        حالة 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          اكسر;

        حالة DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          اكسر;

        حالة NO_PRESERVE_ڕەگ:
          preserve_ڕەگ = false;
          اكسر;

        حالة PRESERVE_ڕەگ:
          preserve_ڕەگ = true;
          اكسر;

        حالة REFERENCE_FILE_OPTION:
          reference_file = optarg;
          اكسر;

        حالة 'R':
          recurse = true;
          اكسر;

        حالة 'f':
          /* ignore */
          اكسر;

        حالة 'v':
          verbose = true;
          اكسر;

        حالة 'u':
          specified_user = optarg;
          component_specified = true;
          اكسر;

        حالة 'r':
          specified_role = optarg;
          component_specified = true;
          اكسر;

        حالة 't':
          specified_type = optarg;
          component_specified = true;
          اكسر;

        حالة 'l':
          specified_range = optarg;
          component_specified = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (recurse)
    {
      إذا (bit_flags == FTS_PHYSICAL)
        {
          إذا (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      وإلا
        {
          إذا (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  وإلا
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  إذا (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      إذا (argc <= optind)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  إذا (reference_file)
    {
      حرف *ref_context = عدم;

      إذا (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  وإلا إذا (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = عدم;
    }
  وإلا
    {
      specified_context = argv[optind++];
      إذا (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  إذا (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  إذا (recurse && preserve_ڕەگ)
    {
      ساكن بنية dev_ino dev_ino_buf;
      ڕەگ_dev_ino = get_ڕەگ_dev_ino (&dev_ino_buf);
      إذا (ڕەگ_dev_ino == عدم)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  وإلا
    {
      ڕەگ_dev_ino = عدم;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
