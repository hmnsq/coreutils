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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "chown-core.h"
#अन्तर्भाव "fts-missing.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "xfts.h"

#define FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

गणना RCH_status
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

बाह्य शून्य
chopt_init (संरचना Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->मूल_dev_ino = रिक्त;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = रिक्त;
  chopt->group_name = रिक्त;
}

बाह्य शून्य
chopt_free (संरचना Chown_option *chopt)
{
  मुक्त (chopt->user_name);
  मुक्त (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

स्थिर वर्ण *
uid_to_str (uid_t uid)
{
  वर्ण buf[INT_BUFSIZE_BOUND (intmax_t)];
  प्रत्यागम xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

स्थिर वर्ण *
gid_to_str (gid_t gid)
{
  वर्ण buf[INT_BUFSIZE_BOUND (intmax_t)];
  प्रत्यागम xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

बाह्य वर्ण *
gid_to_name (gid_t gid)
{
  संरचना group *grp = getgrgid (gid);
  प्रत्यागम grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

बाह्य वर्ण *
uid_to_name (uid_t uid)
{
  संरचना passwd *pwd = getpwuid (uid);
  प्रत्यागम pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

स्थिर वर्ण *
user_group_str (वर्ण ध्रुव *user, वर्ण ध्रुव *group)
{
  वर्ण *spec = रिक्त;

  यदि (user)
    {
      यदि (group)
        {
          spec = xmalloc (म_माप (user) + 1 + म_माप (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      अन्यथा
        {
          spec = xstrdup (user);
        }
    }
  अन्यथा यदि (group)
    {
      spec = xstrdup (group);
    }

  प्रत्यागम spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

स्थिर शून्य
describe_change (वर्ण ध्रुव *file, गणना Change_status changed,
                 वर्ण ध्रुव *old_user, वर्ण ध्रुव *old_group,
                 वर्ण ध्रुव *user, वर्ण ध्रुव *group)
{
  यदि (changed == CH_NOT_APPLIED)
    {
      म_लिखो (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      प्रत्यागम;
    }

  वर्ण *spec = user_group_str (user, group);
  वर्ण *old_spec = user_group_str (user ? old_user : रिक्त,
                                   group ? old_group : रिक्त);

  वर्ण ध्रुव *fmt;
  विकल्प (changed)
    {
    पक्ष CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      भङ्ग;
    पक्ष CH_FAILED:
      यदि (old_spec)
        {
          fmt = (user ? _("failed to change ownership of %s from %s to %s\n")
                 : group ? _("failed to change group of %s from %s to %s\n")
                 : _("failed to change ownership of %s\n"));
        }
      अन्यथा
        {
          fmt = (user ? _("failed to change ownership of %s to %s\n")
                 : group ? _("failed to change group of %s to %s\n")
                 : _("failed to change ownership of %s\n"));
          मुक्त (old_spec);
          old_spec = spec;
          spec = रिक्त;
        }
      भङ्ग;
    पक्ष CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      भङ्ग;
    पक्ष CH_NOT_APPLIED:
    स्वभाव:
      affirm (false);
    }

  म_लिखो (fmt, quoteaf (file), old_spec, spec);

  मुक्त (old_spec);
  मुक्त (spec);
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

स्थिर गणना RCH_status
restricted_chown (पूर्णाङ्क cwd_fd, वर्ण ध्रुव *file,
                  संरचना stat ध्रुव *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  यदि (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    प्रत्यागम RC_do_ordinary_chown;

  पूर्णाङ्क open_flags = O_NONBLOCK | O_NOCTTY;
  यदि (! S_ISREG (orig_st->st_mode))
    {
      यदि (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      अन्यथा
        प्रत्यागम RC_do_ordinary_chown;
    }

  पूर्णाङ्क fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  यदि (! (0 <= fd
         || (त्रुटि_सं == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    प्रत्यागम (त्रुटि_सं == EACCES ? RC_do_ordinary_chown : RC_error);

  गणना RCH_status status = RC_ok;
  संरचना stat st;
  यदि (ख_स्थिति (fd, &st) != 0)
    status = RC_error;
  अन्यथा यदि (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  अन्यथा यदि ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#यदि HAVE_FCHOWN
      यदि (fchown (fd, uid, gid) == 0)
        प्रत्यागम बंद (fd) < 0 ? RC_error : RC_ok;
#endif
      status = RC_error;
    }

  पूर्णाङ्क saved_errno = त्रुटि_सं;
  बंद (fd);
  त्रुटि_सं = saved_errno;
  प्रत्यागम status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
स्थिर bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   संरचना Chown_option ध्रुव *chopt)
{
  वर्ण ध्रुव *file_full_name = ent->fts_path;
  वर्ण ध्रुव *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  यदि (ignore_missing && ignorable_fts_error (ent))
    प्रत्यागम true;

  विकल्प (ent->fts_info)
    {
    पक्ष FTS_D:
      यदि (chopt->recurse)
        {
          यदि (मूल_DEV_INO_CHECK (chopt->मूल_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              मूल_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_मूल्य (fts_read (fts));
              प्रत्यागम false;
            }
          प्रत्यागम true;
        }
      भङ्ग;

    पक्ष FTS_DP:
      यदि (! chopt->recurse)
        प्रत्यागम true;
      भङ्ग;

    पक्ष FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      यदि (ent->fts_level == FTS_ROOTLEVEL && ent->fts_संख्या == 0)
        {
          ent->fts_संख्या = 1;
          fts_set (fts, ent, FTS_AGAIN);
          प्रत्यागम true;
        }
      यदि (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      भङ्ग;

    पक्ष FTS_ERR:
      यदि (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      भङ्ग;

    पक्ष FTS_DNR:
      यदि (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      भङ्ग;

    पक्ष FTS_DC:		/* directory that causes cycles */
      यदि (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          प्रत्यागम false;
        }
      भङ्ग;

    स्वभाव:
      भङ्ग;
    }

  bool do_chown;
  संरचना stat stat_buf;
  संरचना stat ध्रुव *file_stats;
  यदि (!ok)
    {
      do_chown = false;
      file_stats = रिक्त;
    }
  अन्यथा यदि (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
           && chopt->verbosity == V_off
           && ! chopt->मूल_dev_ino
           && ! chopt->affect_symlink_referent)
    {
      do_chown = true;
      file_stats = ent->fts_statp;
    }
  अन्यथा
    {
      file_stats = ent->fts_statp;

      /* If this is a symlink and we're dereferencing them,
         stat it to get info on the referent.  */
      यदि (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          यदि (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              यदि (ignore_missing
                  && ignorable_traversal_errno (त्रुटि_सं))
                प्रत्यागम true;
              यदि (! chopt->force_silent)
                error (0, त्रुटि_सं, _("cannot dereference %s"),
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
  यदि (ok
      && FTSENT_IS_DIRECTORY (ent)
      && मूल_DEV_INO_CHECK (chopt->मूल_dev_ino, file_stats))
    {
      मूल_DEV_INO_WARN (file_full_name);
      प्रत्यागम false;
    }

  bool symlink_changed = true;
  यदि (do_chown)
    {
      यदि ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          यदि (!ok && is_ENOTSUP (त्रुटि_सं))
            {
              ok = true;
              symlink_changed = false;
            }
        }
      अन्यथा
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

          गणना RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          विकल्प (err)
            {
            पक्ष RC_ok:
              भङ्ग;

            पक्ष RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              भङ्ग;

            पक्ष RC_error:
              ok = false;
              भङ्ग;

            पक्ष RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            पक्ष RC_excluded:
              do_chown = false;
              ok = false;
              भङ्ग;

            स्वभाव:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      यदि (do_chown && !ok)
        {
          यदि (ignore_missing
              && ignorable_traversal_errno (त्रुटि_सं))
            प्रत्यागम true;
          यदि (! chopt->force_silent)
            error (0, त्रुटि_सं, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  यदि (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      यदि (changed || chopt->verbosity == V_ऊपरी)
        {
          गणना Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          वर्ण *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : रिक्त);
          वर्ण *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : रिक्त);
          वर्ण *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : रिक्त;
          वर्ण *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : रिक्त;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          मुक्त (old_usr);
          मुक्त (old_grp);
          यदि (new_usr != chopt->user_name)
            मुक्त (new_usr);
          यदि (new_grp != chopt->group_name)
            मुक्त (new_grp);
        }
    }

  यदि ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  प्रत्यागम ok;
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
बाह्य bool
chown_files (वर्ण **files, पूर्णाङ्क bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             संरचना Chown_option ध्रुव *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  पूर्णाङ्क stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, रिक्त);

  यावत् (true)
    {
      FTSENT *ent = fts_read (fts);

      यदि (ent == रिक्त)
        {
          यदि (त्रुटि_सं != 0)
            {
              /* FIXME: try to give a better message  */
              यदि (! chopt->force_silent)
                error (0, त्रुटि_सं, _("fts_read failed"));
              ok = false;
            }
          भङ्ग;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  यदि (fts_close (fts) != 0)
    {
      error (0, त्रुटि_सं, _("fts_close failed"));
      ok = false;
    }

  प्रत्यागम ok;
}
