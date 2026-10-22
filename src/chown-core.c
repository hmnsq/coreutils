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

#تعریف_کړه FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

تعداد RCH_حالت
  {
    /* we called fchown and close, and both succeeded */
    RC_ok = 2,

    /* required_uid and/or required_gid are specified, but don't match */
    RC_excluded,

    /* The file was replaced by another file during the requested change.  */
    RC_inode_بدل_شوی,

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
  chopt->ریښه_dev_ino = عدم;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = عدم;
  chopt->ډله_name = عدم;
}

خارجي فراغ
chopt_free (بنية Chown_option *chopt)
{
  حرر (chopt->user_name);
  حرر (chopt->ډله_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ساكن حرف *
uid_منزل_str (uid_t uid)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
  أرجع xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ساكن حرف *
gid_منزل_str (gid_t gid)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
  أرجع xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

خارجي حرف *
gid_منزل_name (gid_t gid)
{
  بنية ډله *grp = getgrgid (gid);
  أرجع grp ? xstrdup (grp->gr_name) : gid_منزل_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

خارجي حرف *
uid_منزل_name (uid_t uid)
{
  بنية passwd *pwd = getpwuid (uid);
  أرجع pwd ? xstrdup (pwd->pw_name) : uid_منزل_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

ساكن حرف *
user_ډله_str (حرف ثابت *user, حرف ثابت *ډله)
{
  حرف *spec = عدم;

  إذا (user)
    {
      إذا (ډله)
        {
          spec = xmalloc (مزي_اوږدوالی (user) + 1 + مزي_اوږدوالی (ډله) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), ډله);
        }
      وإلا
        {
          spec = xstrdup (user);
        }
    }
  وإلا إذا (ډله)
    {
      spec = xstrdup (ډله);
    }

  أرجع spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

ساكن فراغ
describe_change (حرف ثابت *file, تعداد Change_حالت بدل_شوی,
                 حرف ثابت *old_user, حرف ثابت *old_ډله,
                 حرف ثابت *user, حرف ثابت *ډله)
{
  إذا (بدل_شوی == CH_NOT_APPLIED)
    {
      چاپ_کړه (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      أرجع;
    }

  حرف *spec = user_ډله_str (user, ډله);
  حرف *old_spec = user_ډله_str (user ? old_user : عدم,
                                   ډله ? old_ډله : عدم);

  حرف ثابت *fmt;
  بدل (بدل_شوی)
    {
    حالة CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : ډله ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      اكسر;
    حالة CH_FAILED:
      إذا (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : ډله ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      وإلا
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : ډله ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          حرر (old_spec);
          old_spec = spec;
          spec = عدم;
        }
      اكسر;
    حالة CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : ډله ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      اكسر;
    حالة CH_NOT_APPLIED:
    افتراضي:
      affirm (false);
    }

  چاپ_کړه (fmt, quoteaf (file), old_spec, spec);

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

ساكن تعداد RCH_حالت
restricted_chown (صحيح cwd_fd, حرف ثابت *file,
                  بنية stat ثابت *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  إذا (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    أرجع RC_do_ordinary_chown;

  صحيح open_flags = O_NONBLOCK | O_NOCTTY;
  إذا (! S_ISREG (orig_st->st_ډېرپېښ))
    {
      إذا (S_ISDIR (orig_st->st_ډېرپېښ))
        open_flags |= O_DIRECTORY;
      وإلا
        أرجع RC_do_ordinary_chown;
    }

  صحيح fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  إذا (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_ډېرپېښ)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    أرجع (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  تعداد RCH_حالت حالت = RC_ok;
  بنية stat st;
  إذا (fstat (fd, &st) != 0)
    حالت = RC_error;
  وإلا إذا (! psame_inode (orig_st, &st))
    حالت = RC_inode_بدل_شوی;
  وإلا إذا ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#إذا HAVE_FCHOWN
      إذا (fchown (fd, uid, gid) == 0)
        أرجع close (fd) < 0 ? RC_error : RC_ok;
#د_که_پای
      حالت = RC_error;
    }

  صحيح saved_errno = errno;
  close (fd);
  errno = saved_errno;
  أرجع حالت;
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
  حرف ثابت *file_ډک_name = ent->fts_لار;
  حرف ثابت *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_ثبت (ent);

  إذا (ignore_missing && ignorable_fts_error (ent))
    أرجع true;

  بدل (ent->fts_info)
    {
    حالة FTS_D:
      إذا (chopt->recurse)
        {
          إذا (ریښه_DEV_INO_CHECK (chopt->ریښه_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ریښه_DEV_INO_WARN (file_ډک_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_سېټ (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ارزښت (fts_read (fts));
              أرجع false;
            }
          أرجع true;
        }
      اكسر;

    حالة FTS_DP:
      إذا (! chopt->recurse)
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
      إذا (ent->fts_level == FTS_ROOTLEVEL && ent->fts_شمېره == 0)
        {
          ent->fts_شمېره = 1;
          fts_سېټ (fts, ent, FTS_AGAIN);
          أرجع true;
        }
      إذا (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_ډک_name));
      ok = false;
      اكسر;

    حالة FTS_ERR:
      إذا (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_ډک_name));
      ok = false;
      اكسر;

    حالة FTS_DNR:
      إذا (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_ډک_name));
      ok = false;
      اكسر;

    حالة FTS_DC:		/* directory that causes cycles */
      إذا (دوره_warning_required (fts, ent))
        {
          emit_دوره_warning (file_ډک_name);
          أرجع false;
        }
      اكسر;

    افتراضي:
      اكسر;
    }

  bool do_chown;
  بنية stat stat_buf;
  بنية stat ثابت *file_stats;
  إذا (!ok)
    {
      do_chown = false;
      file_stats = عدم;
    }
  وإلا إذا (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->ریښه_dev_ino
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
      إذا (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_ډېرپېښ))
        {
          إذا (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              إذا (ignore_missing
                  && ignorable_traversal_errno (errno))
                أرجع true;
              إذا (! chopt->force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_ډک_name));
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
  إذا (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ریښه_DEV_INO_CHECK (chopt->ریښه_dev_ino, file_stats))
    {
      ریښه_DEV_INO_WARN (file_ډک_name);
      أرجع false;
    }

  bool symlink_بدل_شوی = true;
  إذا (do_chown)
    {
      إذا ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          إذا (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_بدل_شوی = false;
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

          تعداد RCH_حالت err
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

            حالة RC_inode_بدل_شوی:
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

      إذا (do_chown && !ok)
        {
          إذا (ignore_missing
              && ignorable_traversal_errno (errno))
            أرجع true;
          إذا (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_ډک_name));
        }
    }

  إذا (chopt->verbosity != V_off)
    {
      bool بدل_شوی =
        ((do_chown && ok && symlink_بدل_شوی)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      إذا (بدل_شوی || chopt->verbosity == V_لوړ)
        {
          تعداد Change_حالت ch_حالت =
            (!ok ? CH_FAILED
             : !symlink_بدل_شوی ? CH_NOT_APPLIED
             : !بدل_شوی ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          حرف *old_usr = (file_stats
                           ? uid_منزل_name (file_stats->st_uid) : عدم);
          حرف *old_grp = (file_stats
                           ? gid_منزل_name (file_stats->st_gid) : عدم);
          حرف *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_منزل_str (uid) : عدم;
          حرف *new_grp = chopt->ډله_name
                          ? chopt->ډله_name : gid != -1
                                               ? gid_منزل_str (gid) : عدم;
          describe_change (file_ډک_name, ch_حالت,
                           old_usr, old_grp,
                           new_usr, new_grp);
          حرر (old_usr);
          حرر (old_grp);
          إذا (new_usr != chopt->user_name)
            حرر (new_usr);
          إذا (new_grp != chopt->ډله_name)
            حرر (new_grp);
        }
    }

  إذا ( ! chopt->recurse)
    fts_سېټ (fts, ent, FTS_SKIP);

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
chown_files (حرف **files, صحيح بټ_flags,
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

  FTS *fts = xfts_open (files, بټ_flags | stat_flags, عدم);

  طالما (true)
    {
      FTSENT *ent = fts_read (fts);

      إذا (ent == عدم)
        {
          إذا (errno != 0)
            {
              /* FIXME: try to give a better message  */
              إذا (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          اكسر;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  إذا (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  أرجع ok;
}
