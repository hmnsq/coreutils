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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "chown-core.h"
#అన్తర్భావ "fts-missing.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "xfts.h"

#నిర్వచించు FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

గణన RCH_స్థితి
  {
    /* we called fchown and close, and both succeeded */
    RC_ok = 2,

    /* required_uid and/or required_gid are specified, but don't match */
    RC_excluded,

    /* The file was replaced by another file during the requested change.  */
    RC_inode_మారింది,

    /* open/fchown isn't needed, isn't safe, or doesn't work due to
       permissions problems; fall back on chown */
    RC_do_ordinary_chown,

    /* open, fstat, fchown, or close failed */
    RC_error
  };

బాహ్య శూన్య
chopt_init (నిర్మాణం Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->మూలం_dev_ino = ఖాళీ;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ఖాళీ;
  chopt->సమూహం_name = ఖాళీ;
}

బాహ్య శూన్య
chopt_free (నిర్మాణం Chown_option *chopt)
{
  విడుదల (chopt->user_name);
  విడుదల (chopt->సమూహం_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

స్థిర అక్షరం *
uid_గమ్యం_str (uid_t uid)
{
  అక్షరం buf[INT_BUFSIZE_BOUND (intmax_t)];
  తిరిగి xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

స్థిర అక్షరం *
gid_గమ్యం_str (gid_t gid)
{
  అక్షరం buf[INT_BUFSIZE_BOUND (intmax_t)];
  తిరిగి xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

బాహ్య అక్షరం *
gid_గమ్యం_name (gid_t gid)
{
  నిర్మాణం సమూహం *grp = getgrgid (gid);
  తిరిగి grp ? xstrdup (grp->gr_name) : gid_గమ్యం_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

బాహ్య అక్షరం *
uid_గమ్యం_name (uid_t uid)
{
  నిర్మాణం passwd *pwd = getpwuid (uid);
  తిరిగి pwd ? xstrdup (pwd->pw_name) : uid_గమ్యం_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

స్థిర అక్షరం *
user_సమూహం_str (అక్షరం స్థిరరాశి *user, అక్షరం స్థిరరాశి *సమూహం)
{
  అక్షరం *spec = ఖాళీ;

  ఒకవేళ (user)
    {
      ఒకవేళ (సమూహం)
        {
          spec = xmalloc (అక్షరశ్రేణి_పొడవు (user) + 1 + అక్షరశ్రేణి_పొడవు (సమూహం) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), సమూహం);
        }
      లేకపోతే
        {
          spec = xstrdup (user);
        }
    }
  లేకపోతే ఒకవేళ (సమూహం)
    {
      spec = xstrdup (సమూహం);
    }

  తిరిగి spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

స్థిర శూన్య
describe_change (అక్షరం స్థిరరాశి *file, గణన Change_స్థితి మారింది,
                 అక్షరం స్థిరరాశి *old_user, అక్షరం స్థిరరాశి *old_సమూహం,
                 అక్షరం స్థిరరాశి *user, అక్షరం స్థిరరాశి *సమూహం)
{
  ఒకవేళ (మారింది == CH_NOT_APPLIED)
    {
      రాయి (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      తిరిగి;
    }

  అక్షరం *spec = user_సమూహం_str (user, సమూహం);
  అక్షరం *old_spec = user_సమూహం_str (user ? old_user : ఖాళీ,
                                   సమూహం ? old_సమూహం : ఖాళీ);

  అక్షరం స్థిరరాశి *fmt;
  ఎంపిక (మారింది)
    {
    స్థితి CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : సమూహం ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      ఆపు;
    స్థితి CH_FAILED:
      ఒకవేళ (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : సమూహం ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      లేకపోతే
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : సమూహం ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          విడుదల (old_spec);
          old_spec = spec;
          spec = ఖాళీ;
        }
      ఆపు;
    స్థితి CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : సమూహం ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      ఆపు;
    స్థితి CH_NOT_APPLIED:
    అప్రమేయ:
      affirm (false);
    }

  రాయి (fmt, quoteaf (file), old_spec, spec);

  విడుదల (old_spec);
  విడుదల (spec);
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

స్థిర గణన RCH_స్థితి
restricted_chown (పూర్ణాంకం cwd_fd, అక్షరం స్థిరరాశి *file,
                  నిర్మాణం stat స్థిరరాశి *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  ఒకవేళ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    తిరిగి RC_do_ordinary_chown;

  పూర్ణాంకం open_flags = O_NONBLOCK | O_NOCTTY;
  ఒకవేళ (! S_ISREG (orig_st->st_బహుళకం))
    {
      ఒకవేళ (S_ISDIR (orig_st->st_బహుళకం))
        open_flags |= O_DIRECTORY;
      లేకపోతే
        తిరిగి RC_do_ordinary_chown;
    }

  పూర్ణాంకం fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  ఒకవేళ (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_బహుళకం)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    తిరిగి (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  గణన RCH_స్థితి స్థితి_2 = RC_ok;
  నిర్మాణం stat st;
  ఒకవేళ (fstat (fd, &st) != 0)
    స్థితి_2 = RC_error;
  లేకపోతే ఒకవేళ (! psame_inode (orig_st, &st))
    స్థితి_2 = RC_inode_మారింది;
  లేకపోతే ఒకవేళ ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#ఒకవేళ HAVE_FCHOWN
      ఒకవేళ (fchown (fd, uid, gid) == 0)
        తిరిగి close (fd) < 0 ? RC_error : RC_ok;
#అయితే_ముగింపు
      స్థితి_2 = RC_error;
    }

  పూర్ణాంకం saved_errno = errno;
  close (fd);
  errno = saved_errno;
  తిరిగి స్థితి_2;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
స్థిర bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   నిర్మాణం Chown_option స్థిరరాశి *chopt)
{
  అక్షరం స్థిరరాశి *file_నిండిన_name = ent->fts_మార్గం;
  అక్షరం స్థిరరాశి *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_నమోదు (ent);

  ఒకవేళ (ignore_missing && ignorable_fts_error (ent))
    తిరిగి true;

  ఎంపిక (ent->fts_info)
    {
    స్థితి FTS_D:
      ఒకవేళ (chopt->recurse)
        {
          ఒకవేళ (మూలం_DEV_INO_CHECK (chopt->మూలం_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              మూలం_DEV_INO_WARN (file_నిండిన_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_సమితి (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_విలువ (fts_read (fts));
              తిరిగి false;
            }
          తిరిగి true;
        }
      ఆపు;

    స్థితి FTS_DP:
      ఒకవేళ (! chopt->recurse)
        తిరిగి true;
      ఆపు;

    స్థితి FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ఒకవేళ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_సంఖ్య == 0)
        {
          ent->fts_సంఖ్య = 1;
          fts_సమితి (fts, ent, FTS_AGAIN);
          తిరిగి true;
        }
      ఒకవేళ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_నిండిన_name));
      ok = false;
      ఆపు;

    స్థితి FTS_ERR:
      ఒకవేళ (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_నిండిన_name));
      ok = false;
      ఆపు;

    స్థితి FTS_DNR:
      ఒకవేళ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_నిండిన_name));
      ok = false;
      ఆపు;

    స్థితి FTS_DC:		/* directory that causes cycles */
      ఒకవేళ (చక్రం_warning_required (fts, ent))
        {
          emit_చక్రం_warning (file_నిండిన_name);
          తిరిగి false;
        }
      ఆపు;

    అప్రమేయ:
      ఆపు;
    }

  bool do_chown;
  నిర్మాణం stat stat_buf;
  నిర్మాణం stat స్థిరరాశి *file_stats;
  ఒకవేళ (!ok)
    {
      do_chown = false;
      file_stats = ఖాళీ;
    }
  లేకపోతే ఒకవేళ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->మూలం_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  లేకపోతే
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      ఒకవేళ (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_బహుళకం))
        {
          ఒకవేళ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ఒకవేళ (ignore_missing
                  && ignorable_traversal_errno (errno))
                తిరిగి true;
              ఒకవేళ (! chopt->force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_నిండిన_name));
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
  ఒకవేళ (ok
      && FTSENT_IS_DIRECTORY (ent)
      && మూలం_DEV_INO_CHECK (chopt->మూలం_dev_ino, file_stats))
    {
      మూలం_DEV_INO_WARN (file_నిండిన_name);
      తిరిగి false;
    }

  bool symlink_మారింది = true;
  ఒకవేళ (do_chown)
    {
      ఒకవేళ ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          ఒకవేళ (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_మారింది = false;
            }
        }
      లేకపోతే
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

          గణన RCH_స్థితి err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          ఎంపిక (err)
            {
            స్థితి RC_ok:
              ఆపు;

            స్థితి RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              ఆపు;

            స్థితి RC_error:
              ok = false;
              ఆపు;

            స్థితి RC_inode_మారింది:
              /* FIXME: give a diagnostic in this case?  */
            స్థితి RC_excluded:
              do_chown = false;
              ok = false;
              ఆపు;

            అప్రమేయ:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      ఒకవేళ (do_chown && !ok)
        {
          ఒకవేళ (ignore_missing
              && ignorable_traversal_errno (errno))
            తిరిగి true;
          ఒకవేళ (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_నిండిన_name));
        }
    }

  ఒకవేళ (chopt->verbosity != V_off)
    {
      bool మారింది =
        ((do_chown && ok && symlink_మారింది)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      ఒకవేళ (మారింది || chopt->verbosity == V_ఎగువ)
        {
          గణన Change_స్థితి ch_స్థితి =
            (!ok ? CH_FAILED
             : !symlink_మారింది ? CH_NOT_APPLIED
             : !మారింది ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          అక్షరం *old_usr = (file_stats
                           ? uid_గమ్యం_name (file_stats->st_uid) : ఖాళీ);
          అక్షరం *old_grp = (file_stats
                           ? gid_గమ్యం_name (file_stats->st_gid) : ఖాళీ);
          అక్షరం *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_గమ్యం_str (uid) : ఖాళీ;
          అక్షరం *new_grp = chopt->సమూహం_name
                          ? chopt->సమూహం_name : gid != -1
                                               ? gid_గమ్యం_str (gid) : ఖాళీ;
          describe_change (file_నిండిన_name, ch_స్థితి,
                           old_usr, old_grp,
                           new_usr, new_grp);
          విడుదల (old_usr);
          విడుదల (old_grp);
          ఒకవేళ (new_usr != chopt->user_name)
            విడుదల (new_usr);
          ఒకవేళ (new_grp != chopt->సమూహం_name)
            విడుదల (new_grp);
        }
    }

  ఒకవేళ ( ! chopt->recurse)
    fts_సమితి (fts, ent, FTS_SKIP);

  తిరిగి ok;
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
బాహ్య bool
chown_files (అక్షరం **files, పూర్ణాంకం బిట్టు_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             నిర్మాణం Chown_option స్థిరరాశి *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  పూర్ణాంకం stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, బిట్టు_flags | stat_flags, ఖాళీ);

  అయితే (true)
    {
      FTSENT *ent = fts_read (fts);

      ఒకవేళ (ent == ఖాళీ)
        {
          ఒకవేళ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ఒకవేళ (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ఆపు;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  ఒకవేళ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  తిరిగి ok;
}
