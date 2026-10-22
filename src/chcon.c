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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <selinux/selinux.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "fts-missing.h"
#ଅନ୍ତର୍ଭାଵ "ignore-value.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "root-dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "selinux-at.h"
#ଅନ୍ତର୍ଭାଵ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
ସ୍ଥିର bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
ସ୍ଥିର bool recurse;

/* Level of verbosity. */
ସ୍ଥିର bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ସ୍ଥିର ସଂରଚନା dev_ino *ମୂଳ_dev_ino;

/* The name of the context file is being given. */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *specified_context;

/* Specific components of the context */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *specified_user;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *specified_role;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *specified_range;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ମୂଳ,
  PRESERVE_ମୂଳ,
  REFERENCE_FILE_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"recursive", no_argument, ଖାଲି, 'R'},
  {"dereference", no_argument, ଖାଲି, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, ଖାଲି, 'h'},
  {"no-preserve-root", no_argument, ଖାଲି, NO_PRESERVE_ମୂଳ},
  {"preserve-root", no_argument, ଖାଲି, PRESERVE_ମୂଳ},
  {"reference", required_argument, ଖାଲି, REFERENCE_FILE_OPTION},
  {"user", required_argument, ଖାଲି, 'u'},
  {"role", required_argument, ଖାଲି, 'r'},
  {"type", required_argument, ଖାଲି, 't'},
  {"range", required_argument, ଖାଲି, 'l'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compute_context_from_mask (ଅକ୍ଷର ଅଚଳ *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  ଯଦି (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ଫେରନ୍ତୁ 1;
    }

#define SET_COMPONENT(C, comp)						\
   କର									\
     {									\
       ଯଦି (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   ଯେତେବେଳେ (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  ଯଦି (!ok)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ଫେରନ୍ତୁ 1;
    }

  *ret = new_context;
  ଫେରନ୍ତୁ 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
change_file_context (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *file, bool ignore_missing)
{
  ଅକ୍ଷର *file_context = ଖାଲି;
  context_t context IF_LINT (= ଖାଲି);
  ଅକ୍ଷର ଅଚଳ * context_string;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା errors = 0;

  ଯଦି (specified_context == ଖାଲି)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      ଯଦି (status < 0 && errno != ENODATA)
        {
          ଯଦି (ignore_missing && ignorable_traversal_errno (errno))
            ଫେରନ୍ତୁ 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ଫେରନ୍ତୁ 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      ଯଦି (file_context == ଖାଲି)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ଫେରନ୍ତୁ 1;
        }

      ଯଦି (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          ଫେରନ୍ତୁ 1;
        }

      context_string = context_str (context);
    }
  ଅନ୍ୟଥା
    {
      context_string = specified_context;
    }

  ଯଦି (file_context == ଖାଲି || ! streq (context_string, file_context))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      ଯଦି (fail)
        {
          ଯଦି (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  ଯଦି (specified_context == ଖାଲି)
    {
      context_free (context);
      freecon (file_context);
    }

  ଫେରନ୍ତୁ errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ସ୍ଥିର bool
process_file (FTS *fts, FTSENT *ent)
{
  ଅକ୍ଷର ଅଚଳ *file_full_name = ent->fts_path;
  ଅକ୍ଷର ଅଚଳ *file = ent->fts_accpath;
  ଅଚଳ ସଂରଚନା stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ଯଦି (ignore_missing && ignorable_fts_error (ent))
    ଫେରନ୍ତୁ true;

  ବାଛ (ent->fts_info)
    {
    ସ୍ଥିତି FTS_D:
      ଯଦି (recurse)
        {
          ଯଦି (ମୂଳ_DEV_INO_CHECK (ମୂଳ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ମୂଳ_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ମୂଲ୍ୟ (fts_read (fts));
              ଫେରନ୍ତୁ false;
            }
          ଫେରନ୍ତୁ true;
        }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DP:
      ଯଦି (! recurse)
        ଫେରନ୍ତୁ true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ଯଦି (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ସଂଖ୍ୟା == 0)
        {
          ent->fts_ସଂଖ୍ୟା = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ଫେରନ୍ତୁ true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DC:		/* directory that causes cycles */
      ଯଦି (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ଫେରନ୍ତୁ false;
        }
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଭାଙ୍ଗ;
    }

  ଯଦି (ent->fts_info == FTS_DP
      && ok && ମୂଳ_DEV_INO_CHECK (ମୂଳ_dev_ino, file_stats))
    {
      ମୂଳ_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  ଯଦି (ok)
    {
      ଯଦି (verbose)
        printf (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      ଯଦି (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  ଯଦି ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ଫେରନ୍ତୁ ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ସ୍ଥିର bool
process_files (ଅକ୍ଷର **files, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ଖାଲି);

  ଯେତେବେଳେ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ଯଦି (ent == ଖାଲି)
        {
          ଯଦି (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ଭାଙ୍ଗ;
        }

      ok &= process_file (fts, ent);
    }

  ଯଦି (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ଫେରନ୍ତୁ ok;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         the ପୂର୍ବନିର୍ଦ୍ଧାରିତ), rather than the symbolic link itself\n\
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
      --no-preserve-ମୂଳ\n\
         କର not treat '/' specially (the ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
      oputs (_("\
      --preserve-ମୂଳ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT ମୂଲ୍ୟ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ପାଇଁ every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  /* Bit flags that control how fts works.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dereference = -1;

  bool ok;
  bool preserve_ମୂଳ = false;
  bool component_specified = false;
  ଅକ୍ଷର *reference_file = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି NO_PRESERVE_ମୂଳ:
          preserve_ମୂଳ = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PRESERVE_ମୂଳ:
          preserve_ମୂଳ = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'R':
          recurse = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          /* ignore */
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':
          verbose = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          specified_user = optarg;
          component_specified = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'r':
          specified_role = optarg;
          component_specified = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          specified_type = optarg;
          component_specified = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'l':
          specified_range = optarg;
          component_specified = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (recurse)
    {
      ଯଦି (bit_flags == FTS_PHYSICAL)
        {
          ଯଦି (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  ଅନ୍ୟଥା
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  ଯଦି (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      ଯଦି (argc <= optind)
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ଯଦି (reference_file)
    {
      ଅକ୍ଷର *ref_context = ଖାଲି;

      ଯଦି (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  ଅନ୍ୟଥା ଯଦି (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ଖାଲି;
    }
  ଅନ୍ୟଥା
    {
      specified_context = argv[optind++];
      ଯଦି (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  ଯଦି (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (recurse && preserve_ମୂଳ)
    {
      ସ୍ଥିର ସଂରଚନା dev_ino dev_ino_buf;
      ମୂଳ_dev_ino = get_ମୂଳ_dev_ino (&dev_ino_buf);
      ଯଦି (ମୂଳ_dev_ino == ଖାଲି)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ଅନ୍ୟଥା
    {
      ମୂଳ_dev_ino = ଖାଲି;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
