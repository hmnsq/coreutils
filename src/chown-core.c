/* chown-core.c -- core functions for changing ownership.
   Copyright (C) 2000-2026 Free Software Foundation, Inc.

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

/* Extracted from chown.c/chgrp.c and librarified by Jim Meyering.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "chown-core.h"
#ಅನ್ತರ್ಭಾವ "fts-missing.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"

#ವ್ಯಾಖ್ಯಾನಿಸು FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

ಎಣಿಕೆ RCH_ಸ್ಥಿತಿ
  {
    /* we called fchown and close, and both succeeded */
    RC_ok = 2,

    /* required_uid and/or required_gid are specified, but don't match */
    RC_excluded,

    /* The file was replaced by another file during the requested change.  */
    RC_inode_ಬದಲಾಯಿತು,

    /* open/fchown isn't needed, isn't safe, or doesn't work due to
       permissions problems; fall back on chown */
    RC_do_ordinary_chown,

    /* open, fstat, fchown, or close failed */
    RC_error
  };

ಬಾಹ್ಯ ಶೂನ್ಯ
chopt_init (ರಚನೆ Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->ಬೇರು_dev_ino = ಖಾಲಿ;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ಖಾಲಿ;
  chopt->ಗುಂಪು_name = ಖಾಲಿ;
}

ಬಾಹ್ಯ ಶೂನ್ಯ
chopt_free (ರಚನೆ Chown_option *chopt)
{
  ಬಿಡುಗಡೆ (chopt->user_name);
  ಬಿಡುಗಡೆ (chopt->ಗುಂಪು_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
uid_ಗಮ್ಯ_str (uid_t uid)
{
  ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (intmax_t)];
  ಹಿಂತಿರುಗಿ xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
gid_ಗಮ್ಯ_str (gid_t gid)
{
  ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (intmax_t)];
  ಹಿಂತಿರುಗಿ xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

ಬಾಹ್ಯ ಅಕ್ಷರ *
gid_ಗಮ್ಯ_name (gid_t gid)
{
  ರಚನೆ ಗುಂಪು *grp = getgrgid (gid);
  ಹಿಂತಿರುಗಿ grp ? xstrdup (grp->gr_name) : gid_ಗಮ್ಯ_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

ಬಾಹ್ಯ ಅಕ್ಷರ *
uid_ಗಮ್ಯ_name (uid_t uid)
{
  ರಚನೆ passwd *pwd = getpwuid (uid);
  ಹಿಂತಿರುಗಿ pwd ? xstrdup (pwd->pw_name) : uid_ಗಮ್ಯ_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
user_ಗುಂಪು_str (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *user, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುಂಪು)
{
  ಅಕ್ಷರ *spec = ಖಾಲಿ;

  ಆದರೆ (user)
    {
      ಆದರೆ (ಗುಂಪು)
        {
          spec = xmalloc (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (user) + 1 + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಗುಂಪು) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), ಗುಂಪು);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          spec = xstrdup (user);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗುಂಪು)
    {
      spec = xstrdup (ಗುಂಪು);
    }

  ಹಿಂತಿರುಗಿ spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

ಸ್ಥಿರ ಶೂನ್ಯ
describe_change (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಎಣಿಕೆ Change_ಸ್ಥಿತಿ ಬದಲಾಯಿತು,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *old_user, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *old_ಗುಂಪು,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *user, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುಂಪು)
{
  ಆದರೆ (ಬದಲಾಯಿತು == CH_NOT_APPLIED)
    {
      ಬರೆ (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      ಹಿಂತಿರುಗಿ;
    }

  ಅಕ್ಷರ *spec = user_ಗುಂಪು_str (user, ಗುಂಪು);
  ಅಕ್ಷರ *old_spec = user_ಗುಂಪು_str (user ? old_user : ಖಾಲಿ,
                                   ಗುಂಪು ? old_ಗುಂಪು : ಖಾಲಿ);

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt;
  ಆಯ್ಕೆ (ಬದಲಾಯಿತು)
    {
    ಸ್ಥಿತಿ CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : ಗುಂಪು ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CH_FAILED:
      ಆದರೆ (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : ಗುಂಪು ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : ಗುಂಪು ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          ಬಿಡುಗಡೆ (old_spec);
          old_spec = spec;
          spec = ಖಾಲಿ;
        }
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : ಗುಂಪು ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CH_NOT_APPLIED:
    ಪೂರ್ವನಿಯೋಜಿತ:
      affirm (false);
    }

  ಬರೆ (fmt, quoteaf (file), old_spec, spec);

  ಬಿಡುಗಡೆ (old_spec);
  ಬಿಡುಗಡೆ (spec);
}

/* Change the owner and/or group of the FILE to UID and/or GID (safely)
   only if REQUIRED_UID and REQUIRED_GID match the owner and group IDs
   of FILE.  ORIG_ST must be the result of 'stat'ing FILE.

   The 'safely' part above means that we can't simply use chown(2),
   since FILE might be replaced with some other file between the time
   of the preceding stat/lstat and this chown call.  So here we open
   FILE and do everything else via the resulting file descriptor.
   We first call fstat and verify that the dev/inode match those from
   the preceding stat call, and only then, if appropriate (given the
   required_uid and required_gid constraints) do we call fchown.

   Return RC_do_ordinary_chown if we can't open FILE, or if FILE is a
   special file that might have undesirable side effects when opening.
   In this case the caller can use the less-safe ordinary chown.

   Return one of the RCH_status values.  */

ಸ್ಥಿರ ಎಣಿಕೆ RCH_ಸ್ಥಿತಿ
restricted_chown (ಪೂರ್ಣಾಂಕ cwd_fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file,
                  ರಚನೆ stat ಸ್ಥಿರಾಂಕ *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  ಆದರೆ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    ಹಿಂತಿರುಗಿ RC_do_ordinary_chown;

  ಪೂರ್ಣಾಂಕ open_flags = O_NONBLOCK | O_NOCTTY;
  ಆದರೆ (! S_ISREG (orig_st->st_ಬಹುಲಕ))
    {
      ಆದರೆ (S_ISDIR (orig_st->st_ಬಹುಲಕ))
        open_flags |= O_DIRECTORY;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ RC_do_ordinary_chown;
    }

  ಪೂರ್ಣಾಂಕ fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  ಆದರೆ (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_ಬಹುಲಕ)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    ಹಿಂತಿರುಗಿ (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ಎಣಿಕೆ RCH_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = RC_ok;
  ರಚನೆ stat st;
  ಆದರೆ (fstat (fd, &st) != 0)
    ಸ್ಥಿತಿ_2 = RC_error;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! psame_inode (orig_st, &st))
    ಸ್ಥಿತಿ_2 = RC_inode_ಬದಲಾಯಿತು;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#ಆದರೆ HAVE_FCHOWN
      ಆದರೆ (fchown (fd, uid, gid) == 0)
        ಹಿಂತಿರುಗಿ close (fd) < 0 ? RC_error : RC_ok;
#ಆದರೆ_ಅಂತ್ಯ
      ಸ್ಥಿತಿ_2 = RC_error;
    }

  ಪೂರ್ಣಾಂಕ saved_errno = errno;
  close (fd);
  errno = saved_errno;
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
ಸ್ಥಿರ bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   ರಚನೆ Chown_option ಸ್ಥಿರಾಂಕ *chopt)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_ತುಂಬಿದ_name = ent->fts_ಮಾರ್ಗ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_ನಮೂದು (ent);

  ಆದರೆ (ignore_missing && ignorable_fts_error (ent))
    ಹಿಂತಿರುಗಿ true;

  ಆಯ್ಕೆ (ent->fts_info)
    {
    ಸ್ಥಿತಿ FTS_D:
      ಆದರೆ (chopt->recurse)
        {
          ಆದರೆ (ಬೇರು_DEV_INO_CHECK (chopt->ಬೇರು_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ಬೇರು_DEV_INO_WARN (file_ತುಂಬಿದ_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_ಗಣ (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ಮೌಲ್ಯ (fts_read (fts));
              ಹಿಂತಿರುಗಿ false;
            }
          ಹಿಂತಿರುಗಿ true;
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DP:
      ಆದರೆ (! chopt->recurse)
        ಹಿಂತಿರುಗಿ true;
      ನಿಲ್ಲಿಸು;

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
      ಆದರೆ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_ತುಂಬಿದ_name));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_ERR:
      ಆದರೆ (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_ತುಂಬಿದ_name));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DNR:
      ಆದರೆ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_ತುಂಬಿದ_name));
      ok = false;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ FTS_DC:		/* directory that causes cycles */
      ಆದರೆ (ಚಕ್ರ_warning_required (fts, ent))
        {
          emit_ಚಕ್ರ_warning (file_ತುಂಬಿದ_name);
          ಹಿಂತಿರುಗಿ false;
        }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ನಿಲ್ಲಿಸು;
    }

  bool do_chown;
  ರಚನೆ stat stat_buf;
  ರಚನೆ stat ಸ್ಥಿರಾಂಕ *file_stats;
  ಆದರೆ (!ok)
    {
      do_chown = false;
      file_stats = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->ಬೇರು_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      ಆದರೆ (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_ಬಹುಲಕ))
        {
          ಆದರೆ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ಆದರೆ (ignore_missing
                  && ignorable_traversal_errno (errno))
                ಹಿಂತಿರುಗಿ true;
              ಆದರೆ (! chopt->force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_ತುಂಬಿದ_name));
              ok = false;
            }

          file_stats = &stat_buf;
        }

      do_chown = (ok
                  && (required_uid == (uid_t) -1
                      || required_uid == file_stats->st_uid)
                  && (required_gid == (gid_t) -1
                      || required_gid == file_stats->st_gid));
    }

  /* This happens when chown -LR --preserve-root encounters a symlink-to-/.  */
  ಆದರೆ (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ಬೇರು_DEV_INO_CHECK (chopt->ಬೇರು_dev_ino, file_stats))
    {
      ಬೇರು_DEV_INO_WARN (file_ತುಂಬಿದ_name);
      ಹಿಂತಿರುಗಿ false;
    }

  bool symlink_ಬದಲಾಯಿತು = true;
  ಆದರೆ (do_chown)
    {
      ಆದರೆ ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          ಆದರೆ (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_ಬದಲಾಯಿತು = false;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* If possible, avoid a race condition with --from=O:G and without the
             (-h) --no-dereference option.  If fts's stat call determined
             that the uid/gid of FILE matched the --from=O:G-selected
             owner and group IDs, blindly using chown(2) here could lead
             chown(1) or chgrp(1) mistakenly to dereference a *symlink*
             to an arbitrary file that an attacker had moved into the
             place of FILE during the window between the stat and
             chown(2) calls.  If FILE is a regular file or a directory
             that can be opened, this race condition can be avoided safely.  */

          ಎಣಿಕೆ RCH_ಸ್ಥಿತಿ err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          ಆಯ್ಕೆ (err)
            {
            ಸ್ಥಿತಿ RC_ok:
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ RC_error:
              ok = false;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ RC_inode_ಬದಲಾಯಿತು:
              /* FIXME: give a diagnostic in this case?  */
            ಸ್ಥಿತಿ RC_excluded:
              do_chown = false;
              ok = false;
              ನಿಲ್ಲಿಸು;

            ಪೂರ್ವನಿಯೋಜಿತ:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      ಆದರೆ (do_chown && !ok)
        {
          ಆದರೆ (ignore_missing
              && ignorable_traversal_errno (errno))
            ಹಿಂತಿರುಗಿ true;
          ಆದರೆ (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_ತುಂಬಿದ_name));
        }
    }

  ಆದರೆ (chopt->verbosity != V_off)
    {
      bool ಬದಲಾಯಿತು =
        ((do_chown && ok && symlink_ಬದಲಾಯಿತು)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      ಆದರೆ (ಬದಲಾಯಿತು || chopt->verbosity == V_ಮೇಲಿನ)
        {
          ಎಣಿಕೆ Change_ಸ್ಥಿತಿ ch_ಸ್ಥಿತಿ =
            (!ok ? CH_FAILED
             : !symlink_ಬದಲಾಯಿತು ? CH_NOT_APPLIED
             : !ಬದಲಾಯಿತು ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          ಅಕ್ಷರ *old_usr = (file_stats
                           ? uid_ಗಮ್ಯ_name (file_stats->st_uid) : ಖಾಲಿ);
          ಅಕ್ಷರ *old_grp = (file_stats
                           ? gid_ಗಮ್ಯ_name (file_stats->st_gid) : ಖಾಲಿ);
          ಅಕ್ಷರ *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_ಗಮ್ಯ_str (uid) : ಖಾಲಿ;
          ಅಕ್ಷರ *new_grp = chopt->ಗುಂಪು_name
                          ? chopt->ಗುಂಪು_name : gid != -1
                                               ? gid_ಗಮ್ಯ_str (gid) : ಖಾಲಿ;
          describe_change (file_ತುಂಬಿದ_name, ch_ಸ್ಥಿತಿ,
                           old_usr, old_grp,
                           new_usr, new_grp);
          ಬಿಡುಗಡೆ (old_usr);
          ಬಿಡುಗಡೆ (old_grp);
          ಆದರೆ (new_usr != chopt->user_name)
            ಬಿಡುಗಡೆ (new_usr);
          ಆದರೆ (new_grp != chopt->ಗುಂಪು_name)
            ಬಿಡುಗಡೆ (new_grp);
        }
    }

  ಆದರೆ ( ! chopt->recurse)
    fts_ಗಣ (fts, ent, FTS_SKIP);

  ಹಿಂತಿರುಗಿ ok;
}

/* Change the owner and/or group of the specified FILES.
   BIT_FLAGS specifies how to treat each symlink-to-directory
   that is encountered during a recursive traversal.
   CHOPT specifies additional options.
   If UID is not -1, then change the owner id of each file to UID.
   If GID is not -1, then change the group id of each file to GID.
   If REQUIRED_UID and/or REQUIRED_GID is not -1, then change only
   files with user ID and group ID that match the non-(-1) value(s).
   Return true if successful.  */
ಬಾಹ್ಯ bool
chown_files (ಅಕ್ಷರ **files, ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             ರಚನೆ Chown_option ಸ್ಥಿರಾಂಕ *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  ಪೂರ್ಣಾಂಕ stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, ಬಿಟ್_flags | stat_flags, ಖಾಲಿ);

  ಆಗ (true)
    {
      FTSENT *ent = fts_read (fts);

      ಆದರೆ (ent == ಖಾಲಿ)
        {
          ಆದರೆ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ಆದರೆ (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  ಆದರೆ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ಹಿಂತಿರುಗಿ ok;
}
