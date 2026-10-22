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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "file-type.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "remove.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "write-any-file.h"
#అన్తర్భావ "xfts.h"
#అన్తర్భావ "yesno.h"

/* The prompt function may be called twice for a given directory.
   The first time, we ask whether to descend into it, and the
   second time, we ask whether to remove it.  */
గణన Prompt_action
  {
    PA_DESCEND_INTO_DIR = 2,
    PA_REMOVE_DIR
  };

/* D_TYPE(D) is the type of directory entry D if known, DT_UNKNOWN
   otherwise.  */
#ఒకవేళ ! HAVE_STRUCT_DIRENT_D_TYPE
/* Any int values will do here, so long as they're distinct.
   Undef any existing macros out of the way.  */
# undef DT_UNKNOWN
# undef DT_DIR
# undef DT_LNK
# define DT_UNKNOWN 0
# define DT_DIR 1
# define DT_LNK 2
#అయితే_ముగింపు

/* Like fstatat, but cache on POSIX-compatible systems.  */
స్థిర పూర్ణాంకం
cache_fstatat (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *file, నిర్మాణం stat *st, పూర్ణాంకం flag)
{
#ఒకవేళ HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  /* If ST->st_atim.tv_nsec is -1, the status has not been gotten yet.
     If less than -1, fstatat failed with errno == ST->st_ino.
     Otherwise, the status has already been gotten, so return 0.  */
  ఒకవేళ (0 <= st->st_atim.tv_nsec)
    తిరిగి 0;
  ఒకవేళ (st->st_atim.tv_nsec == -1)
    {
      ఒకవేళ (fstatat (fd, file, st, flag) == 0)
        తిరిగి 0;
      st->st_atim.tv_nsec = -2;
      st->st_ino = errno;
    }
  errno = st->st_ino;
  తిరిగి -1;
#లేకపోతే
  తిరిగి fstatat (fd, file, st, flag);
#అయితే_ముగింపు
}

/* Initialize a fstatat cache *ST.  Return ST for convenience.  */
స్థిర అంతఃవరుస నిర్మాణం stat *
cache_stat_init (నిర్మాణం stat *st)
{
#ఒకవేళ HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  st->st_atim.tv_nsec = -1;
#అయితే_ముగింపు
  తిరిగి st;
}

/* Return 1 if FILE is an unwritable non-symlink,
   0 if it is writable or some other type of file,
   -1 and set errno if there is some problem in determining the answer.
   Set *BUF to the file status.  */
స్థిర పూర్ణాంకం
write_protected_non_symlink (పూర్ణాంకం fd_cwd,
                             అక్షరం స్థిరరాశి *file,
                             నిర్మాణం stat *buf)
{
  ఒకవేళ (can_write_any_file ())
    తిరిగి 0;
  ఒకవేళ (cache_fstatat (fd_cwd, file, buf, AT_SYMLINK_NOFOLLOW) != 0)
    తిరిగి -1;
  ఒకవేళ (S_ISLNK (buf->st_బహుళకం))
    తిరిగి 0;
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
    ఒకవేళ (faccessat (fd_cwd, file, W_OK, AT_EACCESS) == 0)
      తిరిగి 0;

    తిరిగి errno == EACCES ? 1 : -1;
  }
}

/* Return the status of the directory identified by FTS and ENT.
   This is -1 if the directory is empty, 0 if it is nonempty,
   and a positive error number if there was trouble determining the status,
   e.g., it is not a directory, or permissions problems, or I/O errors.
   Use *DIR_STATUS as a cache for the status.  */
స్థిర పూర్ణాంకం
get_dir_స్థితి (FTS స్థిరరాశి *fts, FTSENT స్థిరరాశి *ent, పూర్ణాంకం *dir_స్థితి)
{
  ఒకవేళ (*dir_స్థితి == DS_UNKNOWN)
    *dir_స్థితి = directory_స్థితి (fts->fts_cwd_fd, ent->fts_accpath);
  తిరిగి *dir_స్థితి;
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
స్థిర గణన RM_స్థితి
prompt (FTS స్థిరరాశి *fts, FTSENT స్థిరరాశి *ent, bool is_dir,
        నిర్మాణం rm_options స్థిరరాశి *x, గణన Prompt_action బహుళకం_2,
        పూర్ణాంకం *dir_స్థితి)
{
  పూర్ణాంకం fd_cwd = fts->fts_cwd_fd;
  అక్షరం స్థిరరాశి *నిండిన_name = ent->fts_మార్గం;
  అక్షరం స్థిరరాశి *filename = ent->fts_accpath;
  నిర్మాణం stat st;
  నిర్మాణం stat *sbuf = &st;
  cache_stat_init (sbuf);

  పూర్ణాంకం dirent_type = is_dir ? DT_DIR : DT_UNKNOWN;
  పూర్ణాంకం write_protected = 0;

  /* When nonzero, this indicates that we failed to remove a child entry,
     either because the user declined an interactive prompt, or due to
     some other failure, like permissions.  */
  ఒకవేళ (ent->fts_సంఖ్య)
    తిరిగి RM_USER_DECLINED;

  ఒకవేళ (x->interactive == RMI_NEVER)
    తిరిగి RM_OK;

  పూర్ణాంకం wp_errno = 0;
  ఒకవేళ (!x->ignore_missing_files
      && (x->interactive == RMI_ALWAYS || x->stdin_tty)
      && dirent_type != DT_LNK)
    {
      write_protected = write_protected_non_symlink (fd_cwd, filename, sbuf);
      wp_errno = errno;
    }

  ఒకవేళ (write_protected || x->interactive == RMI_ALWAYS)
    {
      ఒకవేళ (0 <= write_protected && dirent_type == DT_UNKNOWN)
        {
          ఒకవేళ (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) == 0)
            {
              ఒకవేళ (S_ISLNK (sbuf->st_బహుళకం))
                dirent_type = DT_LNK;
              లేకపోతే ఒకవేళ (S_ISDIR (sbuf->st_బహుళకం))
                dirent_type = DT_DIR;
              /* Otherwise it doesn't matter, so leave it DT_UNKNOWN.  */
            }
          లేకపోతే
            {
              /* This happens, e.g., with 'rm '''.  */
              write_protected = -1;
              wp_errno = errno;
            }
        }

      ఒకవేళ (0 <= write_protected)
        ఎంపిక (dirent_type)
          {
          స్థితి DT_LNK:
            /* Using permissions doesn't make sense for symlinks.  */
            ఒకవేళ (x->interactive != RMI_ALWAYS)
              తిరిగి RM_OK;
            ఆపు;

          స్థితి DT_DIR:
             /* Unless we're either deleting directories or deleting
                recursively, we want to raise an EISDIR error rather than
                prompting the user  */
            ఒకవేళ ( ! (x->recursive
                    || (x->remove_ఖాళీలు_directories
                        && get_dir_స్థితి (fts, ent, dir_స్థితి) != 0)))
              {
                write_protected = -1;
                wp_errno = *dir_స్థితి <= 0 ? EISDIR : *dir_స్థితి;
              }
            ఆపు;
          }

      అక్షరం స్థిరరాశి *quoted_name = quoteaf (నిండిన_name);

      ఒకవేళ (write_protected < 0)
        {
          error (0, wp_errno, _("cannot remove %s"), quoted_name);
          తిరిగి RM_ERROR;
        }

      /* Issue the prompt.  */
      ఒకవేళ (dirent_type == DT_DIR
          && బహుళకం_2 == PA_DESCEND_INTO_DIR
          && get_dir_స్థితి (fts, ent, dir_స్థితి) == DS_NONEMPTY)
        fprintf (stderr,
                 (write_protected
                  ? _("%s: descend into write-protected directory %s? ")
                  : _("%s: descend into directory %s? ")),
                 కార్యక్రమం_name, quoted_name);
      లేకపోతే ఒకవేళ (0 < *dir_స్థితి)
        {
          ఒకవేళ ( ! (x->remove_ఖాళీలు_directories && *dir_స్థితి == EACCES))
            {
              error (0, *dir_స్థితి, _("cannot remove %s"), quoted_name);
              తిరిగి RM_ERROR;
            }

          /* The following code can lead to a successful deletion only with
             the --dir (-d) option (remove_empty_directories) and an empty
             inaccessible directory. In the first prompt call for a directory,
             we'd normally ask whether to descend into it, but in this case
             (it's inaccessible), that is not possible, so don't prompt.  */
          ఒకవేళ (బహుళకం_2 == PA_DESCEND_INTO_DIR)
            తిరిగి RM_OK;

          fprintf (stderr,
               _("%s: attempt removal of inaccessible directory %s? "),
                   కార్యక్రమం_name, quoted_name);
        }
      లేకపోతే
        {
          ఒకవేళ (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, errno, _("cannot remove %s"), quoted_name);
              తిరిగి RM_ERROR;
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
                   కార్యక్రమం_name, file_type (sbuf), quoted_name);
        }

      తిరిగి yesno () ? RM_USER_ACCEPTED : RM_USER_DECLINED;
    }
  తిరిగి RM_OK;
}

/* When a function like unlink, rmdir, or fstatat fails with an errno
   value of ERRNUM, return true if the specified file system object
   is guaranteed not to exist;  otherwise, return false.  */
స్థిర అంతఃవరుస bool
nonexistent_file_errno (పూర్ణాంకం errnum)
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

  ఎంపిక (errnum)
    {
    స్థితి EILSEQ:
    స్థితి EINVAL:
    స్థితి ENOENT:
    స్థితి ENOTDIR:
      తిరిగి true;
    అప్రమేయ:
      తిరిగి false;
    }
}

/* Encapsulate the test for whether the errno value, ERRNUM, is ignorable.  */
స్థిర అంతఃవరుస bool
ignorable_missing (నిర్మాణం rm_options స్థిరరాశి *x, పూర్ణాంకం errnum)
{
  తిరిగి x->ignore_missing_files && nonexistent_file_errno (errnum);
}

/* Tell fts not to traverse into the hierarchy at ENT.  */
స్థిర శూన్య
fts_skip_వృక్షం (FTS *fts, FTSENT *ent)
{
  fts_సమితి (fts, ent, FTS_SKIP);
  /* Ensure that we do not process ENT a second time.  */
  ignore_విలువ (fts_read (fts));
}

/* Upon unlink failure, or when the user declines to remove ENT, mark
   each of its ancestor directories, so that we know not to prompt for
   its removal.  */
స్థిర శూన్య
mark_ancestor_dirs (FTSENT *ent)
{
  కొరకు (FTSENT *p = ent->fts_మాతృ; FTS_ROOTLEVEL <= p->fts_level;
       p = p->fts_మాతృ)
    {
      ఒకవేళ (p->fts_సంఖ్య)
        ఆపు;
      p->fts_సంఖ్య = 1;
    }
}

/* Remove the file system object specified by ENT.  IS_DIR specifies
   whether it is expected to be a directory or non-directory.
   Return RM_OK upon success, else RM_ERROR.  */
స్థిర గణన RM_స్థితి
excise (FTS *fts, FTSENT *ent, నిర్మాణం rm_options స్థిరరాశి *x, bool is_dir)
{
  పూర్ణాంకం flag = is_dir ? AT_REMOVEDIR : 0;
  ఒకవేళ (unlinkat (fts->fts_cwd_fd, ent->fts_accpath, flag) == 0)
    {
      ఒకవేళ (x->verbose)
        {
          రాయి ((is_dir
                   ? _("removed directory %s\n")
                   : _("removed %s\n")), quoteaf (ent->fts_మార్గం));
        }
      తిరిగి RM_OK;
    }

  /* The unlinkat from kernels like linux-2.6.32 reports EROFS even for
     nonexistent files.  When the file is indeed missing, map that to ENOENT,
     so that rm -f ignores it, as required.  Even without -f, this is useful
     because it makes rm print the more precise diagnostic.  */
  ఒకవేళ (errno == EROFS)
    {
      నిర్మాణం stat st;
      ఒకవేళ ( ! (fstatat (fts->fts_cwd_fd, ent->fts_accpath, &st,
                       AT_SYMLINK_NOFOLLOW)
              && errno == ENOENT))
        errno = EROFS;
    }

  ఒకవేళ (ignorable_missing (x, errno))
    తిరిగి RM_OK;

  /* When failing to rmdir an unreadable directory, we see errno values
     like EISDIR or ENOTDIR (or, on Solaris 10, EEXIST), but they would be
     meaningless in a diagnostic.  When that happens, use the earlier, more
     descriptive errno value.  */
  ఒకవేళ (ent->fts_info == FTS_DNR
      && (errno == ENOTEMPTY || errno == EISDIR || errno == ENOTDIR
          || errno == EEXIST)
      && ent->fts_errno != 0)
    errno = ent->fts_errno;
  error (0, errno, _("cannot remove %s"), quoteaf (ent->fts_మార్గం));
  mark_ancestor_dirs (ent);
  తిరిగి RM_ERROR;
}

/* This function is called once for every file system object that fts
   encounters.  fts performs a depth-first traversal.
   A directory is usually processed twice, first with fts_info == FTS_D,
   and later, after all of its entries have been processed, with FTS_DP.
   Return RM_ERROR upon error, RM_USER_DECLINED for a negative response
   to an interactive prompt, and otherwise, RM_OK.  */
స్థిర గణన RM_స్థితి
rm_fts (FTS *fts, FTSENT *ent, నిర్మాణం rm_options స్థిరరాశి *x)
{
  పూర్ణాంకం dir_స్థితి = DS_UNKNOWN;

  ఎంపిక (ent->fts_info)
    {
    స్థితి FTS_D:			/* preorder directory */
      ఒకవేళ (!x->recursive)
        {
          /* Not recursive, so skip contents, and fail now unless
             removing empty directories.  */
          fts_సమితి (fts, ent, FTS_SKIP);
          ఒకవేళ (x->remove_ఖాళీలు_directories)
            తిరిగి RM_OK;
          error (0, EISDIR, _("cannot remove %s"), quoteaf (ent->fts_మార్గం));
          ignore_విలువ (fts_read (fts));
          తిరిగి RM_ERROR;
        }

      /* Perform checks that can apply only for command-line arguments.  */
      ఒకవేళ (ent->fts_level == FTS_ROOTLEVEL)
        {
          /* POSIX says:
             If the basename of a command line argument is "." or "..",
             diagnose it and do nothing more with that argument.  */
          ఒకవేళ (dot_or_dotdot (చివరి_component (ent->fts_accpath)))
            {
              error (0, 0,
                     _("refusing to remove %s or %s directory: skipping %s"),
                     quoteaf_n (0, "."), quoteaf_n (1, ".."),
                     quoteaf_n (2, ent->fts_మార్గం));
              fts_skip_వృక్షం (fts, ent);
              తిరిగి RM_ERROR;
            }

          /* POSIX also says:
             If a command line argument resolves to "/" (and --preserve-root
             is in effect -- default) diagnose and skip it.  */
          ఒకవేళ (మూలం_DEV_INO_CHECK (x->మూలం_dev_ino, ent->fts_statp))
            {
              మూలం_DEV_INO_WARN (ent->fts_మార్గం);
              fts_skip_వృక్షం (fts, ent);
              తిరిగి RM_ERROR;
            }

          /* If a command line argument is a mount point and
             --preserve-root=all is in effect, diagnose and skip it.
             This doesn't handle "/", but that's handled above.  */
          ఒకవేళ (x->preserve_all_మూలం)
            {
              bool failed = false;
              అక్షరం *మాతృ = file_name_concat (ent->fts_accpath, "..", ఖాళీ);
              నిర్మాణం stat statbuf;

              ఒకవేళ (!మాతృ || lstat (మాతృ, &statbuf))
                {
                  error (0, 0,
                         _("failed to stat %s: skipping %s"),
                         quoteaf_n (0, మాతృ),
                         quoteaf_n (1, ent->fts_accpath));
                  failed = true;
                }

              విడుదల (మాతృ);

              ఒకవేళ (failed || fts->fts_dev != statbuf.st_dev)
                {
                  ఒకవేళ (! failed)
                    {
                      error (0, 0,
                             _("skipping %s, since it's on a different device"),
                             quoteaf (ent->fts_మార్గం));
                      error (0, 0, _("and --preserve-root=all is in effect"));
                    }
                  fts_skip_వృక్షం (fts, ent);
                  తిరిగి RM_ERROR;
                }
            }
        }

      {
        గణన RM_స్థితి s = prompt (fts, ent, true /*is_dir*/, x,
                                   PA_DESCEND_INTO_DIR, &dir_స్థితి);

        ఒకవేళ (s == RM_USER_ACCEPTED && dir_స్థితి == DS_ఖాళీలు)
          {
            /* When we know (from prompt when in interactive mode)
               that this is an empty directory, don't prompt twice.  */
            s = excise (fts, ent, x, true);
            ఒకవేళ (s == RM_OK)
              fts_skip_వృక్షం (fts, ent);
          }

        ఒకవేళ (! (s == RM_OK || s == RM_USER_ACCEPTED))
          {
            mark_ancestor_dirs (ent);
            fts_skip_వృక్షం (fts, ent);
          }

        తిరిగి s;
      }

    స్థితి FTS_F:			/* regular file */
    స్థితి FTS_NS:		/* stat(2) failed */
    స్థితి FTS_SL:		/* symbolic link */
    స్థితి FTS_SLNONE:		/* symbolic link without target */
    స్థితి FTS_DP:		/* postorder directory */
    స్థితి FTS_DNR:		/* unreadable directory */
    స్థితి FTS_NSOK:		/* e.g., dangling symlink */
    స్థితి FTS_DEFAULT:		/* none of the above */
      {
        /* With --one-file-system, do not attempt to remove a mount point.
           fts' FTS_XDEV ensures that we don't process any entries under
           the mount point.  */
        ఒకవేళ (ent->fts_info == FTS_DP
            && x->one_file_system
            && FTS_ROOTLEVEL < ent->fts_level
            && ent->fts_statp->st_dev != fts->fts_dev)
          {
            mark_ancestor_dirs (ent);
            error (0, 0, _("skipping %s, since it's on a different device"),
                   quoteaf (ent->fts_మార్గం));
            తిరిగి RM_ERROR;
          }

        bool is_dir = ent->fts_info == FTS_DP || ent->fts_info == FTS_DNR;
        గణన RM_స్థితి s = prompt (fts, ent, is_dir, x, PA_REMOVE_DIR,
                                   &dir_స్థితి);
        ఒకవేళ (! (s == RM_OK || s == RM_USER_ACCEPTED))
          తిరిగి s;
        తిరిగి excise (fts, ent, x, is_dir);
      }

    స్థితి FTS_DC:		/* directory that causes cycles */
      emit_చక్రం_warning (ent->fts_మార్గం);
      fts_skip_వృక్షం (fts, ent);
      తిరిగి RM_ERROR;

    స్థితి FTS_ERR:
      /* Various failures, from opendir to ENOMEM, to failure to "return"
         to preceding directory, can provoke this.  */
      error (0, ent->fts_errno, _("traversal failed: %s"),
             quotef (ent->fts_మార్గం));
      fts_skip_వృక్షం (fts, ent);
      తిరిగి RM_ERROR;

    అప్రమేయ:
      error (0, 0, _("unexpected failure: fts_info=%d: %s\n"
                     "please report to %s"),
             ent->fts_info,
             quotef (ent->fts_మార్గం),
             PACKAGE_BUGREPORT);
      abort ();
    }
}

/* Remove FILEs, honoring options specified via X.
   Return RM_OK if successful.  */
గణన RM_స్థితి
rm (అక్షరం *స్థిరరాశి *file, నిర్మాణం rm_options స్థిరరాశి *x)
{
  గణన RM_స్థితి rm_స్థితి = RM_OK;

  ఒకవేళ (*file)
    {
      పూర్ణాంకం బిట్టు_flags = (FTS_CWDFD
                       | FTS_NOSTAT
                       | FTS_PHYSICAL);

      ఒకవేళ (x->one_file_system)
        బిట్టు_flags |= FTS_XDEV;

      FTS *fts = xfts_open (file, బిట్టు_flags, ఖాళీ);

      అయితే (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ఒకవేళ (ent == ఖాళీ)
            {
              ఒకవేళ (errno != 0)
                {
                  error (0, errno, _("fts_read failed"));
                  rm_స్థితి = RM_ERROR;
                }
              ఆపు;
            }

          గణన RM_స్థితి s = rm_fts (fts, ent, x);

          affirm (VALID_స్థితి (s));
          UPDATE_స్థితి (rm_స్థితి, s);
        }

      ఒకవేళ (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          rm_స్థితి = RM_ERROR;
        }
    }

  తిరిగి rm_స్థితి;
}
