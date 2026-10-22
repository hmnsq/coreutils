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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "file-type.h"
#அந்தர்பாவ "filenamecat.h"
#அந்தர்பாவ "ignore-value.h"
#அந்தர்பாவ "remove.h"
#அந்தர்பாவ "root-dev-ino.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "write-any-file.h"
#அந்தர்பாவ "xfts.h"
#அந்தர்பாவ "yesno.h"

/* The prompt function may be called twice for a given directory.
   The first time, we ask whether to descend into it, and the
   second time, we ask whether to remove it.  */
எண்ணிக்கை Prompt_action
  {
    PA_DESCEND_INTO_DIR = 2,
    PA_REMOVE_DIR
  };

/* D_TYPE(D) is the type of directory entry D if known, DT_UNKNOWN
   otherwise.  */
#எனில் ! HAVE_STRUCT_DIRENT_D_TYPE
/* Any int values will do here, so long as they're distinct.
   Undef any existing macros out of the way.  */
# undef DT_UNKNOWN
# undef DT_DIR
# undef DT_LNK
# define DT_UNKNOWN 0
# define DT_DIR 1
# define DT_LNK 2
#endif

/* Like fstatat, but cache on POSIX-compatible systems.  */
நிலையான முழு
cache_fstatat (முழு fd, எழுத்து மாறிலி *file, கட்டமைப்பு stat *st, முழு flag)
{
#எனில் HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  /* If ST->st_atim.tv_nsec is -1, the status has not been gotten yet.
     If less than -1, fstatat failed with errno == ST->st_ino.
     Otherwise, the status has already been gotten, so return 0.  */
  எனில் (0 <= st->st_atim.tv_nsec)
    திரும்பு 0;
  எனில் (st->st_atim.tv_nsec == -1)
    {
      எனில் (fstatat (fd, file, st, flag) == 0)
        திரும்பு 0;
      st->st_atim.tv_nsec = -2;
      st->st_ino = errno;
    }
  errno = st->st_ino;
  திரும்பு -1;
#இல்லையெனில்
  திரும்பு fstatat (fd, file, st, flag);
#endif
}

/* Initialize a fstatat cache *ST.  Return ST for convenience.  */
நிலையான உள்வரி கட்டமைப்பு stat *
cache_stat_init (கட்டமைப்பு stat *st)
{
#எனில் HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  st->st_atim.tv_nsec = -1;
#endif
  திரும்பு st;
}

/* Return 1 if FILE is an unwritable non-symlink,
   0 if it is writable or some other type of file,
   -1 and set errno if there is some problem in determining the answer.
   Set *BUF to the file status.  */
நிலையான முழு
write_protected_non_symlink (முழு fd_cwd,
                             எழுத்து மாறிலி *file,
                             கட்டமைப்பு stat *buf)
{
  எனில் (can_write_any_file ())
    திரும்பு 0;
  எனில் (cache_fstatat (fd_cwd, file, buf, AT_SYMLINK_NOFOLLOW) != 0)
    திரும்பு -1;
  எனில் (S_ISLNK (buf->st_mode))
    திரும்பு 0;
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
    எனில் (faccessat (fd_cwd, file, W_OK, AT_EACCESS) == 0)
      திரும்பு 0;

    திரும்பு errno == EACCES ? 1 : -1;
  }
}

/* Return the status of the directory identified by FTS and ENT.
   This is -1 if the directory is empty, 0 if it is nonempty,
   and a positive error number if there was trouble determining the status,
   e.g., it is not a directory, or permissions problems, or I/O errors.
   Use *DIR_STATUS as a cache for the status.  */
நிலையான முழு
get_dir_status (FTS மாறிலி *fts, FTSENT மாறிலி *ent, முழு *dir_status)
{
  எனில் (*dir_status == DS_UNKNOWN)
    *dir_status = directory_status (fts->fts_cwd_fd, ent->fts_accpath);
  திரும்பு *dir_status;
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
நிலையான எண்ணிக்கை RM_status
prompt (FTS மாறிலி *fts, FTSENT மாறிலி *ent, bool is_dir,
        கட்டமைப்பு rm_options மாறிலி *x, எண்ணிக்கை Prompt_action mode,
        முழு *dir_status)
{
  முழு fd_cwd = fts->fts_cwd_fd;
  எழுத்து மாறிலி *full_name = ent->fts_path;
  எழுத்து மாறிலி *filename = ent->fts_accpath;
  கட்டமைப்பு stat st;
  கட்டமைப்பு stat *sbuf = &st;
  cache_stat_init (sbuf);

  முழு dirent_type = is_dir ? DT_DIR : DT_UNKNOWN;
  முழு write_protected = 0;

  /* When nonzero, this indicates that we failed to remove a child entry,
     either because the user declined an interactive prompt, or due to
     some other failure, like permissions.  */
  எனில் (ent->fts_எண்)
    திரும்பு RM_USER_DECLINED;

  எனில் (x->interactive == RMI_NEVER)
    திரும்பு RM_OK;

  முழு wp_errno = 0;
  எனில் (!x->ignore_missing_files
      && (x->interactive == RMI_ALWAYS || x->stdin_tty)
      && dirent_type != DT_LNK)
    {
      write_protected = write_protected_non_symlink (fd_cwd, filename, sbuf);
      wp_errno = errno;
    }

  எனில் (write_protected || x->interactive == RMI_ALWAYS)
    {
      எனில் (0 <= write_protected && dirent_type == DT_UNKNOWN)
        {
          எனில் (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) == 0)
            {
              எனில் (S_ISLNK (sbuf->st_mode))
                dirent_type = DT_LNK;
              இல்லையெனில் எனில் (S_ISDIR (sbuf->st_mode))
                dirent_type = DT_DIR;
              /* Otherwise it doesn't matter, so leave it DT_UNKNOWN.  */
            }
          இல்லையெனில்
            {
              /* This happens, e.g., with 'rm '''.  */
              write_protected = -1;
              wp_errno = errno;
            }
        }

      எனில் (0 <= write_protected)
        தேர்வு (dirent_type)
          {
          நிலை DT_LNK:
            /* Using permissions doesn't make sense for symlinks.  */
            எனில் (x->interactive != RMI_ALWAYS)
              திரும்பு RM_OK;
            நிறுத்து;

          நிலை DT_DIR:
             /* Unless we're either deleting directories or deleting
                recursively, we want to raise an EISDIR error rather than
                prompting the user  */
            எனில் ( ! (x->recursive
                    || (x->remove_empty_directories
                        && get_dir_status (fts, ent, dir_status) != 0)))
              {
                write_protected = -1;
                wp_errno = *dir_status <= 0 ? EISDIR : *dir_status;
              }
            நிறுத்து;
          }

      எழுத்து மாறிலி *quoted_name = quoteaf (full_name);

      எனில் (write_protected < 0)
        {
          error (0, wp_errno, _("cannot remove %s"), quoted_name);
          திரும்பு RM_ERROR;
        }

      /* Issue the prompt.  */
      எனில் (dirent_type == DT_DIR
          && mode == PA_DESCEND_INTO_DIR
          && get_dir_status (fts, ent, dir_status) == DS_NONEMPTY)
        fprintf (stderr,
                 (write_protected
                  ? _("%s: descend into write-protected directory %s? ")
                  : _("%s: descend into directory %s? ")),
                 program_name, quoted_name);
      இல்லையெனில் எனில் (0 < *dir_status)
        {
          எனில் ( ! (x->remove_empty_directories && *dir_status == EACCES))
            {
              error (0, *dir_status, _("cannot remove %s"), quoted_name);
              திரும்பு RM_ERROR;
            }

          /* The following code can lead to a successful deletion only with
             the --dir (-d) option (remove_empty_directories) and an empty
             inaccessible directory. In the first prompt call for a directory,
             we'd normally ask whether to descend into it, but in this case
             (it's inaccessible), that is not possible, so don't prompt.  */
          எனில் (mode == PA_DESCEND_INTO_DIR)
            திரும்பு RM_OK;

          fprintf (stderr,
               _("%s: attempt removal of inaccessible directory %s? "),
                   program_name, quoted_name);
        }
      இல்லையெனில்
        {
          எனில் (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, errno, _("cannot remove %s"), quoted_name);
              திரும்பு RM_ERROR;
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
                   program_name, file_type (sbuf), quoted_name);
        }

      திரும்பு yesno () ? RM_USER_ACCEPTED : RM_USER_DECLINED;
    }
  திரும்பு RM_OK;
}

/* When a function like unlink, rmdir, or fstatat fails with an errno
   value of ERRNUM, return true if the specified file system object
   is guaranteed not to exist;  otherwise, return false.  */
நிலையான உள்வரி bool
nonexistent_file_errno (முழு errnum)
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

  தேர்வு (errnum)
    {
    நிலை EILSEQ:
    நிலை EINVAL:
    நிலை ENOENT:
    நிலை ENOTDIR:
      திரும்பு true;
    இயல்பு:
      திரும்பு false;
    }
}

/* Encapsulate the test for whether the errno value, ERRNUM, is ignorable.  */
நிலையான உள்வரி bool
ignorable_missing (கட்டமைப்பு rm_options மாறிலி *x, முழு errnum)
{
  திரும்பு x->ignore_missing_files && nonexistent_file_errno (errnum);
}

/* Tell fts not to traverse into the hierarchy at ENT.  */
நிலையான வெறுமை
fts_skip_மரம் (FTS *fts, FTSENT *ent)
{
  fts_set (fts, ent, FTS_SKIP);
  /* Ensure that we do not process ENT a second time.  */
  ignore_மதிப்பு (fts_read (fts));
}

/* Upon unlink failure, or when the user declines to remove ENT, mark
   each of its ancestor directories, so that we know not to prompt for
   its removal.  */
நிலையான வெறுமை
mark_ancestor_dirs (FTSENT *ent)
{
  க்கு (FTSENT *p = ent->fts_parent; FTS_ROOTLEVEL <= p->fts_level;
       p = p->fts_parent)
    {
      எனில் (p->fts_எண்)
        நிறுத்து;
      p->fts_எண் = 1;
    }
}

/* Remove the file system object specified by ENT.  IS_DIR specifies
   whether it is expected to be a directory or non-directory.
   Return RM_OK upon success, else RM_ERROR.  */
நிலையான எண்ணிக்கை RM_status
excise (FTS *fts, FTSENT *ent, கட்டமைப்பு rm_options மாறிலி *x, bool is_dir)
{
  முழு flag = is_dir ? AT_REMOVEDIR : 0;
  எனில் (unlinkat (fts->fts_cwd_fd, ent->fts_accpath, flag) == 0)
    {
      எனில் (x->verbose)
        {
          printf ((is_dir
                   ? _("removed directory %s\n")
                   : _("removed %s\n")), quoteaf (ent->fts_path));
        }
      திரும்பு RM_OK;
    }

  /* The unlinkat from kernels like linux-2.6.32 reports EROFS even for
     nonexistent files.  When the file is indeed missing, map that to ENOENT,
     so that rm -f ignores it, as required.  Even without -f, this is useful
     because it makes rm print the more precise diagnostic.  */
  எனில் (errno == EROFS)
    {
      கட்டமைப்பு stat st;
      எனில் ( ! (fstatat (fts->fts_cwd_fd, ent->fts_accpath, &st,
                       AT_SYMLINK_NOFOLLOW)
              && errno == ENOENT))
        errno = EROFS;
    }

  எனில் (ignorable_missing (x, errno))
    திரும்பு RM_OK;

  /* When failing to rmdir an unreadable directory, we see errno values
     like EISDIR or ENOTDIR (or, on Solaris 10, EEXIST), but they would be
     meaningless in a diagnostic.  When that happens, use the earlier, more
     descriptive errno value.  */
  எனில் (ent->fts_info == FTS_DNR
      && (errno == ENOTEMPTY || errno == EISDIR || errno == ENOTDIR
          || errno == EEXIST)
      && ent->fts_errno != 0)
    errno = ent->fts_errno;
  error (0, errno, _("cannot remove %s"), quoteaf (ent->fts_path));
  mark_ancestor_dirs (ent);
  திரும்பு RM_ERROR;
}

/* This function is called once for every file system object that fts
   encounters.  fts performs a depth-first traversal.
   A directory is usually processed twice, first with fts_info == FTS_D,
   and later, after all of its entries have been processed, with FTS_DP.
   Return RM_ERROR upon error, RM_USER_DECLINED for a negative response
   to an interactive prompt, and otherwise, RM_OK.  */
நிலையான எண்ணிக்கை RM_status
rm_fts (FTS *fts, FTSENT *ent, கட்டமைப்பு rm_options மாறிலி *x)
{
  முழு dir_status = DS_UNKNOWN;

  தேர்வு (ent->fts_info)
    {
    நிலை FTS_D:			/* preorder directory */
      எனில் (!x->recursive)
        {
          /* Not recursive, so skip contents, and fail now unless
             removing empty directories.  */
          fts_set (fts, ent, FTS_SKIP);
          எனில் (x->remove_empty_directories)
            திரும்பு RM_OK;
          error (0, EISDIR, _("cannot remove %s"), quoteaf (ent->fts_path));
          ignore_மதிப்பு (fts_read (fts));
          திரும்பு RM_ERROR;
        }

      /* Perform checks that can apply only for command-line arguments.  */
      எனில் (ent->fts_level == FTS_ROOTLEVEL)
        {
          /* POSIX says:
             If the basename of a command line argument is "." or "..",
             diagnose it and do nothing more with that argument.  */
          எனில் (dot_or_dotdot (last_component (ent->fts_accpath)))
            {
              error (0, 0,
                     _("refusing to remove %s or %s directory: skipping %s"),
                     quoteaf_n (0, "."), quoteaf_n (1, ".."),
                     quoteaf_n (2, ent->fts_path));
              fts_skip_மரம் (fts, ent);
              திரும்பு RM_ERROR;
            }

          /* POSIX also says:
             If a command line argument resolves to "/" (and --preserve-root
             is in effect -- default) diagnose and skip it.  */
          எனில் (வேர்_DEV_INO_CHECK (x->வேர்_dev_ino, ent->fts_statp))
            {
              வேர்_DEV_INO_WARN (ent->fts_path);
              fts_skip_மரம் (fts, ent);
              திரும்பு RM_ERROR;
            }

          /* If a command line argument is a mount point and
             --preserve-root=all is in effect, diagnose and skip it.
             This doesn't handle "/", but that's handled above.  */
          எனில் (x->preserve_all_வேர்)
            {
              bool failed = false;
              எழுத்து *parent = file_name_concat (ent->fts_accpath, "..", வெற்று);
              கட்டமைப்பு stat statbuf;

              எனில் (!parent || lstat (parent, &statbuf))
                {
                  error (0, 0,
                         _("failed to stat %s: skipping %s"),
                         quoteaf_n (0, parent),
                         quoteaf_n (1, ent->fts_accpath));
                  failed = true;
                }

              விடுவி (parent);

              எனில் (failed || fts->fts_dev != statbuf.st_dev)
                {
                  எனில் (! failed)
                    {
                      error (0, 0,
                             _("skipping %s, since it's on a different device"),
                             quoteaf (ent->fts_path));
                      error (0, 0, _("and --preserve-root=all is in effect"));
                    }
                  fts_skip_மரம் (fts, ent);
                  திரும்பு RM_ERROR;
                }
            }
        }

      {
        எண்ணிக்கை RM_status s = prompt (fts, ent, true /*is_dir*/, x,
                                   PA_DESCEND_INTO_DIR, &dir_status);

        எனில் (s == RM_USER_ACCEPTED && dir_status == DS_EMPTY)
          {
            /* When we know (from prompt when in interactive mode)
               that this is an empty directory, don't prompt twice.  */
            s = excise (fts, ent, x, true);
            எனில் (s == RM_OK)
              fts_skip_மரம் (fts, ent);
          }

        எனில் (! (s == RM_OK || s == RM_USER_ACCEPTED))
          {
            mark_ancestor_dirs (ent);
            fts_skip_மரம் (fts, ent);
          }

        திரும்பு s;
      }

    நிலை FTS_F:			/* regular file */
    நிலை FTS_NS:		/* stat(2) failed */
    நிலை FTS_SL:		/* symbolic link */
    நிலை FTS_SLNONE:		/* symbolic link without target */
    நிலை FTS_DP:		/* postorder directory */
    நிலை FTS_DNR:		/* unreadable directory */
    நிலை FTS_NSOK:		/* e.g., dangling symlink */
    நிலை FTS_DEFAULT:		/* none of the above */
      {
        /* With --one-file-system, do not attempt to remove a mount point.
           fts' FTS_XDEV ensures that we don't process any entries under
           the mount point.  */
        எனில் (ent->fts_info == FTS_DP
            && x->one_file_system
            && FTS_ROOTLEVEL < ent->fts_level
            && ent->fts_statp->st_dev != fts->fts_dev)
          {
            mark_ancestor_dirs (ent);
            error (0, 0, _("skipping %s, since it's on a different device"),
                   quoteaf (ent->fts_path));
            திரும்பு RM_ERROR;
          }

        bool is_dir = ent->fts_info == FTS_DP || ent->fts_info == FTS_DNR;
        எண்ணிக்கை RM_status s = prompt (fts, ent, is_dir, x, PA_REMOVE_DIR,
                                   &dir_status);
        எனில் (! (s == RM_OK || s == RM_USER_ACCEPTED))
          திரும்பு s;
        திரும்பு excise (fts, ent, x, is_dir);
      }

    நிலை FTS_DC:		/* directory that causes cycles */
      emit_cycle_warning (ent->fts_path);
      fts_skip_மரம் (fts, ent);
      திரும்பு RM_ERROR;

    நிலை FTS_ERR:
      /* Various failures, from opendir to ENOMEM, to failure to "return"
         to preceding directory, can provoke this.  */
      error (0, ent->fts_errno, _("traversal failed: %s"),
             quotef (ent->fts_path));
      fts_skip_மரம் (fts, ent);
      திரும்பு RM_ERROR;

    இயல்பு:
      error (0, 0, _("unexpected failure: fts_info=%d: %s\n"
                     "please report to %s"),
             ent->fts_info,
             quotef (ent->fts_path),
             PACKAGE_BUGREPORT);
      abort ();
    }
}

/* Remove FILEs, honoring options specified via X.
   Return RM_OK if successful.  */
எண்ணிக்கை RM_status
rm (எழுத்து *மாறிலி *file, கட்டமைப்பு rm_options மாறிலி *x)
{
  எண்ணிக்கை RM_status rm_status = RM_OK;

  எனில் (*file)
    {
      முழு bit_flags = (FTS_CWDFD
                       | FTS_NOSTAT
                       | FTS_PHYSICAL);

      எனில் (x->one_file_system)
        bit_flags |= FTS_XDEV;

      FTS *fts = xfts_open (file, bit_flags, வெற்று);

      போது (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          எனில் (ent == வெற்று)
            {
              எனில் (errno != 0)
                {
                  error (0, errno, _("fts_read failed"));
                  rm_status = RM_ERROR;
                }
              நிறுத்து;
            }

          எண்ணிக்கை RM_status s = rm_fts (fts, ent, x);

          affirm (VALID_STATUS (s));
          UPDATE_STATUS (rm_status, s);
        }

      எனில் (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          rm_status = RM_ERROR;
        }
    }

  திரும்பு rm_status;
}
