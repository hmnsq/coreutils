/* chmod -- change permission modes of files
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu> */

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "dev-ino.h"
#অন্তর্ভাব "filemode.h"
#অন্তর্ভাব "fts-missing.h"
#অন্তর্ভাব "ignore-value.h"
#অন্তর্ভাব "modechange.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "root-dev-ino.h"
#অন্তর্ভাব "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

কাঠামো change_status
{
  গণনা
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    status;
  mode_t old_mode;
  mode_t new_mode;
};

গণনা Verbosity
{
  /* Print a message for each file that is processed.  */
  V_উপরের,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
স্থির কাঠামো mode_change *change;

/* The initial umask value, if it might be needed.  */
স্থির mode_t umask_মান;

/* If true, change the modes of directories recursively. */
স্থির bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
স্থির পূর্ণসংখ্যা dereference = -1;

/* If true, force silence (suppress most of error messages). */
স্থির bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
স্থির bool diagnose_surprises;

/* Level of verbosity.  */
স্থির গণনা Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
স্থির কাঠামো dev_ino *মূল_dev_ino;

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
  {"changes", no_argument, ফাঁকা, 'c'},
  {"dereference", no_argument, ফাঁকা, DEREFERENCE_OPTION},
  {"recursive", no_argument, ফাঁকা, 'R'},
  {"no-dereference", no_argument, ফাঁকা, 'h'},
  {"no-preserve-root", no_argument, ফাঁকা, NO_PRESERVE_মূল},
  {"preserve-root", no_argument, ফাঁকা, PRESERVE_মূল},
  {"quiet", no_argument, ফাঁকা, 'f'},
  {"reference", required_argument, ফাঁকা, REFERENCE_FILE_OPTION},
  {"silent", no_argument, ফাঁকা, 'f'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

স্থির bool
mode_changed (পূর্ণসংখ্যা dir_fd, অক্ষর ধ্রুবক *file, অক্ষর ধ্রুবক *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  যদি (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      কাঠামো stat new_stats;

      যদি (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          যদি (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          ফেরত false;
        }

      new_mode = new_stats.st_mode;
    }

  ফেরত ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

স্থির শূন্য
describe_change (অক্ষর ধ্রুবক *file, কাঠামো change_status ধ্রুবক *ch)
{
  অক্ষর perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  অক্ষর old_perms[12];
  অক্ষর ধ্রুবক *fmt;
  অক্ষর ধ্রুবক *quoted_file = quoteaf (file);

  নির্বাচন (ch->status)
    {
    ক্ষেত্র CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      ফেরত;

    ক্ষেত্র CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      ফেরত;

    ক্ষেত্র CH_FAILED: ক্ষেত্র CH_NO_CHANGE_REQUESTED: ক্ষেত্র CH_SUCCEEDED:
      বিরতি;
  }

  unsigned long পূর্ণসংখ্যা
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  নির্বাচন (ch->status)
    {
    ক্ষেত্র CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      বিরতি;
    ক্ষেত্র CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      বিরতি;
    ক্ষেত্র CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      ফেরত;
    ক্ষেত্র CH_NO_STAT: ক্ষেত্র CH_NOT_APPLIED: পূর্বনির্ধারিত:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

স্থির bool
process_file (FTS *fts, FTSENT *ent)
{
  অক্ষর ধ্রুবক *file_full_name = ent->fts_path;
  অক্ষর ধ্রুবক *file = ent->fts_accpath;
  ধ্রুবক কাঠামো stat *file_stats = ent->fts_statp;
  কাঠামো change_status ch = {0};
  ch.status = CH_NO_STAT;
  কাঠামো stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    ফেরত true;

  নির্বাচন (ent->fts_info)
    {
    ক্ষেত্র FTS_DP:
      ফেরত true;

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
      যদি (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      বিরতি;

    ক্ষেত্র FTS_ERR:
      যদি (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      বিরতি;

    ক্ষেত্র FTS_DNR:
      যদি (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      বিরতি;

    ক্ষেত্র FTS_SLNONE:
      যদি (dereference)
        {
          যদি (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          বিরতি;
        }
      ch.status = CH_NOT_APPLIED;
      বিরতি;

    ক্ষেত্র FTS_SL:
      যদি (dereference == 1)
        {
          যদি (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              যদি (ignore_missing
                  && ignorable_traversal_errno (errno))
                ফেরত true;
              যদি (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              বিরতি;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      বিরতি;

    ক্ষেত্র FTS_DC:		/* directory that causes cycles */
      যদি (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ফেরত false;
        }
      FALLTHROUGH;
    পূর্বনির্ধারিত:
      ch.status = CH_NOT_APPLIED;
      বিরতি;
    }

  যদি (ch.status == CH_NOT_APPLIED
      && মূল_DEV_INO_CHECK (মূল_dev_ino, file_stats))
    {
      মূল_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_মান (fts_read (fts));
      ফেরত false;
    }

  যদি (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_মান, change, ফাঁকা);
      bool follow_symlink = !!dereference;
      যদি (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      যদি (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      অন্যথা
        {
          যদি (ignore_missing
              && ignorable_traversal_errno (errno))
            ফেরত true;
          যদি (! is_ENOTSUP (errno))
            {
              যদি (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  যদি (verbosity != V_off)
    {
      bool file_missing;
      যদি (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          যদি (file_missing)
            ফেরত true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      যদি (ch.status == CH_SUCCEEDED || verbosity == V_উপরের)
        describe_change (file_full_name, &ch);
    }

  যদি (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, ফাঁকা);
      যদি (ch.new_mode & ~naively_expected_mode)
        {
          অক্ষর new_perms[12];
          অক্ষর naively_expected_perms[12];
          strmode (ch.new_mode, new_perms);
          strmode (naively_expected_mode, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_full_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.status = CH_FAILED;
        }
    }

  যদি ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ফেরত CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
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
              যদি (! force_silent)
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
Usage: %s [OPTION]... MODE[,MODE]... FILE...\n\
  or:  %s [OPTION]... OCTAL-MODE FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Change the mode of each FILE to MODE.\n\
With --reference, change the mode of each FILE to that of RFILE.\n\
\n\
"), stdout);
      oputs (_("\
  -c, --changes\n\
         like verbose but report only when a change is made\n\
"));
      oputs (_("\
  -f, --silent, --quiet\n\
         suppress most error messages\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic জন্য every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
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
         use RFILE's mode instead of specifying MODE মানসমূহ.\n\
         RFILE is always dereferenced যদি a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each MODE is of the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  অক্ষর *mode = ফাঁকা;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_মূল = false;
  অক্ষর ধ্রুবক *reference_file = ফাঁকা;
  পূর্ণসংখ্যা c;
  পূর্ণসংখ্যা bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ফাঁকা))
         != -1)
    {
      নির্বাচন (c)
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

        ক্ষেত্র 'r':
        ক্ষেত্র 'w':
        ক্ষেত্র 'x':
        ক্ষেত্র 'X':
        ক্ষেত্র 's':
        ক্ষেত্র 't':
        ক্ষেত্র 'u':
        ক্ষেত্র 'g':
        ক্ষেত্র 'o':
        ক্ষেত্র 'a':
        ক্ষেত্র ',':
        ক্ষেত্র '+':
        ক্ষেত্র '=':
        ক্ষেত্র '0': ক্ষেত্র '1': ক্ষেত্র '2': ক্ষেত্র '3':
        ক্ষেত্র '4': ক্ষেত্র '5': ক্ষেত্র '6': ক্ষেত্র '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            অক্ষর ধ্রুবক *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            যদি (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
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
        ক্ষেত্র 'c':
          verbosity = V_changes_only;
          বিরতি;
        ক্ষেত্র 'f':
          force_silent = true;
          বিরতি;
        ক্ষেত্র 'v':
          verbosity = V_উপরের;
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
          dereference = 0;
        }
    }

  যদি (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  যদি (reference_file)
    {
      যদি (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  অন্যথা
    {
      যদি (!mode)
        mode = argv[optind++];
    }

  যদি (optind >= argc)
    {
      যদি (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  যদি (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      যদি (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  অন্যথা
    {
      change = mode_compile (mode);
      যদি (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_মান = umask (0);
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

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
