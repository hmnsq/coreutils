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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "chown-core.h"
#അന്തര്ഭാവ "fts-missing.h"
#അന്തര്ഭാവ "ignore-value.h"
#അന്തര്ഭാവ "root-dev-ino.h"
#അന്തര്ഭാവ "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

എണ്ണൽ RCH_status
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

ബാഹ്യ ശൂന്യം
chopt_init (ഘടന Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->വേര്_dev_ino = ശൂന്യ;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = ശൂന്യ;
  chopt->group_name = ശൂന്യ;
}

ബാഹ്യ ശൂന്യം
chopt_free (ഘടന Chown_option *chopt)
{
  സ്വതന്ത്രം (chopt->user_name);
  സ്വതന്ത്രം (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

സ്ഥിതം അക്ഷരം *
uid_to_str (uid_t uid)
{
  അക്ഷരം buf[INT_BUFSIZE_BOUND (intmax_t)];
  തിരികെ xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

സ്ഥിതം അക്ഷരം *
gid_to_str (gid_t gid)
{
  അക്ഷരം buf[INT_BUFSIZE_BOUND (intmax_t)];
  തിരികെ xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

ബാഹ്യ അക്ഷരം *
gid_to_name (gid_t gid)
{
  ഘടന group *grp = getgrgid (gid);
  തിരികെ grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

ബാഹ്യ അക്ഷരം *
uid_to_name (uid_t uid)
{
  ഘടന passwd *pwd = getpwuid (uid);
  തിരികെ pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

സ്ഥിതം അക്ഷരം *
user_group_str (അക്ഷരം സ്ഥിരം *user, അക്ഷരം സ്ഥിരം *group)
{
  അക്ഷരം *spec = ശൂന്യ;

  എങ്കിൽ (user)
    {
      എങ്കിൽ (group)
        {
          spec = xmalloc (strlen (user) + 1 + strlen (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      അല്ലെങ്കിൽ
        {
          spec = xstrdup (user);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (group)
    {
      spec = xstrdup (group);
    }

  തിരികെ spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

സ്ഥിതം ശൂന്യം
describe_change (അക്ഷരം സ്ഥിരം *file, എണ്ണൽ Change_status changed,
                 അക്ഷരം സ്ഥിരം *old_user, അക്ഷരം സ്ഥിരം *old_group,
                 അക്ഷരം സ്ഥിരം *user, അക്ഷരം സ്ഥിരം *group)
{
  എങ്കിൽ (changed == CH_NOT_APPLIED)
    {
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      തിരികെ;
    }

  അക്ഷരം *spec = user_group_str (user, group);
  അക്ഷരം *old_spec = user_group_str (user ? old_user : ശൂന്യ,
                                   group ? old_group : ശൂന്യ);

  അക്ഷരം സ്ഥിരം *fmt;
  തിരഞ്ഞെടുപ്പ് (changed)
    {
    അവസ്ഥ CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      നിർത്തുക;
    അവസ്ഥ CH_FAILED:
      എങ്കിൽ (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      അല്ലെങ്കിൽ
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          സ്വതന്ത്രം (old_spec);
          old_spec = spec;
          spec = ശൂന്യ;
        }
      നിർത്തുക;
    അവസ്ഥ CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      നിർത്തുക;
    അവസ്ഥ CH_NOT_APPLIED:
    സ്ഥിരസ്ഥിതി:
      affirm (false);
    }

  printf (fmt, quoteaf (file), old_spec, spec);

  സ്വതന്ത്രം (old_spec);
  സ്വതന്ത്രം (spec);
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

സ്ഥിതം എണ്ണൽ RCH_status
restricted_chown (പൂർണ്ണസംഖ്യ cwd_fd, അക്ഷരം സ്ഥിരം *file,
                  ഘടന stat സ്ഥിരം *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  എങ്കിൽ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    തിരികെ RC_do_ordinary_chown;

  പൂർണ്ണസംഖ്യ open_flags = O_NONBLOCK | O_NOCTTY;
  എങ്കിൽ (! S_ISREG (orig_st->st_mode))
    {
      എങ്കിൽ (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      അല്ലെങ്കിൽ
        തിരികെ RC_do_ordinary_chown;
    }

  പൂർണ്ണസംഖ്യ fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  എങ്കിൽ (! (0 <= fd
         || (errno == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    തിരികെ (errno == EACCES ? RC_do_ordinary_chown : RC_error);

  എണ്ണൽ RCH_status status = RC_ok;
  ഘടന stat st;
  എങ്കിൽ (fstat (fd, &st) != 0)
    status = RC_error;
  അല്ലെങ്കിൽ എങ്കിൽ (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  അല്ലെങ്കിൽ എങ്കിൽ ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#എങ്കിൽ HAVE_FCHOWN
      എങ്കിൽ (fchown (fd, uid, gid) == 0)
        തിരികെ close (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  പൂർണ്ണസംഖ്യ saved_errno = errno;
  close (fd);
  errno = saved_errno;
  തിരികെ status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
സ്ഥിതം bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   ഘടന Chown_option സ്ഥിരം *chopt)
{
  അക്ഷരം സ്ഥിരം *file_full_name = ent->fts_path;
  അക്ഷരം സ്ഥിരം *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  എങ്കിൽ (ignore_missing && ignorable_fts_error (ent))
    തിരികെ true;

  തിരഞ്ഞെടുപ്പ് (ent->fts_info)
    {
    അവസ്ഥ FTS_D:
      എങ്കിൽ (chopt->recurse)
        {
          എങ്കിൽ (വേര്_DEV_INO_CHECK (chopt->വേര്_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              വേര്_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_മൂല്യം (fts_read (fts));
              തിരികെ false;
            }
          തിരികെ true;
        }
      നിർത്തുക;

    അവസ്ഥ FTS_DP:
      എങ്കിൽ (! chopt->recurse)
        തിരികെ true;
      നിർത്തുക;

    അവസ്ഥ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      എങ്കിൽ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_സംഖ്യ == 0)
        {
          ent->fts_സംഖ്യ = 1;
          fts_set (fts, ent, FTS_AGAIN);
          തിരികെ true;
        }
      എങ്കിൽ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ FTS_ERR:
      എങ്കിൽ (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ FTS_DNR:
      എങ്കിൽ (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      നിർത്തുക;

    അവസ്ഥ FTS_DC:		/* directory that causes cycles */
      എങ്കിൽ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          തിരികെ false;
        }
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      നിർത്തുക;
    }

  bool do_chown;
  ഘടന stat stat_buf;
  ഘടന stat സ്ഥിരം *file_stats;
  എങ്കിൽ (!ok)
    {
      do_chown = false;
      file_stats = ശൂന്യ;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->വേര്_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  അല്ലെങ്കിൽ
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      എങ്കിൽ (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          എങ്കിൽ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              എങ്കിൽ (ignore_missing
                  && ignorable_traversal_errno (errno))
                തിരികെ true;
              എങ്കിൽ (! chopt->force_silent)
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
  എങ്കിൽ (ok
      && FTSENT_IS_DIRECTORY (ent)
      && വേര്_DEV_INO_CHECK (chopt->വേര്_dev_ino, file_stats))
    {
      വേര്_DEV_INO_WARN (file_full_name);
      തിരികെ false;
    }

  bool symlink_changed = true;
  എങ്കിൽ (do_chown)
    {
      എങ്കിൽ ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          എങ്കിൽ (!ok && is_ENOTSUP (errno))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      അല്ലെങ്കിൽ
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

          എണ്ണൽ RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          തിരഞ്ഞെടുപ്പ് (err)
            {
            അവസ്ഥ RC_ok:
              നിർത്തുക;

            അവസ്ഥ RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              നിർത്തുക;

            അവസ്ഥ RC_error:
              ok = false;
              നിർത്തുക;

            അവസ്ഥ RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            അവസ്ഥ RC_excluded:
              do_chown = false;
              ok = false;
              നിർത്തുക;

            സ്ഥിരസ്ഥിതി:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      എങ്കിൽ (do_chown && !ok)
        {
          എങ്കിൽ (ignore_missing
              && ignorable_traversal_errno (errno))
            തിരികെ true;
          എങ്കിൽ (! chopt->force_silent)
            error (0, errno, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  എങ്കിൽ (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      എങ്കിൽ (changed || chopt->verbosity == V_മുകളിലെ)
        {
          എണ്ണൽ Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          അക്ഷരം *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : ശൂന്യ);
          അക്ഷരം *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : ശൂന്യ);
          അക്ഷരം *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : ശൂന്യ;
          അക്ഷരം *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : ശൂന്യ;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          സ്വതന്ത്രം (old_usr);
          സ്വതന്ത്രം (old_grp);
          എങ്കിൽ (new_usr != chopt->user_name)
            സ്വതന്ത്രം (new_usr);
          എങ്കിൽ (new_grp != chopt->group_name)
            സ്വതന്ത്രം (new_grp);
        }
    }

  എങ്കിൽ ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  തിരികെ ok;
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
ബാഹ്യ bool
chown_files (അക്ഷരം **files, പൂർണ്ണസംഖ്യ bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             ഘടന Chown_option സ്ഥിരം *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  പൂർണ്ണസംഖ്യ stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, ശൂന്യ);

  ആകുമ്പോൾ (true)
    {
      FTSENT *ent = fts_read (fts);

      എങ്കിൽ (ent == ശൂന്യ)
        {
          എങ്കിൽ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              എങ്കിൽ (! chopt->force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          നിർത്തുക;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  എങ്കിൽ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  തിരികെ ok;
}
