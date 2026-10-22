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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "chown-core.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fts-missing.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xfts.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

ਗਿਣਤੀ RCH_ਸਥਿਤੀ
  {
    /* we called fchown and close, and both succeeded */
    RC_ok = 2,

    /* required_uid and/or required_gid are specified, but don't match */
    RC_excluded,

    /* The file was replaced by another file during the requested change.  */
    RC_inode_ਬਦਲਿਆ,

    /* open/fchown isn't needed, isn't safe, or doesn't work due to
       permissions problems; fall back on chown */
    RC_do_ordinary_chown,

    /* open, fstat, fchown, or close failed */
    RC_error
  };

ਬਾਹਰੀ ਖਾਲੀ
chopt_init (ਬਣਤਰ Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->ਜੜ੍ਹ_dev_ino = ਨਿਰਾ;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ਨਿਰਾ;
  chopt->ਸਮੂਹ_name = ਨਿਰਾ;
}

ਬਾਹਰੀ ਖਾਲੀ
chopt_free (ਬਣਤਰ Chown_option *chopt)
{
  ਮੁਕਤ (chopt->user_name);
  ਮੁਕਤ (chopt->ਸਮੂਹ_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ਸਥਿਰ ਅੱਖਰ *
uid_ਮੰਜ਼ਿਲ_str (uid_t uid)
{
  ਅੱਖਰ buf[INT_BUFSIZE_BOUND (intmax_t)];
  ਵਾਪਸ xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ਸਥਿਰ ਅੱਖਰ *
gid_ਮੰਜ਼ਿਲ_str (gid_t gid)
{
  ਅੱਖਰ buf[INT_BUFSIZE_BOUND (intmax_t)];
  ਵਾਪਸ xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

ਬਾਹਰੀ ਅੱਖਰ *
gid_ਮੰਜ਼ਿਲ_name (gid_t gid)
{
  ਬਣਤਰ ਸਮੂਹ *grp = getgrgid (gid);
  ਵਾਪਸ grp ? xstrdup (grp->gr_name) : gid_ਮੰਜ਼ਿਲ_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

ਬਾਹਰੀ ਅੱਖਰ *
uid_ਮੰਜ਼ਿਲ_name (uid_t uid)
{
  ਬਣਤਰ passwd *pwd = getpwuid (uid);
  ਵਾਪਸ pwd ? xstrdup (pwd->pw_name) : uid_ਮੰਜ਼ਿਲ_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

ਸਥਿਰ ਅੱਖਰ *
user_ਸਮੂਹ_str (ਅੱਖਰ ਅਚਲ *user, ਅੱਖਰ ਅਚਲ *ਸਮੂਹ)
{
  ਅੱਖਰ *spec = ਨਿਰਾ;

  ਜੇ (user)
    {
      ਜੇ (ਸਮੂਹ)
        {
          spec = xmalloc (ਲੜੀ_ਲੰਬਾਈ (user) + 1 + ਲੜੀ_ਲੰਬਾਈ (ਸਮੂਹ) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), ਸਮੂਹ);
        }
      ਨਹੀਂਤਾਂ
        {
          spec = xstrdup (user);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਸਮੂਹ)
    {
      spec = xstrdup (ਸਮੂਹ);
    }

  ਵਾਪਸ spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

ਸਥਿਰ ਖਾਲੀ
describe_change (ਅੱਖਰ ਅਚਲ *file, ਗਿਣਤੀ Change_ਸਥਿਤੀ ਬਦਲਿਆ,
                 ਅੱਖਰ ਅਚਲ *old_user, ਅੱਖਰ ਅਚਲ *old_ਸਮੂਹ,
                 ਅੱਖਰ ਅਚਲ *user, ਅੱਖਰ ਅਚਲ *ਸਮੂਹ)
{
  ਜੇ (ਬਦਲਿਆ == CH_NOT_APPLIED)
    {
      ਲਿਖੋ (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      ਵਾਪਸ;
    }

  ਅੱਖਰ *spec = user_ਸਮੂਹ_str (user, ਸਮੂਹ);
  ਅੱਖਰ *old_spec = user_ਸਮੂਹ_str (user ? old_user : ਨਿਰਾ,
                                   ਸਮੂਹ ? old_ਸਮੂਹ : ਨਿਰਾ);

  ਅੱਖਰ ਅਚਲ *fmt;
  ਚੋਣ (ਬਦਲਿਆ)
    {
    ਹਾਲਤ CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : ਸਮੂਹ ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      ਰੋਕੋ;
    ਹਾਲਤ CH_FAILED:
      ਜੇ (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : ਸਮੂਹ ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      ਨਹੀਂਤਾਂ
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : ਸਮੂਹ ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          ਮੁਕਤ (old_spec);
          old_spec = spec;
          spec = ਨਿਰਾ;
        }
      ਰੋਕੋ;
    ਹਾਲਤ CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : ਸਮੂਹ ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      ਰੋਕੋ;
    ਹਾਲਤ CH_NOT_APPLIED:
    ਮੂਲ:
      affirm (false);
    }

  ਲਿਖੋ (fmt, quoteaf (file), old_spec, spec);

  ਮੁਕਤ (old_spec);
  ਮੁਕਤ (spec);
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

ਸਥਿਰ ਗਿਣਤੀ RCH_ਸਥਿਤੀ
restricted_chown (ਪੂਰਨ cwd_fd, ਅੱਖਰ ਅਚਲ *file,
                  ਬਣਤਰ stat ਅਚਲ *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  ਜੇ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    ਵਾਪਸ RC_do_ordinary_chown;

  ਪੂਰਨ open_flags = O_NONBLOCK | O_NOCTTY;
  ਜੇ (! S_ISREG (orig_st->st_ਬਹੁਲਕ))
    {
      ਜੇ (S_ISDIR (orig_st->st_ਬਹੁਲਕ))
        open_flags |= O_DIRECTORY;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ RC_do_ordinary_chown;
    }

  ਪੂਰਨ fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  ਜੇ (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_ਬਹੁਲਕ)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    ਵਾਪਸ (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ਗਿਣਤੀ RCH_ਸਥਿਤੀ ਸਥਿਤੀ = RC_ok;
  ਬਣਤਰ stat st;
  ਜੇ (fstat (fd, &st) != 0)
    ਸਥਿਤੀ = RC_error;
  ਨਹੀਂਤਾਂ ਜੇ (! psame_inode (orig_st, &st))
    ਸਥਿਤੀ = RC_inode_ਬਦਲਿਆ;
  ਨਹੀਂਤਾਂ ਜੇ ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#ਜੇ HAVE_FCHOWN
      ਜੇ (fchown (fd, uid, gid) == 0)
        ਵਾਪਸ close (fd) < 0 ? RC_error : RC_ok;
#ਜੇ_ਅੰਤ
      ਸਥਿਤੀ = RC_error;
    }

  ਪੂਰਨ saved_errno = errno;
  close (fd);
  errno = saved_errno;
  ਵਾਪਸ ਸਥਿਤੀ;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
ਸਥਿਰ bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   ਬਣਤਰ Chown_option ਅਚਲ *chopt)
{
  ਅੱਖਰ ਅਚਲ *file_ਭਰਿਆ_name = ent->fts_ਰਸਤਾ;
  ਅੱਖਰ ਅਚਲ *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_ਇੰਦਰਾਜ (ent);

  ਜੇ (ignore_missing && ignorable_fts_error (ent))
    ਵਾਪਸ true;

  ਚੋਣ (ent->fts_info)
    {
    ਹਾਲਤ FTS_D:
      ਜੇ (chopt->recurse)
        {
          ਜੇ (ਜੜ੍ਹ_DEV_INO_CHECK (chopt->ਜੜ੍ਹ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ਜੜ੍ਹ_DEV_INO_WARN (file_ਭਰਿਆ_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_ਸੈੱਟ (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ਮੁੱਲ (fts_read (fts));
              ਵਾਪਸ false;
            }
          ਵਾਪਸ true;
        }
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DP:
      ਜੇ (! chopt->recurse)
        ਵਾਪਸ true;
      ਰੋਕੋ;

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
      ਜੇ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_ਭਰਿਆ_name));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_ERR:
      ਜੇ (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_ਭਰਿਆ_name));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DNR:
      ਜੇ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_ਭਰਿਆ_name));
      ok = false;
      ਰੋਕੋ;

    ਹਾਲਤ FTS_DC:		/* directory that causes cycles */
      ਜੇ (ਚੱਕਰ_warning_required (fts, ent))
        {
          emit_ਚੱਕਰ_warning (file_ਭਰਿਆ_name);
          ਵਾਪਸ false;
        }
      ਰੋਕੋ;

    ਮੂਲ:
      ਰੋਕੋ;
    }

  bool do_chown;
  ਬਣਤਰ stat stat_buf;
  ਬਣਤਰ stat ਅਚਲ *file_stats;
  ਜੇ (!ok)
    {
      do_chown = false;
      file_stats = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->ਜੜ੍ਹ_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  ਨਹੀਂਤਾਂ
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      ਜੇ (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_ਬਹੁਲਕ))
        {
          ਜੇ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ਜੇ (ignore_missing
                  && ignorable_traversal_errno (errno))
                ਵਾਪਸ true;
              ਜੇ (! chopt->force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_ਭਰਿਆ_name));
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
  ਜੇ (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ਜੜ੍ਹ_DEV_INO_CHECK (chopt->ਜੜ੍ਹ_dev_ino, file_stats))
    {
      ਜੜ੍ਹ_DEV_INO_WARN (file_ਭਰਿਆ_name);
      ਵਾਪਸ false;
    }

  bool symlink_ਬਦਲਿਆ = true;
  ਜੇ (do_chown)
    {
      ਜੇ ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          ਜੇ (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_ਬਦਲਿਆ = false;
            }
        }
      ਨਹੀਂਤਾਂ
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

          ਗਿਣਤੀ RCH_ਸਥਿਤੀ err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          ਚੋਣ (err)
            {
            ਹਾਲਤ RC_ok:
              ਰੋਕੋ;

            ਹਾਲਤ RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              ਰੋਕੋ;

            ਹਾਲਤ RC_error:
              ok = false;
              ਰੋਕੋ;

            ਹਾਲਤ RC_inode_ਬਦਲਿਆ:
              /* FIXME: give a diagnostic in this case?  */
            ਹਾਲਤ RC_excluded:
              do_chown = false;
              ok = false;
              ਰੋਕੋ;

            ਮੂਲ:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      ਜੇ (do_chown && !ok)
        {
          ਜੇ (ignore_missing
              && ignorable_traversal_errno (errno))
            ਵਾਪਸ true;
          ਜੇ (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_ਭਰਿਆ_name));
        }
    }

  ਜੇ (chopt->verbosity != V_off)
    {
      bool ਬਦਲਿਆ =
        ((do_chown && ok && symlink_ਬਦਲਿਆ)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      ਜੇ (ਬਦਲਿਆ || chopt->verbosity == V_ਉੱਚਾ)
        {
          ਗਿਣਤੀ Change_ਸਥਿਤੀ ch_ਸਥਿਤੀ =
            (!ok ? CH_FAILED
             : !symlink_ਬਦਲਿਆ ? CH_NOT_APPLIED
             : !ਬਦਲਿਆ ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          ਅੱਖਰ *old_usr = (file_stats
                           ? uid_ਮੰਜ਼ਿਲ_name (file_stats->st_uid) : ਨਿਰਾ);
          ਅੱਖਰ *old_grp = (file_stats
                           ? gid_ਮੰਜ਼ਿਲ_name (file_stats->st_gid) : ਨਿਰਾ);
          ਅੱਖਰ *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_ਮੰਜ਼ਿਲ_str (uid) : ਨਿਰਾ;
          ਅੱਖਰ *new_grp = chopt->ਸਮੂਹ_name
                          ? chopt->ਸਮੂਹ_name : gid != -1
                                               ? gid_ਮੰਜ਼ਿਲ_str (gid) : ਨਿਰਾ;
          describe_change (file_ਭਰਿਆ_name, ch_ਸਥਿਤੀ,
                           old_usr, old_grp,
                           new_usr, new_grp);
          ਮੁਕਤ (old_usr);
          ਮੁਕਤ (old_grp);
          ਜੇ (new_usr != chopt->user_name)
            ਮੁਕਤ (new_usr);
          ਜੇ (new_grp != chopt->ਸਮੂਹ_name)
            ਮੁਕਤ (new_grp);
        }
    }

  ਜੇ ( ! chopt->recurse)
    fts_ਸੈੱਟ (fts, ent, FTS_SKIP);

  ਵਾਪਸ ok;
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
ਬਾਹਰੀ bool
chown_files (ਅੱਖਰ **files, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             ਬਣਤਰ Chown_option ਅਚਲ *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  ਪੂਰਨ stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, ਦੋਆਧਾਰੀ_ਅੰਕ_flags | stat_flags, ਨਿਰਾ);

  ਜਦੋਂ (true)
    {
      FTSENT *ent = fts_read (fts);

      ਜੇ (ent == ਨਿਰਾ)
        {
          ਜੇ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ਜੇ (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ਰੋਕੋ;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  ਜੇ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ਵਾਪਸ ok;
}
