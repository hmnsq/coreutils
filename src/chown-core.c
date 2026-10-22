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
  ආපසු xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

static var_na *
gid_to_str (gid_t gid)
{
  var_na buf[INT_BUFSIZE_BOUND (intmax_t)];
  ආපසු xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

extern var_na *
gid_to_name (gid_t gid)
{
  sa_nracanaa group *grp = getgrgid (gid);
  ආපසු grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

extern var_na *
uid_to_name (uid_t uid)
{
  sa_nracanaa passwd *pwd = getpwuid (uid);
  ආපසු pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

static var_na *
user_group_str (var_na const *user, var_na const *group)
{
  var_na *spec = NULL;

  නම් (user)
    {
      නම් (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      නැතහොත්
        {
          spec = xstrdup (user);
        }
    }
  නැතහොත් නම් (group)
    {
      spec = xstrdup (group);
    }

  ආපසු spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

static void
describe_change (var_na const *file, ga_nanaa Change_status changed,
                 var_na const *old_user, var_na const *old_group,
                 var_na const *user, var_na const *group)
{
  නම් (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      ආපසු;
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
      නම් (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      නැතහොත්
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
restricted_chown (පූර්ණ_සංඛ්‍යා cwd_fd, var_na const *file,
                  sa_nracanaa stat const *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  නම් (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    ආපසු RC_do_ordinary_chown;

  පූර්ණ_සංඛ්‍යා open_flags = O_NONBLOCK | O_NOCTTY;
  නම් (! S_ISREG (orig_st->st_mode))
    {
      නම් (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      නැතහොත්
        ආපසු RC_do_ordinary_chown;
    }

  පූර්ණ_සංඛ්‍යා fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  නම් (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    ආපසු (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ga_nanaa RCH_status status = RC_ok;
  sa_nracanaa stat st;
  නම් (fstat (fd, &st) != 0)
    status = RC_error;
  නැතහොත් නම් (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  නැතහොත් නම් ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#නම් HAVE_FCHOWN
      නම් (fchown (fd, uid, gid) == 0)
        ආපසු close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  පූර්ණ_සංඛ්‍යා saved_errno = errno;
  close (fd);
  errno = saved_errno;
  ආපසු status;
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

  නම් (ignore_missing && ignorable_fts_error (ent))
    ආපසු true;

  switch (ent->fts_info)
    {
    pak_sa FTS_D:
      නම් (chopt->recurse)
        {
          නම් (ROOT_DEV_INO_CHECK (chopt->root_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ROOT_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_value (fts_read (fts));
              ආපසු false;
            }
          ආපසු true;
        }
      bha_nga;

    pak_sa FTS_DP:
      නම් (! chopt->recurse)
        ආපසු true;
      bha_nga;

    pak_sa FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      නම් (ent->fts_level == FTS_ROOTLEVEL && ent->fts_number == 0)
        {
          ent->fts_number = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ආපසු true;
        }
      නම් (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_ERR:
      නම් (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_DNR:
      නම් (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      bha_nga;

    pak_sa FTS_DC:		/* directory that causes cycles */
      නම් (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ආපසු false;
        }
      bha_nga;

    default:
      bha_nga;
    }

  bool do_chown;
  sa_nracanaa stat stat_buf;
  sa_nracanaa stat const *file_stats;
  නම් (!ok)
    {
      do_chown = false;
      file_stats = NULL;
    }
  නැතහොත් නම් (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->root_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  නැතහොත්
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      නම් (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          නම් (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              නම් (ignore_missing
                  && ignorable_traversal_errno (errno))
                ආපසු true;
              නම් (! chopt->force_silent)
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
  නම් (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ROOT_DEV_INO_CHECK (chopt->root_dev_ino, file_stats))
    {
      ROOT_DEV_INO_WARN (file_full_name);
      ආපසු false;
    }

  bool symlink_changed = true;
  නම් (do_chown)
    {
      නම් ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          නම් (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      නැතහොත්
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

      නම් (do_chown && !ok)
        {
          නම් (ignore_missing
              && ignorable_traversal_errno (errno))
            ආපසු true;
          නම් (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  නම් (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      නම් (changed || chopt->verbosity == V_high)
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
          නම් (new_usr != chopt->user_name)
            free (new_usr);
          නම් (new_grp != chopt->group_name)
            free (new_grp);
        }
    }

  නම් ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  ආපසු ok;
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
chown_files (var_na **files, පූර්ණ_සංඛ්‍යා bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             sa_nracanaa Chown_option const *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  පූර්ණ_සංඛ්‍යා stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, NULL);

  අතරතුර (true)
    {
      FTSENT *ent = fts_read (fts);

      නම් (ent == NULL)
        {
          නම් (errno != 0)
            {
              /* FIXME: try to give a better message  */
              නම් (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          bha_nga;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  නම් (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ආපසු ok;
}
