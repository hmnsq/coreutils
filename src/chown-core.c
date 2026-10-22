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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include "system.h"
#include "assure.h"
#include "chown-core.h"
#include "fts-missing.h"
#include "ignore-value.h"
#include "root-dev-ino.h"
#include "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

ga_nanaa RCH_status
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

extern void
chopt_init (sa_nracanaa Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->root_dev_ino = NULL;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = NULL;
  chopt->group_name = NULL;
}

extern void
chopt_free (sa_nracanaa Chown_option *chopt)
{
  free (chopt->user_name);
  free (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

static var_na *
uid_to_str (uid_t uid)
{
  var_na buf[INT_BUFSIZE_BOUND (intmax_t)];
  คืน xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

static var_na *
gid_to_str (gid_t gid)
{
  var_na buf[INT_BUFSIZE_BOUND (intmax_t)];
  คืน xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

extern var_na *
gid_to_name (gid_t gid)
{
  sa_nracanaa group *grp = getgrgid (gid);
  คืน grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

extern var_na *
uid_to_name (uid_t uid)
{
  sa_nracanaa passwd *pwd = getpwuid (uid);
  คืน pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

static var_na *
user_group_str (var_na const *user, var_na const *group)
{
  var_na *spec = NULL;

  ถ้า (user)
    {
      ถ้า (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      มิฉะนั้น
        {
          spec = xstrdup (user);
        }
    }
  มิฉะนั้น ถ้า (group)
    {
      spec = xstrdup (group);
    }

  คืน spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

static void
describe_change (var_na const *file, ga_nanaa Change_status changed,
                 var_na const *old_user, var_na const *old_group,
                 var_na const *user, var_na const *group)
{
  ถ้า (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      คืน;
    }

  var_na *spec = user_group_str (user, group);
  var_na *old_spec = user_group_str (user ? old_user : NULL,
                                   group ? old_group : NULL);

  var_na const *fmt;
  switch (changed)
    {
    pak_sa CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      bha_nga;
    pak_sa CH_FAILED:
      ถ้า (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      มิฉะนั้น
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          free (old_spec);
          old_spec = spec;
          spec = NULL;
        }
      bha_nga;
    pak_sa CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      bha_nga;
    pak_sa CH_NOT_APPLIED:
    default:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  free (old_spec);
  free (spec);
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

static ga_nanaa RCH_status
restricted_chown (จำนวนเต็ม cwd_fd, var_na const *file,
                  sa_nracanaa stat const *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  ถ้า (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    คืน RC_do_ordinary_chown;

  จำนวนเต็ม open_flags = O_NONBLOCK | O_NOCTTY;
  ถ้า (! S_ISREG (orig_st->st_mode))
    {
      ถ้า (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      มิฉะนั้น
        คืน RC_do_ordinary_chown;
    }

  จำนวนเต็ม fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  ถ้า (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    คืน (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ga_nanaa RCH_status status = RC_ok;
  sa_nracanaa stat st;
  ถ้า (fstat (fd, &st) != 0)
    status = RC_error;
  มิฉะนั้น ถ้า (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  มิฉะนั้น ถ้า ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#ถ้า HAVE_FCHOWN
      ถ้า (fchown (fd, uid, gid) == 0)
        คืน close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  จำนวนเต็ม saved_errno = errno;
  close (fd);
  errno = saved_errno;
  คืน status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
static bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   sa_nracanaa Chown_option const *chopt)
{
  var_na const *file_full_name = ent->fts_path;
  var_na const *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ถ้า (ignore_missing && ignorable_fts_error (ent))
    คืน true;

  switch (ent->fts_info)
    {
    pak_sa FTS_D:
      ถ้า (chopt->recurse)
        {
          ถ้า (ROOT_DEV_INO_CHECK (chopt->root_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ROOT_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_value (fts_read (fts));
              คืน false;
            }
          คืน true;
        }
      bha_nga;

    pak_sa FTS_DP:
      ถ้า (! chopt->recurse)
        คืน true;
      bha_nga;

    pak_sa FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ถ้า (ent->fts_level == FTS_ROOTLEVEL && ent->fts_number == 0)
        {
          ent->fts_number = 1;
          fts_set (fts, ent, FTS_AGAIN);
          คืน true;
        }
      ถ้า (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_ERR:
      ถ้า (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_DNR:
      ถ้า (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_DC:		/* directory that causes cycles */
      ถ้า (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          คืน false;
        }
      bha_nga;

    default:
      bha_nga;
    }

  bool do_chown;
  sa_nracanaa stat stat_buf;
  sa_nracanaa stat const *file_stats;
  ถ้า (!ok)
    {
      do_chown = false;
      file_stats = NULL;
    }
  มิฉะนั้น ถ้า (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->root_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  มิฉะนั้น
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      ถ้า (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          ถ้า (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ถ้า (ignore_missing
                  && ignorable_traversal_errno (errno))
                คืน true;
              ถ้า (! chopt->force_silent)
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
  ถ้า (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ROOT_DEV_INO_CHECK (chopt->root_dev_ino, file_stats))
    {
      ROOT_DEV_INO_WARN (file_full_name);
      คืน false;
    }

  bool symlink_changed = true;
  ถ้า (do_chown)
    {
      ถ้า ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          ถ้า (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      มิฉะนั้น
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

          ga_nanaa RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          switch (err)
            {
            pak_sa RC_ok:
              bha_nga;

            pak_sa RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              bha_nga;

            pak_sa RC_error:
              ok = false;
              bha_nga;

            pak_sa RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            pak_sa RC_excluded:
              do_chown = false;
              ok = false;
              bha_nga;

            default:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      ถ้า (do_chown && !ok)
        {
          ถ้า (ignore_missing
              && ignorable_traversal_errno (errno))
            คืน true;
          ถ้า (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  ถ้า (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      ถ้า (changed || chopt->verbosity == V_high)
        {
          ga_nanaa Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          var_na *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : NULL);
          var_na *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : NULL);
          var_na *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : NULL;
          var_na *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : NULL;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          free (old_usr);
          free (old_grp);
          ถ้า (new_usr != chopt->user_name)
            free (new_usr);
          ถ้า (new_grp != chopt->group_name)
            free (new_grp);
        }
    }

  ถ้า ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  คืน ok;
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
extern bool
chown_files (var_na **files, จำนวนเต็ม bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             sa_nracanaa Chown_option const *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  จำนวนเต็ม stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, NULL);

  ขณะ (true)
    {
      FTSENT *ent = fts_read (fts);

      ถ้า (ent == NULL)
        {
          ถ้า (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ถ้า (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          bha_nga;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  ถ้า (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  คืน ok;
}
