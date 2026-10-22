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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "dev-ino.h"
#ಅನ್ತರ್ಭಾವ "filemode.h"
#ಅನ್ತರ್ಭಾವ "fts-missing.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "modechange.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ಸಂರಚನಾ change_status
{
  ಗಣನಾ
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

ಗಣನಾ Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ಮೇಲಿನ,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
ಸ್ಥಿರ ಸಂರಚನಾ mode_change *change;

/* The initial umask value, if it might be needed.  */
ಸ್ಥಿರ mode_t umask_ಮೌಲ್ಯ;

/* If true, change the modes of directories recursively. */
ಸ್ಥಿರ bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ dereference = -1;

/* If true, force silence (suppress most of error messages). */
ಸ್ಥಿರ bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
ಸ್ಥಿರ bool diagnose_surprises;

/* Level of verbosity.  */
ಸ್ಥಿರ ಗಣನಾ Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ಸ್ಥಿರ ಸಂರಚನಾ dev_ino *ಬೇರು_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  REFERENCE_FILE_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"changes", no_argument, ರಿಕ್ತ, 'c'},
  {"dereference", no_argument, ರಿಕ್ತ, DEREFERENCE_OPTION},
  {"recursive", no_argument, ರಿಕ್ತ, 'R'},
  {"no-dereference", no_argument, ರಿಕ್ತ, 'h'},
  {"no-preserve-root", no_argument, ರಿಕ್ತ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", no_argument, ರಿಕ್ತ, PRESERVE_ಬೇರು},
  {"quiet", no_argument, ರಿಕ್ತ, 'f'},
  {"reference", required_argument, ರಿಕ್ತ, REFERENCE_FILE_OPTION},
  {"silent", no_argument, ರಿಕ್ತ, 'f'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

ಸ್ಥಿರ bool
mode_changed (ಪೂರ್ಣಾಙ್ಕ dir_fd, ವರ್ಣ ಧ್ರುವ *file, ವರ್ಣ ಧ್ರುವ *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  ಯದಿ (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      ಸಂರಚನಾ stat new_stats;

      ಯದಿ (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          ಯದಿ (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          ಪ್ರತ್ಯಾಗಮ false;
        }

      new_mode = new_stats.st_mode;
    }

  ಪ್ರತ್ಯಾಗಮ ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

ಸ್ಥಿರ ಶೂನ್ಯ
describe_change (ವರ್ಣ ಧ್ರುವ *file, ಸಂರಚನಾ change_status ಧ್ರುವ *ch)
{
  ವರ್ಣ perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  ವರ್ಣ old_perms[12];
  ವರ್ಣ ಧ್ರುವ *fmt;
  ವರ್ಣ ಧ್ರುವ *quoted_file = quoteaf (file);

  ವಿಕಲ್ಪ (ch->status)
    {
    ಪಕ್ಷ CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      ಪ್ರತ್ಯಾಗಮ;

    ಪಕ್ಷ CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      ಪ್ರತ್ಯಾಗಮ;

    ಪಕ್ಷ CH_FAILED: ಪಕ್ಷ CH_NO_CHANGE_REQUESTED: ಪಕ್ಷ CH_SUCCEEDED:
      ಭಙ್ಗ;
  }

  unsigned long ಪೂರ್ಣಾಙ್ಕ
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  ವಿಕಲ್ಪ (ch->status)
    {
    ಪಕ್ಷ CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      ಭಙ್ಗ;
    ಪಕ್ಷ CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      ಭಙ್ಗ;
    ಪಕ್ಷ CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ CH_NO_STAT: ಪಕ್ಷ CH_NOT_APPLIED: ಸ್ವಭಾವ:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ಸ್ಥಿರ bool
process_file (FTS *fts, FTSENT *ent)
{
  ವರ್ಣ ಧ್ರುವ *file_full_name = ent->fts_path;
  ವರ್ಣ ಧ್ರುವ *file = ent->fts_accpath;
  ಧ್ರುವ ಸಂರಚನಾ stat *file_stats = ent->fts_statp;
  ಸಂರಚನಾ change_status ch = {0};
  ch.status = CH_NO_STAT;
  ಸಂರಚನಾ stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ಯದಿ (ignore_missing && ignorable_fts_error (ent))
    ಪ್ರತ್ಯಾಗಮ true;

  ವಿಕಲ್ಪ (ent->fts_info)
    {
    ಪಕ್ಷ FTS_DP:
      ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ಯದಿ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ಸಂಖ್ಯೆ == 0)
        {
          ent->fts_ಸಂಖ್ಯೆ = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ಪ್ರತ್ಯಾಗಮ true;
        }
      ಯದಿ (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_ERR:
      ಯದಿ (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DNR:
      ಯದಿ (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_SLNONE:
      ಯದಿ (dereference)
        {
          ಯದಿ (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          ಭಙ್ಗ;
        }
      ch.status = CH_NOT_APPLIED;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_SL:
      ಯದಿ (dereference == 1)
        {
          ಯದಿ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ಯದಿ (ignore_missing
                  && ignorable_traversal_errno (errno))
                ಪ್ರತ್ಯಾಗಮ true;
              ಯದಿ (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              ಭಙ್ಗ;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DC:		/* directory that causes cycles */
      ಯದಿ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ಪ್ರತ್ಯಾಗಮ false;
        }
      FALLTHROUGH;
    ಸ್ವಭಾವ:
      ch.status = CH_NOT_APPLIED;
      ಭಙ್ಗ;
    }

  ಯದಿ (ch.status == CH_NOT_APPLIED
      && ಬೇರು_DEV_INO_CHECK (ಬೇರು_dev_ino, file_stats))
    {
      ಬೇರು_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_ಮೌಲ್ಯ (fts_read (fts));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_ಮೌಲ್ಯ, change, ರಿಕ್ತ);
      bool follow_symlink = !!dereference;
      ಯದಿ (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      ಯದಿ (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (ignore_missing
              && ignorable_traversal_errno (errno))
            ಪ್ರತ್ಯಾಗಮ true;
          ಯದಿ (! is_ENOTSUP (errno))
            {
              ಯದಿ (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  ಯದಿ (verbosity != V_off)
    {
      bool file_missing;
      ಯದಿ (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          ಯದಿ (file_missing)
            ಪ್ರತ್ಯಾಗಮ true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      ಯದಿ (ch.status == CH_SUCCEEDED || verbosity == V_ಮೇಲಿನ)
        describe_change (file_full_name, &ch);
    }

  ಯದಿ (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, ರಿಕ್ತ);
      ಯದಿ (ch.new_mode & ~naively_expected_mode)
        {
          ವರ್ಣ new_perms[12];
          ವರ್ಣ naively_expected_perms[12];
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

  ಯದಿ ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ಪ್ರತ್ಯಾಗಮ CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ಸ್ಥಿರ bool
process_files (ವರ್ಣ **files, ಪೂರ್ಣಾಙ್ಕ bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ರಿಕ್ತ);

  ಯಾವತ್ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ಯದಿ (ent == ರಿಕ್ತ)
        {
          ಯದಿ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ಯದಿ (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ಭಙ್ಗ;
        }

      ok &= process_file (fts, ent);
    }

  ಯದಿ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
         output a diagnostic ಪ್ರತಿ every file processed\n\
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
      --no-preserve-ಬೇರು\n\
         ಕುರು not treat '/' specially (the ಸ್ವಭಾವ)\n\
"));
      oputs (_("\
      --preserve-ಬೇರು\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE ಮೌಲ್ಯಗಳು.\n\
         RFILE is always dereferenced ಯದಿ a symbolic link.\n\
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ *mode = ರಿಕ್ತ;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_ಬೇರು = false;
  ವರ್ಣ ಧ್ರುವ *reference_file = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ c;
  ಪೂರ್ಣಾಙ್ಕ bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {

        ಪಕ್ಷ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
        ಪಕ್ಷ 'w':
        ಪಕ್ಷ 'x':
        ಪಕ್ಷ 'X':
        ಪಕ್ಷ 's':
        ಪಕ್ಷ 't':
        ಪಕ್ಷ 'u':
        ಪಕ್ಷ 'g':
        ಪಕ್ಷ 'o':
        ಪಕ್ಷ 'a':
        ಪಕ್ಷ ',':
        ಪಕ್ಷ '+':
        ಪಕ್ಷ '=':
        ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3':
        ಪಕ್ಷ '4': ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            ವರ್ಣ ಧ್ರುವ *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            ಯದಿ (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          ಭಙ್ಗ;
        ಪಕ್ಷ NO_PRESERVE_ಬೇರು:
          preserve_ಬೇರು = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ PRESERVE_ಬೇರು:
          preserve_ಬೇರು = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'R':
          recurse = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'c':
          verbosity = V_changes_only;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'f':
          force_silent = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          verbosity = V_ಮೇಲಿನ;
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (recurse)
    {
      ಯದಿ (bit_flags == FTS_PHYSICAL)
        {
          ಯದಿ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  ಯದಿ (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  ಯದಿ (reference_file)
    {
      ಯದಿ (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (!mode)
        mode = argv[optind++];
    }

  ಯದಿ (optind >= argc)
    {
      ಯದಿ (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      ಯದಿ (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  ಅನ್ಯಥಾ
    {
      change = mode_compile (mode);
      ಯದಿ (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_ಮೌಲ್ಯ = umask (0);
    }

  ಯದಿ (recurse && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ಸಂರಚನಾ dev_ino dev_ino_buf;
      ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಯದಿ (ಬೇರು_dev_ino == ರಿಕ್ತ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ಅನ್ಯಥಾ
    {
      ಬೇರು_dev_ino = ರಿಕ್ತ;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
