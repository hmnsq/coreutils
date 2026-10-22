/* remove.c -- core functions for removing files and directories
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/* Extracted from rm.c, librarified, then rewritten twice by Jim Meyering.  */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "file-type.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "remove.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "root-dev-ino.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-any-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xfts.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "yesno.h"

/* The prompt function may be called twice for a given directory.
   The first time, we ask whether to descend into it, and the
   second time, we ask whether to remove it.  */
ਗਿਣਤੀ Prompt_action
  {
    PA_DESCEND_INTO_DIR = 2,
    PA_REMOVE_DIR
  };

/* D_TYPE(D) is the type of directory entry D if known, DT_UNKNOWN
   otherwise.  */
#ਜੇ ! HAVE_STRUCT_DIRENT_D_TYPE
/* Any int values will do here, so long as they're distinct.
   Undef any existing macros out of the way.  */
# undef DT_UNKNOWN
# undef DT_DIR
# undef DT_LNK
# define DT_UNKNOWN 0
# define DT_DIR 1
# define DT_LNK 2
#ਜੇ_ਅੰਤ

/* Like fstatat, but cache on POSIX-compatible systems.  */
ਸਥਿਰ ਪੂਰਨ
cache_fstatat (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ stat *st, ਪੂਰਨ flag)
{
#ਜੇ HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  /* If ST->st_atim.tv_nsec is -1, the status has not been gotten yet.
     If less than -1, fstatat failed with errno == ST->st_ino.
     Otherwise, the status has already been gotten, so return 0.  */
  ਜੇ (0 <= st->st_atim.tv_nsec)
    ਵਾਪਸ 0;
  ਜੇ (st->st_atim.tv_nsec == -1)
    {
      ਜੇ (fstatat (fd, file, st, flag) == 0)
        ਵਾਪਸ 0;
      st->st_atim.tv_nsec = -2;
      st->st_ino = errno;
    }
  errno = st->st_ino;
  ਵਾਪਸ -1;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ fstatat (fd, file, st, flag);
#ਜੇ_ਅੰਤ
}

/* Initialize a fstatat cache *ST.  Return ST for convenience.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ stat *
cache_stat_init (ਬਣਤਰ stat *st)
{
#ਜੇ HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  st->st_atim.tv_nsec = -1;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ st;
}

/* Return 1 if FILE is an unwritable non-symlink,
   0 if it is writable or some other type of file,
   -1 and set errno if there is some problem in determining the answer.
   Set *BUF to the file status.  */
ਸਥਿਰ ਪੂਰਨ
write_protected_non_symlink (ਪੂਰਨ fd_cwd,
                             ਅੱਖਰ ਅਚਲ *file,
                             ਬਣਤਰ stat *buf)
{
  ਜੇ (can_write_any_file ())
    ਵਾਪਸ 0;
  ਜੇ (cache_fstatat (fd_cwd, file, buf, AT_SYMLINK_NOFOLLOW) != 0)
    ਵਾਪਸ -1;
  ਜੇ (S_ISLNK (buf->st_ਬਹੁਲਕ))
    ਵਾਪਸ 0;
  /* Here, we know FILE is not a symbolic link.  */

  /* In order to be reentrant -- i.e., to avoid changing the working
     directory, and at the same time to be able to deal with alternate
     access control mechanisms (ACLs, xattr-style attributes) and
     arbitrarily deep trees -- we need a function like eaccessat, i.e.,
     like Solaris' eaccess, but fd-relative, in the spirit of openat.  */

  /* In the absence of a native eaccessat function, here are some of
     the implementation choices [#4 and #5 were suggested by Paul Eggert]:
     1) call openat with O_WRONLY|O_NOCTTY
        Disadvantage: may create the file and doesn't work for directory,
        may mistakenly report 'unwritable' for EROFS or ACLs even though
        perm bits say the file is writable.

     2) fake eaccessat (save_cwd, fchdir, call euidaccess, restore_cwd)
        Disadvantage: changes working directory (not reentrant) and can't
        work if save_cwd fails.

     3) if (euidaccess (full_name, W_OK) == 0)
        Disadvantage: doesn't work if full_name is too long.
        Inefficient for very deep trees (O(Depth^2)).

     4) If the full pathname is sufficiently short (say, less than
        PATH_MAX or 8192 bytes, whichever is shorter):
        use method (3) (i.e., euidaccess (full_name, W_OK));
        Otherwise: vfork, fchdir in the child, run euidaccess in the
        child, then the child exits with a status that tells the parent
        whether euidaccess succeeded.

        This avoids the O(N**2) algorithm of method (3), and it also avoids
        the failure-due-to-too-long-file-names of method (3), but it's fast
        in the normal shallow case.  It also avoids the lack-of-reentrancy
        and the save_cwd problems.
        Disadvantage; it uses a process slot for very-long file names,
        and would be very slow for hierarchies with many such files.

     5) If the full file name is sufficiently short (say, less than
        PATH_MAX or 8192 bytes, whichever is shorter):
        use method (3) (i.e., euidaccess (full_name, W_OK));
        Otherwise: look just at the file bits.  Perhaps issue a warning
        the first time this occurs.

        This is like (4), except for the "Otherwise" case where it isn't as
        "perfect" as (4) but is considerably faster.  It conforms to current
        POSIX, and is uniformly better than what Solaris and FreeBSD do (they
        mess up with long file names). */

  {
    ਜੇ (faccessat (fd_cwd, file, W_OK, AT_EACCESS) == 0)
      ਵਾਪਸ 0;

    ਵਾਪਸ errno == EACCES ? 1 : -1;
  }
}

/* Return the status of the directory identified by FTS and ENT.
   This is -1 if the directory is empty, 0 if it is nonempty,
   and a positive error number if there was trouble determining the status,
   e.g., it is not a directory, or permissions problems, or I/O errors.
   Use *DIR_STATUS as a cache for the status.  */
ਸਥਿਰ ਪੂਰਨ
get_dir_ਸਥਿਤੀ (FTS ਅਚਲ *fts, FTSENT ਅਚਲ *ent, ਪੂਰਨ *dir_ਸਥਿਤੀ)
{
  ਜੇ (*dir_ਸਥਿਤੀ == DS_UNKNOWN)
    *dir_ਸਥਿਤੀ = directory_ਸਥਿਤੀ (fts->fts_cwd_fd, ent->fts_accpath);
  ਵਾਪਸ *dir_ਸਥਿਤੀ;
}

/* Prompt whether to remove FILENAME, if required via a combination of
   the options specified by X and/or file attributes.  If the file may
   be removed, return RM_OK or RM_USER_ACCEPTED, the latter if the user
   was prompted and accepted.  If the user declines to remove the file,
   return RM_USER_DECLINED.  If not ignoring missing files and we
   cannot lstat FILENAME, then return RM_ERROR.

   IS_DIR is true if ENT designates a directory, false otherwise.

   Depending on MODE, ask whether to 'descend into' or to 'remove' the
   directory FILENAME.  MODE is ignored when FILENAME is not a directory.
   Use and update *DIR_STATUS as needed, via the conventions of
   get_dir_status.  */
ਸਥਿਰ ਗਿਣਤੀ RM_ਸਥਿਤੀ
prompt (FTS ਅਚਲ *fts, FTSENT ਅਚਲ *ent, bool is_dir,
        ਬਣਤਰ rm_options ਅਚਲ *x, ਗਿਣਤੀ Prompt_action ਬਹੁਲਕ_2,
        ਪੂਰਨ *dir_ਸਥਿਤੀ)
{
  ਪੂਰਨ fd_cwd = fts->fts_cwd_fd;
  ਅੱਖਰ ਅਚਲ *ਭਰਿਆ_name = ent->fts_ਰਸਤਾ;
  ਅੱਖਰ ਅਚਲ *filename = ent->fts_accpath;
  ਬਣਤਰ stat st;
  ਬਣਤਰ stat *sbuf = &st;
  cache_stat_init (sbuf);

  ਪੂਰਨ dirent_type = is_dir ? DT_DIR : DT_UNKNOWN;
  ਪੂਰਨ write_protected = 0;

  /* When nonzero, this indicates that we failed to remove a child entry,
     either because the user declined an interactive prompt, or due to
     some other failure, like permissions.  */
  ਜੇ (ent->fts_ਸੰਖਿਆ)
    ਵਾਪਸ RM_USER_DECLINED;

  ਜੇ (x->interactive == RMI_NEVER)
    ਵਾਪਸ RM_OK;

  ਪੂਰਨ wp_errno = 0;
  ਜੇ (!x->ignore_missing_files
      && (x->interactive == RMI_ALWAYS || x->stdin_tty)
      && dirent_type != DT_LNK)
    {
      write_protected = write_protected_non_symlink (fd_cwd, filename, sbuf);
      wp_errno = errno;
    }

  ਜੇ (write_protected || x->interactive == RMI_ALWAYS)
    {
      ਜੇ (0 <= write_protected && dirent_type == DT_UNKNOWN)
        {
          ਜੇ (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) == 0)
            {
              ਜੇ (S_ISLNK (sbuf->st_ਬਹੁਲਕ))
                dirent_type = DT_LNK;
              ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR (sbuf->st_ਬਹੁਲਕ))
                dirent_type = DT_DIR;
              /* Otherwise it doesn't matter, so leave it DT_UNKNOWN.  */
            }
          ਨਹੀਂਤਾਂ
            {
              /* This happens, e.g., with 'rm '''.  */
              write_protected = -1;
              wp_errno = errno;
            }
        }

      ਜੇ (0 <= write_protected)
        ਚੋਣ (dirent_type)
          {
          ਹਾਲਤ DT_LNK:
            /* Using permissions doesn't make sense for symlinks.  */
            ਜੇ (x->interactive != RMI_ALWAYS)
              ਵਾਪਸ RM_OK;
            ਰੋਕੋ;

          ਹਾਲਤ DT_DIR:
             /* Unless we're either deleting directories or deleting
                recursively, we want to raise an EISDIR error rather than
                prompting the user  */
            ਜੇ ( ! (x->recursive
                    || (x->remove_ਖਾਲੀ_ਥਾਵਾਂ_directories
                        && get_dir_ਸਥਿਤੀ (fts, ent, dir_ਸਥਿਤੀ) != 0)))
              {
                write_protected = -1;
                wp_errno = *dir_ਸਥਿਤੀ <= 0 ? EISDIR : *dir_ਸਥਿਤੀ;
              }
            ਰੋਕੋ;
          }

      ਅੱਖਰ ਅਚਲ *quoted_name = quoteaf (ਭਰਿਆ_name);

      ਜੇ (write_protected < 0)
        {
          error (0, wp_errno, _("cannot remove %s"), quoted_name);
          ਵਾਪਸ RM_ERROR;
        }

      /* Issue the prompt.  */
      ਜੇ (dirent_type == DT_DIR
          && ਬਹੁਲਕ_2 == PA_DESCEND_INTO_DIR
          && get_dir_ਸਥਿਤੀ (fts, ent, dir_ਸਥਿਤੀ) == DS_NONEMPTY)
        fprintf (stderr,
                 (write_protected
                  ? _("%s: descend into write-protected directory %s? ")
                  : _("%s: descend into directory %s? ")),
                 ਪ੍ਰੋਗਰਾਮ_name, quoted_name);
      ਨਹੀਂਤਾਂ ਜੇ (0 < *dir_ਸਥਿਤੀ)
        {
          ਜੇ ( ! (x->remove_ਖਾਲੀ_ਥਾਵਾਂ_directories && *dir_ਸਥਿਤੀ == EACCES))
            {
              error (0, *dir_ਸਥਿਤੀ, _("cannot remove %s"), quoted_name);
              ਵਾਪਸ RM_ERROR;
            }

          /* The following code can lead to a successful deletion only with
             the --dir (-d) option (remove_empty_directories) and an empty
             inaccessible directory. In the first prompt call for a directory,
             we'd normally ask whether to descend into it, but in this case
             (it's inaccessible), that is not possible, so don't prompt.  */
          ਜੇ (ਬਹੁਲਕ_2 == PA_DESCEND_INTO_DIR)
            ਵਾਪਸ RM_OK;

          fprintf (stderr,
               _("%s: attempt removal of inaccessible directory %s? "),
                   ਪ੍ਰੋਗਰਾਮ_name, quoted_name);
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, errno, _("cannot remove %s"), quoted_name);
              ਵਾਪਸ RM_ERROR;
            }

          fprintf (stderr,
                   (write_protected
                    /* TRANSLATORS: In the next two strings the second %s is
                       replaced by the type of the file.  To avoid grammatical
                       problems, it may be more convenient to translate these
                       strings instead as: "%1$s: %3$s is write-protected and
                       is of type '%2$s' -- remove it? ".  */
                    ? _("%s: remove write-protected %s %s? ")
                    : _("%s: remove %s %s? ")),
                   ਪ੍ਰੋਗਰਾਮ_name, file_type (sbuf), quoted_name);
        }

      ਵਾਪਸ yesno () ? RM_USER_ACCEPTED : RM_USER_DECLINED;
    }
  ਵਾਪਸ RM_OK;
}

/* When a function like unlink, rmdir, or fstatat fails with an errno
   value of ERRNUM, return true if the specified file system object
   is guaranteed not to exist;  otherwise, return false.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
nonexistent_file_errno (ਪੂਰਨ errnum)
{
  /* Do not include ELOOP here, since the specified file may indeed
     exist, but be (in)accessible only via too long a symlink chain.
     Likewise for ENAMETOOLONG, since rm -f ./././.../foo may fail
     if the "..." part expands to a long enough sequence of "./"s,
     even though ./foo does indeed exist.

     Another case to consider is when a particular name is invalid for
     a given file system.  In 2011, smbfs returns EINVAL, but the next
     revision of POSIX will require EILSEQ for that situation:
     https://austingroupbugs.net/view.php?id=293
  */

  ਚੋਣ (errnum)
    {
    ਹਾਲਤ EILSEQ:
    ਹਾਲਤ EINVAL:
    ਹਾਲਤ ENOENT:
    ਹਾਲਤ ENOTDIR:
      ਵਾਪਸ true;
    ਮੂਲ:
      ਵਾਪਸ false;
    }
}

/* Encapsulate the test for whether the errno value, ERRNUM, is ignorable.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ignorable_missing (ਬਣਤਰ rm_options ਅਚਲ *x, ਪੂਰਨ errnum)
{
  ਵਾਪਸ x->ignore_missing_files && nonexistent_file_errno (errnum);
}

/* Tell fts not to traverse into the hierarchy at ENT.  */
ਸਥਿਰ ਖਾਲੀ
fts_skip_ਰੁੱਖ (FTS *fts, FTSENT *ent)
{
  fts_ਸੈੱਟ (fts, ent, FTS_SKIP);
  /* Ensure that we do not process ENT a second time.  */
  ignore_ਮੁੱਲ (fts_read (fts));
}

/* Upon unlink failure, or when the user declines to remove ENT, mark
   each of its ancestor directories, so that we know not to prompt for
   its removal.  */
ਸਥਿਰ ਖਾਲੀ
mark_ancestor_dirs (FTSENT *ent)
{
  ਲਈ (FTSENT *p = ent->fts_ਮਾਪਾ; FTS_ROOTLEVEL <= p->fts_level;
       p = p->fts_ਮਾਪਾ)
    {
      ਜੇ (p->fts_ਸੰਖਿਆ)
        ਰੋਕੋ;
      p->fts_ਸੰਖਿਆ = 1;
    }
}

/* Remove the file system object specified by ENT.  IS_DIR specifies
   whether it is expected to be a directory or non-directory.
   Return RM_OK upon success, else RM_ERROR.  */
ਸਥਿਰ ਗਿਣਤੀ RM_ਸਥਿਤੀ
excise (FTS *fts, FTSENT *ent, ਬਣਤਰ rm_options ਅਚਲ *x, bool is_dir)
{
  ਪੂਰਨ flag = is_dir ? AT_REMOVEDIR : 0;
  ਜੇ (unlinkat (fts->fts_cwd_fd, ent->fts_accpath, flag) == 0)
    {
      ਜੇ (x->verbose)
        {
          ਲਿਖੋ ((is_dir
                   ? _("removed directory %s\n")
                   : _("removed %s\n")), quoteaf (ent->fts_ਰਸਤਾ));
        }
      ਵਾਪਸ RM_OK;
    }

  /* The unlinkat from kernels like linux-2.6.32 reports EROFS even for
     nonexistent files.  When the file is indeed missing, map that to ENOENT,
     so that rm -f ignores it, as required.  Even without -f, this is useful
     because it makes rm print the more precise diagnostic.  */
  ਜੇ (errno == EROFS)
    {
      ਬਣਤਰ stat st;
      ਜੇ ( ! (fstatat (fts->fts_cwd_fd, ent->fts_accpath, &st,
                       AT_SYMLINK_NOFOLLOW)
              && errno == ENOENT))
        errno = EROFS;
    }

  ਜੇ (ignorable_missing (x, errno))
    ਵਾਪਸ RM_OK;

  /* When failing to rmdir an unreadable directory, we see errno values
     like EISDIR or ENOTDIR (or, on Solaris 10, EEXIST), but they would be
     meaningless in a diagnostic.  When that happens, use the earlier, more
     descriptive errno value.  */
  ਜੇ (ent->fts_info == FTS_DNR
      && (errno == ENOTEMPTY || errno == EISDIR || errno == ENOTDIR
          || errno == EEXIST)
      && ent->fts_errno != 0)
    errno = ent->fts_errno;
  error (0, errno, _("cannot remove %s"), quoteaf (ent->fts_ਰਸਤਾ));
  mark_ancestor_dirs (ent);
  ਵਾਪਸ RM_ERROR;
}

/* This function is called once for every file system object that fts
   encounters.  fts performs a depth-first traversal.
   A directory is usually processed twice, first with fts_info == FTS_D,
   and later, after all of its entries have been processed, with FTS_DP.
   Return RM_ERROR upon error, RM_USER_DECLINED for a negative response
   to an interactive prompt, and otherwise, RM_OK.  */
ਸਥਿਰ ਗਿਣਤੀ RM_ਸਥਿਤੀ
rm_fts (FTS *fts, FTSENT *ent, ਬਣਤਰ rm_options ਅਚਲ *x)
{
  ਪੂਰਨ dir_ਸਥਿਤੀ = DS_UNKNOWN;

  ਚੋਣ (ent->fts_info)
    {
    ਹਾਲਤ FTS_D:			/* preorder directory */
      ਜੇ (!x->recursive)
        {
          /* Not recursive, so skip contents, and fail now unless
             removing empty directories.  */
          fts_ਸੈੱਟ (fts, ent, FTS_SKIP);
          ਜੇ (x->remove_ਖਾਲੀ_ਥਾਵਾਂ_directories)
            ਵਾਪਸ RM_OK;
          error (0, EISDIR, _("cannot remove %s"), quoteaf (ent->fts_ਰਸਤਾ));
          ignore_ਮੁੱਲ (fts_read (fts));
          ਵਾਪਸ RM_ERROR;
        }

      /* Perform checks that can apply only for command-line arguments.  */
      ਜੇ (ent->fts_level == FTS_ROOTLEVEL)
        {
          /* POSIX says:
             If the basename of a command line argument is "." or "..",
             diagnose it and do nothing more with that argument.  */
          ਜੇ (dot_or_dotdot (ਆਖਰੀ_component (ent->fts_accpath)))
            {
              error (0, 0,
                     _("refusing to remove %s or %s directory: skipping %s"),
                     quoteaf_n (0, "."), quoteaf_n (1, ".."),
                     quoteaf_n (2, ent->fts_ਰਸਤਾ));
              fts_skip_ਰੁੱਖ (fts, ent);
              ਵਾਪਸ RM_ERROR;
            }

          /* POSIX also says:
             If a command line argument resolves to "/" (and --preserve-root
             is in effect -- default) diagnose and skip it.  */
          ਜੇ (ਜੜ੍ਹ_DEV_INO_CHECK (x->ਜੜ੍ਹ_dev_ino, ent->fts_statp))
            {
              ਜੜ੍ਹ_DEV_INO_WARN (ent->fts_ਰਸਤਾ);
              fts_skip_ਰੁੱਖ (fts, ent);
              ਵਾਪਸ RM_ERROR;
            }

          /* If a command line argument is a mount point and
             --preserve-root=all is in effect, diagnose and skip it.
             This doesn't handle "/", but that's handled above.  */
          ਜੇ (x->preserve_all_ਜੜ੍ਹ)
            {
              bool failed = false;
              ਅੱਖਰ *ਮਾਪਾ = file_name_concat (ent->fts_accpath, "..", ਨਿਰਾ);
              ਬਣਤਰ stat statbuf;

              ਜੇ (!ਮਾਪਾ || lstat (ਮਾਪਾ, &statbuf))
                {
                  error (0, 0,
                         _("failed to stat %s: skipping %s"),
                         quoteaf_n (0, ਮਾਪਾ),
                         quoteaf_n (1, ent->fts_accpath));
                  failed = true;
                }

              ਮੁਕਤ (ਮਾਪਾ);

              ਜੇ (failed || fts->fts_dev != statbuf.st_dev)
                {
                  ਜੇ (! failed)
                    {
                      error (0, 0,
                             _("skipping %s, since it's on a different device"),
                             quoteaf (ent->fts_ਰਸਤਾ));
                      error (0, 0, _("and --preserve-root=all is in effect"));
                    }
                  fts_skip_ਰੁੱਖ (fts, ent);
                  ਵਾਪਸ RM_ERROR;
                }
            }
        }

      {
        ਗਿਣਤੀ RM_ਸਥਿਤੀ s = prompt (fts, ent, true /*is_dir*/, x,
                                   PA_DESCEND_INTO_DIR, &dir_ਸਥਿਤੀ);

        ਜੇ (s == RM_USER_ACCEPTED && dir_ਸਥਿਤੀ == DS_ਖਾਲੀ_ਥਾਵਾਂ)
          {
            /* When we know (from prompt when in interactive mode)
               that this is an empty directory, don't prompt twice.  */
            s = excise (fts, ent, x, true);
            ਜੇ (s == RM_OK)
              fts_skip_ਰੁੱਖ (fts, ent);
          }

        ਜੇ (! (s == RM_OK || s == RM_USER_ACCEPTED))
          {
            mark_ancestor_dirs (ent);
            fts_skip_ਰੁੱਖ (fts, ent);
          }

        ਵਾਪਸ s;
      }

    ਹਾਲਤ FTS_F:			/* regular file */
    ਹਾਲਤ FTS_NS:		/* stat(2) failed */
    ਹਾਲਤ FTS_SL:		/* symbolic link */
    ਹਾਲਤ FTS_SLNONE:		/* symbolic link without target */
    ਹਾਲਤ FTS_DP:		/* postorder directory */
    ਹਾਲਤ FTS_DNR:		/* unreadable directory */
    ਹਾਲਤ FTS_NSOK:		/* e.g., dangling symlink */
    ਹਾਲਤ FTS_DEFAULT:		/* none of the above */
      {
        /* With --one-file-system, do not attempt to remove a mount point.
           fts' FTS_XDEV ensures that we don't process any entries under
           the mount point.  */
        ਜੇ (ent->fts_info == FTS_DP
            && x->one_file_system
            && FTS_ROOTLEVEL < ent->fts_level
            && ent->fts_statp->st_dev != fts->fts_dev)
          {
            mark_ancestor_dirs (ent);
            error (0, 0, _("skipping %s, since it's on a different device"),
                   quoteaf (ent->fts_ਰਸਤਾ));
            ਵਾਪਸ RM_ERROR;
          }

        bool is_dir = ent->fts_info == FTS_DP || ent->fts_info == FTS_DNR;
        ਗਿਣਤੀ RM_ਸਥਿਤੀ s = prompt (fts, ent, is_dir, x, PA_REMOVE_DIR,
                                   &dir_ਸਥਿਤੀ);
        ਜੇ (! (s == RM_OK || s == RM_USER_ACCEPTED))
          ਵਾਪਸ s;
        ਵਾਪਸ excise (fts, ent, x, is_dir);
      }

    ਹਾਲਤ FTS_DC:		/* directory that causes cycles */
      emit_ਚੱਕਰ_warning (ent->fts_ਰਸਤਾ);
      fts_skip_ਰੁੱਖ (fts, ent);
      ਵਾਪਸ RM_ERROR;

    ਹਾਲਤ FTS_ERR:
      /* Various failures, from opendir to ENOMEM, to failure to "return"
         to preceding directory, can provoke this.  */
      error (0, ent->fts_errno, _("traversal failed: %s"),
             quotef (ent->fts_ਰਸਤਾ));
      fts_skip_ਰੁੱਖ (fts, ent);
      ਵਾਪਸ RM_ERROR;

    ਮੂਲ:
      error (0, 0, _("unexpected failure: fts_info=%d: %s\n"
                     "please report to %s"),
             ent->fts_info,
             quotef (ent->fts_ਰਸਤਾ),
             PACKAGE_BUGREPORT);
      abort ();
    }
}

/* Remove FILEs, honoring options specified via X.
   Return RM_OK if successful.  */
ਗਿਣਤੀ RM_ਸਥਿਤੀ
rm (ਅੱਖਰ *ਅਚਲ *file, ਬਣਤਰ rm_options ਅਚਲ *x)
{
  ਗਿਣਤੀ RM_ਸਥਿਤੀ rm_ਸਥਿਤੀ = RM_OK;

  ਜੇ (*file)
    {
      ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags = (FTS_CWDFD
                       | FTS_NOSTAT
                       | FTS_PHYSICAL);

      ਜੇ (x->one_file_system)
        ਦੋਆਧਾਰੀ_ਅੰਕ_flags |= FTS_XDEV;

      FTS *fts = xfts_open (file, ਦੋਆਧਾਰੀ_ਅੰਕ_flags, ਨਿਰਾ);

      ਜਦੋਂ (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ਜੇ (ent == ਨਿਰਾ)
            {
              ਜੇ (errno != 0)
                {
                  error (0, errno, _("fts_read failed"));
                  rm_ਸਥਿਤੀ = RM_ERROR;
                }
              ਰੋਕੋ;
            }

          ਗਿਣਤੀ RM_ਸਥਿਤੀ s = rm_fts (fts, ent, x);

          affirm (VALID_ਸਥਿਤੀ (s));
          UPDATE_ਸਥਿਤੀ (rm_ਸਥਿਤੀ, s);
        }

      ਜੇ (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          rm_ਸਥਿਤੀ = RM_ERROR;
        }
    }

  ਵਾਪਸ rm_ਸਥਿਤੀ;
}
