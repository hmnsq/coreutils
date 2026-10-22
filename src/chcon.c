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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/selinux.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fts-missing.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "selinux-at.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "chcon"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
ਸਥਿਰ bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
ਸਥਿਰ bool recurse;

/* Level of verbosity. */
ਸਥਿਰ bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ਸਥਿਰ ਬਣਤਰ dev_ino *ਜੜ੍ਹ_dev_ino;

/* The name of the context file is being given. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *specified_context;

/* Specific components of the context */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *specified_user;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *specified_role;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *specified_range;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ਜੜ੍ਹ,
  PRESERVE_ਜੜ੍ਹ,
  REFERENCE_FILE_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"recursive", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, DEREFERENCE_OPTION},
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"no-preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, NO_PRESERVE_ਜੜ੍ਹ},
  {"preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, PRESERVE_ਜੜ੍ਹ},
  {"reference", required_ਦਲੀਲ, ਨਿਰਾ, REFERENCE_FILE_OPTION},
  {"user", required_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"role", required_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"type", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"range", required_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ਸਥਿਰ ਪੂਰਨ
compute_context_from_ਨਕਾਬ (ਅੱਖਰ ਅਚਲ *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  ਜੇ (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      ਵਾਪਸ 1;
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_COMPONENT(C, comp)						\
   ਕਰੋ									\
     {									\
       ਜੇ (specified_ ## comp						\
           && context_ ## comp ## ਸੈੱਟ ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   ਜਦੋਂ (0)

  ਸੈੱਟ_COMPONENT (new_context, user);
  ਸੈੱਟ_COMPONENT (new_context, range);
  ਸੈੱਟ_COMPONENT (new_context, role);
  ਸੈੱਟ_COMPONENT (new_context, type);

  ਜੇ (!ok)
    {
      ਪੂਰਨ saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      ਵਾਪਸ 1;
    }

  *ret = new_context;
  ਵਾਪਸ 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ਸਥਿਰ ਪੂਰਨ
change_file_context (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *file, bool ignore_missing)
{
  ਅੱਖਰ *file_context = ਨਿਰਾ;
  context_t context IF_LINT (= ਨਿਰਾ);
  ਅੱਖਰ ਅਚਲ * context_string;
  ਪੂਰਨ errors = 0;

  ਜੇ (specified_context == ਨਿਰਾ)
    {
      ਪੂਰਨ ਸਥਿਤੀ = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      ਜੇ (ਸਥਿਤੀ < 0 && errno != ENODATA)
        {
          ਜੇ (ignore_missing && ignorable_traversal_errno (errno))
            ਵਾਪਸ 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          ਵਾਪਸ 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      ਜੇ (file_context == ਨਿਰਾ)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          ਵਾਪਸ 1;
        }

      ਜੇ (compute_context_from_ਨਕਾਬ (file_context, &context))
        {
          freecon (file_context);
          ਵਾਪਸ 1;
        }

      context_string = context_str (context);
    }
  ਨਹੀਂਤਾਂ
    {
      context_string = specified_context;
    }

  ਜੇ (file_context == ਨਿਰਾ || ! streq (context_string, file_context))
    {
      ਪੂਰਨ fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      ਜੇ (fail)
        {
          ਜੇ (! (ignore_missing && ignorable_traversal_errno (errno)))
            {
              errors = 1;
              error (0, errno, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  ਜੇ (specified_context == ਨਿਰਾ)
    {
      context_free (context);
      freecon (file_context);
    }

  ਵਾਪਸ errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ਸਥਿਰ bool
process_file (FTS *fts, FTSENT *ent)
{
  ਅੱਖਰ ਅਚਲ *file_ਭਰਿਆ_name = ent->fts_ਰਸਤਾ;
  ਅੱਖਰ ਅਚਲ *file = ent->fts_accpath;
  ਅਚਲ ਬਣਤਰ stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_ਇੰਦਰਾਜ (ent);

  ਜੇ (ignore_missing && ignorable_fts_error (ent))
    ਵਾਪਸ true;

  ਚੋਣ (ent->fts_info)
    {
    ਹਾਲਤ FTS_D:
      ਜੇ (recurse)
        {
          ਜੇ (ਜੜ੍ਹ_DEV_INO_CHECK (ਜੜ੍ਹ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              ਜੜ੍ਹ_DEV_INO_WARN (file_ਭਰਿਆ_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_ਸੈੱਟ (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ਮੁੱਲ (fts_read (fts));
              ਵਾਪਸ false;
            }
          ਵਾਪਸ true;
        }
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DP:
      ਜੇ (! recurse)
        ਵਾਪਸ true;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ਜੇ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ਸੰਖਿਆ == 0)
        {
          ent->fts_ਸੰਖਿਆ = 1;
          fts_ਸੈੱਟ (fts, ent, FTS_AGAIN);
          ਵਾਪਸ true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_ਭਰਿਆ_name));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_ਭਰਿਆ_name));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_ਭਰਿਆ_name));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DC:		/* directory that causes cycles */
      ਜੇ (ਚੱਕਰ_warning_required (fts, ent))
        {
          emit_ਚੱਕਰ_warning (file_ਭਰਿਆ_name);
          ਵਾਪਸ false;
        }
      ਰੋਕੋ;

    ਮੂਲ:
      ਰੋਕੋ;
    }

  ਜੇ (ent->fts_info == FTS_DP
      && ok && ਜੜ੍ਹ_DEV_INO_CHECK (ਜੜ੍ਹ_dev_ino, file_stats))
    {
      ਜੜ੍ਹ_DEV_INO_WARN (file_ਭਰਿਆ_name);
      ok = false;
    }

  ਜੇ (ok)
    {
      ਜੇ (verbose)
        ਲਿਖੋ (_("changing security context of %s\n"),
                quoteaf (file_ਭਰਿਆ_name));

      ਜੇ (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  ਜੇ ( ! recurse)
    fts_ਸੈੱਟ (fts, ent, FTS_SKIP);

  ਵਾਪਸ ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ਸਥਿਰ bool
process_files (ਅੱਖਰ **files, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, ਦੋਆਧਾਰੀ_ਅੰਕ_flags, ਨਿਰਾ);

  ਜਦੋਂ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ਜੇ (ent == ਨਿਰਾ)
        {
          ਜੇ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ਰੋਕੋ;
        }

      ok &= process_file (fts, ent);
    }

  ਜੇ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ਵਾਪਸ ok;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... CONTEXT FILE...\n\
  or:  %s [OPTION]... [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
        ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Change the SELinux security context ਦਾ each FILE ਮੰਜ਼ਿਲ CONTEXT.\n\
With --reference, change the security context ਦਾ each FILE ਮੰਜ਼ਿਲ that ਦਾ RFILE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --dereference\n\
         affect the referent ਦਾ each symbolic link (this is\n\
         the ਮੂਲ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead ਦਾ any referenced file\n\
"));
      oputs (_("\
  -u, --user=USER\n\
         ਸੈੱਟ_2 user USER ਵਿੱਚ the ਨਿਸ਼ਾਨਾ security context\n\
"));
      oputs (_("\
  -r, --role=ROLE\n\
         ਸੈੱਟ_2 role ROLE ਵਿੱਚ the ਨਿਸ਼ਾਨਾ security context\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         ਸੈੱਟ_2 type TYPE ਵਿੱਚ the ਨਿਸ਼ਾਨਾ security context\n\
"));
      oputs (_("\
  -l, --range=RANGE\n\
         ਸੈੱਟ_2 range RANGE ਵਿੱਚ the ਨਿਸ਼ਾਨਾ security context\n\
"));
      oputs (_("\
      --no-preserve-ਜੜ੍ਹ\n\
         ਕਰੋ not treat '/' specially (the ਮੂਲ)\n\
"));
      oputs (_("\
      --preserve-ਜੜ੍ਹ\n\
         fail ਮੰਜ਼ਿਲ ਚਲਾਓ recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT ਮੁੱਲ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         ਚਲਾਓ on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ਲਈ every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* Bit flags that control how fts works.  */
  ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  ਪੂਰਨ dereference = -1;

  bool ok;
  bool preserve_ਜੜ੍ਹ = false;
  bool component_specified = false;
  ਅੱਖਰ *reference_file = ਨਿਰਾ;
  ਪੂਰਨ optc;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'H': /* Traverse command-line symlinks-to-directories.  */
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'L': /* Traverse all symlinks-to-directories.  */
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_LOGICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'P': /* Traverse no symlinks-to-directories.  */
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_PHYSICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ਰੋਕੋ;

        ਹਾਲਤ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ਰੋਕੋ;

        ਹਾਲਤ NO_PRESERVE_ਜੜ੍ਹ:
          preserve_ਜੜ੍ਹ = false;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_ਜੜ੍ਹ:
          preserve_ਜੜ੍ਹ = true;
          ਰੋਕੋ;

        ਹਾਲਤ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'R':
          recurse = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          /* ignore */
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          verbose = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          specified_user = optarg;
          component_specified = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
          specified_role = optarg;
          component_specified = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          specified_type = optarg;
          component_specified = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          specified_range = optarg;
          component_specified = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (recurse)
    {
      ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ_flags == FTS_PHYSICAL)
        {
          ਜੇ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  ਜੇ (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      ਜੇ (argc <= optind)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ਜੇ (reference_file)
    {
      ਅੱਖਰ *ref_context = ਨਿਰਾ;

      ਜੇ (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  ਨਹੀਂਤਾਂ ਜੇ (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ
    {
      specified_context = argv[optind++];
      ਜੇ (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (EXIT_FAILURE, errno, _("invalid context: %s"),
               quote (specified_context));
    }

  ਜੇ (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (recurse && preserve_ਜੜ੍ਹ)
    {
      ਸਥਿਰ ਬਣਤਰ dev_ino dev_ino_buf;
      ਜੜ੍ਹ_dev_ino = get_ਜੜ੍ਹ_dev_ino (&dev_ino_buf);
      ਜੇ (ਜੜ੍ਹ_dev_ino == ਨਿਰਾ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੜ੍ਹ_dev_ino = ਨਿਰਾ;
    }

  ok = process_files (argv + optind, ਦੋਆਧਾਰੀ_ਅੰਕ_flags | FTS_NOSTAT);

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
