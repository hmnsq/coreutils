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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "file-type.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "remove.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "write-any-file.h"
#अन्तर्भाव "xfts.h"
#अन्तर्भाव "yesno.h"

/* The prompt function may be called twice for a given directory.
   The first time, we ask whether to descend into it, and the
   second time, we ask whether to remove it.  */
गणना Prompt_action
  {
    PA_DESCEND_INTO_DIR = 2,
    PA_REMOVE_DIR
  };

/* D_TYPE(D) is the type of directory entry D if known, DT_UNKNOWN
   otherwise.  */
#जर ! HAVE_STRUCT_DIRENT_D_TYPE
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
स्थिर पूर्णांक
cache_fstatat (पूर्णांक fd, अक्षर स्थिरांक *file, संरचना stat *st, पूर्णांक flag)
{
#जर HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  /* If ST->st_atim.tv_nsec is -1, the status has not been gotten yet.
     If less than -1, fstatat failed with errno == ST->st_ino.
     Otherwise, the status has already been gotten, so return 0.  */
  जर (0 <= st->st_atim.tv_nsec)
    परत 0;
  जर (st->st_atim.tv_nsec == -1)
    {
      जर (fstatat (fd, file, st, flag) == 0)
        परत 0;
      st->st_atim.tv_nsec = -2;
      st->st_ino = त्रुटि_सं;
    }
  त्रुटि_सं = st->st_ino;
  परत -1;
#अन्यथा
  परत fstatat (fd, file, st, flag);
#endif
}

/* Initialize a fstatat cache *ST.  Return ST for convenience.  */
स्थिर ओळीत संरचना stat *
cache_stat_init (संरचना stat *st)
{
#जर HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  st->st_atim.tv_nsec = -1;
#endif
  परत st;
}

/* Return 1 if FILE is an unwritable non-symlink,
   0 if it is writable or some other type of file,
   -1 and set errno if there is some problem in determining the answer.
   Set *BUF to the file status.  */
स्थिर पूर्णांक
write_protected_non_symlink (पूर्णांक fd_cwd,
                             अक्षर स्थिरांक *file,
                             संरचना stat *buf)
{
  जर (can_write_any_file ())
    परत 0;
  जर (cache_fstatat (fd_cwd, file, buf, AT_SYMLINK_NOFOLLOW) != 0)
    परत -1;
  जर (S_ISLNK (buf->st_mode))
    परत 0;
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
    जर (faccessat (fd_cwd, file, W_OK, AT_EACCESS) == 0)
      परत 0;

    परत त्रुटि_सं == EACCES ? 1 : -1;
  }
}

/* Return the status of the directory identified by FTS and ENT.
   This is -1 if the directory is empty, 0 if it is nonempty,
   and a positive error number if there was trouble determining the status,
   e.g., it is not a directory, or permissions problems, or I/O errors.
   Use *DIR_STATUS as a cache for the status.  */
स्थिर पूर्णांक
get_dir_status (FTS स्थिरांक *fts, FTSENT स्थिरांक *ent, पूर्णांक *dir_status)
{
  जर (*dir_status == DS_UNKNOWN)
    *dir_status = directory_status (fts->fts_cwd_fd, ent->fts_accpath);
  परत *dir_status;
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
स्थिर गणना RM_status
prompt (FTS स्थिरांक *fts, FTSENT स्थिरांक *ent, bool is_dir,
        संरचना rm_options स्थिरांक *x, गणना Prompt_action mode,
        पूर्णांक *dir_status)
{
  पूर्णांक fd_cwd = fts->fts_cwd_fd;
  अक्षर स्थिरांक *full_name = ent->fts_path;
  अक्षर स्थिरांक *filename = ent->fts_accpath;
  संरचना stat st;
  संरचना stat *sbuf = &st;
  cache_stat_init (sbuf);

  पूर्णांक dirent_type = is_dir ? DT_DIR : DT_UNKNOWN;
  पूर्णांक write_protected = 0;

  /* When nonzero, this indicates that we failed to remove a child entry,
     either because the user declined an interactive prompt, or due to
     some other failure, like permissions.  */
  जर (ent->fts_संख्या)
    परत RM_USER_DECLINED;

  जर (x->interactive == RMI_NEVER)
    परत RM_OK;

  पूर्णांक wp_errno = 0;
  जर (!x->ignore_missing_files
      && (x->interactive == RMI_ALWAYS || x->stdin_tty)
      && dirent_type != DT_LNK)
    {
      write_protected = write_protected_non_symlink (fd_cwd, filename, sbuf);
      wp_errno = त्रुटि_सं;
    }

  जर (write_protected || x->interactive == RMI_ALWAYS)
    {
      जर (0 <= write_protected && dirent_type == DT_UNKNOWN)
        {
          जर (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) == 0)
            {
              जर (S_ISLNK (sbuf->st_mode))
                dirent_type = DT_LNK;
              अन्यथा जर (S_ISDIR (sbuf->st_mode))
                dirent_type = DT_DIR;
              /* Otherwise it doesn't matter, so leave it DT_UNKNOWN.  */
            }
          अन्यथा
            {
              /* This happens, e.g., with 'rm '''.  */
              write_protected = -1;
              wp_errno = त्रुटि_सं;
            }
        }

      जर (0 <= write_protected)
        निवड (dirent_type)
          {
          स्थिती DT_LNK:
            /* Using permissions doesn't make sense for symlinks.  */
            जर (x->interactive != RMI_ALWAYS)
              परत RM_OK;
            थांबा;

          स्थिती DT_DIR:
             /* Unless we're either deleting directories or deleting
                recursively, we want to raise an EISDIR error rather than
                prompting the user  */
            जर ( ! (x->recursive
                    || (x->remove_empty_directories
                        && get_dir_status (fts, ent, dir_status) != 0)))
              {
                write_protected = -1;
                wp_errno = *dir_status <= 0 ? EISDIR : *dir_status;
              }
            थांबा;
          }

      अक्षर स्थिरांक *quoted_name = quoteaf (full_name);

      जर (write_protected < 0)
        {
          error (0, wp_errno, _("cannot remove %s"), quoted_name);
          परत RM_ERROR;
        }

      /* Issue the prompt.  */
      जर (dirent_type == DT_DIR
          && mode == PA_DESCEND_INTO_DIR
          && get_dir_status (fts, ent, dir_status) == DS_NONEMPTY)
        ख_लिखो (मानक_त्रुटि,
                 (write_protected
                  ? _("%s: descend into write-protected directory %s? ")
                  : _("%s: descend into directory %s? ")),
                 program_name, quoted_name);
      अन्यथा जर (0 < *dir_status)
        {
          जर ( ! (x->remove_empty_directories && *dir_status == EACCES))
            {
              error (0, *dir_status, _("cannot remove %s"), quoted_name);
              परत RM_ERROR;
            }

          /* The following code can lead to a successful deletion only with
             the --dir (-d) option (remove_empty_directories) and an empty
             inaccessible directory. In the first prompt call for a directory,
             we'd normally ask whether to descend into it, but in this case
             (it's inaccessible), that is not possible, so don't prompt.  */
          जर (mode == PA_DESCEND_INTO_DIR)
            परत RM_OK;

          ख_लिखो (मानक_त्रुटि,
               _("%s: attempt removal of inaccessible directory %s? "),
                   program_name, quoted_name);
        }
      अन्यथा
        {
          जर (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, त्रुटि_सं, _("cannot remove %s"), quoted_name);
              परत RM_ERROR;
            }

          ख_लिखो (मानक_त्रुटि,
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

      परत yesno () ? RM_USER_ACCEPTED : RM_USER_DECLINED;
    }
  परत RM_OK;
}

/* When a function like unlink, rmdir, or fstatat fails with an errno
   value of ERRNUM, return true if the specified file system object
   is guaranteed not to exist;  otherwise, return false.  */
स्थिर ओळीत bool
nonexistent_file_errno (पूर्णांक errnum)
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

  निवड (errnum)
    {
    स्थिती EILSEQ:
    स्थिती EINVAL:
    स्थिती ENOENT:
    स्थिती ENOTDIR:
      परत true;
    पूर्वनिर्धारित:
      परत false;
    }
}

/* Encapsulate the test for whether the errno value, ERRNUM, is ignorable.  */
स्थिर ओळीत bool
ignorable_missing (संरचना rm_options स्थिरांक *x, पूर्णांक errnum)
{
  परत x->ignore_missing_files && nonexistent_file_errno (errnum);
}

/* Tell fts not to traverse into the hierarchy at ENT.  */
स्थिर शून्य
fts_skip_वृक्ष (FTS *fts, FTSENT *ent)
{
  fts_set (fts, ent, FTS_SKIP);
  /* Ensure that we do not process ENT a second time.  */
  ignore_मूल्य (fts_read (fts));
}

/* Upon unlink failure, or when the user declines to remove ENT, mark
   each of its ancestor directories, so that we know not to prompt for
   its removal.  */
स्थिर शून्य
mark_ancestor_dirs (FTSENT *ent)
{
  साठी (FTSENT *p = ent->fts_parent; FTS_ROOTLEVEL <= p->fts_level;
       p = p->fts_parent)
    {
      जर (p->fts_संख्या)
        थांबा;
      p->fts_संख्या = 1;
    }
}

/* Remove the file system object specified by ENT.  IS_DIR specifies
   whether it is expected to be a directory or non-directory.
   Return RM_OK upon success, else RM_ERROR.  */
स्थिर गणना RM_status
excise (FTS *fts, FTSENT *ent, संरचना rm_options स्थिरांक *x, bool is_dir)
{
  पूर्णांक flag = is_dir ? AT_REMOVEDIR : 0;
  जर (unlinkat (fts->fts_cwd_fd, ent->fts_accpath, flag) == 0)
    {
      जर (x->verbose)
        {
          म_लिखो ((is_dir
                   ? _("removed directory %s\n")
                   : _("removed %s\n")), quoteaf (ent->fts_path));
        }
      परत RM_OK;
    }

  /* The unlinkat from kernels like linux-2.6.32 reports EROFS even for
     nonexistent files.  When the file is indeed missing, map that to ENOENT,
     so that rm -f ignores it, as required.  Even without -f, this is useful
     because it makes rm print the more precise diagnostic.  */
  जर (त्रुटि_सं == EROFS)
    {
      संरचना stat st;
      जर ( ! (fstatat (fts->fts_cwd_fd, ent->fts_accpath, &st,
                       AT_SYMLINK_NOFOLLOW)
              && त्रुटि_सं == ENOENT))
        त्रुटि_सं = EROFS;
    }

  जर (ignorable_missing (x, त्रुटि_सं))
    परत RM_OK;

  /* When failing to rmdir an unreadable directory, we see errno values
     like EISDIR or ENOTDIR (or, on Solaris 10, EEXIST), but they would be
     meaningless in a diagnostic.  When that happens, use the earlier, more
     descriptive errno value.  */
  जर (ent->fts_info == FTS_DNR
      && (त्रुटि_सं == ENOTEMPTY || त्रुटि_सं == EISDIR || त्रुटि_सं == ENOTDIR
          || त्रुटि_सं == EEXIST)
      && ent->fts_errno != 0)
    त्रुटि_सं = ent->fts_errno;
  error (0, त्रुटि_सं, _("cannot remove %s"), quoteaf (ent->fts_path));
  mark_ancestor_dirs (ent);
  परत RM_ERROR;
}

/* This function is called once for every file system object that fts
   encounters.  fts performs a depth-first traversal.
   A directory is usually processed twice, first with fts_info == FTS_D,
   and later, after all of its entries have been processed, with FTS_DP.
   Return RM_ERROR upon error, RM_USER_DECLINED for a negative response
   to an interactive prompt, and otherwise, RM_OK.  */
स्थिर गणना RM_status
rm_fts (FTS *fts, FTSENT *ent, संरचना rm_options स्थिरांक *x)
{
  पूर्णांक dir_status = DS_UNKNOWN;

  निवड (ent->fts_info)
    {
    स्थिती FTS_D:			/* preorder directory */
      जर (!x->recursive)
        {
          /* Not recursive, so skip contents, and fail now unless
             removing empty directories.  */
          fts_set (fts, ent, FTS_SKIP);
          जर (x->remove_empty_directories)
            परत RM_OK;
          error (0, EISDIR, _("cannot remove %s"), quoteaf (ent->fts_path));
          ignore_मूल्य (fts_read (fts));
          परत RM_ERROR;
        }

      /* Perform checks that can apply only for command-line arguments.  */
      जर (ent->fts_level == FTS_ROOTLEVEL)
        {
          /* POSIX says:
             If the basename of a command line argument is "." or "..",
             diagnose it and do nothing more with that argument.  */
          जर (dot_or_dotdot (last_component (ent->fts_accpath)))
            {
              error (0, 0,
                     _("refusing to remove %s or %s directory: skipping %s"),
                     quoteaf_n (0, "."), quoteaf_n (1, ".."),
                     quoteaf_n (2, ent->fts_path));
              fts_skip_वृक्ष (fts, ent);
              परत RM_ERROR;
            }

          /* POSIX also says:
             If a command line argument resolves to "/" (and --preserve-root
             is in effect -- default) diagnose and skip it.  */
          जर (मूळ_DEV_INO_CHECK (x->मूळ_dev_ino, ent->fts_statp))
            {
              मूळ_DEV_INO_WARN (ent->fts_path);
              fts_skip_वृक्ष (fts, ent);
              परत RM_ERROR;
            }

          /* If a command line argument is a mount point and
             --preserve-root=all is in effect, diagnose and skip it.
             This doesn't handle "/", but that's handled above.  */
          जर (x->preserve_all_मूळ)
            {
              bool failed = false;
              अक्षर *parent = file_name_concat (ent->fts_accpath, "..", रिक्त);
              संरचना stat statbuf;

              जर (!parent || lstat (parent, &statbuf))
                {
                  error (0, 0,
                         _("failed to stat %s: skipping %s"),
                         quoteaf_n (0, parent),
                         quoteaf_n (1, ent->fts_accpath));
                  failed = true;
                }

              मुक्त (parent);

              जर (failed || fts->fts_dev != statbuf.st_dev)
                {
                  जर (! failed)
                    {
                      error (0, 0,
                             _("skipping %s, since it's on a different device"),
                             quoteaf (ent->fts_path));
                      error (0, 0, _("and --preserve-root=all is in effect"));
                    }
                  fts_skip_वृक्ष (fts, ent);
                  परत RM_ERROR;
                }
            }
        }

      {
        गणना RM_status s = prompt (fts, ent, true /*is_dir*/, x,
                                   PA_DESCEND_INTO_DIR, &dir_status);

        जर (s == RM_USER_ACCEPTED && dir_status == DS_EMPTY)
          {
            /* When we know (from prompt when in interactive mode)
               that this is an empty directory, don't prompt twice.  */
            s = excise (fts, ent, x, true);
            जर (s == RM_OK)
              fts_skip_वृक्ष (fts, ent);
          }

        जर (! (s == RM_OK || s == RM_USER_ACCEPTED))
          {
            mark_ancestor_dirs (ent);
            fts_skip_वृक्ष (fts, ent);
          }

        परत s;
      }

    स्थिती FTS_F:			/* regular file */
    स्थिती FTS_NS:		/* stat(2) failed */
    स्थिती FTS_SL:		/* symbolic link */
    स्थिती FTS_SLNONE:		/* symbolic link without target */
    स्थिती FTS_DP:		/* postorder directory */
    स्थिती FTS_DNR:		/* unreadable directory */
    स्थिती FTS_NSOK:		/* e.g., dangling symlink */
    स्थिती FTS_DEFAULT:		/* none of the above */
      {
        /* With --one-file-system, do not attempt to remove a mount point.
           fts' FTS_XDEV ensures that we don't process any entries under
           the mount point.  */
        जर (ent->fts_info == FTS_DP
            && x->one_file_system
            && FTS_ROOTLEVEL < ent->fts_level
            && ent->fts_statp->st_dev != fts->fts_dev)
          {
            mark_ancestor_dirs (ent);
            error (0, 0, _("skipping %s, since it's on a different device"),
                   quoteaf (ent->fts_path));
            परत RM_ERROR;
          }

        bool is_dir = ent->fts_info == FTS_DP || ent->fts_info == FTS_DNR;
        गणना RM_status s = prompt (fts, ent, is_dir, x, PA_REMOVE_DIR,
                                   &dir_status);
        जर (! (s == RM_OK || s == RM_USER_ACCEPTED))
          परत s;
        परत excise (fts, ent, x, is_dir);
      }

    स्थिती FTS_DC:		/* directory that causes cycles */
      emit_cycle_warning (ent->fts_path);
      fts_skip_वृक्ष (fts, ent);
      परत RM_ERROR;

    स्थिती FTS_ERR:
      /* Various failures, from opendir to ENOMEM, to failure to "return"
         to preceding directory, can provoke this.  */
      error (0, ent->fts_errno, _("traversal failed: %s"),
             quotef (ent->fts_path));
      fts_skip_वृक्ष (fts, ent);
      परत RM_ERROR;

    पूर्वनिर्धारित:
      error (0, 0, _("unexpected failure: fts_info=%d: %s\n"
                     "please report to %s"),
             ent->fts_info,
             quotef (ent->fts_path),
             PACKAGE_BUGREPORT);
      पात ();
    }
}

/* Remove FILEs, honoring options specified via X.
   Return RM_OK if successful.  */
गणना RM_status
rm (अक्षर *स्थिरांक *file, संरचना rm_options स्थिरांक *x)
{
  गणना RM_status rm_status = RM_OK;

  जर (*file)
    {
      पूर्णांक bit_flags = (FTS_CWDFD
                       | FTS_NOSTAT
                       | FTS_PHYSICAL);

      जर (x->one_file_system)
        bit_flags |= FTS_XDEV;

      FTS *fts = xfts_open (file, bit_flags, रिक्त);

      पर्यंत (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          जर (ent == रिक्त)
            {
              जर (त्रुटि_सं != 0)
                {
                  error (0, त्रुटि_सं, _("fts_read failed"));
                  rm_status = RM_ERROR;
                }
              थांबा;
            }

          गणना RM_status s = rm_fts (fts, ent, x);

          affirm (VALID_STATUS (s));
          UPDATE_STATUS (rm_status, s);
        }

      जर (fts_close (fts) != 0)
        {
          error (0, त्रुटि_सं, _("fts_close failed"));
          rm_status = RM_ERROR;
        }
    }

  परत rm_status;
}
