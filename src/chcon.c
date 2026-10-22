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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <selinux/selinux.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "dev-ino.h"
#அந்தர்பாவ "fts-missing.h"
#அந்தர்பாவ "ignore-value.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "root-dev-ino.h"
#அந்தர்பாவ "selinux-at.h"
#அந்தர்பாவ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
நிலையான bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
நிலையான bool recurse;

/* Level of verbosity. */
நிலையான bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
நிலையான கட்டமைப்பு dev_ino *வேர்_dev_ino;

/* The name of the context file is being given. */
நிலையான எழுத்து மாறிலி *specified_context;

/* Specific components of the context */
நிலையான எழுத்து மாறிலி *specified_user;
நிலையான எழுத்து மாறிலி *specified_role;
நிலையான எழுத்து மாறிலி *specified_range;
நிலையான எழுத்து மாறிலி *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_வேர்,
  PRESERVE_வேர்,
  REFERENCE_FILE_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"recursive", no_argument, வெற்று, 'R'},
  {"dereference", no_argument, வெற்று, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, வெற்று, 'h'},
  {"no-preserve-root", no_argument, வெற்று, NO_PRESERVE_வேர்},
  {"preserve-root", no_argument, வெற்று, PRESERVE_வேர்},
  {"reference", required_argument, வெற்று, REFERENCE_FILE_OPTION},
  {"user", required_argument, வெற்று, 'u'},
  {"role", required_argument, வெற்று, 'r'},
  {"type", required_argument, வெற்று, 't'},
  {"range", required_argument, வெற்று, 'l'},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
நிலையான முழு
compute_context_from_mask (எழுத்து மாறிலி *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  எனில் (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      திரும்பு 1;
    }

#define SET_COMPONENT(C, comp)						\
   செய்									\
     {									\
       எனில் (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   போது (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  எனில் (!ok)
    {
      முழு saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      திரும்பு 1;
    }

  *ret = new_context;
  திரும்பு 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

நிலையான முழு
change_file_context (முழு fd, எழுத்து மாறிலி *file, bool ignore_missing)
{
  எழுத்து *file_context = வெற்று;
  context_t context IF_LINT (= வெற்று);
  எழுத்து மாறிலி * context_string;
  முழு errors = 0;

  எனில் (specified_context == வெற்று)
    {
      முழு status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      எனில் (status < 0 && errno != ENODATA)
        {
          எனில் (ignore_missing && ignorable_traversal_errno (errno))
            திரும்பு 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          திரும்பு 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      எனில் (file_context == வெற்று)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          திரும்பு 1;
        }

      எனில் (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          திரும்பு 1;
        }

      context_string = context_str (context);
    }
  இல்லையெனில்
    {
      context_string = specified_context;
    }

  எனில் (file_context == வெற்று || ! streq (context_string, file_context))
    {
      முழு fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      எனில் (fail)
        {
          எனில் (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  எனில் (specified_context == வெற்று)
    {
      context_free (context);
      freecon (file_context);
    }

  திரும்பு errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

நிலையான bool
process_file (FTS *fts, FTSENT *ent)
{
  எழுத்து மாறிலி *file_full_name = ent->fts_path;
  எழுத்து மாறிலி *file = ent->fts_accpath;
  மாறிலி கட்டமைப்பு stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  எனில் (ignore_missing && ignorable_fts_error (ent))
    திரும்பு true;

  தேர்வு (ent->fts_info)
    {
    நிலை FTS_D:
      எனில் (recurse)
        {
          எனில் (வேர்_DEV_INO_CHECK (வேர்_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              வேர்_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_மதிப்பு (fts_read (fts));
              திரும்பு false;
            }
          திரும்பு true;
        }
      நிறுத்து;

    நிலை FTS_DP:
      எனில் (! recurse)
        திரும்பு true;
      நிறுத்து;

    நிலை FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      எனில் (ent->fts_level == FTS_ROOTLEVEL && ent->fts_எண் == 0)
        {
          ent->fts_எண் = 1;
          fts_set (fts, ent, FTS_AGAIN);
          திரும்பு true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      நிறுத்து;

    நிலை FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      நிறுத்து;

    நிலை FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      நிறுத்து;

    நிலை FTS_DC:		/* directory that causes cycles */
      எனில் (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          திரும்பு false;
        }
      நிறுத்து;

    இயல்பு:
      நிறுத்து;
    }

  எனில் (ent->fts_info == FTS_DP
      && ok && வேர்_DEV_INO_CHECK (வேர்_dev_ino, file_stats))
    {
      வேர்_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  எனில் (ok)
    {
      எனில் (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      எனில் (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  எனில் ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  திரும்பு ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

நிலையான bool
process_files (எழுத்து **files, முழு bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, வெற்று);

  போது (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      எனில் (ent == வெற்று)
        {
          எனில் (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          நிறுத்து;
        }

      ok &= process_file (fts, ent);
    }

  எனில் (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  திரும்பு ok;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         the இயல்பு), rather than the symbolic link itself\n\
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
      --no-preserve-வேர்\n\
         செய் not treat '/' specially (the இயல்பு)\n\
"));
      oputs (_("\
      --preserve-வேர்\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT மதிப்பு\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic க்கு every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  /* Bit flags that control how fts works.  */
  முழு bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  முழு dereference = -1;

  bool ok;
  bool preserve_வேர் = false;
  bool component_specified = false;
  எழுத்து *reference_file = வெற்று;
  முழு optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, வெற்று))
         != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          நிறுத்து;

        நிலை 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          நிறுத்து;

        நிலை DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          நிறுத்து;

        நிலை NO_PRESERVE_வேர்:
          preserve_வேர் = false;
          நிறுத்து;

        நிலை PRESERVE_வேர்:
          preserve_வேர் = true;
          நிறுத்து;

        நிலை REFERENCE_FILE_OPTION:
          reference_file = optarg;
          நிறுத்து;

        நிலை 'R':
          recurse = true;
          நிறுத்து;

        நிலை 'f':
          /* ignore */
          நிறுத்து;

        நிலை 'v':
          verbose = true;
          நிறுத்து;

        நிலை 'u':
          specified_user = optarg;
          component_specified = true;
          நிறுத்து;

        நிலை 'r':
          specified_role = optarg;
          component_specified = true;
          நிறுத்து;

        நிலை 't':
          specified_type = optarg;
          component_specified = true;
          நிறுத்து;

        நிலை 'l':
          specified_range = optarg;
          component_specified = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (recurse)
    {
      எனில் (bit_flags == FTS_PHYSICAL)
        {
          எனில் (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      இல்லையெனில்
        {
          எனில் (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  இல்லையெனில்
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  எனில் (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      எனில் (argc <= optind)
        error (0, 0, _("missing operand"));
      இல்லையெனில்
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  எனில் (reference_file)
    {
      எழுத்து *ref_context = வெற்று;

      எனில் (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  இல்லையெனில் எனில் (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = வெற்று;
    }
  இல்லையெனில்
    {
      specified_context = argv[optind++];
      எனில் (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  எனில் (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  எனில் (recurse && preserve_வேர்)
    {
      நிலையான கட்டமைப்பு dev_ino dev_ino_buf;
      வேர்_dev_ino = get_வேர்_dev_ino (&dev_ino_buf);
      எனில் (வேர்_dev_ino == வெற்று)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  இல்லையெனில்
    {
      வேர்_dev_ino = வெற்று;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
