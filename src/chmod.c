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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "dev-ino.h"
#অন্তর্ভাৱ "filemode.h"
#অন্তর্ভাৱ "fts-missing.h"
#অন্তর্ভাৱ "ignore-value.h"
#অন্তর্ভাৱ "modechange.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "root-dev-ino.h"
#অন্তর্ভাৱ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

সংরচনা change_status
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
  V_উপৰেৰ,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
স্থির সংরচনা mode_change *change;

/* The initial umask value, if it might be needed.  */
স্থির mode_t umask_মান;

/* If true, change the modes of directories recursively. */
স্থির bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
স্থির পূর্ণাঙ্ক dereference = -1;

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
স্থির সংরচনা dev_ino *মূল_dev_ino;

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
  {"changes", no_argument, রিক্ত, 'c'},
  {"dereference", no_argument, রিক্ত, DEREFERENCE_OPTION},
  {"recursive", no_argument, রিক্ত, 'R'},
  {"no-dereference", no_argument, রিক্ত, 'h'},
  {"no-preserve-root", no_argument, রিক্ত, NO_PRESERVE_মূল},
  {"preserve-root", no_argument, রিক্ত, PRESERVE_মূল},
  {"quiet", no_argument, রিক্ত, 'f'},
  {"reference", required_argument, রিক্ত, REFERENCE_FILE_OPTION},
  {"silent", no_argument, রিক্ত, 'f'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

স্থির bool
mode_changed (পূর্ণাঙ্ক dir_fd, ৱর্ণ ধ্রুৱ *file, ৱর্ণ ধ্রুৱ *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  যদি (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      সংরচনা stat new_stats;

      যদি (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          যদি (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          ঘূৰাই false;
        }

      new_mode = new_stats.st_mode;
    }

  ঘূৰাই ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

স্থির শূন্য
describe_change (ৱর্ণ ধ্রুৱ *file, সংরচনা change_status ধ্রুৱ *ch)
{
  ৱর্ণ perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  ৱর্ণ old_perms[12];
  ৱর্ণ ধ্রুৱ *fmt;
  ৱর্ণ ধ্রুৱ *quoted_file = quoteaf (file);

  ৱিকল্প (ch->status)
    {
    পক্ষ CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      ঘূৰাই;

    পক্ষ CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      ঘূৰাই;

    পক্ষ CH_FAILED: পক্ষ CH_NO_CHANGE_REQUESTED: পক্ষ CH_SUCCEEDED:
      ভঙ্গ;
  }

  unsigned long পূর্ণাঙ্ক
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  ৱিকল্প (ch->status)
    {
    পক্ষ CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      ভঙ্গ;
    পক্ষ CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      ভঙ্গ;
    পক্ষ CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      ঘূৰাই;
    পক্ষ CH_NO_STAT: পক্ষ CH_NOT_APPLIED: স্ৱভাৱ:
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
  ৱর্ণ ধ্রুৱ *file_full_name = ent->fts_path;
  ৱর্ণ ধ্রুৱ *file = ent->fts_accpath;
  ধ্রুৱ সংরচনা stat *file_stats = ent->fts_statp;
  সংরচনা change_status ch = {0};
  ch.status = CH_NO_STAT;
  সংরচনা stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    ঘূৰাই true;

  ৱিকল্প (ent->fts_info)
    {
    পক্ষ FTS_DP:
      ঘূৰাই true;

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
      যদি (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ভঙ্গ;

    পক্ষ FTS_ERR:
      যদি (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ভঙ্গ;

    পক্ষ FTS_DNR:
      যদি (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ভঙ্গ;

    পক্ষ FTS_SLNONE:
      যদি (dereference)
        {
          যদি (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          ভঙ্গ;
        }
      ch.status = CH_NOT_APPLIED;
      ভঙ্গ;

    পক্ষ FTS_SL:
      যদি (dereference == 1)
        {
          যদি (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              যদি (ignore_missing
                  && ignorable_traversal_errno (errno))
                ঘূৰাই true;
              যদি (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              ভঙ্গ;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      ভঙ্গ;

    পক্ষ FTS_DC:		/* directory that causes cycles */
      যদি (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ঘূৰাই false;
        }
      FALLTHROUGH;
    স্ৱভাৱ:
      ch.status = CH_NOT_APPLIED;
      ভঙ্গ;
    }

  যদি (ch.status == CH_NOT_APPLIED
      && মূল_DEV_INO_CHECK (মূল_dev_ino, file_stats))
    {
      মূল_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_মান (fts_read (fts));
      ঘূৰাই false;
    }

  যদি (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_মান, change, রিক্ত);
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
            ঘূৰাই true;
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
            ঘূৰাই true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      যদি (ch.status == CH_SUCCEEDED || verbosity == V_উপৰেৰ)
        describe_change (file_full_name, &ch);
    }

  যদি (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, রিক্ত);
      যদি (ch.new_mode & ~naively_expected_mode)
        {
          ৱর্ণ new_perms[12];
          ৱর্ণ naively_expected_perms[12];
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

  ঘূৰাই CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
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
              যদি (! force_silent)
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
         output a diagnostic বাবে every file processed\n\
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
         কৰা not treat '/' specially (the স্ৱভাৱ)\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ৱর্ণ *mode = রিক্ত;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_মূল = false;
  ৱর্ণ ধ্রুৱ *reference_file = রিক্ত;
  পূর্ণাঙ্ক c;
  পূর্ণাঙ্ক bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, রিক্ত))
         != -1)
    {
      ৱিকল্প (c)
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

        পক্ষ 'r':
        পক্ষ 'w':
        পক্ষ 'x':
        পক্ষ 'X':
        পক্ষ 's':
        পক্ষ 't':
        পক্ষ 'u':
        পক্ষ 'g':
        পক্ষ 'o':
        পক্ষ 'a':
        পক্ষ ',':
        পক্ষ '+':
        পক্ষ '=':
        পক্ষ '0': পক্ষ '1': পক্ষ '2': পক্ষ '3':
        পক্ষ '4': পক্ষ '5': পক্ষ '6': পক্ষ '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            ৱর্ণ ধ্রুৱ *arg = argv[optind - 1];
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
        পক্ষ 'c':
          verbosity = V_changes_only;
          ভঙ্গ;
        পক্ষ 'f':
          force_silent = true;
          ভঙ্গ;
        পক্ষ 'v':
          verbosity = V_উপৰেৰ;
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

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
