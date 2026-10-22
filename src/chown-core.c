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

বাহ্য শূন্য
chopt_init (সংরচনা Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->মূল_dev_ino = রিক্ত;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = রিক্ত;
  chopt->group_name = রিক্ত;
}

বাহ্য শূন্য
chopt_free (সংরচনা Chown_option *chopt)
{
  মুক্ত (chopt->user_name);
  মুক্ত (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

স্থির বর্ণ *
uid_to_str (uid_t uid)
{
  বর্ণ buf[INT_BUFSIZE_BOUND (intmax_t)];
  প্রত্যাগম xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

স্থির বর্ণ *
gid_to_str (gid_t gid)
{
  বর্ণ buf[INT_BUFSIZE_BOUND (intmax_t)];
  প্রত্যাগম xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

বাহ্য বর্ণ *
gid_to_name (gid_t gid)
{
  সংরচনা group *grp = getgrgid (gid);
  প্রত্যাগম grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

বাহ্য বর্ণ *
uid_to_name (uid_t uid)
{
  সংরচনা passwd *pwd = getpwuid (uid);
  প্রত্যাগম pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

স্থির বর্ণ *
user_group_str (বর্ণ ধ্রুব *user, বর্ণ ধ্রুব *group)
{
  বর্ণ *spec = রিক্ত;

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

  প্রত্যাগম spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

স্থির শূন্য
describe_change (বর্ণ ধ্রুব *file, গণনা Change_status changed,
                 বর্ণ ধ্রুব *old_user, বর্ণ ধ্রুব *old_group,
                 বর্ণ ধ্রুব *user, বর্ণ ধ্রুব *group)
{
  যদি (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      প্রত্যাগম;
    }

  বর্ণ *spec = user_group_str (user, group);
  বর্ণ *old_spec = user_group_str (user ? old_user : রিক্ত,
                                   group ? old_group : রিক্ত);

  বর্ণ ধ্রুব *fmt;
  বিকল্প (changed)
    {
    পক্ষ CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      ভঙ্গ;
    পক্ষ CH_FAILED:
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
          spec = রিক্ত;
        }
      ভঙ্গ;
    পক্ষ CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      ভঙ্গ;
    পক্ষ CH_NOT_APPLIED:
    স্বভাব:
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
restricted_chown (পূর্ণাঙ্ক cwd_fd, বর্ণ ধ্রুব *file,
                  সংরচনা stat ধ্রুব *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  যদি (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    প্রত্যাগম RC_do_ordinary_chown;

  পূর্ণাঙ্ক open_flags = O_NONBLOCK | O_NOCTTY;
  যদি (! S_ISREG (orig_st->st_mode))
    {
      যদি (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      অন্যথা
        প্রত্যাগম RC_do_ordinary_chown;
    }

  পূর্ণাঙ্ক fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  যদি (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    প্রত্যাগম (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  গণনা RCH_status status = RC_ok;
  সংরচনা stat st;
  যদি (fstat (fd, &st) != 0)
    status = RC_error;
  অন্যথা যদি (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  অন্যথা যদি ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#যদি HAVE_FCHOWN
      যদি (fchown (fd, uid, gid) == 0)
        প্রত্যাগম close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  পূর্ণাঙ্ক saved_errno = errno;
  close (fd);
  errno = saved_errno;
  প্রত্যাগম status;
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
                   সংরচনা Chown_option ধ্রুব *chopt)
{
  বর্ণ ধ্রুব *file_full_name = ent->fts_path;
  বর্ণ ধ্রুব *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    প্রত্যাগম true;

  বিকল্প (ent->fts_info)
    {
    পক্ষ FTS_D:
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
              প্রত্যাগম false;
            }
          প্রত্যাগম true;
        }
      ভঙ্গ;

    পক্ষ FTS_DP:
      যদি (! chopt->recurse)
        প্রত্যাগম true;
      ভঙ্গ;

    পক্ষ FTS_NS:
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
          প্রত্যাগম true;
        }
      যদি (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      ভঙ্গ;

    পক্ষ FTS_ERR:
      যদি (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      ভঙ্গ;

    পক্ষ FTS_DNR:
      যদি (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      ভঙ্গ;

    পক্ষ FTS_DC:		/* directory that causes cycles */
      যদি (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          প্রত্যাগম false;
        }
      ভঙ্গ;

    স্বভাব:
      ভঙ্গ;
    }

  bool do_chown;
  সংরচনা stat stat_buf;
  সংরচনা stat ধ্রুব *file_stats;
  যদি (!ok)
    {
      do_chown = false;
      file_stats = রিক্ত;
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
                প্রত্যাগম true;
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
      প্রত্যাগম false;
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
          বিকল্প (err)
            {
            পক্ষ RC_ok:
              ভঙ্গ;

            পক্ষ RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              ভঙ্গ;

            পক্ষ RC_error:
              ok = false;
              ভঙ্গ;

            পক্ষ RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            পক্ষ RC_excluded:
              do_chown = false;
              ok = false;
              ভঙ্গ;

            স্বভাব:
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
            প্রত্যাগম true;
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
          বর্ণ *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : রিক্ত);
          বর্ণ *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : রিক্ত);
          বর্ণ *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : রিক্ত;
          বর্ণ *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : রিক্ত;
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

  প্রত্যাগম ok;
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
বাহ্য bool
chown_files (বর্ণ **files, পূর্ণাঙ্ক bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             সংরচনা Chown_option ধ্রুব *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  পূর্ণাঙ্ক stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, রিক্ত);

  যাবত্ (true)
    {
      FTSENT *ent = fts_read (fts);

      যদি (ent == রিক্ত)
        {
          যদি (errno != 0)
            {
              /* FIXME: try to give a better message  */
              যদি (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ভঙ্গ;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  যদি (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  প্রত্যাগম ok;
}
