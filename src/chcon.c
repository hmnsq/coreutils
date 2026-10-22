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
#تعریف_کرو پروگرام_NAME "chcon"

#تعریف_کرو AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
ساکن bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
ساکن bool recurse;

/* Level of verbosity. */
ساکن bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ساکن ساخت dev_ino *جڑ_dev_ino;

/* The name of the context file is being given. */
ساکن حرف ثابت *specified_context;

/* Specific components of the context */
ساکن حرف ثابت *specified_user;
ساکن حرف ثابت *specified_role;
ساکن حرف ثابت *specified_range;
ساکن حرف ثابت *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_جڑ,
  PRESERVE_جڑ,
  REFERENCE_FILE_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"recursive", no_دلیل, تہی, 'R'},
  {"dereference", no_دلیل, تہی, DEREFERENCE_OPTION},
  {"no-dereference", no_دلیل, تہی, 'h'},
  {"no-preserve-root", no_دلیل, تہی, NO_PRESERVE_جڑ},
  {"preserve-root", no_دلیل, تہی, PRESERVE_جڑ},
  {"reference", required_دلیل, تہی, REFERENCE_FILE_OPTION},
  {"user", required_دلیل, تہی, 'u'},
  {"role", required_دلیل, تہی, 'r'},
  {"type", required_دلیل, تہی, 't'},
  {"range", required_دلیل, تہی, 'l'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
ساکن صحیح
compute_context_from_نقاب (حرف ثابت *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  اگر (!new_context)
    {
      error (0, errno, _("failed to create security context: %s"),
             quote (context));
      واپس 1;
    }

#تعریف_کرو سیٹ_COMPONENT(C, comp)						\
   کرو									\
     {									\
       اگر (specified_ ## comp						\
           && context_ ## comp ## سیٹ ((C), specified_ ## comp))	\
         {								\
            error (0, errno,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   جب (0)

  سیٹ_COMPONENT (new_context, user);
  سیٹ_COMPONENT (new_context, range);
  سیٹ_COMPONENT (new_context, role);
  سیٹ_COMPONENT (new_context, type);

  اگر (!ok)
    {
      صحیح saved_errno = errno;
      context_free (new_context);
      errno = saved_errno;
      واپس 1;
    }

  *ret = new_context;
  واپس 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

ساکن صحیح
change_file_context (صحیح fd, حرف ثابت *file, bool ignore_missing)
{
  حرف *file_context = تہی;
  context_t context IF_LINT (= تہی);
  حرف ثابت * context_string;
  صحیح errors = 0;

  اگر (specified_context == تہی)
    {
      صحیح حالت_2 = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      اگر (حالت_2 < 0 && errno != ENODATA)
        {
          اگر (ignore_missing && ignorable_traversal_errno (errno))
            واپس 0;
          error (0, errno, _("failed to get security context of %s"),
                 quoteaf (file));
          واپس 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      اگر (file_context == تہی)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          واپس 1;
        }

      اگر (compute_context_from_نقاب (file_context, &context))
        {
          freecon (file_context);
          واپس 1;
        }

      context_string = context_str (context);
    }
  ورنہ
    {
      context_string = specified_context;
    }

  اگر (file_context == تہی || ! streq (context_string, file_context))
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

  اگر (specified_context == تہی)
    {
      context_free (context);
      freecon (file_context);
    }

  واپس errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ساکن bool
process_file (FTS *fts, FTSENT *ent)
{
  حرف ثابت *file_بھرا_name = ent->fts_راستہ;
  حرف ثابت *file = ent->fts_accpath;
  ثابت ساخت stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_اندراج (ent);

  اگر (ignore_missing && ignorable_fts_error (ent))
    واپس true;

  انتخاب (ent->fts_info)
    {
    حالت FTS_D:
      اگر (recurse)
        {
          اگر (جڑ_DEV_INO_CHECK (جڑ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              جڑ_DEV_INO_WARN (file_بھرا_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_سیٹ (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_قدر (fts_read (fts));
              واپس false;
            }
          واپس true;
        }
      توڑو;

    حالت FTS_DP:
      اگر (! recurse)
        واپس true;
      توڑو;

    حالت FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      اگر (ent->fts_level == FTS_ROOTLEVEL && ent->fts_نمبر == 0)
        {
          ent->fts_نمبر = 1;
          fts_سیٹ (fts, ent, FTS_AGAIN);
          واپس true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_بھرا_name));
      ok = false;
      توڑو;

    حالت FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_بھرا_name));
      ok = false;
      توڑو;

    حالت FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_بھرا_name));
      ok = false;
      توڑو;

    حالت FTS_DC:		/* directory that causes cycles */
      اگر (چکر_warning_required (fts, ent))
        {
          emit_چکر_warning (file_بھرا_name);
          واپس false;
        }
      توڑو;

    طےشدہ:
      توڑو;
    }

  اگر (ent->fts_info == FTS_DP
      && ok && جڑ_DEV_INO_CHECK (جڑ_dev_ino, file_stats))
    {
      جڑ_DEV_INO_WARN (file_بھرا_name);
      ok = false;
    }

  اگر (ok)
    {
      اگر (verbose)
        لکھو (_("changing security context of %s\n"),
                quoteaf (file_بھرا_name));

      اگر (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  اگر ( ! recurse)
    fts_سیٹ (fts, ent, FTS_SKIP);

  واپس ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساکن bool
process_files (حرف **files, صحیح ثنائی_ہندسہ_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, ثنائی_ہندسہ_flags, تہی);

  جب (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      اگر (ent == تہی)
        {
          اگر (errno != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, errno, _("fts_read failed"));
              ok = false;
            }
          توڑو;
        }

      ok &= process_file (fts, ent);
    }

  اگر (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  واپس ok;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... CONTEXT FILE...\n\
  or:  %s [OPTION]... [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
        پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Change the SELinux security context کا each FILE مقصد CONTEXT.\n\
With --reference, change the security context کا each FILE مقصد that کا RFILE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --dereference\n\
         affect the referent کا each symbolic link (this is\n\
         the طےشدہ), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead کا any referenced file\n\
"));
      oputs (_("\
  -u, --user=USER\n\
         سیٹ_2 user USER اندر the ہدف security context\n\
"));
      oputs (_("\
  -r, --role=ROLE\n\
         سیٹ_2 role ROLE اندر the ہدف security context\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         سیٹ_2 type TYPE اندر the ہدف security context\n\
"));
      oputs (_("\
  -l, --range=RANGE\n\
         سیٹ_2 range RANGE اندر the ہدف security context\n\
"));
      oputs (_("\
      --no-preserve-جڑ\n\
         کرو not treat '/' specially (the طےشدہ)\n\
"));
      oputs (_("\
      --preserve-جڑ\n\
         fail مقصد عمل_کرو recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's security context rather than specifying a CONTEXT قدر\n\
"));
      oputs (_("\
  -R, --recursive\n\
         عمل_کرو on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic برائے every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  /* Bit flags that control how fts works.  */
  صحیح ثنائی_ہندسہ_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  صحیح dereference = -1;

  bool ok;
  bool preserve_جڑ = false;
  bool component_specified = false;
  حرف *reference_file = تہی;
  صحیح optc;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, تہی))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'H': /* Traverse command-line symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          توڑو;

        حالت 'L': /* Traverse all symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_LOGICAL;
          توڑو;

        حالت 'P': /* Traverse no symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_PHYSICAL;
          توڑو;

        حالت 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          توڑو;

        حالت DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          توڑو;

        حالت NO_PRESERVE_جڑ:
          preserve_جڑ = false;
          توڑو;

        حالت PRESERVE_جڑ:
          preserve_جڑ = true;
          توڑو;

        حالت REFERENCE_FILE_OPTION:
          reference_file = optarg;
          توڑو;

        حالت 'R':
          recurse = true;
          توڑو;

        حالت 'f':
          /* ignore */
          توڑو;

        حالت 'v':
          verbose = true;
          توڑو;

        حالت 'u':
          specified_user = optarg;
          component_specified = true;
          توڑو;

        حالت 'r':
          specified_role = optarg;
          component_specified = true;
          توڑو;

        حالت 't':
          specified_type = optarg;
          component_specified = true;
          توڑو;

        حالت 'l':
          specified_range = optarg;
          component_specified = true;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (recurse)
    {
      اگر (ثنائی_ہندسہ_flags == FTS_PHYSICAL)
        {
          اگر (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      ورنہ
        {
          اگر (dereference == 0)
            error (EXIT_FAILURE, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  ورنہ
    {
      ثنائی_ہندسہ_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  اگر (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      ورنہ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  اگر (reference_file)
    {
      حرف *ref_context = تہی;

      اگر (getfilecon (reference_file, &ref_context) < 0)
        error (EXIT_FAILURE, errno, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  ورنہ اگر (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = تہی;
    }
  ورنہ
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

  اگر (recurse && preserve_جڑ)
    {
      ساکن ساخت dev_ino dev_ino_buf;
      جڑ_dev_ino = get_جڑ_dev_ino (&dev_ino_buf);
      اگر (جڑ_dev_ino == تہی)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ورنہ
    {
      جڑ_dev_ino = تہی;
    }

  ok = process_files (argv + optind, ثنائی_ہندسہ_flags | FTS_NOSTAT);

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
