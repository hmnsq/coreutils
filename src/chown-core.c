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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "chown-core.h"
#ضمن "fts-missing.h"
#ضمن "ignore-value.h"
#ضمن "root-dev-ino.h"
#ضمن "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

تعداد RCH_status
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

خارجي فراغ
chopt_init (بنية Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->root_dev_ino = عدم;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = عدم;
  chopt->group_name = عدم;
}

خارجي فراغ
chopt_free (بنية Chown_option *chopt)
{
  حرر (chopt->user_name);
  حرر (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ساكن حرف *
uid_to_str (uid_t uid)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
  أرجع xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ساكن حرف *
gid_to_str (gid_t gid)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
  أرجع xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

خارجي حرف *
gid_to_name (gid_t gid)
{
  بنية group *grp = getgrgid (gid);
  أرجع grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

خارجي حرف *
uid_to_name (uid_t uid)
{
  بنية passwd *pwd = getpwuid (uid);
  أرجع pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

ساكن حرف *
user_group_str (حرف ثابت *user, حرف ثابت *group)
{
  حرف *spec = عدم;

  𐡄𐡍 (user)
    {
      𐡄𐡍 (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      وإلا
        {
          spec = xstrdup (user);
        }
    }
  وإلا 𐡄𐡍 (group)
    {
      spec = xstrdup (group);
    }

  أرجع spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

ساكن فراغ
describe_change (حرف ثابت *file, تعداد Change_status changed,
                 حرف ثابت *old_user, حرف ثابت *old_group,
                 حرف ثابت *user, حرف ثابت *group)
{
  𐡄𐡍 (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      أرجع;
    }

  حرف *spec = user_group_str (user, group);
  حرف *old_spec = user_group_str (user ? old_user : عدم,
                                   group ? old_group : عدم);

  حرف ثابت *fmt;
  بدل (changed)
    {
    حالة CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      اكسر;
    حالة CH_FAILED:
      𐡄𐡍 (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      وإلا
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          حرر (old_spec);
          old_spec = spec;
          spec = عدم;
        }
      اكسر;
    حالة CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      اكسر;
    حالة CH_NOT_APPLIED:
    افتراضي:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  حرر (old_spec);
  حرر (spec);
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

ساكن تعداد RCH_status
restricted_chown (صحيح cwd_fd, حرف ثابت *file,
                  بنية stat ثابت *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  𐡄𐡍 (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    أرجع RC_do_ordinary_chown;

  صحيح open_flags = O_NONBLOCK | O_NOCTTY;
  𐡄𐡍 (! S_ISREG (orig_st->st_mode))
    {
      𐡄𐡍 (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      وإلا
        أرجع RC_do_ordinary_chown;
    }

  صحيح fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  𐡄𐡍 (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    أرجع (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  تعداد RCH_status status = RC_ok;
  بنية stat st;
  𐡄𐡍 (fstat (fd, &st) != 0)
    status = RC_error;
  وإلا 𐡄𐡍 (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  وإلا 𐡄𐡍 ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#𐡄𐡍 HAVE_FCHOWN
      𐡄𐡍 (fchown (fd, uid, gid) == 0)
        أرجع close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  صحيح saved_errno = errno;
  close (fd);
  errno = saved_errno;
  أرجع status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
ساكن bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   بنية Chown_option ثابت *chopt)
{
  حرف ثابت *file_full_name = ent->fts_path;
  حرف ثابت *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  𐡄𐡍 (ignore_missing && ignorable_fts_error (ent))
    أرجع true;

  بدل (ent->fts_info)
    {
    حالة FTS_D:
      𐡄𐡍 (chopt->recurse)
        {
          𐡄𐡍 (ROOT_DEV_INO_CHECK (chopt->root_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ROOT_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_value (fts_read (fts));
              أرجع false;
            }
          أرجع true;
        }
      اكسر;

    حالة FTS_DP:
      𐡄𐡍 (! chopt->recurse)
        أرجع true;
      اكسر;

    حالة FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      𐡄𐡍 (ent->fts_level == FTS_ROOTLEVEL && ent->fts_number == 0)
        {
          ent->fts_number = 1;
          fts_set (fts, ent, FTS_AGAIN);
          أرجع true;
        }
      𐡄𐡍 (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      اكسر;

    حالة FTS_ERR:
      𐡄𐡍 (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      اكسر;

    حالة FTS_DNR:
      𐡄𐡍 (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      اكسر;

    حالة FTS_DC:		/* directory that causes cycles */
      𐡄𐡍 (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          أرجع false;
        }
      اكسر;

    افتراضي:
      اكسر;
    }

  bool do_chown;
  بنية stat stat_buf;
  بنية stat ثابت *file_stats;
  𐡄𐡍 (!ok)
    {
      do_chown = false;
      file_stats = عدم;
    }
  وإلا 𐡄𐡍 (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->root_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  وإلا
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      𐡄𐡍 (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          𐡄𐡍 (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              𐡄𐡍 (ignore_missing
                  && ignorable_traversal_errno (errno))
                أرجع true;
              𐡄𐡍 (! chopt->force_silent)
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
  𐡄𐡍 (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ROOT_DEV_INO_CHECK (chopt->root_dev_ino, file_stats))
    {
      ROOT_DEV_INO_WARN (file_full_name);
      أرجع false;
    }

  bool symlink_changed = true;
  𐡄𐡍 (do_chown)
    {
      𐡄𐡍 ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          𐡄𐡍 (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      وإلا
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

          تعداد RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          بدل (err)
            {
            حالة RC_ok:
              اكسر;

            حالة RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              اكسر;

            حالة RC_error:
              ok = false;
              اكسر;

            حالة RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            حالة RC_excluded:
              do_chown = false;
              ok = false;
              اكسر;

            افتراضي:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      𐡄𐡍 (do_chown && !ok)
        {
          𐡄𐡍 (ignore_missing
              && ignorable_traversal_errno (errno))
            أرجع true;
          𐡄𐡍 (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  𐡄𐡍 (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      𐡄𐡍 (changed || chopt->verbosity == V_high)
        {
          تعداد Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          حرف *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : عدم);
          حرف *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : عدم);
          حرف *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : عدم;
          حرف *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : عدم;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          حرر (old_usr);
          حرر (old_grp);
          𐡄𐡍 (new_usr != chopt->user_name)
            حرر (new_usr);
          𐡄𐡍 (new_grp != chopt->group_name)
            حرر (new_grp);
        }
    }

  𐡄𐡍 ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  أرجع ok;
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
خارجي bool
chown_files (حرف **files, صحيح bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             بنية Chown_option ثابت *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  صحيح stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, عدم);

  طالما (true)
    {
      FTSENT *ent = fts_read (fts);

      𐡄𐡍 (ent == عدم)
        {
          𐡄𐡍 (errno != 0)
            {
              /* FIXME: try to give a better message  */
              𐡄𐡍 (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          اكسر;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  𐡄𐡍 (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  أرجع ok;
}
