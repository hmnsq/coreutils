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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "chown-core.h"
#অন্তর্ভাব "fts-missing.h"
#অন্তর্ভাব "ignore-value.h"
#অন্তর্ভাব "root-dev-ino.h"
#অন্তর্ভাব "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

গণনা RCH_status
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

বাহ্যিক শূন্য
chopt_init (কাঠামো Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->মূল_dev_ino = ফাঁকা;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ফাঁকা;
  chopt->group_name = ফাঁকা;
}

বাহ্যিক শূন্য
chopt_free (কাঠামো Chown_option *chopt)
{
  মুক্ত (chopt->user_name);
  মুক্ত (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

স্থির অক্ষর *
uid_to_str (uid_t uid)
{
  অক্ষর buf[INT_BUFSIZE_BOUND (intmax_t)];
  ফেরত xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

স্থির অক্ষর *
gid_to_str (gid_t gid)
{
  অক্ষর buf[INT_BUFSIZE_BOUND (intmax_t)];
  ফেরত xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

বাহ্যিক অক্ষর *
gid_to_name (gid_t gid)
{
  কাঠামো group *grp = getgrgid (gid);
  ফেরত grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

বাহ্যিক অক্ষর *
uid_to_name (uid_t uid)
{
  কাঠামো passwd *pwd = getpwuid (uid);
  ফেরত pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

স্থির অক্ষর *
user_group_str (অক্ষর ধ্রুবক *user, অক্ষর ধ্রুবক *group)
{
  অক্ষর *spec = ফাঁকা;

  যদি (user)
    {
      যদি (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      অন্যথা
        {
          spec = xstrdup (user);
        }
    }
  অন্যথা যদি (group)
    {
      spec = xstrdup (group);
    }

  ফেরত spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

স্থির শূন্য
describe_change (অক্ষর ধ্রুবক *file, গণনা Change_status changed,
                 অক্ষর ধ্রুবক *old_user, অক্ষর ধ্রুবক *old_group,
                 অক্ষর ধ্রুবক *user, অক্ষর ধ্রুবক *group)
{
  যদি (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      ফেরত;
    }

  অক্ষর *spec = user_group_str (user, group);
  অক্ষর *old_spec = user_group_str (user ? old_user : ফাঁকা,
                                   group ? old_group : ফাঁকা);

  অক্ষর ধ্রুবক *fmt;
  নির্বাচন (changed)
    {
    ক্ষেত্র CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      বিরতি;
    ক্ষেত্র CH_FAILED:
      যদি (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      অন্যথা
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          মুক্ত (old_spec);
          old_spec = spec;
          spec = ফাঁকা;
        }
      বিরতি;
    ক্ষেত্র CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      বিরতি;
    ক্ষেত্র CH_NOT_APPLIED:
    পূর্বনির্ধারিত:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  মুক্ত (old_spec);
  মুক্ত (spec);
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

স্থির গণনা RCH_status
restricted_chown (পূর্ণসংখ্যা cwd_fd, অক্ষর ধ্রুবক *file,
                  কাঠামো stat ধ্রুবক *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  যদি (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    ফেরত RC_do_ordinary_chown;

  পূর্ণসংখ্যা open_flags = O_NONBLOCK | O_NOCTTY;
  যদি (! S_ISREG (orig_st->st_mode))
    {
      যদি (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      অন্যথা
        ফেরত RC_do_ordinary_chown;
    }

  পূর্ণসংখ্যা fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  যদি (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    ফেরত (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  গণনা RCH_status status = RC_ok;
  কাঠামো stat st;
  যদি (fstat (fd, &st) != 0)
    status = RC_error;
  অন্যথা যদি (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  অন্যথা যদি ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#যদি HAVE_FCHOWN
      যদি (fchown (fd, uid, gid) == 0)
        ফেরত close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  পূর্ণসংখ্যা saved_errno = errno;
  close (fd);
  errno = saved_errno;
  ফেরত status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
স্থির bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   কাঠামো Chown_option ধ্রুবক *chopt)
{
  অক্ষর ধ্রুবক *file_full_name = ent->fts_path;
  অক্ষর ধ্রুবক *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    ফেরত true;

  নির্বাচন (ent->fts_info)
    {
    ক্ষেত্র FTS_D:
      যদি (chopt->recurse)
        {
          যদি (মূল_DEV_INO_CHECK (chopt->মূল_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              মূল_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_মান (fts_read (fts));
              ফেরত false;
            }
          ফেরত true;
        }
      বিরতি;

    ক্ষেত্র FTS_DP:
      যদি (! chopt->recurse)
        ফেরত true;
      বিরতি;

    ক্ষেত্র FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      যদি (ent->fts_level == FTS_ROOTLEVEL && ent->fts_সংখ্যা == 0)
        {
          ent->fts_সংখ্যা = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ফেরত true;
        }
      যদি (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      বিরতি;

    ক্ষেত্র FTS_ERR:
      যদি (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      বিরতি;

    ক্ষেত্র FTS_DNR:
      যদি (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      বিরতি;

    ক্ষেত্র FTS_DC:		/* directory that causes cycles */
      যদি (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ফেরত false;
        }
      বিরতি;

    পূর্বনির্ধারিত:
      বিরতি;
    }

  bool do_chown;
  কাঠামো stat stat_buf;
  কাঠামো stat ধ্রুবক *file_stats;
  যদি (!ok)
    {
      do_chown = false;
      file_stats = ফাঁকা;
    }
  অন্যথা যদি (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->মূল_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  অন্যথা
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      যদি (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          যদি (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              যদি (ignore_missing
                  && ignorable_traversal_errno (errno))
                ফেরত true;
              যদি (! chopt->force_silent)
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
  যদি (ok
      && FTSENT_IS_DIRECTORY (ent)
      && মূল_DEV_INO_CHECK (chopt->মূল_dev_ino, file_stats))
    {
      মূল_DEV_INO_WARN (file_full_name);
      ফেরত false;
    }

  bool symlink_changed = true;
  যদি (do_chown)
    {
      যদি ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          যদি (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      অন্যথা
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

          গণনা RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          নির্বাচন (err)
            {
            ক্ষেত্র RC_ok:
              বিরতি;

            ক্ষেত্র RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              বিরতি;

            ক্ষেত্র RC_error:
              ok = false;
              বিরতি;

            ক্ষেত্র RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            ক্ষেত্র RC_excluded:
              do_chown = false;
              ok = false;
              বিরতি;

            পূর্বনির্ধারিত:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      যদি (do_chown && !ok)
        {
          যদি (ignore_missing
              && ignorable_traversal_errno (errno))
            ফেরত true;
          যদি (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  যদি (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      যদি (changed || chopt->verbosity == V_উপরের)
        {
          গণনা Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          অক্ষর *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : ফাঁকা);
          অক্ষর *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : ফাঁকা);
          অক্ষর *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : ফাঁকা;
          অক্ষর *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : ফাঁকা;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          মুক্ত (old_usr);
          মুক্ত (old_grp);
          যদি (new_usr != chopt->user_name)
            মুক্ত (new_usr);
          যদি (new_grp != chopt->group_name)
            মুক্ত (new_grp);
        }
    }

  যদি ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  ফেরত ok;
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
বাহ্যিক bool
chown_files (অক্ষর **files, পূর্ণসংখ্যা bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             কাঠামো Chown_option ধ্রুবক *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  পূর্ণসংখ্যা stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, ফাঁকা);

  যখন (true)
    {
      FTSENT *ent = fts_read (fts);

      যদি (ent == ফাঁকা)
        {
          যদি (errno != 0)
            {
              /* FIXME: try to give a better message  */
              যদি (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          বিরতি;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  যদি (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ফেরত ok;
}
