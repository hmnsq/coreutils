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
#यदि ! HAVE_STRUCT_DIRENT_D_TYPE
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
स्थिर पूर्णाङ्क
cache_fstatat (पूर्णाङ्क fd, वर्ण ध्रुव *file, संरचना stat *st, पूर्णाङ्क flag)
{
#यदि HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  /* If ST->st_atim.tv_nsec is -1, the status has not been gotten yet.
     If less than -1, fstatat failed with errno == ST->st_ino.
     Otherwise, the status has already been gotten, so return 0.  */
  यदि (0 <= st->st_atim.tv_nsec)
    प्रत्यागम 0;
  यदि (st->st_atim.tv_nsec == -1)
    {
      यदि (fstatat (fd, file, st, flag) == 0)
        प्रत्यागम 0;
      st->st_atim.tv_nsec = -2;
      st->st_ino = त्रुटि_सं;
    }
  त्रुटि_सं = st->st_ino;
  प्रत्यागम -1;
#अन्यथा
  प्रत्यागम fstatat (fd, file, st, flag);
#endif
}

/* Initialize a fstatat cache *ST.  Return ST for convenience.  */
स्थिर अन्तःस्थ संरचना stat *
cache_stat_init (संरचना stat *st)
{
#यदि HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
  st->st_atim.tv_nsec = -1;
#endif
  प्रत्यागम st;
}

/* Return 1 if FILE is an unwritable non-symlink,
   0 if it is writable or some other type of file,
   -1 and set errno if there is some problem in determining the answer.
   Set *BUF to the file status.  */
स्थिर पूर्णाङ्क
write_protected_non_symlink (पूर्णाङ्क fd_cwd,
                             वर्ण ध्रुव *file,
                             संरचना stat *buf)
{
  यदि (can_write_any_file ())
    प्रत्यागम 0;
  यदि (cache_fstatat (fd_cwd, file, buf, AT_SYMLINK_NOFOLLOW) != 0)
    प्रत्यागम -1;
  यदि (S_ISLNK (buf->st_mode))
    प्रत्यागम 0;
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
    यदि (faccessat (fd_cwd, file, W_OK, AT_EACCESS) == 0)
      प्रत्यागम 0;

    प्रत्यागम त्रुटि_सं == EACCES ? 1 : -1;
  }
}

/* Return the status of the directory identified by FTS and ENT.
   This is -1 if the directory is empty, 0 if it is nonempty,
   and a positive error number if there was trouble determining the status,
   e.g., it is not a directory, or permissions problems, or I/O errors.
   Use *DIR_STATUS as a cache for the status.  */
स्थिर पूर्णाङ्क
get_dir_status (FTS ध्रुव *fts, FTSENT ध्रुव *ent, पूर्णाङ्क *dir_status)
{
  यदि (*dir_status == DS_UNKNOWN)
    *dir_status = directory_status (fts->fts_cwd_fd, ent->fts_accpath);
  प्रत्यागम *dir_status;
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
prompt (FTS ध्रुव *fts, FTSENT ध्रुव *ent, bool is_dir,
        संरचना rm_options ध्रुव *x, गणना Prompt_action mode,
        पूर्णाङ्क *dir_status)
{
  पूर्णाङ्क fd_cwd = fts->fts_cwd_fd;
  वर्ण ध्रुव *full_name = ent->fts_path;
  वर्ण ध्रुव *filename = ent->fts_accpath;
  संरचना stat st;
  संरचना stat *sbuf = &st;
  cache_stat_init (sbuf);

  पूर्णाङ्क dirent_type = is_dir ? DT_DIR : DT_UNKNOWN;
  पूर्णाङ्क write_protected = 0;

  /* When nonzero, this indicates that we failed to remove a child entry,
     either because the user declined an interactive prompt, or due to
     some other failure, like permissions.  */
  यदि (ent->fts_संख्या)
    प्रत्यागम RM_USER_DECLINED;

  यदि (x->interactive == RMI_NEVER)
    प्रत्यागम RM_OK;

  पूर्णाङ्क wp_errno = 0;
  यदि (!x->ignore_missing_files
      && (x->interactive == RMI_ALWAYS || x->stdin_tty)
      && dirent_type != DT_LNK)
    {
      write_protected = write_protected_non_symlink (fd_cwd, filename, sbuf);
      wp_errno = त्रुटि_सं;
    }

  यदि (write_protected || x->interactive == RMI_ALWAYS)
    {
      यदि (0 <= write_protected && dirent_type == DT_UNKNOWN)
        {
          यदि (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) == 0)
            {
              यदि (S_ISLNK (sbuf->st_mode))
                dirent_type = DT_LNK;
              अन्यथा यदि (S_ISDIR (sbuf->st_mode))
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

      यदि (0 <= write_protected)
        विकल्प (dirent_type)
          {
          पक्ष DT_LNK:
            /* Using permissions doesn't make sense for symlinks.  */
            यदि (x->interactive != RMI_ALWAYS)
              प्रत्यागम RM_OK;
            भङ्ग;

          पक्ष DT_DIR:
             /* Unless we're either deleting directories or deleting
                recursively, we want to raise an EISDIR error rather than
                prompting the user  */
            यदि ( ! (x->recursive
                    || (x->remove_empty_directories
                        && get_dir_status (fts, ent, dir_status) != 0)))
              {
                write_protected = -1;
                wp_errno = *dir_status <= 0 ? EISDIR : *dir_status;
              }
            भङ्ग;
          }

      वर्ण ध्रुव *quoted_name = quoteaf (full_name);

      यदि (write_protected < 0)
        {
          error (0, wp_errno, _("cannot remove %s"), quoted_name);
          प्रत्यागम RM_ERROR;
        }

      /* Issue the prompt.  */
      यदि (dirent_type == DT_DIR
          && mode == PA_DESCEND_INTO_DIR
          && get_dir_status (fts, ent, dir_status) == DS_NONEMPTY)
        ख_लिखो (मानक_त्रुटि,
                 (write_protected
                  ? _("%s: descend into write-protected directory %s? ")
                  : _("%s: descend into directory %s? ")),
                 program_name, quoted_name);
      अन्यथा यदि (0 < *dir_status)
        {
          यदि ( ! (x->remove_empty_directories && *dir_status == EACCES))
            {
              error (0, *dir_status, _("cannot remove %s"), quoted_name);
              प्रत्यागम RM_ERROR;
            }

          /* The following code can lead to a successful deletion only with
             the --dir (-d) option (remove_empty_directories) and an empty
             inaccessible directory. In the first prompt call for a directory,
             we'd normally ask whether to descend into it, but in this case
             (it's inaccessible), that is not possible, so don't prompt.  */
          यदि (mode == PA_DESCEND_INTO_DIR)
            प्रत्यागम RM_OK;

          ख_लिखो (मानक_त्रुटि,
               _("%s: attempt removal of inaccessible directory %s? "),
                   program_name, quoted_name);
        }
      अन्यथा
        {
          यदि (cache_fstatat (fd_cwd, filename, sbuf, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, त्रुटि_सं, _("cannot remove %s"), quoted_name);
              प्रत्यागम RM_ERROR;
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

      प्रत्यागम yesno () ? RM_USER_ACCEPTED : RM_USER_DECLINED;
    }
  प्रत्यागम RM_OK;
}

/* When a function like unlink, rmdir, or fstatat fails with an errno
   value of ERRNUM, return true if the specified file system object
   is guaranteed not to exist;  otherwise, return false.  */
स्थिर अन्तःस्थ bool
nonexistent_file_errno (पूर्णाङ्क errnum)
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

  विकल्प (errnum)
    {
    पक्ष EILSEQ:
    पक्ष EINVAL:
    पक्ष ENOENT:
    पक्ष ENOTDIR:
      प्रत्यागम true;
    स्वभाव:
      प्रत्यागम false;
    }
}

/* Encapsulate the test for whether the errno value, ERRNUM, is ignorable.  */
स्थिर अन्तःस्थ bool
ignorable_missing (संरचना rm_options ध्रुव *x, पूर्णाङ्क errnum)
{
  प्रत्यागम x->ignore_missing_files && nonexistent_file_errno (errnum);
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
  प्रति (FTSENT *p = ent->fts_parent; FTS_ROOTLEVEL <= p->fts_level;
       p = p->fts_parent)
    {
      यदि (p->fts_संख्या)
        भङ्ग;
      p->fts_संख्या = 1;
    }
}

/* Remove the file system object specified by ENT.  IS_DIR specifies
   whether it is expected to be a directory or non-directory.
   Return RM_OK upon success, else RM_ERROR.  */
स्थिर गणना RM_status
excise (FTS *fts, FTSENT *ent, संरचना rm_options ध्रुव *x, bool is_dir)
{
  पूर्णाङ्क flag = is_dir ? AT_REMOVEDIR : 0;
  यदि (unlinkat (fts->fts_cwd_fd, ent->fts_accpath, flag) == 0)
    {
      यदि (x->verbose)
        {
          म_लिखो ((is_dir
                   ? _("removed directory %s\n")
                   : _("removed %s\n")), quoteaf (ent->fts_path));
        }
      प्रत्यागम RM_OK;
    }

  /* The unlinkat from kernels like linux-2.6.32 reports EROFS even for
     nonexistent files.  When the file is indeed missing, map that to ENOENT,
     so that rm -f ignores it, as required.  Even without -f, this is useful
     because it makes rm print the more precise diagnostic.  */
  यदि (त्रुटि_सं == EROFS)
    {
      संरचना stat st;
      यदि ( ! (fstatat (fts->fts_cwd_fd, ent->fts_accpath, &st,
                       AT_SYMLINK_NOFOLLOW)
              && त्रुटि_सं == ENOENT))
        त्रुटि_सं = EROFS;
    }

  यदि (ignorable_missing (x, त्रुटि_सं))
    प्रत्यागम RM_OK;

  /* When failing to rmdir an unreadable directory, we see errno values
     like EISDIR or ENOTDIR (or, on Solaris 10, EEXIST), but they would be
     meaningless in a diagnostic.  When that happens, use the earlier, more
     descriptive errno value.  */
  यदि (ent->fts_info == FTS_DNR
      && (त्रुटि_सं == ENOTEMPTY || त्रुटि_सं == EISDIR || त्रुटि_सं == ENOTDIR
          || त्रुटि_सं == EEXIST)
      && ent->fts_errno != 0)
    त्रुटि_सं = ent->fts_errno;
  error (0, त्रुटि_सं, _("cannot remove %s"), quoteaf (ent->fts_path));
  mark_ancestor_dirs (ent);
  प्रत्यागम RM_ERROR;
}

/* This function is called once for every file system object that fts
   encounters.  fts performs a depth-first traversal.
   A directory is usually processed twice, first with fts_info == FTS_D,
   and later, after all of its entries have been processed, with FTS_DP.
   Return RM_ERROR upon error, RM_USER_DECLINED for a negative response
   to an interactive prompt, and otherwise, RM_OK.  */
स्थिर गणना RM_status
rm_fts (FTS *fts, FTSENT *ent, संरचना rm_options ध्रुव *x)
{
  पूर्णाङ्क dir_status = DS_UNKNOWN;

  विकल्प (ent->fts_info)
    {
    पक्ष FTS_D:			/* preorder directory */
      यदि (!x->recursive)
        {
          /* Not recursive, so skip contents, and fail now unless
             removing empty directories.  */
          fts_set (fts, ent, FTS_SKIP);
          यदि (x->remove_empty_directories)
            प्रत्यागम RM_OK;
          error (0, EISDIR, _("cannot remove %s"), quoteaf (ent->fts_path));
          ignore_मूल्य (fts_read (fts));
          प्रत्यागम RM_ERROR;
        }

      /* Perform checks that can apply only for command-line arguments.  */
      यदि (ent->fts_level == FTS_ROOTLEVEL)
        {
          /* POSIX says:
             If the basename of a command line argument is "." or "..",
             diagnose it and do nothing more with that argument.  */
          यदि (dot_or_dotdot (last_component (ent->fts_accpath)))
            {
              error (0, 0,
                     _("refusing to remove %s or %s directory: skipping %s"),
                     quoteaf_n (0, "."), quoteaf_n (1, ".."),
                     quoteaf_n (2, ent->fts_path));
              fts_skip_वृक्ष (fts, ent);
              प्रत्यागम RM_ERROR;
            }

          /* POSIX also says:
             If a command line argument resolves to "/" (and --preserve-root
             is in effect -- default) diagnose and skip it.  */
          यदि (मूल_DEV_INO_CHECK (x->मूल_dev_ino, ent->fts_statp))
            {
              मूल_DEV_INO_WARN (ent->fts_path);
              fts_skip_वृक्ष (fts, ent);
              प्रत्यागम RM_ERROR;
            }

          /* If a command line argument is a mount point and
             --preserve-root=all is in effect, diagnose and skip it.
             This doesn't handle "/", but that's handled above.  */
          यदि (x->preserve_all_मूल)
            {
              bool failed = false;
              वर्ण *parent = file_name_concat (ent->fts_accpath, "..", रिक्त);
              संरचना stat statbuf;

              यदि (!parent || lstat (parent, &statbuf))
                {
                  error (0, 0,
                         _("failed to stat %s: skipping %s"),
                         quoteaf_n (0, parent),
                         quoteaf_n (1, ent->fts_accpath));
                  failed = true;
                }

              मुक्त (parent);

              यदि (failed || fts->fts_dev != statbuf.st_dev)
                {
                  यदि (! failed)
                    {
                      error (0, 0,
                             _("skipping %s, since it's on a different device"),
                             quoteaf (ent->fts_path));
                      error (0, 0, _("and --preserve-root=all is in effect"));
                    }
                  fts_skip_वृक्ष (fts, ent);
                  प्रत्यागम RM_ERROR;
                }
            }
        }

      {
        गणना RM_status s = prompt (fts, ent, true /*is_dir*/, x,
                                   PA_DESCEND_INTO_DIR, &dir_status);

        यदि (s == RM_USER_ACCEPTED && dir_status == DS_EMPTY)
          {
            /* When we know (from prompt when in interactive mode)
               that this is an empty directory, don't prompt twice.  */
            s = excise (fts, ent, x, true);
            यदि (s == RM_OK)
              fts_skip_वृक्ष (fts, ent);
          }

        यदि (! (s == RM_OK || s == RM_USER_ACCEPTED))
          {
            mark_ancestor_dirs (ent);
            fts_skip_वृक्ष (fts, ent);
          }

        प्रत्यागम s;
      }

    पक्ष FTS_F:			/* regular file */
    पक्ष FTS_NS:		/* stat(2) failed */
    पक्ष FTS_SL:		/* symbolic link */
    पक्ष FTS_SLNONE:		/* symbolic link without target */
    पक्ष FTS_DP:		/* postorder directory */
    पक्ष FTS_DNR:		/* unreadable directory */
    पक्ष FTS_NSOK:		/* e.g., dangling symlink */
    पक्ष FTS_DEFAULT:		/* none of the above */
      {
        /* With --one-file-system, do not attempt to remove a mount point.
           fts' FTS_XDEV ensures that we don't process any entries under
           the mount point.  */
        यदि (ent->fts_info == FTS_DP
            && x->one_file_system
            && FTS_ROOTLEVEL < ent->fts_level
            && ent->fts_statp->st_dev != fts->fts_dev)
          {
            mark_ancestor_dirs (ent);
            error (0, 0, _("skipping %s, since it's on a different device"),
                   quoteaf (ent->fts_path));
            प्रत्यागम RM_ERROR;
          }

        bool is_dir = ent->fts_info == FTS_DP || ent->fts_info == FTS_DNR;
        गणना RM_status s = prompt (fts, ent, is_dir, x, PA_REMOVE_DIR,
                                   &dir_status);
        यदि (! (s == RM_OK || s == RM_USER_ACCEPTED))
          प्रत्यागम s;
        प्रत्यागम excise (fts, ent, x, is_dir);
      }

    पक्ष FTS_DC:		/* directory that causes cycles */
      emit_cycle_warning (ent->fts_path);
      fts_skip_वृक्ष (fts, ent);
      प्रत्यागम RM_ERROR;

    पक्ष FTS_ERR:
      /* Various failures, from opendir to ENOMEM, to failure to "return"
         to preceding directory, can provoke this.  */
      error (0, ent->fts_errno, _("traversal failed: %s"),
             quotef (ent->fts_path));
      fts_skip_वृक्ष (fts, ent);
      प्रत्यागम RM_ERROR;

    स्वभाव:
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
rm (वर्ण *ध्रुव *file, संरचना rm_options ध्रुव *x)
{
  गणना RM_status rm_status = RM_OK;

  यदि (*file)
    {
      पूर्णाङ्क bit_flags = (FTS_CWDFD
                       | FTS_NOSTAT
                       | FTS_PHYSICAL);

      यदि (x->one_file_system)
        bit_flags |= FTS_XDEV;

      FTS *fts = xfts_open (file, bit_flags, रिक्त);

      यावत् (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          यदि (ent == रिक्त)
            {
              यदि (त्रुटि_सं != 0)
                {
                  error (0, त्रुटि_सं, _("fts_read failed"));
                  rm_status = RM_ERROR;
                }
              भङ्ग;
            }

          गणना RM_status s = rm_fts (fts, ent, x);

          affirm (VALID_STATUS (s));
          UPDATE_STATUS (rm_status, s);
        }

      यदि (fts_close (fts) != 0)
        {
          error (0, त्रुटि_सं, _("fts_close failed"));
          rm_status = RM_ERROR;
        }
    }

  प्रत्यागम rm_status;
}
