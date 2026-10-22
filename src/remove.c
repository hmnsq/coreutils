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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "file-type.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "remove.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "write-any-file.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"
#ಅನ್ತರ್ಭಾವ "yesno.h"

/* The prompt function may be called twice for a given directory.
   The first time, we ask whether to descend into it, and the
   second time, we ask whether to remove it.  */
ಎಣಿಕೆ Prompt_action
  {
    PA_DESCEND_INTO_DIR = 2,
    PA_REMOVE_DIR
  };

/* D_TYPE(D) is the type of directory entry D if known, DT_UNKNOWN
   otherwise.  */
#ಆದರೆ ! HAVE_STRUCT_DIRENT_D_TYPE
/* Any int values will do here, so long as they're distinct.
   Undef any existing macros out of the way.  */
# undef DT_UNKNOWN
# undef DT_DIR
# undef DT_LNK
# define DT_UNKNOWN 0
# define DT_DIR 1
# define DT_LNK 2
#ಆದರೆ_ಅಂತ್ಯ

/* Like fstatat, but cache on POSIX-compatible systems.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cache_fstatat (ಪೂರ್ಣಾಂಕ fd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ stat *st, ಪೂರ್ಣಾಂಕ flag)
{
#ಆದರೆ HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  /* If ST->st_atim.tv_nsec is -1, the status has not been gotten yet.
     If less than -1, fstatat failed with errno == ST->st_ino.
     Otherwise, the status has already been gotten, so return 0.  */
  ಆದರೆ (0 <= st->st_atim.tv_nsec)
    ಹಿಂತಿರುಗಿ 0;
  ಆದರೆ (st->st_atim.tv_nsec == -1)
    {
      ಆದರೆ (fstatat (fd, file, st, flag) == 0)
        ಹಿಂತಿರುಗಿ 0;
      st->st_atim.tv_nsec = -2;
      st->st_ino = errno;
    }
  errno = st->st_ino;
  ಹಿಂತಿರುಗಿ -1;
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ fstatat (fd, file, st, flag);
#ಆದರೆ_ಅಂತ್ಯ
}

/* Initialize a fstatat cache *ST.  Return ST for convenience.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ stat *
cache_stat_init (ರಚನೆ stat *st)
{
#ಆದರೆ HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  st->st_atim.tv_nsec = -1;
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ st;
}

/* Return 1 if FILE is an unwritable non-symlink,
   0 if it is writable or some other type of file,
   -1 and set errno if there is some problem in determining the answer.
   Set *BUF to the file status.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_protected_non_symlink (ಪೂರ್ಣಾಂಕ fd_cwd,
                             ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file,
                             ರಚನೆ stat *buf)
{
  ಆದರೆ (can_write_any_file ())
    ಹಿಂತಿರುಗಿ 0;
  ಆದರೆ (cache_fstatat (fd_cwd, file, buf, AT_SYMLINK_NOFOLLOW) != 0)
    ಹಿಂತಿರುಗಿ -1;
  ಆದರೆ (S_ISLNK (buf->st_ಬಹುಲಕ))
    ಹಿಂತಿರುಗಿ 0;
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
    ಆದರೆ (faccessat (fd_cwd, file, W_OK, AT_EACCESS) == 0)
      ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ errno == EACCES ? 1 : -1;
  }
}

/* Return the status of the directory identified by FTS and ENT.
   This is -1 if the directory is empty, 0 if it is nonempty,
   and a positive error number if there was trouble determining the status,
   e.g., it is not a directory, or permissions problems, or I/O errors.
   Use *DIR_STATUS as a cache for the status.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_dir_ಸ್ಥಿತಿ (FTS ಸ್ಥಿರಾಂಕ *fts, FTSENT ಸ್ಥಿರಾಂಕ *ent, ಪೂರ್ಣಾಂಕ *dir_ಸ್ಥಿತಿ)
{
  ಆದರೆ (*dir_ಸ್ಥಿತಿ == DS_UNKNOWN)
    *dir_ಸ್ಥಿತಿ = directory_ಸ್ಥಿತಿ (fts->fts_cwd_fd, ent->fts_accpath);
  ಹಿಂತಿರುಗಿ *dir_ಸ್ಥಿತಿ;
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
ಸ್ಥಿರ ಎಣಿಕೆ RM_ಸ್ಥಿತಿ
prompt (FTS ಸ್ಥಿರಾಂಕ *fts, FTSENT ಸ್ಥಿರಾಂಕ *ent, bool is_dir,
        ರಚನೆ rm_options ಸ್ಥಿರಾಂಕ *x, ಎಣಿಕೆ Prompt_action ಬಹುಲಕ_2,
        ಪೂರ್ಣಾಂಕ *dir_ಸ್ಥಿತಿ)
{
  ಪೂರ್ಣಾಂಕ fd_cwd = fts->fts_cwd_fd;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತುಂಬಿದ_name = ent->fts_ಮಾರ್ಗ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename = ent->fts_accpath;
  ರಚನೆ stat st;
  ರಚನೆ stat *sbuf = &st;
  cache_stat_init (sbuf);

  ಪೂರ್ಣಾಂಕ dirent_type = is_dir ? DT_DIR : DT_UNKNOWN;
  ಪೂರ್ಣಾಂಕ write_protected = 0;

  /* When nonzero, this indicates that we failed to remove a child entry,
     either because the user declined an interactive prompt, or due to
     some other failure, like permissions.  */
  ಆದರೆ (ent->fts_ಸಂಖ್ಯೆ)
    ಹಿಂತಿರುಗಿ RM_USER_DECLINED;

  ಆದರೆ (x->interactive == RMI_NEVER)
    ಹಿಂತಿರುಗಿ RM_OK;

  ಪೂರ್ಣಾಂಕ wp_errno = 0;
  ಆದರೆ (!x->ignore_missing_files
      && (x->interactive == RMI_ALWAYS || x->stdin_tty)
      && dirent_type != DT_LNK)
    {
      write_protected = write_protected_non_symlink (fd_cwd, filename, sbuf);
      wp_errno = errno;
    }

  ಆದರೆ (write_protected || x->interactive == RMI_ALWAYS)
    {
      ಆದರೆ (0 <= write_protected && dirent_type == DT_UNKNOWN)
        {
          ಆದರೆ (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) == 0)
            {
              ಆದರೆ (S_ISLNK (sbuf->st_ಬಹುಲಕ))
                dirent_type = DT_LNK;
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR (sbuf->st_ಬಹುಲಕ))
                dirent_type = DT_DIR;
              /* Otherwise it doesn't matter, so leave it DT_UNKNOWN.  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* This happens, e.g., with 'rm '''.  */
              write_protected = -1;
              wp_errno = errno;
            }
        }

      ಆದರೆ (0 <= write_protected)
        ಆಯ್ಕೆ (dirent_type)
          {
          ಸ್ಥಿತಿ DT_LNK:
            /* Using permissions doesn't make sense for symlinks.  */
            ಆದರೆ (x->interactive != RMI_ALWAYS)
              ಹಿಂತಿರುಗಿ RM_OK;
            ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ DT_DIR:
             /* Unless we're either deleting directories or deleting
                recursively, we want to raise an EISDIR error rather than
                prompting the user  */
            ಆದರೆ ( ! (x->recursive
                    || (x->remove_ಖಾಲಿಗಳು_directories
                        && get_dir_ಸ್ಥಿತಿ (fts, ent, dir_ಸ್ಥಿತಿ) != 0)))
              {
                write_protected = -1;
                wp_errno = *dir_ಸ್ಥಿತಿ <= 0 ? EISDIR : *dir_ಸ್ಥಿತಿ;
              }
            ನಿಲ್ಲಿಸು;
          }

      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *quoted_name = quoteaf (ತುಂಬಿದ_name);

      ಆದರೆ (write_protected < 0)
        {
          error (0, wp_errno, _("cannot remove %s"), quoted_name);
          ಹಿಂತಿರುಗಿ RM_ERROR;
        }

      /* Issue the prompt.  */
      ಆದರೆ (dirent_type == DT_DIR
          && ಬಹುಲಕ_2 == PA_DESCEND_INTO_DIR
          && get_dir_ಸ್ಥಿತಿ (fts, ent, dir_ಸ್ಥಿತಿ) == DS_NONEMPTY)
        fprintf (stderr,
                 (write_protected
                  ? _("%s: descend into write-protected directory %s? ")
                  : _("%s: descend into directory %s? ")),
                 ಕಾರ್ಯಕ್ರಮ_name, quoted_name);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 < *dir_ಸ್ಥಿತಿ)
        {
          ಆದರೆ ( ! (x->remove_ಖಾಲಿಗಳು_directories && *dir_ಸ್ಥಿತಿ == EACCES))
            {
              error (0, *dir_ಸ್ಥಿತಿ, _("cannot remove %s"), quoted_name);
              ಹಿಂತಿರುಗಿ RM_ERROR;
            }

          /* The following code can lead to a successful deletion only with
             the --dir (-d) option (remove_empty_directories) and an empty
             inaccessible directory. In the first prompt call for a directory,
             we'd normally ask whether to descend into it, but in this case
             (it's inaccessible), that is not possible, so don't prompt.  */
          ಆದರೆ (ಬಹುಲಕ_2 == PA_DESCEND_INTO_DIR)
            ಹಿಂತಿರುಗಿ RM_OK;

          fprintf (stderr,
               _("%s: attempt removal of inaccessible directory %s? "),
                   ಕಾರ್ಯಕ್ರಮ_name, quoted_name);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, errno, _("cannot remove %s"), quoted_name);
              ಹಿಂತಿರುಗಿ RM_ERROR;
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
                   ಕಾರ್ಯಕ್ರಮ_name, file_type (sbuf), quoted_name);
        }

      ಹಿಂತಿರುಗಿ yesno () ? RM_USER_ACCEPTED : RM_USER_DECLINED;
    }
  ಹಿಂತಿರುಗಿ RM_OK;
}

/* When a function like unlink, rmdir, or fstatat fails with an errno
   value of ERRNUM, return true if the specified file system object
   is guaranteed not to exist;  otherwise, return false.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
nonexistent_file_errno (ಪೂರ್ಣಾಂಕ errnum)
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

  ಆಯ್ಕೆ (errnum)
    {
    ಸ್ಥಿತಿ EILSEQ:
    ಸ್ಥಿತಿ EINVAL:
    ಸ್ಥಿತಿ ENOENT:
    ಸ್ಥಿತಿ ENOTDIR:
      ಹಿಂತಿರುಗಿ true;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ false;
    }
}

/* Encapsulate the test for whether the errno value, ERRNUM, is ignorable.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ignorable_missing (ರಚನೆ rm_options ಸ್ಥಿರಾಂಕ *x, ಪೂರ್ಣಾಂಕ errnum)
{
  ಹಿಂತಿರುಗಿ x->ignore_missing_files && nonexistent_file_errno (errnum);
}

/* Tell fts not to traverse into the hierarchy at ENT.  */
ಸ್ಥಿರ ಶೂನ್ಯ
fts_skip_ವೃಕ್ಷ (FTS *fts, FTSENT *ent)
{
  fts_ಗಣ (fts, ent, FTS_SKIP);
  /* Ensure that we do not process ENT a second time.  */
  ignore_ಮೌಲ್ಯ (fts_read (fts));
}

/* Upon unlink failure, or when the user declines to remove ENT, mark
   each of its ancestor directories, so that we know not to prompt for
   its removal.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mark_ancestor_dirs (FTSENT *ent)
{
  ಗಾಗಿ (FTSENT *p = ent->fts_ಪೋಷಕ; FTS_ROOTLEVEL <= p->fts_level;
       p = p->fts_ಪೋಷಕ)
    {
      ಆದರೆ (p->fts_ಸಂಖ್ಯೆ)
        ನಿಲ್ಲಿಸು;
      p->fts_ಸಂಖ್ಯೆ = 1;
    }
}

/* Remove the file system object specified by ENT.  IS_DIR specifies
   whether it is expected to be a directory or non-directory.
   Return RM_OK upon success, else RM_ERROR.  */
ಸ್ಥಿರ ಎಣಿಕೆ RM_ಸ್ಥಿತಿ
excise (FTS *fts, FTSENT *ent, ರಚನೆ rm_options ಸ್ಥಿರಾಂಕ *x, bool is_dir)
{
  ಪೂರ್ಣಾಂಕ flag = is_dir ? AT_REMOVEDIR : 0;
  ಆದರೆ (unlinkat (fts->fts_cwd_fd, ent->fts_accpath, flag) == 0)
    {
      ಆದರೆ (x->verbose)
        {
          ಬರೆ ((is_dir
                   ? _("removed directory %s\n")
                   : _("removed %s\n")), quoteaf (ent->fts_ಮಾರ್ಗ));
        }
      ಹಿಂತಿರುಗಿ RM_OK;
    }

  /* The unlinkat from kernels like linux-2.6.32 reports EROFS even for
     nonexistent files.  When the file is indeed missing, map that to ENOENT,
     so that rm -f ignores it, as required.  Even without -f, this is useful
     because it makes rm print the more precise diagnostic.  */
  ಆದರೆ (errno == EROFS)
    {
      ರಚನೆ stat st;
      ಆದರೆ ( ! (fstatat (fts->fts_cwd_fd, ent->fts_accpath, &st,
                       AT_SYMLINK_NOFOLLOW)
              && errno == ENOENT))
        errno = EROFS;
    }

  ಆದರೆ (ignorable_missing (x, errno))
    ಹಿಂತಿರುಗಿ RM_OK;

  /* When failing to rmdir an unreadable directory, we see errno values
     like EISDIR or ENOTDIR (or, on Solaris 10, EEXIST), but they would be
     meaningless in a diagnostic.  When that happens, use the earlier, more
     descriptive errno value.  */
  ಆದರೆ (ent->fts_info == FTS_DNR
      && (errno == ENOTEMPTY || errno == EISDIR || errno == ENOTDIR
          || errno == EEXIST)
      && ent->fts_errno != 0)
    errno = ent->fts_errno;
  error (0, errno, _("cannot remove %s"), quoteaf (ent->fts_ಮಾರ್ಗ));
  mark_ancestor_dirs (ent);
  ಹಿಂತಿರುಗಿ RM_ERROR;
}

/* This function is called once for every file system object that fts
   encounters.  fts performs a depth-first traversal.
   A directory is usually processed twice, first with fts_info == FTS_D,
   and later, after all of its entries have been processed, with FTS_DP.
   Return RM_ERROR upon error, RM_USER_DECLINED for a negative response
   to an interactive prompt, and otherwise, RM_OK.  */
ಸ್ಥಿರ ಎಣಿಕೆ RM_ಸ್ಥಿತಿ
rm_fts (FTS *fts, FTSENT *ent, ರಚನೆ rm_options ಸ್ಥಿರಾಂಕ *x)
{
  ಪೂರ್ಣಾಂಕ dir_ಸ್ಥಿತಿ = DS_UNKNOWN;

  ಆಯ್ಕೆ (ent->fts_info)
    {
    ಸ್ಥಿತಿ FTS_D:			/* preorder directory */
      ಆದರೆ (!x->recursive)
        {
          /* Not recursive, so skip contents, and fail now unless
             removing empty directories.  */
          fts_ಗಣ (fts, ent, FTS_SKIP);
          ಆದರೆ (x->remove_ಖಾಲಿಗಳು_directories)
            ಹಿಂತಿರುಗಿ RM_OK;
          error (0, EISDIR, _("cannot remove %s"), quoteaf (ent->fts_ಮಾರ್ಗ));
          ignore_ಮೌಲ್ಯ (fts_read (fts));
          ಹಿಂತಿರುಗಿ RM_ERROR;
        }

      /* Perform checks that can apply only for command-line arguments.  */
      ಆದರೆ (ent->fts_level == FTS_ROOTLEVEL)
        {
          /* POSIX says:
             If the basename of a command line argument is "." or "..",
             diagnose it and do nothing more with that argument.  */
          ಆದರೆ (dot_or_dotdot (ಕೊನೆಯ_component (ent->fts_accpath)))
            {
              error (0, 0,
                     _("refusing to remove %s or %s directory: skipping %s"),
                     quoteaf_n (0, "."), quoteaf_n (1, ".."),
                     quoteaf_n (2, ent->fts_ಮಾರ್ಗ));
              fts_skip_ವೃಕ್ಷ (fts, ent);
              ಹಿಂತಿರುಗಿ RM_ERROR;
            }

          /* POSIX also says:
             If a command line argument resolves to "/" (and --preserve-root
             is in effect -- default) diagnose and skip it.  */
          ಆದರೆ (ಬೇರು_DEV_INO_CHECK (x->ಬೇರು_dev_ino, ent->fts_statp))
            {
              ಬೇರು_DEV_INO_WARN (ent->fts_ಮಾರ್ಗ);
              fts_skip_ವೃಕ್ಷ (fts, ent);
              ಹಿಂತಿರುಗಿ RM_ERROR;
            }

          /* If a command line argument is a mount point and
             --preserve-root=all is in effect, diagnose and skip it.
             This doesn't handle "/", but that's handled above.  */
          ಆದರೆ (x->preserve_all_ಬೇರು)
            {
              bool failed = false;
              ಅಕ್ಷರ *ಪೋಷಕ = file_name_concat (ent->fts_accpath, "..", ಖಾಲಿ);
              ರಚನೆ stat statbuf;

              ಆದರೆ (!ಪೋಷಕ || lstat (ಪೋಷಕ, &statbuf))
                {
                  error (0, 0,
                         _("failed to stat %s: skipping %s"),
                         quoteaf_n (0, ಪೋಷಕ),
                         quoteaf_n (1, ent->fts_accpath));
                  failed = true;
                }

              ಬಿಡುಗಡೆ (ಪೋಷಕ);

              ಆದರೆ (failed || fts->fts_dev != statbuf.st_dev)
                {
                  ಆದರೆ (! failed)
                    {
                      error (0, 0,
                             _("skipping %s, since it's on a different device"),
                             quoteaf (ent->fts_ಮಾರ್ಗ));
                      error (0, 0, _("and --preserve-root=all is in effect"));
                    }
                  fts_skip_ವೃಕ್ಷ (fts, ent);
                  ಹಿಂತಿರುಗಿ RM_ERROR;
                }
            }
        }

      {
        ಎಣಿಕೆ RM_ಸ್ಥಿತಿ s = prompt (fts, ent, true /*is_dir*/, x,
                                   PA_DESCEND_INTO_DIR, &dir_ಸ್ಥಿತಿ);

        ಆದರೆ (s == RM_USER_ACCEPTED && dir_ಸ್ಥಿತಿ == DS_ಖಾಲಿಗಳು)
          {
            /* When we know (from prompt when in interactive mode)
               that this is an empty directory, don't prompt twice.  */
            s = excise (fts, ent, x, true);
            ಆದರೆ (s == RM_OK)
              fts_skip_ವೃಕ್ಷ (fts, ent);
          }

        ಆದರೆ (! (s == RM_OK || s == RM_USER_ACCEPTED))
          {
            mark_ancestor_dirs (ent);
            fts_skip_ವೃಕ್ಷ (fts, ent);
          }

        ಹಿಂತಿರುಗಿ s;
      }

    ಸ್ಥಿತಿ FTS_F:			/* regular file */
    ಸ್ಥಿತಿ FTS_NS:		/* stat(2) failed */
    ಸ್ಥಿತಿ FTS_SL:		/* symbolic link */
    ಸ್ಥಿತಿ FTS_SLNONE:		/* symbolic link without target */
    ಸ್ಥಿತಿ FTS_DP:		/* postorder directory */
    ಸ್ಥಿತಿ FTS_DNR:		/* unreadable directory */
    ಸ್ಥಿತಿ FTS_NSOK:		/* e.g., dangling symlink */
    ಸ್ಥಿತಿ FTS_DEFAULT:		/* none of the above */
      {
        /* With --one-file-system, do not attempt to remove a mount point.
           fts' FTS_XDEV ensures that we don't process any entries under
           the mount point.  */
        ಆದರೆ (ent->fts_info == FTS_DP
            && x->one_file_system
            && FTS_ROOTLEVEL < ent->fts_level
            && ent->fts_statp->st_dev != fts->fts_dev)
          {
            mark_ancestor_dirs (ent);
            error (0, 0, _("skipping %s, since it's on a different device"),
                   quoteaf (ent->fts_ಮಾರ್ಗ));
            ಹಿಂತಿರುಗಿ RM_ERROR;
          }

        bool is_dir = ent->fts_info == FTS_DP || ent->fts_info == FTS_DNR;
        ಎಣಿಕೆ RM_ಸ್ಥಿತಿ s = prompt (fts, ent, is_dir, x, PA_REMOVE_DIR,
                                   &dir_ಸ್ಥಿತಿ);
        ಆದರೆ (! (s == RM_OK || s == RM_USER_ACCEPTED))
          ಹಿಂತಿರುಗಿ s;
        ಹಿಂತಿರುಗಿ excise (fts, ent, x, is_dir);
      }

    ಸ್ಥಿತಿ FTS_DC:		/* directory that causes cycles */
      emit_ಚಕ್ರ_warning (ent->fts_ಮಾರ್ಗ);
      fts_skip_ವೃಕ್ಷ (fts, ent);
      ಹಿಂತಿರುಗಿ RM_ERROR;

    ಸ್ಥಿತಿ FTS_ERR:
      /* Various failures, from opendir to ENOMEM, to failure to "return"
         to preceding directory, can provoke this.  */
      error (0, ent->fts_errno, _("traversal failed: %s"),
             quotef (ent->fts_ಮಾರ್ಗ));
      fts_skip_ವೃಕ್ಷ (fts, ent);
      ಹಿಂತಿರುಗಿ RM_ERROR;

    ಪೂರ್ವನಿಯೋಜಿತ:
      error (0, 0, _("unexpected failure: fts_info=%d: %s\n"
                     "please report to %s"),
             ent->fts_info,
             quotef (ent->fts_ಮಾರ್ಗ),
             PACKAGE_BUGREPORT);
      abort ();
    }
}

/* Remove FILEs, honoring options specified via X.
   Return RM_OK if successful.  */
ಎಣಿಕೆ RM_ಸ್ಥಿತಿ
rm (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *file, ರಚನೆ rm_options ಸ್ಥಿರಾಂಕ *x)
{
  ಎಣಿಕೆ RM_ಸ್ಥಿತಿ rm_ಸ್ಥಿತಿ = RM_OK;

  ಆದರೆ (*file)
    {
      ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags = (FTS_CWDFD
                       | FTS_NOSTAT
                       | FTS_PHYSICAL);

      ಆದರೆ (x->one_file_system)
        ಬಿಟ್_flags |= FTS_XDEV;

      FTS *fts = xfts_open (file, ಬಿಟ್_flags, ಖಾಲಿ);

      ಆಗ (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ಆದರೆ (ent == ಖಾಲಿ)
            {
              ಆದರೆ (errno != 0)
                {
                  error (0, errno, _("fts_read failed"));
                  rm_ಸ್ಥಿತಿ = RM_ERROR;
                }
              ನಿಲ್ಲಿಸು;
            }

          ಎಣಿಕೆ RM_ಸ್ಥಿತಿ s = rm_fts (fts, ent, x);

          affirm (VALID_ಸ್ಥಿತಿ (s));
          UPDATE_ಸ್ಥಿತಿ (rm_ಸ್ಥಿತಿ, s);
        }

      ಆದರೆ (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          rm_ಸ್ಥಿತಿ = RM_ERROR;
        }
    }

  ಹಿಂತಿರುಗಿ rm_ಸ್ಥಿತಿ;
}
