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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filemode.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fts-missing.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "modechange.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "chmod"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ਬਣਤਰ change_ਸਥਿਤੀ
{
  ਗਿਣਤੀ
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    ਸਥਿਤੀ;
  mode_t old_ਬਹੁਲਕ;
  mode_t new_ਬਹੁਲਕ;
};

ਗਿਣਤੀ Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ਉੱਚਾ,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
ਸਥਿਰ ਬਣਤਰ ਬਹੁਲਕ_change *change;

/* The initial umask value, if it might be needed.  */
ਸਥਿਰ mode_t umask_ਮੁੱਲ;

/* If true, change the modes of directories recursively. */
ਸਥਿਰ bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
ਸਥਿਰ ਪੂਰਨ dereference = -1;

/* If true, force silence (suppress most of error messages). */
ਸਥਿਰ bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
ਸਥਿਰ bool diagnose_surprises;

/* Level of verbosity.  */
ਸਥਿਰ ਗਿਣਤੀ Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ਸਥਿਰ ਬਣਤਰ dev_ino *ਜੜ੍ਹ_dev_ino;

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
  {"changes", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, DEREFERENCE_OPTION},
  {"recursive", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"no-preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, NO_PRESERVE_ਜੜ੍ਹ},
  {"preserve-root", no_ਦਲੀਲ, ਨਿਰਾ, PRESERVE_ਜੜ੍ਹ},
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"reference", required_ਦਲੀਲ, ਨਿਰਾ, REFERENCE_FILE_OPTION},
  {"silent", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

ਸਥਿਰ bool
ਬਹੁਲਕ_ਬਦਲਿਆ (ਪੂਰਨ dir_fd, ਅੱਖਰ ਅਚਲ *file, ਅੱਖਰ ਅਚਲ *file_ਭਰਿਆ_name,
              mode_t old_ਬਹੁਲਕ, mode_t new_ਬਹੁਲਕ, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  ਜੇ (new_ਬਹੁਲਕ & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      ਬਣਤਰ stat new_stats;

      ਜੇ (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          ਜੇ (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_ਭਰਿਆ_name));
          ਵਾਪਸ false;
        }

      new_ਬਹੁਲਕ = new_stats.st_ਬਹੁਲਕ;
    }

  ਵਾਪਸ ((old_ਬਹੁਲਕ ^ new_ਬਹੁਲਕ) & CHMOD_ਬਹੁਲਕ_ਬਿੱਟ) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

ਸਥਿਰ ਖਾਲੀ
describe_change (ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ change_ਸਥਿਤੀ ਅਚਲ *ch)
{
  ਅੱਖਰ perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  ਅੱਖਰ old_perms[12];
  ਅੱਖਰ ਅਚਲ *fmt;
  ਅੱਖਰ ਅਚਲ *quoted_file = quoteaf (file);

  ਚੋਣ (ch->ਸਥਿਤੀ)
    {
    ਹਾਲਤ CH_NOT_APPLIED:
      ਲਿਖੋ (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      ਵਾਪਸ;

    ਹਾਲਤ CH_NO_STAT:
      ਲਿਖੋ (_("%s could not be accessed\n"), quoted_file);
      ਵਾਪਸ;

    ਹਾਲਤ CH_FAILED: ਹਾਲਤ CH_NO_CHANGE_REQUESTED: ਹਾਲਤ CH_SUCCEEDED:
      ਰੋਕੋ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ
    old_m = ch->old_ਬਹੁਲਕ & CHMOD_ਬਹੁਲਕ_ਬਿੱਟ,
    m = ch->new_ਬਹੁਲਕ & CHMOD_ਬਹੁਲਕ_ਬਿੱਟ;

  strmode (ch->new_ਬਹੁਲਕ, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_ਬਹੁਲਕ, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  ਚੋਣ (ch->ਸਥਿਤੀ)
    {
    ਹਾਲਤ CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      ਰੋਕੋ;
    ਹਾਲਤ CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      ਰੋਕੋ;
    ਹਾਲਤ CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      ਲਿਖੋ (fmt, quoted_file, m, &perms[1]);
      ਵਾਪਸ;
    ਹਾਲਤ CH_NO_STAT: ਹਾਲਤ CH_NOT_APPLIED: ਮੂਲ:
      affirm (false);
    }
  ਲਿਖੋ (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ਸਥਿਰ bool
process_file (FTS *fts, FTSENT *ent)
{
  ਅੱਖਰ ਅਚਲ *file_ਭਰਿਆ_name = ent->fts_ਰਸਤਾ;
  ਅੱਖਰ ਅਚਲ *file = ent->fts_accpath;
  ਅਚਲ ਬਣਤਰ stat *file_stats = ent->fts_statp;
  ਬਣਤਰ change_ਸਥਿਤੀ ch = {0};
  ch.ਸਥਿਤੀ = CH_NO_STAT;
  ਬਣਤਰ stat stat_buf;
  bool ignore_missing = ignore_missing_fts_ਇੰਦਰਾਜ (ent);

  ਜੇ (ignore_missing && ignorable_fts_error (ent))
    ਵਾਪਸ true;

  ਚੋਣ (ent->fts_info)
    {
    ਹਾਲਤ FTS_DP:
      ਵਾਪਸ true;

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
      ਜੇ (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_ਭਰਿਆ_name));
      ਰੋਕੋ;

    ਹਾਲਤ FTS_ERR:
      ਜੇ (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_ਭਰਿਆ_name));
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DNR:
      ਜੇ (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_ਭਰਿਆ_name));
      ਰੋਕੋ;

    ਹਾਲਤ FTS_SLNONE:
      ਜੇ (dereference)
        {
          ਜੇ (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_ਭਰਿਆ_name));
          ਰੋਕੋ;
        }
      ch.ਸਥਿਤੀ = CH_NOT_APPLIED;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_SL:
      ਜੇ (dereference == 1)
        {
          ਜੇ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ਜੇ (ignore_missing
                  && ignorable_traversal_errno (errno))
                ਵਾਪਸ true;
              ਜੇ (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_ਭਰਿਆ_name));
              ਰੋਕੋ;
            }

          file_stats = &stat_buf;
        }
      ch.ਸਥਿਤੀ = CH_NOT_APPLIED;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DC:		/* directory that causes cycles */
      ਜੇ (ਚੱਕਰ_warning_required (fts, ent))
        {
          emit_ਚੱਕਰ_warning (file_ਭਰਿਆ_name);
          ਵਾਪਸ false;
        }
      FALLTHROUGH;
    ਮੂਲ:
      ch.ਸਥਿਤੀ = CH_NOT_APPLIED;
      ਰੋਕੋ;
    }

  ਜੇ (ch.ਸਥਿਤੀ == CH_NOT_APPLIED
      && ਜੜ੍ਹ_DEV_INO_CHECK (ਜੜ੍ਹ_dev_ino, file_stats))
    {
      ਜੜ੍ਹ_DEV_INO_WARN (file_ਭਰਿਆ_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_ਸੈੱਟ (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_ਮੁੱਲ (fts_read (fts));
      ਵਾਪਸ false;
    }

  ਜੇ (ch.ਸਥਿਤੀ == CH_NOT_APPLIED)
    {
      ch.old_ਬਹੁਲਕ = file_stats->st_ਬਹੁਲਕ;
      ch.new_ਬਹੁਲਕ = ਬਹੁਲਕ_adjust (ch.old_ਬਹੁਲਕ, S_ISDIR (ch.old_ਬਹੁਲਕ) != 0,
                                 umask_ਮੁੱਲ, change, ਨਿਰਾ);
      bool follow_symlink = !!dereference;
      ਜੇ (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      ਜੇ (fchmodat (fts->fts_cwd_fd, file, ch.new_ਬਹੁਲਕ,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.ਸਥਿਤੀ = CH_SUCCEEDED;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (ignore_missing
              && ignorable_traversal_errno (errno))
            ਵਾਪਸ true;
          ਜੇ (! is_ENOTSUP (errno))
            {
              ਜੇ (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_ਭਰਿਆ_name));

              ch.ਸਥਿਤੀ = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  ਜੇ (verbosity != V_off)
    {
      bool file_missing;
      ਜੇ (ch.ਸਥਿਤੀ == CH_SUCCEEDED
          && !ਬਹੁਲਕ_ਬਦਲਿਆ (fts->fts_cwd_fd, file, file_ਭਰਿਆ_name,
                            ch.old_ਬਹੁਲਕ, ch.new_ਬਹੁਲਕ, ignore_missing,
                            &file_missing))
        {
          ਜੇ (file_missing)
            ਵਾਪਸ true;
          ch.ਸਥਿਤੀ = CH_NO_CHANGE_REQUESTED;
        }

      ਜੇ (ch.ਸਥਿਤੀ == CH_SUCCEEDED || verbosity == V_ਉੱਚਾ)
        describe_change (file_ਭਰਿਆ_name, &ch);
    }

  ਜੇ (CH_NO_CHANGE_REQUESTED <= ch.ਸਥਿਤੀ && diagnose_surprises)
    {
      mode_t naively_expected_ਬਹੁਲਕ =
        ਬਹੁਲਕ_adjust (ch.old_ਬਹੁਲਕ, S_ISDIR (ch.old_ਬਹੁਲਕ) != 0,
                     0, change, ਨਿਰਾ);
      ਜੇ (ch.new_ਬਹੁਲਕ & ~naively_expected_ਬਹੁਲਕ)
        {
          ਅੱਖਰ new_perms[12];
          ਅੱਖਰ naively_expected_perms[12];
          strmode (ch.new_ਬਹੁਲਕ, new_perms);
          strmode (naively_expected_ਬਹੁਲਕ, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_ਭਰਿਆ_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.ਸਥਿਤੀ = CH_FAILED;
        }
    }

  ਜੇ ( ! recurse)
    fts_ਸੈੱਟ (fts, ent, FTS_SKIP);

  ਵਾਪਸ CH_NOT_APPLIED <= ch.ਸਥਿਤੀ;
}

/* Recursively change the modes of the specified FILES (the last entry
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
              ਜੇ (! force_silent)
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
Usage: %s [OPTION]... ਬਹੁਲਕ[,ਬਹੁਲਕ]... FILE...\n\
  or:  %s [OPTION]... OCTAL-ਬਹੁਲਕ FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Change the ਬਹੁਲਕ_2 ਦਾ each FILE ਮੰਜ਼ਿਲ ਬਹੁਲਕ.\n\
With --reference, change the ਬਹੁਲਕ_2 ਦਾ each FILE ਮੰਜ਼ਿਲ that ਦਾ RFILE.\n\
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
         output a diagnostic ਲਈ every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent ਦਾ each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
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
         use RFILE's ਬਹੁਲਕ_2 instead ਦਾ specifying ਬਹੁਲਕ ਮੁੱਲਾਂ.\n\
         RFILE is always dereferenced ਜੇ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each ਬਹੁਲਕ is ਦਾ the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ *ਬਹੁਲਕ_2 = ਨਿਰਾ;
  idx_t ਬਹੁਲਕ_len = 0;
  idx_t ਬਹੁਲਕ_alloc = 0;
  bool ok;
  bool preserve_ਜੜ੍ਹ = false;
  ਅੱਖਰ ਅਚਲ *reference_file = ਨਿਰਾ;
  ਪੂਰਨ c;
  ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
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

        ਹਾਲਤ 'r':
        ਹਾਲਤ 'w':
        ਹਾਲਤ 'x':
        ਹਾਲਤ 'X':
        ਹਾਲਤ 's':
        ਹਾਲਤ 't':
        ਹਾਲਤ 'u':
        ਹਾਲਤ 'g':
        ਹਾਲਤ 'o':
        ਹਾਲਤ 'a':
        ਹਾਲਤ ',':
        ਹਾਲਤ '+':
        ਹਾਲਤ '=':
        ਹਾਲਤ '0': ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3':
        ਹਾਲਤ '4': ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            ਅੱਖਰ ਅਚਲ *arg = argv[optind - 1];
            idx_t arg_len = ਲੜੀ_ਲੰਬਾਈ (arg);
            idx_t ਬਹੁਲਕ_comma_len = ਬਹੁਲਕ_len + !!ਬਹੁਲਕ_len;
            idx_t new_ਬਹੁਲਕ_len = ਬਹੁਲਕ_comma_len + arg_len;
            assume (0 <= new_ਬਹੁਲਕ_len);  /* Pacify GCC bug #109613.  */
            ਜੇ (ਬਹੁਲਕ_alloc <= new_ਬਹੁਲਕ_len)
              ਬਹੁਲਕ_2 = xpalloc (ਬਹੁਲਕ_2, &ਬਹੁਲਕ_alloc,
                              new_ਬਹੁਲਕ_len + 1 - ਬਹੁਲਕ_alloc, -1, 1);
            ਬਹੁਲਕ_2[ਬਹੁਲਕ_len] = ',';
            ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਬਹੁਲਕ_2 + ਬਹੁਲਕ_comma_len, arg, arg_len + 1);
            ਬਹੁਲਕ_len = new_ਬਹੁਲਕ_len;

            diagnose_surprises = true;
          }
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
        ਹਾਲਤ 'c':
          verbosity = V_changes_only;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          force_silent = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          verbosity = V_ਉੱਚਾ;
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
          dereference = 0;
        }
    }

  ਜੇ (dereference == -1 && ਦੋਆਧਾਰੀ_ਅੰਕ_flags == FTS_LOGICAL)
    dereference = 1;

  ਜੇ (reference_file)
    {
      ਜੇ (ਬਹੁਲਕ_2)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (!ਬਹੁਲਕ_2)
        ਬਹੁਲਕ_2 = argv[optind++];
    }

  ਜੇ (optind >= argc)
    {
      ਜੇ (!ਬਹੁਲਕ_2 || ਬਹੁਲਕ_2 != argv[optind - 1])
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ਜੇ (reference_file)
    {
      change = ਬਹੁਲਕ_create_from_ref (reference_file);
      ਜੇ (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  ਨਹੀਂਤਾਂ
    {
      change = ਬਹੁਲਕ_compile (ਬਹੁਲਕ_2);
      ਜੇ (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (ਬਹੁਲਕ_2));
          usage (EXIT_FAILURE);
        }
      umask_ਮੁੱਲ = umask (0);
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

  ਦੋਆਧਾਰੀ_ਅੰਕ_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, ਦੋਆਧਾਰੀ_ਅੰਕ_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
