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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <grp.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "chown-core.h"
#அந்தர்பாவ "fts-missing.h"
#அந்தர்பாவ "ignore-value.h"
#அந்தர்பாவ "root-dev-ino.h"
#அந்தர்பாவ "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

எண்ணிக்கை RCH_status
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

வெளி வெறுமை
chopt_init (கட்டமைப்பு Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->வேர்_dev_ino = வெற்று;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = வெற்று;
  chopt->group_name = வெற்று;
}

வெளி வெறுமை
chopt_free (கட்டமைப்பு Chown_option *chopt)
{
  விடுவி (chopt->user_name);
  விடுவி (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

நிலையான எழுத்து *
uid_to_str (uid_t uid)
{
  எழுத்து buf[INT_BUFSIZE_BOUND (intmax_t)];
  திரும்பு xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

நிலையான எழுத்து *
gid_to_str (gid_t gid)
{
  எழுத்து buf[INT_BUFSIZE_BOUND (intmax_t)];
  திரும்பு xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

வெளி எழுத்து *
gid_to_name (gid_t gid)
{
  கட்டமைப்பு group *grp = getgrgid (gid);
  திரும்பு grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

வெளி எழுத்து *
uid_to_name (uid_t uid)
{
  கட்டமைப்பு passwd *pwd = getpwuid (uid);
  திரும்பு pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

நிலையான எழுத்து *
user_group_str (எழுத்து மாறிலி *user, எழுத்து மாறிலி *group)
{
  எழுத்து *spec = வெற்று;

  எனில் (user)
    {
      எனில் (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      இல்லையெனில்
        {
          spec = xstrdup (user);
        }
    }
  இல்லையெனில் எனில் (group)
    {
      spec = xstrdup (group);
    }

  திரும்பு spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

நிலையான வெறுமை
describe_change (எழுத்து மாறிலி *file, எண்ணிக்கை Change_status changed,
                 எழுத்து மாறிலி *old_user, எழுத்து மாறிலி *old_group,
                 எழுத்து மாறிலி *user, எழுத்து மாறிலி *group)
{
  எனில் (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      திரும்பு;
    }

  எழுத்து *spec = user_group_str (user, group);
  எழுத்து *old_spec = user_group_str (user ? old_user : வெற்று,
                                   group ? old_group : வெற்று);

  எழுத்து மாறிலி *fmt;
  தேர்வு (changed)
    {
    நிலை CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      நிறுத்து;
    நிலை CH_FAILED:
      எனில் (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      இல்லையெனில்
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          விடுவி (old_spec);
          old_spec = spec;
          spec = வெற்று;
        }
      நிறுத்து;
    நிலை CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      நிறுத்து;
    நிலை CH_NOT_APPLIED:
    இயல்பு:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  விடுவி (old_spec);
  விடுவி (spec);
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

நிலையான எண்ணிக்கை RCH_status
restricted_chown (முழு cwd_fd, எழுத்து மாறிலி *file,
                  கட்டமைப்பு stat மாறிலி *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  எனில் (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    திரும்பு RC_do_ordinary_chown;

  முழு open_flags = O_NONBLOCK | O_NOCTTY;
  எனில் (! S_ISREG (orig_st->st_mode))
    {
      எனில் (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      இல்லையெனில்
        திரும்பு RC_do_ordinary_chown;
    }

  முழு fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  எனில் (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    திரும்பு (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  எண்ணிக்கை RCH_status status = RC_ok;
  கட்டமைப்பு stat st;
  எனில் (fstat (fd, &st) != 0)
    status = RC_error;
  இல்லையெனில் எனில் (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  இல்லையெனில் எனில் ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#எனில் HAVE_FCHOWN
      எனில் (fchown (fd, uid, gid) == 0)
        திரும்பு close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  முழு saved_errno = errno;
  close (fd);
  errno = saved_errno;
  திரும்பு status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
நிலையான bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   கட்டமைப்பு Chown_option மாறிலி *chopt)
{
  எழுத்து மாறிலி *file_full_name = ent->fts_path;
  எழுத்து மாறிலி *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  எனில் (ignore_missing && ignorable_fts_error (ent))
    திரும்பு true;

  தேர்வு (ent->fts_info)
    {
    நிலை FTS_D:
      எனில் (chopt->recurse)
        {
          எனில் (வேர்_DEV_INO_CHECK (chopt->வேர்_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              வேர்_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_மதிப்பு (fts_read (fts));
              திரும்பு false;
            }
          திரும்பு true;
        }
      நிறுத்து;

    நிலை FTS_DP:
      எனில் (! chopt->recurse)
        திரும்பு true;
      நிறுத்து;

    நிலை FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      எனில் (ent->fts_level == FTS_ROOTLEVEL && ent->fts_எண் == 0)
        {
          ent->fts_எண் = 1;
          fts_set (fts, ent, FTS_AGAIN);
          திரும்பு true;
        }
      எனில் (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      நிறுத்து;

    நிலை FTS_ERR:
      எனில் (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      நிறுத்து;

    நிலை FTS_DNR:
      எனில் (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      நிறுத்து;

    நிலை FTS_DC:		/* directory that causes cycles */
      எனில் (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          திரும்பு false;
        }
      நிறுத்து;

    இயல்பு:
      நிறுத்து;
    }

  bool do_chown;
  கட்டமைப்பு stat stat_buf;
  கட்டமைப்பு stat மாறிலி *file_stats;
  எனில் (!ok)
    {
      do_chown = false;
      file_stats = வெற்று;
    }
  இல்லையெனில் எனில் (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->வேர்_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  இல்லையெனில்
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      எனில் (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          எனில் (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              எனில் (ignore_missing
                  && ignorable_traversal_errno (errno))
                திரும்பு true;
              எனில் (! chopt->force_silent)
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
  எனில் (ok
      && FTSENT_IS_DIRECTORY (ent)
      && வேர்_DEV_INO_CHECK (chopt->வேர்_dev_ino, file_stats))
    {
      வேர்_DEV_INO_WARN (file_full_name);
      திரும்பு false;
    }

  bool symlink_changed = true;
  எனில் (do_chown)
    {
      எனில் ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          எனில் (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      இல்லையெனில்
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

          எண்ணிக்கை RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          தேர்வு (err)
            {
            நிலை RC_ok:
              நிறுத்து;

            நிலை RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              நிறுத்து;

            நிலை RC_error:
              ok = false;
              நிறுத்து;

            நிலை RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            நிலை RC_excluded:
              do_chown = false;
              ok = false;
              நிறுத்து;

            இயல்பு:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      எனில் (do_chown && !ok)
        {
          எனில் (ignore_missing
              && ignorable_traversal_errno (errno))
            திரும்பு true;
          எனில் (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  எனில் (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      எனில் (changed || chopt->verbosity == V_உயர்வு)
        {
          எண்ணிக்கை Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          எழுத்து *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : வெற்று);
          எழுத்து *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : வெற்று);
          எழுத்து *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : வெற்று;
          எழுத்து *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : வெற்று;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          விடுவி (old_usr);
          விடுவி (old_grp);
          எனில் (new_usr != chopt->user_name)
            விடுவி (new_usr);
          எனில் (new_grp != chopt->group_name)
            விடுவி (new_grp);
        }
    }

  எனில் ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  திரும்பு ok;
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
வெளி bool
chown_files (எழுத்து **files, முழு bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             கட்டமைப்பு Chown_option மாறிலி *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  முழு stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, வெற்று);

  போது (true)
    {
      FTSENT *ent = fts_read (fts);

      எனில் (ent == வெற்று)
        {
          எனில் (errno != 0)
            {
              /* FIXME: try to give a better message  */
              எனில் (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          நிறுத்து;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  எனில் (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  திரும்பு ok;
}
