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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "chown-core.h"
#અન્તર્ભાવ "fts-missing.h"
#અન્તર્ભાવ "ignore-value.h"
#અન્તર્ભાવ "root-dev-ino.h"
#અન્તર્ભાવ "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

ગણના RCH_status
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

બાહ્ય શૂન્ય
chopt_init (સંરચના Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->મૂળ_dev_ino = ખાલી;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ખાલી;
  chopt->group_name = ખાલી;
}

બાહ્ય શૂન્ય
chopt_free (સંરચના Chown_option *chopt)
{
  મુક્ત (chopt->user_name);
  મુક્ત (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

સ્થિર અક્ષર *
uid_to_str (uid_t uid)
{
  અક્ષર buf[INT_BUFSIZE_BOUND (intmax_t)];
  પાછા xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

સ્થિર અક્ષર *
gid_to_str (gid_t gid)
{
  અક્ષર buf[INT_BUFSIZE_BOUND (intmax_t)];
  પાછા xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

બાહ્ય અક્ષર *
gid_to_name (gid_t gid)
{
  સંરચના group *grp = getgrgid (gid);
  પાછા grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

બાહ્ય અક્ષર *
uid_to_name (uid_t uid)
{
  સંરચના passwd *pwd = getpwuid (uid);
  પાછા pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

સ્થિર અક્ષર *
user_group_str (અક્ષર અચલ *user, અક્ષર અચલ *group)
{
  અક્ષર *spec = ખાલી;

  જો (user)
    {
      જો (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      નહીંતર
        {
          spec = xstrdup (user);
        }
    }
  નહીંતર જો (group)
    {
      spec = xstrdup (group);
    }

  પાછા spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

સ્થિર શૂન્ય
describe_change (અક્ષર અચલ *file, ગણના Change_status changed,
                 અક્ષર અચલ *old_user, અક્ષર અચલ *old_group,
                 અક્ષર અચલ *user, અક્ષર અચલ *group)
{
  જો (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      પાછા;
    }

  અક્ષર *spec = user_group_str (user, group);
  અક્ષર *old_spec = user_group_str (user ? old_user : ખાલી,
                                   group ? old_group : ખાલી);

  અક્ષર અચલ *fmt;
  પસંદગી (changed)
    {
    સ્થિતિ CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      તોડો;
    સ્થિતિ CH_FAILED:
      જો (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      નહીંતર
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          મુક્ત (old_spec);
          old_spec = spec;
          spec = ખાલી;
        }
      તોડો;
    સ્થિતિ CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      તોડો;
    સ્થિતિ CH_NOT_APPLIED:
    મૂળભૂત:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  મુક્ત (old_spec);
  મુક્ત (spec);
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

સ્થિર ગણના RCH_status
restricted_chown (પૂર્ણાંક cwd_fd, અક્ષર અચલ *file,
                  સંરચના stat અચલ *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  જો (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    પાછા RC_do_ordinary_chown;

  પૂર્ણાંક open_flags = O_NONBLOCK | O_NOCTTY;
  જો (! S_ISREG (orig_st->st_mode))
    {
      જો (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      નહીંતર
        પાછા RC_do_ordinary_chown;
    }

  પૂર્ણાંક fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  જો (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    પાછા (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ગણના RCH_status status = RC_ok;
  સંરચના stat st;
  જો (fstat (fd, &st) != 0)
    status = RC_error;
  નહીંતર જો (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  નહીંતર જો ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#જો HAVE_FCHOWN
      જો (fchown (fd, uid, gid) == 0)
        પાછા close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  પૂર્ણાંક saved_errno = errno;
  close (fd);
  errno = saved_errno;
  પાછા status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
સ્થિર bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   સંરચના Chown_option અચલ *chopt)
{
  અક્ષર અચલ *file_full_name = ent->fts_path;
  અક્ષર અચલ *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  જો (ignore_missing && ignorable_fts_error (ent))
    પાછા true;

  પસંદગી (ent->fts_info)
    {
    સ્થિતિ FTS_D:
      જો (chopt->recurse)
        {
          જો (મૂળ_DEV_INO_CHECK (chopt->મૂળ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              મૂળ_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_કિંમત (fts_read (fts));
              પાછા false;
            }
          પાછા true;
        }
      તોડો;

    સ્થિતિ FTS_DP:
      જો (! chopt->recurse)
        પાછા true;
      તોડો;

    સ્થિતિ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      જો (ent->fts_level == FTS_ROOTLEVEL && ent->fts_સંખ્યા == 0)
        {
          ent->fts_સંખ્યા = 1;
          fts_set (fts, ent, FTS_AGAIN);
          પાછા true;
        }
      જો (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      તોડો;

    સ્થિતિ FTS_ERR:
      જો (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      તોડો;

    સ્થિતિ FTS_DNR:
      જો (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      તોડો;

    સ્થિતિ FTS_DC:		/* directory that causes cycles */
      જો (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          પાછા false;
        }
      તોડો;

    મૂળભૂત:
      તોડો;
    }

  bool do_chown;
  સંરચના stat stat_buf;
  સંરચના stat અચલ *file_stats;
  જો (!ok)
    {
      do_chown = false;
      file_stats = ખાલી;
    }
  નહીંતર જો (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->મૂળ_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  નહીંતર
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      જો (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          જો (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              જો (ignore_missing
                  && ignorable_traversal_errno (errno))
                પાછા true;
              જો (! chopt->force_silent)
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
  જો (ok
      && FTSENT_IS_DIRECTORY (ent)
      && મૂળ_DEV_INO_CHECK (chopt->મૂળ_dev_ino, file_stats))
    {
      મૂળ_DEV_INO_WARN (file_full_name);
      પાછા false;
    }

  bool symlink_changed = true;
  જો (do_chown)
    {
      જો ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          જો (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      નહીંતર
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

          ગણના RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          પસંદગી (err)
            {
            સ્થિતિ RC_ok:
              તોડો;

            સ્થિતિ RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              તોડો;

            સ્થિતિ RC_error:
              ok = false;
              તોડો;

            સ્થિતિ RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            સ્થિતિ RC_excluded:
              do_chown = false;
              ok = false;
              તોડો;

            મૂળભૂત:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      જો (do_chown && !ok)
        {
          જો (ignore_missing
              && ignorable_traversal_errno (errno))
            પાછા true;
          જો (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  જો (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      જો (changed || chopt->verbosity == V_ઉપલું)
        {
          ગણના Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          અક્ષર *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : ખાલી);
          અક્ષર *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : ખાલી);
          અક્ષર *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : ખાલી;
          અક્ષર *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : ખાલી;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          મુક્ત (old_usr);
          મુક્ત (old_grp);
          જો (new_usr != chopt->user_name)
            મુક્ત (new_usr);
          જો (new_grp != chopt->group_name)
            મુક્ત (new_grp);
        }
    }

  જો ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  પાછા ok;
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
બાહ્ય bool
chown_files (અક્ષર **files, પૂર્ણાંક bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             સંરચના Chown_option અચલ *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  પૂર્ણાંક stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, ખાલી);

  જ્યારે (true)
    {
      FTSENT *ent = fts_read (fts);

      જો (ent == ખાલી)
        {
          જો (errno != 0)
            {
              /* FIXME: try to give a better message  */
              જો (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          તોડો;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  જો (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  પાછા ok;
}
