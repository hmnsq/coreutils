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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "chown-core.h"
#ଅନ୍ତର୍ଭାଵ "fts-missing.h"
#ଅନ୍ତର୍ଭାଵ "ignore-value.h"
#ଅନ୍ତର୍ଭାଵ "root-dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

ଗଣନା RCH_status
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

ବାହ୍ୟ ଶୂନ୍ୟ
chopt_init (ସଂରଚନା Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->ମୂଳ_dev_ino = ଖାଲି;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ଖାଲି;
  chopt->group_name = ଖାଲି;
}

ବାହ୍ୟ ଶୂନ୍ୟ
chopt_free (ସଂରଚନା Chown_option *chopt)
{
  ମୁକ୍ତ (chopt->user_name);
  ମୁକ୍ତ (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ସ୍ଥିର ଅକ୍ଷର *
uid_to_str (uid_t uid)
{
  ଅକ୍ଷର buf[INT_BUFSIZE_BOUND (intmax_t)];
  ଫେରନ୍ତୁ xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

ସ୍ଥିର ଅକ୍ଷର *
gid_to_str (gid_t gid)
{
  ଅକ୍ଷର buf[INT_BUFSIZE_BOUND (intmax_t)];
  ଫେରନ୍ତୁ xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

ବାହ୍ୟ ଅକ୍ଷର *
gid_to_name (gid_t gid)
{
  ସଂରଚନା group *grp = getgrgid (gid);
  ଫେରନ୍ତୁ grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

ବାହ୍ୟ ଅକ୍ଷର *
uid_to_name (uid_t uid)
{
  ସଂରଚନା passwd *pwd = getpwuid (uid);
  ଫେରନ୍ତୁ pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

ସ୍ଥିର ଅକ୍ଷର *
user_group_str (ଅକ୍ଷର ଅଚଳ *user, ଅକ୍ଷର ଅଚଳ *group)
{
  ଅକ୍ଷର *spec = ଖାଲି;

  ଯଦି (user)
    {
      ଯଦି (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      ଅନ୍ୟଥା
        {
          spec = xstrdup (user);
        }
    }
  ଅନ୍ୟଥା ଯଦି (group)
    {
      spec = xstrdup (group);
    }

  ଫେରନ୍ତୁ spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

ସ୍ଥିର ଶୂନ୍ୟ
describe_change (ଅକ୍ଷର ଅଚଳ *file, ଗଣନା Change_status changed,
                 ଅକ୍ଷର ଅଚଳ *old_user, ଅକ୍ଷର ଅଚଳ *old_group,
                 ଅକ୍ଷର ଅଚଳ *user, ଅକ୍ଷର ଅଚଳ *group)
{
  ଯଦି (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      ଫେରନ୍ତୁ;
    }

  ଅକ୍ଷର *spec = user_group_str (user, group);
  ଅକ୍ଷର *old_spec = user_group_str (user ? old_user : ଖାଲି,
                                   group ? old_group : ଖାଲି);

  ଅକ୍ଷର ଅଚଳ *fmt;
  ବାଛ (changed)
    {
    ସ୍ଥିତି CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CH_FAILED:
      ଯଦି (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      ଅନ୍ୟଥା
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          ମୁକ୍ତ (old_spec);
          old_spec = spec;
          spec = ଖାଲି;
        }
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CH_NOT_APPLIED:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  ମୁକ୍ତ (old_spec);
  ମୁକ୍ତ (spec);
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

ସ୍ଥିର ଗଣନା RCH_status
restricted_chown (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cwd_fd, ଅକ୍ଷର ଅଚଳ *file,
                  ସଂରଚନା stat ଅଚଳ *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  ଯଦି (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    ଫେରନ୍ତୁ RC_do_ordinary_chown;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags = O_NONBLOCK | O_NOCTTY;
  ଯଦି (! S_ISREG (orig_st->st_mode))
    {
      ଯଦି (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ RC_do_ordinary_chown;
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  ଯଦି (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    ଫେରନ୍ତୁ (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  ଗଣନା RCH_status status = RC_ok;
  ସଂରଚନା stat st;
  ଯଦି (fstat (fd, &st) != 0)
    status = RC_error;
  ଅନ୍ୟଥା ଯଦି (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  ଅନ୍ୟଥା ଯଦି ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#ଯଦି HAVE_FCHOWN
      ଯଦି (fchown (fd, uid, gid) == 0)
        ଫେରନ୍ତୁ close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
  close (fd);
  errno = saved_errno;
  ଫେରନ୍ତୁ status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
ସ୍ଥିର bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   ସଂରଚନା Chown_option ଅଚଳ *chopt)
{
  ଅକ୍ଷର ଅଚଳ *file_full_name = ent->fts_path;
  ଅକ୍ଷର ଅଚଳ *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ଯଦି (ignore_missing && ignorable_fts_error (ent))
    ଫେରନ୍ତୁ true;

  ବାଛ (ent->fts_info)
    {
    ସ୍ଥିତି FTS_D:
      ଯଦି (chopt->recurse)
        {
          ଯଦି (ମୂଳ_DEV_INO_CHECK (chopt->ମୂଳ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              ମୂଳ_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_ମୂଲ୍ୟ (fts_read (fts));
              ଫେରନ୍ତୁ false;
            }
          ଫେରନ୍ତୁ true;
        }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DP:
      ଯଦି (! chopt->recurse)
        ଫେରନ୍ତୁ true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ଯଦି (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ସଂଖ୍ୟା == 0)
        {
          ent->fts_ସଂଖ୍ୟା = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ଫେରନ୍ତୁ true;
        }
      ଯଦି (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_ERR:
      ଯଦି (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DNR:
      ଯଦି (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DC:		/* directory that causes cycles */
      ଯଦି (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ଫେରନ୍ତୁ false;
        }
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଭାଙ୍ଗ;
    }

  bool do_chown;
  ସଂରଚନା stat stat_buf;
  ସଂରଚନା stat ଅଚଳ *file_stats;
  ଯଦି (!ok)
    {
      do_chown = false;
      file_stats = ଖାଲି;
    }
  ଅନ୍ୟଥା ଯଦି (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->ମୂଳ_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  ଅନ୍ୟଥା
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      ଯଦି (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          ଯଦି (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ଯଦି (ignore_missing
                  && ignorable_traversal_errno (errno))
                ଫେରନ୍ତୁ true;
              ଯଦି (! chopt->force_silent)
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
  ଯଦି (ok
      && FTSENT_IS_DIRECTORY (ent)
      && ମୂଳ_DEV_INO_CHECK (chopt->ମୂଳ_dev_ino, file_stats))
    {
      ମୂଳ_DEV_INO_WARN (file_full_name);
      ଫେରନ୍ତୁ false;
    }

  bool symlink_changed = true;
  ଯଦି (do_chown)
    {
      ଯଦି ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          ଯଦି (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      ଅନ୍ୟଥା
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

          ଗଣନା RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          ବାଛ (err)
            {
            ସ୍ଥିତି RC_ok:
              ଭାଙ୍ଗ;

            ସ୍ଥିତି RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              ଭାଙ୍ଗ;

            ସ୍ଥିତି RC_error:
              ok = false;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            ସ୍ଥିତି RC_excluded:
              do_chown = false;
              ok = false;
              ଭାଙ୍ଗ;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      ଯଦି (do_chown && !ok)
        {
          ଯଦି (ignore_missing
              && ignorable_traversal_errno (errno))
            ଫେରନ୍ତୁ true;
          ଯଦି (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  ଯଦି (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      ଯଦି (changed || chopt->verbosity == V_ଉପର)
        {
          ଗଣନା Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          ଅକ୍ଷର *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : ଖାଲି);
          ଅକ୍ଷର *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : ଖାଲି);
          ଅକ୍ଷର *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : ଖାଲି;
          ଅକ୍ଷର *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : ଖାଲି;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          ମୁକ୍ତ (old_usr);
          ମୁକ୍ତ (old_grp);
          ଯଦି (new_usr != chopt->user_name)
            ମୁକ୍ତ (new_usr);
          ଯଦି (new_grp != chopt->group_name)
            ମୁକ୍ତ (new_grp);
        }
    }

  ଯଦି ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  ଫେରନ୍ତୁ ok;
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
ବାହ୍ୟ bool
chown_files (ଅକ୍ଷର **files, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             ସଂରଚନା Chown_option ଅଚଳ *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, ଖାଲି);

  ଯେତେବେଳେ (true)
    {
      FTSENT *ent = fts_read (fts);

      ଯଦି (ent == ଖାଲି)
        {
          ଯଦି (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ଯଦି (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ଭାଙ୍ଗ;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  ଯଦି (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ଫେରନ୍ତୁ ok;
}
