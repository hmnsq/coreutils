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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "chmod"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ರಚನೆ change_ಸ್ಥಿತಿ
{
  ಎಣಿಕೆ
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    ಸ್ಥಿತಿ_2;
  mode_t old_ಬಹುಲಕ;
  mode_t new_ಬಹುಲಕ;
};

ಎಣಿಕೆ Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ಮೇಲಿನ,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
ಸ್ಥಿರ ರಚನೆ ಬಹುಲಕ_change *change;

/* The initial umask value, if it might be needed.  */
ಸ್ಥಿರ mode_t umask_ಮೌಲ್ಯ;

/* If true, change the modes of directories recursively. */
ಸ್ಥಿರ bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ dereference = -1;

/* If true, force silence (suppress most of error messages). */
ಸ್ಥಿರ bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
ಸ್ಥಿರ bool diagnose_surprises;

/* Level of verbosity.  */
ಸ್ಥಿರ ಎಣಿಕೆ Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ಸ್ಥಿರ ರಚನೆ dev_ino *ಬೇರು_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ಬೇರು,
  PRESERVE_ಬೇರು,
  REFERENCE_FILE_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"changes", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"dereference", no_ವಾದ, ಖಾಲಿ, DEREFERENCE_OPTION},
  {"recursive", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"no-preserve-root", no_ವಾದ, ಖಾಲಿ, NO_PRESERVE_ಬೇರು},
  {"preserve-root", no_ವಾದ, ಖಾಲಿ, PRESERVE_ಬೇರು},
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"reference", required_ವಾದ, ಖಾಲಿ, REFERENCE_FILE_OPTION},
  {"silent", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

ಸ್ಥಿರ bool
ಬಹುಲಕ_ಬದಲಾಯಿತು (ಪೂರ್ಣಾಂಕ dir_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_ತುಂಬಿದ_name,
              mode_t old_ಬಹುಲಕ, mode_t new_ಬಹುಲಕ, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  ಆದರೆ (new_ಬಹುಲಕ & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      ರಚನೆ stat new_stats;

      ಆದರೆ (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          ಆದರೆ (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_ತುಂಬಿದ_name));
          ಹಿಂತಿರುಗಿ false;
        }

      new_ಬಹುಲಕ = new_stats.st_ಬಹುಲಕ;
    }

  ಹಿಂತಿರುಗಿ ((old_ಬಹುಲಕ ^ new_ಬಹುಲಕ) & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

ಸ್ಥಿರ ಶೂನ್ಯ
describe_change (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ change_ಸ್ಥಿತಿ ಸ್ಥಿರಾಂಕ *ch)
{
  ಅಕ್ಷರ perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  ಅಕ್ಷರ old_perms[12];
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *quoted_file = quoteaf (file);

  ಆಯ್ಕೆ (ch->ಸ್ಥಿತಿ_2)
    {
    ಸ್ಥಿತಿ CH_NOT_APPLIED:
      ಬರೆ (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿತಿ CH_NO_STAT:
      ಬರೆ (_("%s could not be accessed\n"), quoted_file);
      ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿತಿ CH_FAILED: ಸ್ಥಿತಿ CH_NO_CHANGE_REQUESTED: ಸ್ಥಿತಿ CH_SUCCEEDED:
      ನಿಲ್ಲಿಸು;
  }

  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ
    old_m = ch->old_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು,
    m = ch->new_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು;

  strmode (ch->new_ಬಹುಲಕ, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_ಬಹುಲಕ, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  ಆಯ್ಕೆ (ch->ಸ್ಥಿತಿ_2)
    {
    ಸ್ಥಿತಿ CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      ಬರೆ (fmt, quoted_file, m, &perms[1]);
      ಹಿಂತಿರುಗಿ;
    ಸ್ಥಿತಿ CH_NO_STAT: ಸ್ಥಿತಿ CH_NOT_APPLIED: ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }
  ಬರೆ (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ಸ್ಥಿರ bool
process_file (FTS *fts, FTSENT *ent)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_ತುಂಬಿದ_name = ent->fts_ಮಾರ್ಗ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = ent->fts_accpath;
  ಸ್ಥಿರಾಂಕ ರಚನೆ stat *file_stats = ent->fts_statp;
  ರಚನೆ change_ಸ್ಥಿತಿ ch = {0};
  ch.ಸ್ಥಿತಿ_2 = CH_NO_STAT;
  ರಚನೆ stat stat_buf;
  bool ignore_missing = ignore_missing_fts_ನಮೂದು (ent);

  ಆದರೆ (ignore_missing && ignorable_fts_error (ent))
    ಹಿಂತಿರುಗಿ true;

  ಆಯ್ಕೆ (ent->fts_info)
    {
    ಸ್ಥಿತಿ FTS_DP:
      ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ಆದರೆ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ಸಂಖ್ಯೆ == 0)
        {
          ent->fts_ಸಂಖ್ಯೆ = 1;
          fts_ಗಣ (fts, ent, FTS_AGAIN);
          ಹಿಂತಿರುಗಿ true;
        }
      ಆದರೆ (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_ತುಂಬಿದ_name));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_ERR:
      ಆದರೆ (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_ತುಂಬಿದ_name));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DNR:
      ಆದರೆ (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_ತುಂಬಿದ_name));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_SLNONE:
      ಆದರೆ (dereference)
        {
          ಆದರೆ (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_ತುಂಬಿದ_name));
          ನಿಲ್ಲಿಸು;
        }
      ch.ಸ್ಥಿತಿ_2 = CH_NOT_APPLIED;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_SL:
      ಆದರೆ (dereference == 1)
        {
          ಆದರೆ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ಆದರೆ (ignore_missing
                  && ignorable_traversal_errno (errno))
                ಹಿಂತಿರುಗಿ true;
              ಆದರೆ (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_ತುಂಬಿದ_name));
              ನಿಲ್ಲಿಸು;
            }

          file_stats = &stat_buf;
        }
      ch.ಸ್ಥಿತಿ_2 = CH_NOT_APPLIED;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DC:		/* directory that causes cycles */
      ಆದರೆ (ಚಕ್ರ_warning_required (fts, ent))
        {
          emit_ಚಕ್ರ_warning (file_ತುಂಬಿದ_name);
          ಹಿಂತಿರುಗಿ false;
        }
      FALLTHROUGH;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ch.ಸ್ಥಿತಿ_2 = CH_NOT_APPLIED;
      ನಿಲ್ಲಿಸು;
    }

  ಆದರೆ (ch.ಸ್ಥಿತಿ_2 == CH_NOT_APPLIED
      && ಬೇರು_DEV_INO_CHECK (ಬೇರು_dev_ino, file_stats))
    {
      ಬೇರು_DEV_INO_WARN (file_ತುಂಬಿದ_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_ಗಣ (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_ಮೌಲ್ಯ (fts_read (fts));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (ch.ಸ್ಥಿತಿ_2 == CH_NOT_APPLIED)
    {
      ch.old_ಬಹುಲಕ = file_stats->st_ಬಹುಲಕ;
      ch.new_ಬಹುಲಕ = ಬಹುಲಕ_adjust (ch.old_ಬಹುಲಕ, S_ISDIR (ch.old_ಬಹುಲಕ) != 0,
                                 umask_ಮೌಲ್ಯ, change, ಖಾಲಿ);
      bool follow_symlink = !!dereference;
      ಆದರೆ (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      ಆದರೆ (fchmodat (fts->fts_cwd_fd, file, ch.new_ಬಹುಲಕ,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.ಸ್ಥಿತಿ_2 = CH_SUCCEEDED;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (ignore_missing
              && ignorable_traversal_errno (errno))
            ಹಿಂತಿರುಗಿ true;
          ಆದರೆ (! is_ENOTSUP (errno))
            {
              ಆದರೆ (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_ತುಂಬಿದ_name));

              ch.ಸ್ಥಿತಿ_2 = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  ಆದರೆ (verbosity != V_off)
    {
      bool file_missing;
      ಆದರೆ (ch.ಸ್ಥಿತಿ_2 == CH_SUCCEEDED
          && !ಬಹುಲಕ_ಬದಲಾಯಿತು (fts->fts_cwd_fd, file, file_ತುಂಬಿದ_name,
                            ch.old_ಬಹುಲಕ, ch.new_ಬಹುಲಕ, ignore_missing,
                            &file_missing))
        {
          ಆದರೆ (file_missing)
            ಹಿಂತಿರುಗಿ true;
          ch.ಸ್ಥಿತಿ_2 = CH_NO_CHANGE_REQUESTED;
        }

      ಆದರೆ (ch.ಸ್ಥಿತಿ_2 == CH_SUCCEEDED || verbosity == V_ಮೇಲಿನ)
        describe_change (file_ತುಂಬಿದ_name, &ch);
    }

  ಆದರೆ (CH_NO_CHANGE_REQUESTED <= ch.ಸ್ಥಿತಿ_2 && diagnose_surprises)
    {
      mode_t naively_expected_ಬಹುಲಕ =
        ಬಹುಲಕ_adjust (ch.old_ಬಹುಲಕ, S_ISDIR (ch.old_ಬಹುಲಕ) != 0,
                     0, change, ಖಾಲಿ);
      ಆದರೆ (ch.new_ಬಹುಲಕ & ~naively_expected_ಬಹುಲಕ)
        {
          ಅಕ್ಷರ new_perms[12];
          ಅಕ್ಷರ naively_expected_perms[12];
          strmode (ch.new_ಬಹುಲಕ, new_perms);
          strmode (naively_expected_ಬಹುಲಕ, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_ತುಂಬಿದ_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.ಸ್ಥಿತಿ_2 = CH_FAILED;
        }
    }

  ಆದರೆ ( ! recurse)
    fts_ಗಣ (fts, ent, FTS_SKIP);

  ಹಿಂತಿರುಗಿ CH_NOT_APPLIED <= ch.ಸ್ಥಿತಿ_2;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ಸ್ಥಿರ bool
process_files (ಅಕ್ಷರ **files, ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, ಬಿಟ್_flags, ಖಾಲಿ);

  ಆಗ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ಆದರೆ (ent == ಖಾಲಿ)
        {
          ಆದರೆ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ಆದರೆ (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        }

      ok &= process_file (fts, ent);
    }

  ಆದರೆ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... ಬಹುಲಕ[,ಬಹುಲಕ]... FILE...\n\
  or:  %s [OPTION]... OCTAL-ಬಹುಲಕ FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Change the ಬಹುಲಕ_2 ದ each FILE ಗಮ್ಯ ಬಹುಲಕ.\n\
With --reference, change the ಬಹುಲಕ_2 ದ each FILE ಗಮ್ಯ that ದ RFILE.\n\
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
         output a diagnostic ಗಾಗಿ every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent ದ each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
"));
      oputs (_("\
      --no-preserve-ಬೇರು\n\
         ಮಾಡು not treat '/' specially (the ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
      --preserve-ಬೇರು\n\
         fail ಗಮ್ಯ ನಿರ್ವಹಿಸು recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ಬಹುಲಕ_2 instead ದ specifying ಬಹುಲಕ ಮೌಲ್ಯಗಳು.\n\
         RFILE is always dereferenced ಆದರೆ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each ಬಹುಲಕ is ದ the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ *ಬಹುಲಕ_2 = ಖಾಲಿ;
  idx_t ಬಹುಲಕ_len = 0;
  idx_t ಬಹುಲಕ_alloc = 0;
  bool ok;
  bool preserve_ಬೇರು = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *reference_file = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ c;
  ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {

        ಸ್ಥಿತಿ 'H': /* Traverse command-line symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L': /* Traverse all symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_LOGICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'P': /* Traverse no symlinks-to-directories.  */
          ಬಿಟ್_flags = FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
        ಸ್ಥಿತಿ 'w':
        ಸ್ಥಿತಿ 'x':
        ಸ್ಥಿತಿ 'X':
        ಸ್ಥಿತಿ 's':
        ಸ್ಥಿತಿ 't':
        ಸ್ಥಿತಿ 'u':
        ಸ್ಥಿತಿ 'g':
        ಸ್ಥಿತಿ 'o':
        ಸ್ಥಿತಿ 'a':
        ಸ್ಥಿತಿ ',':
        ಸ್ಥಿತಿ '+':
        ಸ್ಥಿತಿ '=':
        ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3':
        ಸ್ಥಿತಿ '4': ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg = argv[optind - 1];
            idx_t arg_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (arg);
            idx_t ಬಹುಲಕ_comma_len = ಬಹುಲಕ_len + !!ಬಹುಲಕ_len;
            idx_t new_ಬಹುಲಕ_len = ಬಹುಲಕ_comma_len + arg_len;
            assume (0 <= new_ಬಹುಲಕ_len);  /* Pacify GCC bug #109613.  */
            ಆದರೆ (ಬಹುಲಕ_alloc <= new_ಬಹುಲಕ_len)
              ಬಹುಲಕ_2 = xpalloc (ಬಹುಲಕ_2, &ಬಹುಲಕ_alloc,
                              new_ಬಹುಲಕ_len + 1 - ಬಹುಲಕ_alloc, -1, 1);
            ಬಹುಲಕ_2[ಬಹುಲಕ_len] = ',';
            ಸ್ಮರಣೆ_ನಕಲು (ಬಹುಲಕ_2 + ಬಹುಲಕ_comma_len, arg, arg_len + 1);
            ಬಹುಲಕ_len = new_ಬಹುಲಕ_len;

            diagnose_surprises = true;
          }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ NO_PRESERVE_ಬೇರು:
          preserve_ಬೇರು = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ PRESERVE_ಬೇರು:
          preserve_ಬೇರು = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'R':
          recurse = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'c':
          verbosity = V_changes_only;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          force_silent = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          verbosity = V_ಮೇಲಿನ;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (recurse)
    {
      ಆದರೆ (ಬಿಟ್_flags == FTS_PHYSICAL)
        {
          ಆದರೆ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  ಆದರೆ (dereference == -1 && ಬಿಟ್_flags == FTS_LOGICAL)
    dereference = 1;

  ಆದರೆ (reference_file)
    {
      ಆದರೆ (ಬಹುಲಕ_2)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (!ಬಹುಲಕ_2)
        ಬಹುಲಕ_2 = argv[optind++];
    }

  ಆದರೆ (optind >= argc)
    {
      ಆದರೆ (!ಬಹುಲಕ_2 || ಬಹುಲಕ_2 != argv[optind - 1])
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (reference_file)
    {
      change = ಬಹುಲಕ_create_from_ref (reference_file);
      ಆದರೆ (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      change = ಬಹುಲಕ_compile (ಬಹುಲಕ_2);
      ಆದರೆ (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (ಬಹುಲಕ_2));
          usage (EXIT_FAILURE);
        }
      umask_ಮೌಲ್ಯ = umask (0);
    }

  ಆದರೆ (recurse && preserve_ಬೇರು)
    {
      ಸ್ಥಿರ ರಚನೆ dev_ino dev_ino_buf;
      ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);
      ಆದರೆ (ಬೇರು_dev_ino == ಖಾಲಿ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬೇರು_dev_ino = ಖಾಲಿ;
    }

  ಬಿಟ್_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, ಬಿಟ್_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
