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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <selinux/selinux.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "dev-ino.h"
#અન્તર્ભાવ "fts-missing.h"
#અન્તર્ભાવ "ignore-value.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "root-dev-ino.h"
#અન્તર્ભાવ "selinux-at.h"
#અન્તર્ભાવ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
સ્થિર bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
સ્થિર bool recurse;

/* Level of verbosity. */
સ્થિર bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
સ્થિર સંરચના dev_ino *મૂળ_dev_ino;

/* The name of the context file is being given. */
સ્થિર અક્ષર અચલ *specified_context;

/* Specific components of the context */
સ્થિર અક્ષર અચલ *specified_user;
સ્થિર અક્ષર અચલ *specified_role;
સ્થિર અક્ષર અચલ *specified_range;
સ્થિર અક્ષર અચલ *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_મૂળ,
  PRESERVE_મૂળ,
  REFERENCE_FILE_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"recursive", no_argument, ખાલી, 'R'},
  {"dereference", no_argument, ખાલી, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, ખાલી, 'h'},
  {"no-preserve-root", no_argument, ખાલી, NO_PRESERVE_મૂળ},
  {"preserve-root", no_argument, ખાલી, PRESERVE_મૂળ},
  {"reference", required_argument, ખાલી, REFERENCE_FILE_OPTION},
  {"user", required_argument, ખાલી, 'u'},
  {"role", required_argument, ખાલી, 'r'},
  {"type", required_argument, ખાલી, 't'},
  {"range", required_argument, ખાલી, 'l'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
સ્થિર પૂર્ણાંક
compute_context_from_mask (અક્ષર અચલ *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  જો (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      પાછા 1;
    }

#define SET_COMPONENT(C, comp)						\
   કરો									\
     {									\
       જો (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   જ્યારે (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  જો (!ok)
    {
      પૂર્ણાંક saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      પાછા 1;
    }

  *ret = new_context;
  પાછા 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

સ્થિર પૂર્ણાંક
change_file_context (પૂર્ણાંક fd, અક્ષર અચલ *file, bool ignore_missing)
{
  અક્ષર *file_context = ખાલી;
  context_t context IF_LINT (= ખાલી);
  અક્ષર અચલ * context_string;
  પૂર્ણાંક errors = 0;

  જો (specified_context == ખાલી)
    {
      પૂર્ણાંક status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      જો (status < 0 && errno != ENODATA)
        {
          જો (ignore_missing && ignorable_traversal_errno (errno))
            પાછા 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          પાછા 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      જો (file_context == ખાલી)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          પાછા 1;
        }

      જો (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          પાછા 1;
        }

      context_string = context_str (context);
    }
  નહીંતર
    {
      context_string = specified_context;
    }

  જો (file_context == ખાલી || ! streq (context_string, file_context))
    {
      પૂર્ણાંક fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      જો (fail)
        {
          જો (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  જો (specified_context == ખાલી)
    {
      context_free (context);
      freecon (file_context);
    }

  પાછા errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

સ્થિર bool
process_file (FTS *fts, FTSENT *ent)
{
  અક્ષર અચલ *file_full_name = ent->fts_path;
  અક્ષર અચલ *file = ent->fts_accpath;
  અચલ સંરચના stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  જો (ignore_missing && ignorable_fts_error (ent))
    પાછા true;

  પસંદગી (ent->fts_info)
    {
    સ્થિતિ FTS_D:
      જો (recurse)
        {
          જો (મૂળ_DEV_INO_CHECK (મૂળ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              મૂળ_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_કિંમત (fts_read (fts));
              પાછા false;
            }
          પાછા true;
        }
      તોડો;

    સ્થિતિ FTS_DP:
      જો (! recurse)
        પાછા true;
      તોડો;

    સ્થિતિ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      જો (ent->fts_level == FTS_ROOTLEVEL && ent->fts_સંખ્યા == 0)
        {
          ent->fts_સંખ્યા = 1;
          fts_set (fts, ent, FTS_AGAIN);
          પાછા true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      તોડો;

    સ્થિતિ FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      તોડો;

    સ્થિતિ FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      તોડો;

    સ્થિતિ FTS_DC:		/* directory that causes cycles */
      જો (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          પાછા false;
        }
      તોડો;

    મૂળભૂત:
      તોડો;
    }

  જો (ent->fts_info == FTS_DP
      && ok && મૂળ_DEV_INO_CHECK (મૂળ_dev_ino, file_stats))
    {
      મૂળ_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  જો (ok)
    {
      જો (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      જો (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  જો ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  પાછા ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

સ્થિર bool
process_files (અક્ષર **files, પૂર્ણાંક bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ખાલી);

  જ્યારે (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      જો (ent == ખાલી)
        {
          જો (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          તોડો;
        }

      ok &= process_file (fts, ent);
    }

  જો (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  પાછા ok;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         the મૂળભૂત), rather than the symbolic link itself\n\
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
      --no-preserve-મૂળ\n\
         કરો not treat '/' specially (the મૂળભૂત)\n\
"));
      oputs (_("\
      --preserve-મૂળ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT કિંમત\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic માટે every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  /* Bit flags that control how fts works.  */
  પૂર્ણાંક bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  પૂર્ણાંક dereference = -1;

  bool ok;
  bool preserve_મૂળ = false;
  bool component_specified = false;
  અક્ષર *reference_file = ખાલી;
  પૂર્ણાંક optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          તોડો;

        સ્થિતિ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          તોડો;

        સ્થિતિ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          તોડો;

        સ્થિતિ NO_PRESERVE_મૂળ:
          preserve_મૂળ = false;
          તોડો;

        સ્થિતિ PRESERVE_મૂળ:
          preserve_મૂળ = true;
          તોડો;

        સ્થિતિ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          તોડો;

        સ્થિતિ 'R':
          recurse = true;
          તોડો;

        સ્થિતિ 'f':
          /* ignore */
          તોડો;

        સ્થિતિ 'v':
          verbose = true;
          તોડો;

        સ્થિતિ 'u':
          specified_user = optarg;
          component_specified = true;
          તોડો;

        સ્થિતિ 'r':
          specified_role = optarg;
          component_specified = true;
          તોડો;

        સ્થિતિ 't':
          specified_type = optarg;
          component_specified = true;
          તોડો;

        સ્થિતિ 'l':
          specified_range = optarg;
          component_specified = true;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (recurse)
    {
      જો (bit_flags == FTS_PHYSICAL)
        {
          જો (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      નહીંતર
        {
          જો (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  નહીંતર
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  જો (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      જો (argc <= optind)
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  જો (reference_file)
    {
      અક્ષર *ref_context = ખાલી;

      જો (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  નહીંતર જો (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ખાલી;
    }
  નહીંતર
    {
      specified_context = argv[optind++];
      જો (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  જો (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  જો (recurse && preserve_મૂળ)
    {
      સ્થિર સંરચના dev_ino dev_ino_buf;
      મૂળ_dev_ino = get_મૂળ_dev_ino (&dev_ino_buf);
      જો (મૂળ_dev_ino == ખાલી)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  નહીંતર
    {
      મૂળ_dev_ino = ખાલી;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
