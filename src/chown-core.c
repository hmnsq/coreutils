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

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

ಗಣನಾ RCH_status
  {
    /* we called fchown and close, and both succeeded */
    RC_ok = 2,

    /* required_uid and/or required_gid are specified, but don't match */
    RC_excluded,

    /* The file was replaced by another file during the requested change.  */
    RC_inode_changed,

    /* open/fchown isn't needed, isn't safe, or doesn't work due to
       permissions problems; fall back on chown */
    RC_do_ordinary_chown,

    /* open, fstat, fchown, or close failed */
    RC_error
  };

ಬಾಹ್ಯ ಶೂನ್ಯ
chopt_init (ಸಂರಚನಾ Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->ಬೇರು_dev_ino = ರಿಕ್ತ;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ರಿಕ್ತ;
  chopt->group_name = ರಿಕ್ತ;
}

ಬಾಹ್ಯ ಶೂನ್ಯ
chopt_free (ಸಂರಚನಾ Chown_option *chopt)
{
  ಮುಕ್ತ (chopt->user_name);
  ಮುಕ್ತ (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ಸ್ಥಿರ ವರ್ಣ *
uid_to_str (uid_t uid)
{
  ವರ್ಣ buf[INT_BUFSIZE_BOUND (intmax_t)];
  ಪ್ರತ್ಯಾಗಮ xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ಸ್ಥಿರ ವರ್ಣ *
gid_to_str (gid_t gid)
{
  ವರ್ಣ buf[INT_BUFSIZE_BOUND (intmax_t)];
  ಪ್ರತ್ಯಾಗಮ xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

ಬಾಹ್ಯ ವರ್ಣ *
gid_to_name (gid_t gid)
{
  ಸಂರಚನಾ group *grp = getgrgid (gid);
  ಪ್ರತ್ಯಾಗಮ grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

ಬಾಹ್ಯ ವರ್ಣ *
uid_to_name (uid_t uid)
{
  ಸಂರಚನಾ passwd *pwd = getpwuid (uid);
  ಪ್ರತ್ಯಾಗಮ pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

ಸ್ಥಿರ ವರ್ಣ *
user_group_str (ವರ್ಣ ಧ್ರುವ *user, ವರ್ಣ ಧ್ರುವ *group)
{
  ವರ್ಣ *spec = ರಿಕ್ತ;

  ಯದಿ (user)
    {
      ಯದಿ (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      ಅನ್ಯಥಾ
        {
          spec = xstrdup (user);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (group)
    {
      spec = xstrdup (group);
    }

  ಪ್ರತ್ಯಾಗಮ spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

ಸ್ಥಿರ ಶೂನ್ಯ
describe_change (ವರ್ಣ ಧ್ರುವ *file, ಗಣನಾ Change_status changed,
                 ವರ್ಣ ಧ್ರುವ *old_user, ವರ್ಣ ಧ್ರುವ *old_group,
                 ವರ್ಣ ಧ್ರುವ *user, ವರ್ಣ ಧ್ರುವ *group)
{
  ಯದಿ (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      ಪ್ರತ್ಯಾಗಮ;
    }

  ವರ್ಣ *spec = user_group_str (user, group);
  ವರ್ಣ *old_spec = user_group_str (user ? old_user : ರಿಕ್ತ,
                                   group ? old_group : ರಿಕ್ತ);

  ವರ್ಣ ಧ್ರುವ *fmt;
  ವಿಕಲ್ಪ (changed)
    {
    ಪಕ್ಷ CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      ಭಙ್ಗ;
    ಪಕ್ಷ CH_FAILED:
      ಯದಿ (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      ಅನ್ಯಥಾ
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          ಮುಕ್ತ (old_spec);
          old_spec = spec;
          spec = ರಿಕ್ತ;
        }
      ಭಙ್ಗ;
    ಪಕ್ಷ CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      ಭಙ್ಗ;
    ಪಕ್ಷ CH_NOT_APPLIED:
    ಸ್ವಭಾವ:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  ಮುಕ್ತ (old_spec);
  ಮುಕ್ತ (spec);
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

ಸ್ಥಿರ ಗಣನಾ RCH_status
restricted_chown (ಪೂರ್ಣಾಙ್ಕ cwd_fd, ವರ್ಣ ಧ್ರುವ *file,
                  ಸಂರಚನಾ stat ಧ್ರುವ *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  ಯದಿ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    ಪ್ರತ್ಯಾಗಮ RC_do_ordinary_chown;

  ಪೂರ್ಣಾಙ್ಕ open_flags = O_NONBLOCK | O_NOCTTY;
  ಯದಿ (! S_ISREG (orig_st->st_mode))
    {
      ಯದಿ (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ RC_do_ordinary_chown;
    }

  ಪೂರ್ಣಾಙ್ಕ fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  ಯದಿ (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    ಪ್ರತ್ಯಾಗಮ (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ಗಣನಾ RCH_status status = RC_ok;
  ಸಂರಚನಾ stat st;
  ಯದಿ (fstat (fd, &st) != 0)
    status = RC_error;
  ಅನ್ಯಥಾ ಯದಿ (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  ಅನ್ಯಥಾ ಯದಿ ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#ಯದಿ HAVE_FCHOWN
      ಯದಿ (fchown (fd, uid, gid) == 0)
        ಪ್ರತ್ಯಾಗಮ close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
  close (fd);
  errno = saved_errno;
  ಪ್ರತ್ಯಾಗಮ status;
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
                   ಸಂರಚನಾ Chown_option ಧ್ರುವ *chopt)
{
  ವರ್ಣ ಧ್ರುವ *file_full_name = ent->fts_path;
  ವರ್ಣ ಧ್ರುವ *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ಯದಿ (ignore_missing && ignorable_fts_error (ent))
    ಪ್ರತ್ಯಾಗಮ true;

  ವಿಕಲ್ಪ (ent->fts_info)
    {
    ಪಕ್ಷ FTS_D:
      ಯದಿ (chopt->recurse)
        {
          ಯದಿ (ಬೇರು_DEV_INO_CHECK (chopt->ಬೇರು_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ಬೇರು_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ಮೌಲ್ಯ (fts_read (fts));
              ಪ್ರತ್ಯಾಗಮ false;
            }
          ಪ್ರತ್ಯಾಗಮ true;
        }
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DP:
      ಯದಿ (! chopt->recurse)
        ಪ್ರತ್ಯಾಗಮ true;
      ಭಙ್ಗ;

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
      ಯದಿ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_ERR:
      ಯದಿ (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DNR:
      ಯದಿ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      ಭಙ್ಗ;

    ಪಕ್ಷ FTS_DC:		/* directory that causes cycles */
      ಯದಿ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ಪ್ರತ್ಯಾಗಮ false;
        }
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಭಙ್ಗ;
    }

  bool do_chown;
  ಸಂರಚನಾ stat stat_buf;
  ಸಂರಚನಾ stat ಧ್ರುವ *file_stats;
  ಯದಿ (!ok)
    {
      do_chown = false;
      file_stats = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ ಯದಿ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->ಬೇರು_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  ಅನ್ಯಥಾ
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      ಯದಿ (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          ಯದಿ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ಯದಿ (ignore_missing
                  && ignorable_traversal_errno (errno))
                ಪ್ರತ್ಯಾಗಮ true;
              ಯದಿ (! chopt->force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
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
  ಯದಿ (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ಬೇರು_DEV_INO_CHECK (chopt->ಬೇರು_dev_ino, file_stats))
    {
      ಬೇರು_DEV_INO_WARN (file_full_name);
      ಪ್ರತ್ಯಾಗಮ false;
    }

  bool symlink_changed = true;
  ಯದಿ (do_chown)
    {
      ಯದಿ ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          ಯದಿ (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      ಅನ್ಯಥಾ
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

          ಗಣನಾ RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          ವಿಕಲ್ಪ (err)
            {
            ಪಕ್ಷ RC_ok:
              ಭಙ್ಗ;

            ಪಕ್ಷ RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              ಭಙ್ಗ;

            ಪಕ್ಷ RC_error:
              ok = false;
              ಭಙ್ಗ;

            ಪಕ್ಷ RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            ಪಕ್ಷ RC_excluded:
              do_chown = false;
              ok = false;
              ಭಙ್ಗ;

            ಸ್ವಭಾವ:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      ಯದಿ (do_chown && !ok)
        {
          ಯದಿ (ignore_missing
              && ignorable_traversal_errno (errno))
            ಪ್ರತ್ಯಾಗಮ true;
          ಯದಿ (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  ಯದಿ (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      ಯದಿ (changed || chopt->verbosity == V_ಮೇಲಿನ)
        {
          ಗಣನಾ Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          ವರ್ಣ *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : ರಿಕ್ತ);
          ವರ್ಣ *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : ರಿಕ್ತ);
          ವರ್ಣ *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : ರಿಕ್ತ;
          ವರ್ಣ *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : ರಿಕ್ತ;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          ಮುಕ್ತ (old_usr);
          ಮುಕ್ತ (old_grp);
          ಯದಿ (new_usr != chopt->user_name)
            ಮುಕ್ತ (new_usr);
          ಯದಿ (new_grp != chopt->group_name)
            ಮುಕ್ತ (new_grp);
        }
    }

  ಯದಿ ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  ಪ್ರತ್ಯಾಗಮ ok;
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
chown_files (ವರ್ಣ **files, ಪೂರ್ಣಾಙ್ಕ bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             ಸಂರಚನಾ Chown_option ಧ್ರುವ *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  ಪೂರ್ಣಾಙ್ಕ stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, ರಿಕ್ತ);

  ಯಾವತ್ (true)
    {
      FTSENT *ent = fts_read (fts);

      ಯದಿ (ent == ರಿಕ್ತ)
        {
          ಯದಿ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ಯದಿ (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ಭಙ್ಗ;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  ಯದಿ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ಪ್ರತ್ಯಾಗಮ ok;
}
