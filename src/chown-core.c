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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <pwd.h>
#समावेश <grp.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "chown-core.h"
#समावेश "fts-missing.h"
#समावेश "ignore-value.h"
#समावेश "root-dev-ino.h"
#समावेश "xfts.h"

#घोषणा FTSENT_IS_DIRECTORY(E)	\
  ((E)->fts_info == FTS_D	\
   || (E)->fts_info == FTS_DC	\
   || (E)->fts_info == FTS_DP	\
   || (E)->fts_info == FTS_DNR)

क्रमागत RCH_status
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

बाह्य व्योम
chopt_init (काष्ठा Chown_option *chopt)
{
  chopt->verbosity = V_off;
  chopt->मूल_dev_ino = शून्य;
  chopt->affect_symlink_referent = true;
  chopt->recurse = false;
  chopt->force_silent = false;
  chopt->user_name = शून्य;
  chopt->group_name = शून्य;
}

बाह्य व्योम
chopt_free (काष्ठा Chown_option *chopt)
{
  मुक्त (chopt->user_name);
  मुक्त (chopt->group_name);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

अटल अक्षर *
uid_to_str (uid_t uid)
{
  अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];
  वापस xstrdup (TYPE_SIGNED (uid_t) ? imaxtostr (uid, buf)
                  : umaxtostr (uid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  Use the decimal representation of the ID.  */

अटल अक्षर *
gid_to_str (gid_t gid)
{
  अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];
  वापस xstrdup (TYPE_SIGNED (gid_t) ? imaxtostr (gid, buf)
                  : umaxtostr (gid, buf));
}

/* Convert the numeric group-id, GID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding group name, use the decimal
   representation of the ID.  */

बाह्य अक्षर *
gid_to_name (gid_t gid)
{
  काष्ठा group *grp = getgrgid (gid);
  वापस grp ? xstrdup (grp->gr_name) : gid_to_str (gid);
}

/* Convert the numeric user-id, UID, to a string stored in xmalloc'd memory,
   and return it.  If there's no corresponding user name, use the decimal
   representation of the ID.  */

बाह्य अक्षर *
uid_to_name (uid_t uid)
{
  काष्ठा passwd *pwd = getpwuid (uid);
  वापस pwd ? xstrdup (pwd->pw_name) : uid_to_str (uid);
}

/* Allocate a string representing USER and GROUP.  */

अटल अक्षर *
user_group_str (अक्षर स्थिर *user, अक्षर स्थिर *group)
{
  अक्षर *spec = शून्य;

  अगर (user)
    {
      अगर (group)
        {
          spec = xmalloc (म_माप (user) + 1 + म_माप (group) + 1);
          stpcpy (stpcpy (stpcpy (spec, user), ":"), group);
        }
      अन्यथा
        {
          spec = xstrdup (user);
        }
    }
  अन्यथा अगर (group)
    {
      spec = xstrdup (group);
    }

  वापस spec;
}

/* Tell the user how/if the user and group of FILE have been changed.
   If USER is null, give the group-oriented messages.
   CHANGED describes what (if anything) has happened. */

अटल व्योम
describe_change (अक्षर स्थिर *file, क्रमागत Change_status changed,
                 अक्षर स्थिर *old_user, अक्षर स्थिर *old_group,
                 अक्षर स्थिर *user, अक्षर स्थिर *group)
{
  अगर (changed == CH_NOT_APPLIED)
    {
      म_लिखो (_("neither symbolic link %s nor referent has been changed\n"),
              quoteaf (file));
      वापस;
    }

  अक्षर *spec = user_group_str (user, group);
  अक्षर *old_spec = user_group_str (user ? old_user : शून्य,
                                   group ? old_group : शून्य);

  अक्षर स्थिर *fmt;
  चयन (changed)
    {
    हाल CH_SUCCEEDED:
      fmt = (user ? _("changed ownership of %s from %s to %s\n")
             : group ? _("changed group of %s from %s to %s\n")
             : _("no change to ownership of %s\n"));
      अवरोध;
    हाल CH_FAILED:
      अगर (old_spec)
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
          spec = शून्य;
        }
      अवरोध;
    हाल CH_NO_CHANGE_REQUESTED:
      fmt = (user ? _("ownership of %s retained as %s\n")
             : group ? _("group of %s retained as %s\n")
             : _("ownership of %s retained\n"));
      अवरोध;
    हाल CH_NOT_APPLIED:
    शेष:
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

अटल क्रमागत RCH_status
restricted_chown (पूर्णांक cwd_fd, अक्षर स्थिर *file,
                  काष्ठा stat स्थिर *orig_st,
                  uid_t uid, gid_t gid,
                  uid_t required_uid, gid_t required_gid)
{
  अगर (required_uid == (uid_t) -1 && required_gid == (gid_t) -1)
    वापस RC_do_ordinary_chown;

  पूर्णांक open_flags = O_NONBLOCK | O_NOCTTY;
  अगर (! S_ISREG (orig_st->st_mode))
    {
      अगर (S_ISDIR (orig_st->st_mode))
        open_flags |= O_DIRECTORY;
      अन्यथा
        वापस RC_do_ordinary_chown;
    }

  पूर्णांक fd = openat (cwd_fd, file, O_RDONLY | open_flags);
  अगर (! (0 <= fd
         || (त्रुटि_सं == EACCES && S_ISREG (orig_st->st_mode)
             && 0 <= (fd = openat (cwd_fd, file, O_WRONLY | open_flags)))))
    वापस (त्रुटि_सं == EACCES ? RC_do_ordinary_chown : RC_error);

  क्रमागत RCH_status status = RC_ok;
  काष्ठा stat st;
  अगर (ख_स्थिति (fd, &st) != 0)
    status = RC_error;
  अन्यथा अगर (! psame_inode (orig_st, &st))
    status = RC_inode_changed;
  अन्यथा अगर ((required_uid == (uid_t) -1 || required_uid == st.st_uid)
           && (required_gid == (gid_t) -1 || required_gid == st.st_gid))
    {
#अगर HAVE_FCHOWN
      अगर (fchown (fd, uid, gid) == 0)
        वापस बंद (fd) < 0 ? RC_error : RC_ok;
#पूर्ण_अगर
      status = RC_error;
    }

  पूर्णांक saved_errno = त्रुटि_सं;
  बंद (fd);
  त्रुटि_सं = saved_errno;
  वापस status;
}

/* Change the owner and/or group of the file specified by FTS and ENT
   to UID and/or GID as appropriate.
   If REQUIRED_UID is not -1, then skip files with any other user ID.
   If REQUIRED_GID is not -1, then skip files with any other group ID.
   CHOPT specifies additional options.
   Return true if successful.  */
अटल bool
change_file_owner (FTS *fts, FTSENT *ent,
                   uid_t uid, gid_t gid,
                   uid_t required_uid, gid_t required_gid,
                   काष्ठा Chown_option स्थिर *chopt)
{
  अक्षर स्थिर *file_full_name = ent->fts_path;
  अक्षर स्थिर *file = ent->fts_accpath;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  अगर (ignore_missing && ignorable_fts_error (ent))
    वापस true;

  चयन (ent->fts_info)
    {
    हाल FTS_D:
      अगर (chopt->recurse)
        {
          अगर (मूल_DEV_INO_CHECK (chopt->मूल_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chown -R --preserve-root 0 /"
                 and with "chown -RH --preserve-root 0 symlink-to-root".  */
              मूल_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_मूल्य (fts_read (fts));
              वापस false;
            }
          वापस true;
        }
      अवरोध;

    हाल FTS_DP:
      अगर (! chopt->recurse)
        वापस true;
      अवरोध;

    हाल FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      अगर (ent->fts_level == FTS_ROOTLEVEL && ent->fts_संख्या == 0)
        {
          ent->fts_संख्या = 1;
          fts_set (fts, ent, FTS_AGAIN);
          वापस true;
        }
      अगर (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ok = false;
      अवरोध;

    हाल FTS_ERR:
      अगर (! chopt->force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      अवरोध;

    हाल FTS_DNR:
      अगर (! chopt->force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ok = false;
      अवरोध;

    हाल FTS_DC:		/* directory that causes cycles */
      अगर (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          वापस false;
        }
      अवरोध;

    शेष:
      अवरोध;
    }

  bool do_chown;
  काष्ठा stat stat_buf;
  काष्ठा stat स्थिर *file_stats;
  अगर (!ok)
    {
      do_chown = false;
      file_stats = शून्य;
    }
  अन्यथा अगर (required_uid == (uid_t) -1 && required_gid == (gid_t) -1
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
      अगर (chopt->affect_symlink_referent && S_ISLNK (file_stats->st_mode))
        {
          अगर (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              अगर (ignore_missing
                  && ignorable_traversal_errno (त्रुटि_सं))
                वापस true;
              अगर (! chopt->force_silent)
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
  अगर (ok
      && FTSENT_IS_DIRECTORY (ent)
      && मूल_DEV_INO_CHECK (chopt->मूल_dev_ino, file_stats))
    {
      मूल_DEV_INO_WARN (file_full_name);
      वापस false;
    }

  bool symlink_changed = true;
  अगर (do_chown)
    {
      अगर ( ! chopt->affect_symlink_referent)
        {
          ok = (lchownat (fts->fts_cwd_fd, file, uid, gid) == 0);

          /* Ignore any error due to lack of support; POSIX requires
             this behavior for top-level symbolic links with -h, and
             implies that it's required for all symbolic links.  */
          अगर (!ok && is_ENOTSUP (त्रुटि_सं))
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

          क्रमागत RCH_status err
            = restricted_chown (fts->fts_cwd_fd, file, file_stats, uid, gid,
                                required_uid, required_gid);
          चयन (err)
            {
            हाल RC_ok:
              अवरोध;

            हाल RC_do_ordinary_chown:
              ok = (chownat (fts->fts_cwd_fd, file, uid, gid) == 0);
              अवरोध;

            हाल RC_error:
              ok = false;
              अवरोध;

            हाल RC_inode_changed:
              /* FIXME: give a diagnostic in this case?  */
            हाल RC_excluded:
              do_chown = false;
              ok = false;
              अवरोध;

            शेष:
              unreachable ();
            }
        }

      /* On some systems (e.g., GNU/Linux 2.4.x),
         the chown function resets the 'special' permission bits.
         Do *not* restore those bits;  doing so would open a window in
         which a malicious user, M, could subvert a chown command run
         by some other user and operating on files in a directory
         where M has write access.  */

      अगर (do_chown && !ok)
        {
          अगर (ignore_missing
              && ignorable_traversal_errno (त्रुटि_सं))
            वापस true;
          अगर (! chopt->force_silent)
            error (0, त्रुटि_सं, (uid != (uid_t) -1
                              ? _("changing ownership of %s")
                              : _("changing group of %s")),
                   quoteaf (file_full_name));
        }
    }

  अगर (chopt->verbosity != V_off)
    {
      bool changed =
        ((do_chown && ok && symlink_changed)
         && ! ((uid == (uid_t) -1 || uid == file_stats->st_uid)
               && (gid == (gid_t) -1 || gid == file_stats->st_gid)));

      अगर (changed || chopt->verbosity == V_ऊपरी)
        {
          क्रमागत Change_status ch_status =
            (!ok ? CH_FAILED
             : !symlink_changed ? CH_NOT_APPLIED
             : !changed ? CH_NO_CHANGE_REQUESTED
             : CH_SUCCEEDED);
          अक्षर *old_usr = (file_stats
                           ? uid_to_name (file_stats->st_uid) : शून्य);
          अक्षर *old_grp = (file_stats
                           ? gid_to_name (file_stats->st_gid) : शून्य);
          अक्षर *new_usr = chopt->user_name
                          ? chopt->user_name : uid != -1
                                               ? uid_to_str (uid) : शून्य;
          अक्षर *new_grp = chopt->group_name
                          ? chopt->group_name : gid != -1
                                               ? gid_to_str (gid) : शून्य;
          describe_change (file_full_name, ch_status,
                           old_usr, old_grp,
                           new_usr, new_grp);
          मुक्त (old_usr);
          मुक्त (old_grp);
          अगर (new_usr != chopt->user_name)
            मुक्त (new_usr);
          अगर (new_grp != chopt->group_name)
            मुक्त (new_grp);
        }
    }

  अगर ( ! chopt->recurse)
    fts_set (fts, ent, FTS_SKIP);

  वापस ok;
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
chown_files (अक्षर **files, पूर्णांक bit_flags,
             uid_t uid, gid_t gid,
             uid_t required_uid, gid_t required_gid,
             काष्ठा Chown_option स्थिर *chopt)
{
  bool ok = true;

  /* Use lstat and stat only if they're needed.  */
  पूर्णांक stat_flags = ((required_uid != (uid_t) -1 || required_gid != (gid_t) -1
                     || chopt->affect_symlink_referent
                     || chopt->verbosity != V_off)
                    ? 0
                    : FTS_NOSTAT);

  FTS *fts = xfts_open (files, bit_flags | stat_flags, शून्य);

  जबतक (true)
    {
      FTSENT *ent = fts_read (fts);

      अगर (ent == शून्य)
        {
          अगर (त्रुटि_सं != 0)
            {
              /* FIXME: try to give a better message  */
              अगर (! chopt->force_silent)
                error (0, त्रुटि_सं, _("fts_read failed"));
              ok = false;
            }
          अवरोध;
        }

      ok &= change_file_owner (fts, ent, uid, gid,
                               required_uid, required_gid, chopt);
    }

  अगर (fts_close (fts) != 0)
    {
      error (0, त्रुटि_सं, _("fts_close failed"));
      ok = false;
    }

  वापस ok;
}
