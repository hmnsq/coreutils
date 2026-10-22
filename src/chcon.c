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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <selinux/selinux.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "dev-ino.h"
#അന്തര്ഭാവ "fts-missing.h"
#അന്തര്ഭാവ "ignore-value.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "root-dev-ino.h"
#അന്തര്ഭാവ "selinux-at.h"
#അന്തര്ഭാവ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
സ്ഥിതം bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
സ്ഥിതം bool recurse;

/* Level of verbosity. */
സ്ഥിതം bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
സ്ഥിതം ഘടന dev_ino *വേര്_dev_ino;

/* The name of the context file is being given. */
സ്ഥിതം അക്ഷരം സ്ഥിരം *specified_context;

/* Specific components of the context */
സ്ഥിതം അക്ഷരം സ്ഥിരം *specified_user;
സ്ഥിതം അക്ഷരം സ്ഥിരം *specified_role;
സ്ഥിതം അക്ഷരം സ്ഥിരം *specified_range;
സ്ഥിതം അക്ഷരം സ്ഥിരം *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_വേര്,
  PRESERVE_വേര്,
  REFERENCE_FILE_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"recursive", no_argument, ശൂന്യ, 'R'},
  {"dereference", no_argument, ശൂന്യ, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, ശൂന്യ, 'h'},
  {"no-preserve-root", no_argument, ശൂന്യ, NO_PRESERVE_വേര്},
  {"preserve-root", no_argument, ശൂന്യ, PRESERVE_വേര്},
  {"reference", required_argument, ശൂന്യ, REFERENCE_FILE_OPTION},
  {"user", required_argument, ശൂന്യ, 'u'},
  {"role", required_argument, ശൂന്യ, 'r'},
  {"type", required_argument, ശൂന്യ, 't'},
  {"range", required_argument, ശൂന്യ, 'l'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
compute_context_from_mask (അക്ഷരം സ്ഥിരം *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  എങ്കിൽ (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      തിരികെ 1;
    }

#define SET_COMPONENT(C, comp)						\
   ചെയ്യുക									\
     {									\
       എങ്കിൽ (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   ആകുമ്പോൾ (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  എങ്കിൽ (!ok)
    {
      പൂർണ്ണസംഖ്യ saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      തിരികെ 1;
    }

  *ret = new_context;
  തിരികെ 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

സ്ഥിതം പൂർണ്ണസംഖ്യ
change_file_context (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *file, bool ignore_missing)
{
  അക്ഷരം *file_context = ശൂന്യ;
  context_t context IF_LINT (= ശൂന്യ);
  അക്ഷരം സ്ഥിരം * context_string;
  പൂർണ്ണസംഖ്യ errors = 0;

  എങ്കിൽ (specified_context == ശൂന്യ)
    {
      പൂർണ്ണസംഖ്യ status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      എങ്കിൽ (status < 0 && errno != ENODATA)
        {
          എങ്കിൽ (ignore_missing && ignorable_traversal_errno (errno))
            തിരികെ 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          തിരികെ 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      എങ്കിൽ (file_context == ശൂന്യ)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          തിരികെ 1;
        }

      എങ്കിൽ (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          തിരികെ 1;
        }

      context_string = context_str (context);
    }
  അല്ലെങ്കിൽ
    {
      context_string = specified_context;
    }

  എങ്കിൽ (file_context == ശൂന്യ || ! streq (context_string, file_context))
    {
      പൂർണ്ണസംഖ്യ fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      എങ്കിൽ (fail)
        {
          എങ്കിൽ (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  എങ്കിൽ (specified_context == ശൂന്യ)
    {
      context_free (context);
      freecon (file_context);
    }

  തിരികെ errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

സ്ഥിതം bool
process_file (FTS *fts, FTSENT *ent)
{
  അക്ഷരം സ്ഥിരം *file_full_name = ent->fts_path;
  അക്ഷരം സ്ഥിരം *file = ent->fts_accpath;
  സ്ഥിരം ഘടന stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  എങ്കിൽ (ignore_missing && ignorable_fts_error (ent))
    തിരികെ true;

  തിരഞ്ഞെടുപ്പ് (ent->fts_info)
    {
    അവസ്ഥ FTS_D:
      എങ്കിൽ (recurse)
        {
          എങ്കിൽ (വേര്_DEV_INO_CHECK (വേര്_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              വേര്_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_മൂല്യം (fts_read (fts));
              തിരികെ false;
            }
          തിരികെ true;
        }
      നിർത്തുക;

    അവസ്ഥ FTS_DP:
      എങ്കിൽ (! recurse)
        തിരികെ true;
      നിർത്തുക;

    അവസ്ഥ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      എങ്കിൽ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_സംഖ്യ == 0)
        {
          ent->fts_സംഖ്യ = 1;
          fts_set (fts, ent, FTS_AGAIN);
          തിരികെ true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ FTS_DC:		/* directory that causes cycles */
      എങ്കിൽ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          തിരികെ false;
        }
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      നിർത്തുക;
    }

  എങ്കിൽ (ent->fts_info == FTS_DP
      && ok && വേര്_DEV_INO_CHECK (വേര്_dev_ino, file_stats))
    {
      വേര്_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  എങ്കിൽ (ok)
    {
      എങ്കിൽ (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      എങ്കിൽ (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  എങ്കിൽ ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  തിരികെ ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

സ്ഥിതം bool
process_files (അക്ഷരം **files, പൂർണ്ണസംഖ്യ bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ശൂന്യ);

  ആകുമ്പോൾ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      എങ്കിൽ (ent == ശൂന്യ)
        {
          എങ്കിൽ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          നിർത്തുക;
        }

      ok &= process_file (fts, ent);
    }

  എങ്കിൽ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  തിരികെ ok;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         the സ്ഥിരസ്ഥിതി), rather than the symbolic link itself\n\
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
      --no-preserve-വേര്\n\
         ചെയ്യുക not treat '/' specially (the സ്ഥിരസ്ഥിതി)\n\
"));
      oputs (_("\
      --preserve-വേര്\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT മൂല്യം\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic വേണ്ടി every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  /* Bit flags that control how fts works.  */
  പൂർണ്ണസംഖ്യ bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  പൂർണ്ണസംഖ്യ dereference = -1;

  bool ok;
  bool preserve_വേര് = false;
  bool component_specified = false;
  അക്ഷരം *reference_file = ശൂന്യ;
  പൂർണ്ണസംഖ്യ optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          നിർത്തുക;

        അവസ്ഥ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          നിർത്തുക;

        അവസ്ഥ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          നിർത്തുക;

        അവസ്ഥ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          നിർത്തുക;

        അവസ്ഥ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          നിർത്തുക;

        അവസ്ഥ NO_PRESERVE_വേര്:
          preserve_വേര് = false;
          നിർത്തുക;

        അവസ്ഥ PRESERVE_വേര്:
          preserve_വേര് = true;
          നിർത്തുക;

        അവസ്ഥ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          നിർത്തുക;

        അവസ്ഥ 'R':
          recurse = true;
          നിർത്തുക;

        അവസ്ഥ 'f':
          /* ignore */
          നിർത്തുക;

        അവസ്ഥ 'v':
          verbose = true;
          നിർത്തുക;

        അവസ്ഥ 'u':
          specified_user = optarg;
          component_specified = true;
          നിർത്തുക;

        അവസ്ഥ 'r':
          specified_role = optarg;
          component_specified = true;
          നിർത്തുക;

        അവസ്ഥ 't':
          specified_type = optarg;
          component_specified = true;
          നിർത്തുക;

        അവസ്ഥ 'l':
          specified_range = optarg;
          component_specified = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (recurse)
    {
      എങ്കിൽ (bit_flags == FTS_PHYSICAL)
        {
          എങ്കിൽ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  അല്ലെങ്കിൽ
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  എങ്കിൽ (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      എങ്കിൽ (argc <= optind)
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (reference_file)
    {
      അക്ഷരം *ref_context = ശൂന്യ;

      എങ്കിൽ (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ശൂന്യ;
    }
  അല്ലെങ്കിൽ
    {
      specified_context = argv[optind++];
      എങ്കിൽ (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  എങ്കിൽ (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (recurse && preserve_വേര്)
    {
      സ്ഥിതം ഘടന dev_ino dev_ino_buf;
      വേര്_dev_ino = get_വേര്_dev_ino (&dev_ino_buf);
      എങ്കിൽ (വേര്_dev_ino == ശൂന്യ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  അല്ലെങ്കിൽ
    {
      വേര്_dev_ino = ശൂന്യ;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
