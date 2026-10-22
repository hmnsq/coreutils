/* copy.c -- core functions for copying files and directories
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Extracted from cp.c and librarified by Jim Meyering.  */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <selinux/selinux.h>

#ಆದರೆ HAVE_HURD_H
# include <hurd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_PRIV_H
# include <priv.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "acl.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "backupfile.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "cp-hash.h"
#ಅನ್ತರ್ಭಾವ "fcntl--.h"
#ಅನ್ತರ್ಭಾವ "file-set.h"
#ಅನ್ತರ್ಭಾವ "filemode.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "force-link.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "hashcode-file.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "issymlinkat.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "renameatu.h"
#ಅನ್ತರ್ಭಾವ "root-uid.h"
#ಅನ್ತರ್ಭಾವ "same.h"
#ಅನ್ತರ್ಭಾವ "savedir.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "utimecmp.h"
#ಅನ್ತರ್ಭಾವ "utimens.h"
#ಅನ್ತರ್ಭಾವ "write-any-file.h"
#ಅನ್ತರ್ಭಾವ "areadlink.h"
#ಅನ್ತರ್ಭಾವ "yesno.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ USE_XATTR
# define USE_XATTR false
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ USE_XATTR
# include <attr/error_context.h>
# include <attr/libattr.h>
# include <stdarg.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_LINUX_FALLOC_H
# include <linux/falloc.h>
#ಆದರೆ_ಅಂತ್ಯ

/* See HAVE_FALLOCATE workaround when including this file.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_FS_H
# include <linux/fs.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !defined FICLONE && defined __linux__
# define FICLONE _IOW (0x94, 9, ಪೂರ್ಣಾಂಕ)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_FCLONEFILEAT && !USE_XATTR
# include <sys/clonefile.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ USE_ACL
# define USE_ACL 0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು SAME_OWNER(A, B) ((A).st_uid == (B).st_uid)
#ವ್ಯಾಖ್ಯಾನಿಸು SAME_ಗುಂಪು(A, B) ((A).st_gid == (B).st_gid)
#ವ್ಯಾಖ್ಯಾನಿಸು SAME_OWNER_AND_ಗುಂಪು(A, B) (SAME_OWNER (A, B) && SAME_ಗುಂಪು (A, B))

/* LINK_FOLLOWS_SYMLINKS is tri-state; if it is -1, we don't know
   how link() behaves, so assume we can't hardlink symlinks in that case.  */
#ಆದರೆ (defined HAVE_LINKAT && ! LINKAT_SYMLINK_NOTSUP) || ! LINK_FOLLOWS_SYMLINKS
# define CAN_HARDLINK_SYMLINKS 1
#ಇಲ್ಲದಿದ್ದರೆ
# define CAN_HARDLINK_SYMLINKS 0
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ dir_list
{
  ರಚನೆ dir_list *ಪೋಷಕ;
  ino_t st_ino;
  dev_t st_dev;
};

/* Initial size of the cp.dest_info hash table.  */
#ವ್ಯಾಖ್ಯಾನಿಸು DEST_INFO_INITIAL_ಸಾಮರ್ಥ್ಯ 61

ಸ್ಥಿರ bool copy_ಆಂತರಿಕ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
                           ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
                           ಪೂರ್ಣಾಂಕ nonexistent_dst, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *ಪೋಷಕ,
                           ರಚನೆ dir_list *ancestors,
                           ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x,
                           bool command_ಸಾಲು_arg,
                           bool *ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg,
                           bool *copy_into_self,
                           bool *rename_succeeded);
ಸ್ಥಿರ bool owner_failure_ok (ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x);

/* Pointers to the file names:  they're used in the diagnostic that is issued
   when we detect the user is trying to copy a directory into itself.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತುದಿ_level_src_name;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತುದಿ_level_dst_name;

/* debug info about the last file copy.  */
ಸ್ಥಿರ ರಚನೆ copy_debug copy_debug;

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
copy_debug_string (ಎಣಿಕೆ copy_debug_val debug_val)
{
  ಆಯ್ಕೆ (debug_val)
    {
    ಸ್ಥಿತಿ COPY_DEBUG_NO: ಹಿಂತಿರುಗಿ "no";
    ಸ್ಥಿತಿ COPY_DEBUG_YES: ಹಿಂತಿರುಗಿ "yes";
    ಸ್ಥಿತಿ COPY_DEBUG_AVOIDED: ಹಿಂತಿರುಗಿ "avoided";
    ಸ್ಥಿತಿ COPY_DEBUG_UNKNOWN: ಹಿಂತಿರುಗಿ "unknown";

    ಸ್ಥಿತಿ COPY_DEBUG_EXTERNAL:
    ಸ್ಥಿತಿ COPY_DEBUG_EXTERNAL_ಆಂತರಿಕ:
    ಸ್ಥಿತಿ COPY_DEBUG_MAX:
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಆದರೆ (0 < debug_val)
        ಹಿಂತಿರುಗಿ strerror (debug_val);
      affirm (false);
    }
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
copy_debug_sparse_string (ಎಣಿಕೆ copy_debug_val debug_val)
{
  ಆಯ್ಕೆ (debug_val)
    {
    ಸ್ಥಿತಿ COPY_DEBUG_NO: ಹಿಂತಿರುಗಿ "no";
    ಸ್ಥಿತಿ COPY_DEBUG_YES: ಹಿಂತಿರುಗಿ "zeros";
    ಸ್ಥಿತಿ COPY_DEBUG_EXTERNAL: ಹಿಂತಿರುಗಿ "SEEK_HOLE";
    ಸ್ಥಿತಿ COPY_DEBUG_EXTERNAL_ಆಂತರಿಕ: ಹಿಂತಿರುಗಿ "SEEK_HOLE + zeros";
    ಸ್ಥಿತಿ COPY_DEBUG_UNKNOWN: ಹಿಂತಿರುಗಿ "unknown";

    ಸ್ಥಿತಿ COPY_DEBUG_AVOIDED:
    ಸ್ಥಿತಿ COPY_DEBUG_MAX:
    ಪೂರ್ವನಿಯೋಜಿತ: affirm (false);
    }
}

/* Print --debug output on standard output.  */
ಸ್ಥಿರ ಶೂನ್ಯ
emit_debug (ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ಆದರೆ (! x->hard_link && ! x->symbolic_link && x->ದತ್ತಾಂಶ_copy_required)
    ಬರೆ ("copy offload: %s, reflink: %s, sparse detection: %s\n",
            copy_debug_string (copy_debug.offload),
            copy_debug_string (copy_debug.reflink),
            copy_debug_sparse_string (copy_debug.sparse_detection));
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DEV_FD_MIGHT_BE_CHR
# define DEV_FD_MIGHT_BE_CHR false
#ಆದರೆ_ಅಂತ್ಯ

/* Act like fstat (DIRFD, FILENAME, ST, FLAGS), except when following
   symbolic links on Solaris-like systems, treat any character-special
   device like /dev/fd/0 as if it were the file it is open on.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
follow_fstatat (ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ರಚನೆ stat *st, ಪೂರ್ಣಾಂಕ flags)
{
  ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = fstatat (dirfd, filename, st, flags);

  ಆದರೆ (DEV_FD_MIGHT_BE_CHR && ಫಲಿತಾಂಶ == 0 && !(flags & AT_SYMLINK_NOFOLLOW)
      && S_ISCHR (st->st_ಬಹುಲಕ))
    {
      ಸ್ಥಿರ dev_t stdin_rdev;
      ಸ್ಥಿರ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ stdin_rdev_ಸ್ಥಿತಿ;
      ಆದರೆ (stdin_rdev_ಸ್ಥಿತಿ == 0)
        {
          ರಚನೆ stat stdin_st;
          ಆದರೆ (stat ("/dev/stdin", &stdin_st) == 0 && S_ISCHR (stdin_st.st_ಬಹುಲಕ)
              && minor (stdin_st.st_rdev) == STDIN_FILENO)
            {
              stdin_rdev = stdin_st.st_rdev;
              stdin_rdev_ಸ್ಥಿತಿ = 1;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            stdin_rdev_ಸ್ಥಿತಿ = -1;
        }
      ಆದರೆ (0 < stdin_rdev_ಸ್ಥಿತಿ && major (stdin_rdev) == major (st->st_rdev))
        ಫಲಿತಾಂಶ = fstat (minor (st->st_rdev), st);
    }

  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* Perform the O(1) btrfs clone operation, if possible.
   Upon success, return 0.  Otherwise, return -1 and set errno.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
clone_file (MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ dest_fd, MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ src_fd)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FICLONE
  ಹಿಂತಿರುಗಿ ioctl (dest_fd, FICLONE, src_fd);
#ಇಲ್ಲದಿದ್ದರೆ
  errno = ENOTSUP;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* FIXME: describe */
/* FIXME: rewrite this to use a hash table so we avoid the quadratic
   performance hit that's probably noticeable only on trees deeper
   than a few hundred levels.  See use of active_dir_map in remove.c  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
is_ancestor (ಸ್ಥಿರಾಂಕ ರಚನೆ stat *sb, ಸ್ಥಿರಾಂಕ ರಚನೆ dir_list *ancestors)
{
  ಆಗ (ancestors)
    {
      ಆದರೆ (PSAME_INODE (ancestors, sb))
        ಹಿಂತಿರುಗಿ true;
      ancestors = ancestors->ಪೋಷಕ;
    }
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
errno_unsupported (ಪೂರ್ಣಾಂಕ err)
{
  ಹಿಂತಿರುಗಿ err == ENOTSUP || err == ENODATA;
}

#ಆದರೆ USE_XATTR
ATTRIBUTE_FORMAT ((ಬರೆ, 2, 3))
ಸ್ಥಿರ ಶೂನ್ಯ
copy_attr_error (MAYBE_ಬಳಸದ ರಚನೆ error_context *ctx,
                 ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ...)
{
  ಆದರೆ (!errno_unsupported (errno))
    {
      ಪೂರ್ಣಾಂಕ err = errno;
      va_list ap;

      /* use verror module to print error message */
      va_ಆರಂಭ (ap, fmt);
      verror (0, err, fmt, ap);
      va_ಕೊನೆ (ap);
    }
}

ATTRIBUTE_FORMAT ((ಬರೆ, 2, 3))
ಸ್ಥಿರ ಶೂನ್ಯ
copy_attr_allerror (MAYBE_ಬಳಸದ ರಚನೆ error_context *ctx,
                    ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt, ...)
{
  ಪೂರ್ಣಾಂಕ err = errno;
  va_list ap;

  /* use verror module to print error message */
  va_ಆರಂಭ (ap, fmt);
  verror (0, err, fmt, ap);
  va_ಕೊನೆ (ap);
}

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
copy_attr_quote (MAYBE_ಬಳಸದ ರಚನೆ error_context *ctx, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  ಹಿಂತಿರುಗಿ quoteaf (str);
}

ಸ್ಥಿರ ಶೂನ್ಯ
copy_attr_free (MAYBE_ಬಳಸದ ರಚನೆ error_context *ctx,
                MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
}

/* Exclude SELinux extended attributes that are otherwise handled,
   and are problematic to copy again.  Also honor attributes
   configured for exclusion in /etc/xattr.conf.
   FIXME: Should we handle POSIX ACLs similarly?
   Return zero to skip.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_selinux_attr (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ error_context *ctx)
{
  ಹಿಂತಿರುಗಿ STRNCMP_LIT (name, "security.selinux")
         && attr_copy_check_permissions (name, ctx);
}

/* If positive SRC_FD and DST_FD descriptors are passed,
   then copy by fd, otherwise copy by name.  */

ಸ್ಥಿರ bool
copy_attr (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ src_fd,
           ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ dst_fd, ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x)
{
  bool all_errors = (!x->ದತ್ತಾಂಶ_copy_required || x->require_preserve_xattr);
  bool some_errors = (!all_errors && !x->reduce_diagnostics);
  ಪೂರ್ಣಾಂಕ (*check) (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *, ರಚನೆ error_context *)
    = (x->preserve_security_context || x->ಗಣ_security_context
       ? check_selinux_attr : ಖಾಲಿ);

# ಆದರೆ 4 < __GNUC__ + (8 <= __GNUC_MINOR__)
  /* Pacify gcc -Wsuggest-attribute=format through at least GCC 13.2.1.  */
#  pragma GCC diagnostic ತಳ್ಳು
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
# endif
  ರಚನೆ error_context *ctx
    = (all_errors || some_errors
       ? (&(ರಚನೆ error_context) {
           .error = all_errors ? copy_attr_allerror : copy_attr_error,
           .quote = copy_attr_quote,
           .quote_free = copy_attr_free
         })
       : ಖಾಲಿ);
# ಆದರೆ 4 < __GNUC__ + (8 <= __GNUC_MINOR__)
#  pragma GCC diagnostic ತೆಗೆ
# endif

  ಹಿಂತಿರುಗಿ ! (0 <= src_fd && 0 <= dst_fd
            ? attr_copy_fd (src_ಮಾರ್ಗ, src_fd, dst_ಮಾರ್ಗ, dst_fd, check, ctx)
            : attr_copy_file (src_ಮಾರ್ಗ, dst_ಮಾರ್ಗ, check, ctx));
}
#ಇಲ್ಲದಿದ್ದರೆ /* USE_XATTR */

ಸ್ಥಿರ bool
copy_attr (MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_ಮಾರ್ಗ,
           MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ src_fd,
           MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_ಮಾರ್ಗ,
           MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ dst_fd,
           MAYBE_ಬಳಸದ ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x)
{
  ಹಿಂತಿರುಗಿ true;
}
#ಆದರೆ_ಅಂತ್ಯ /* USE_XATTR */

/* Read the contents of the directory SRC_NAME_IN, and recursively
   copy the contents to DST_NAME_IN aka DST_DIRFD+DST_RELNAME_IN.
   NEW_DST is true if DST_NAME_IN is a directory
   that was created previously in the recursion.
   SRC_SB and ANCESTORS describe SRC_NAME_IN.
   Set *COPY_INTO_SELF if SRC_NAME_IN is a parent of
   (or the same as) DST_NAME_IN; otherwise, clear it.
   Propagate *FIRST_DIR_CREATED_PER_COMMAND_LINE_ARG from
   caller to each invocation of copy_internal.  Be careful to
   pass the address of a temporary, and to update
   *FIRST_DIR_CREATED_PER_COMMAND_LINE_ARG only upon completion.
   Return true if successful.  */

ಸ್ಥಿರ bool
copy_dir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name_ಒಳ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name_ಒಳ,
          ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname_ಒಳ, bool new_dst,
          ಸ್ಥಿರಾಂಕ ರಚನೆ stat *src_sb, ರಚನೆ dir_list *ancestors,
          ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x,
          bool *ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg,
          bool *copy_into_self)
{
  ಅಕ್ಷರ *name_space;
  ಅಕ್ಷರ *namep;
  ರಚನೆ cp_options non_command_ಸಾಲು_options = *x;
  bool ok = true;

  name_space = savedir (src_name_ಒಳ, SAVEDIR_ವಿಂಗಡಿಸು_FASTREAD);
  ಆದರೆ (name_space == ಖಾಲಿ)
    {
      /* This diagnostic is a bit vague because savedir can fail in
         several different ways.  */
      error (0, errno, _("cannot access %s"), quoteaf (src_name_ಒಳ));
      ಹಿಂತಿರುಗಿ false;
    }

  /* For cp's -H option, dereference command line arguments, but do not
     dereference symlinks that are found via recursive traversal.  */
  ಆದರೆ (x->dereference == DEREF_COMMAND_ಸಾಲು_ARGUMENTS)
    non_command_ಸಾಲು_options.dereference = DEREF_NEVER;

  bool new_ಮೊದಲ_dir_created = false;
  namep = name_space;
  ಆಗ (*namep != '\0')
    {
      bool local_copy_into_self;
      ಅಕ್ಷರ *src_name = file_name_concat (src_name_ಒಳ, namep, ಖಾಲಿ);
      ಅಕ್ಷರ *dst_name = file_name_concat (dst_name_ಒಳ, namep, ಖಾಲಿ);
      bool ಮೊದಲ_dir_created = *ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg;
      bool rename_succeeded;

      ok &= copy_ಆಂತರಿಕ (src_name, dst_name, dst_dirfd,
                           dst_name + (dst_relname_ಒಳ - dst_name_ಒಳ),
                           new_dst, src_sb,
                           ancestors, &non_command_ಸಾಲು_options, false,
                           &ಮೊದಲ_dir_created,
                           &local_copy_into_self, &rename_succeeded);
      *copy_into_self |= local_copy_into_self;

      ಬಿಡುಗಡೆ (dst_name);
      ಬಿಡುಗಡೆ (src_name);

      /* If we're copying into self, there's no point in continuing,
         and in fact, that would even infloop, now that we record only
         the first created directory per command line argument.  */
      ಆದರೆ (local_copy_into_self)
        ನಿಲ್ಲಿಸು;

      new_ಮೊದಲ_dir_created |= ಮೊದಲ_dir_created;
      namep += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (namep) + 1;
    }
  ಬಿಡುಗಡೆ (name_space);
  *ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg = new_ಮೊದಲ_dir_created;

  ಹಿಂತಿರುಗಿ ok;
}

/* Change the file mode bits of the file identified by DESC or
   DIRFD+NAME to MODE.  Use DESC if DESC is valid and fchmod is
   available, DIRFD+NAME otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fchmod_or_lchmod (ಪೂರ್ಣಾಂಕ desc, ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, mode_t ಬಹುಲಕ_2)
{
#ಆದರೆ HAVE_FCHMOD
  ಆದರೆ (0 <= desc)
    ಹಿಂತಿರುಗಿ fchmod (desc, ಬಹುಲಕ_2);
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ lchmodat (dirfd, name, ಬಹುಲಕ_2);
}

/* Change the ownership of the file identified by DESC or
   DIRFD+NAME to UID+GID.  Use DESC if DESC is valid and fchown is
   available, DIRFD+NAME otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fchown_or_lchown (ಪೂರ್ಣಾಂಕ desc, ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, uid_t uid, gid_t gid)
{
#ಆದರೆ HAVE_FCHOWN
  ಆದರೆ (0 <= desc)
    ಹಿಂತಿರುಗಿ fchown (desc, uid, gid);
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ lchownat (dirfd, name, uid, gid);
}

/* Set the owner and owning group of DEST_DESC to the st_uid and
   st_gid fields of SRC_SB.  If DEST_DESC is undefined (-1), set
   the owner and owning group of DST_NAME aka DST_DIRFD+DST_RELNAME
   instead; for safety prefer lchownat since no
   symbolic links should be involved.  DEST_DESC must
   refer to the same file as DST_NAME if defined.
   Upon failure to set both UID and GID, try to set only the GID.
   NEW_DST is true if the file was newly created; otherwise,
   DST_SB is the status of the destination.
   Return 1 if the initial syscall succeeds, 0 if it fails but it's OK
   not to preserve ownership, -1 otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಗಣ_owner (ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
           ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname, ಪೂರ್ಣಾಂಕ dest_desc,
           ರಚನೆ stat ಸ್ಥಿರಾಂಕ *src_sb, bool new_dst,
           ರಚನೆ stat ಸ್ಥಿರಾಂಕ *dst_sb)
{
  uid_t uid = src_sb->st_uid;
  gid_t gid = src_sb->st_gid;

  /* Naively changing the ownership of an already-existing file before
     changing its permissions would create a window of vulnerability if
     the file's old permissions are too generous for the new owner and
     group.  Avoid the window by first changing to a restrictive
     temporary mode if necessary.  */

  ಆದರೆ (!new_dst && (x->preserve_ಬಹುಲಕ || x->move_ಬಹುಲಕ || x->ಗಣ_ಬಹುಲಕ))
    {
      mode_t old_ಬಹುಲಕ = dst_sb->st_ಬಹುಲಕ;
      mode_t new_ಬಹುಲಕ =
        (x->preserve_ಬಹುಲಕ || x->move_ಬಹುಲಕ ? src_sb->st_ಬಹುಲಕ : x->ಬಹುಲಕ_2);
      mode_t restrictive_ತಾತ್ಕಾಲಿಕ_ಬಹುಲಕ = old_ಬಹುಲಕ & new_ಬಹುಲಕ & S_IRWXU;

      ಆದರೆ ((USE_ACL
           || (old_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು
               & (~new_ಬಹುಲಕ | S_ISUID | S_ISGID | S_ISVTX)))
          && qset_acl (dst_name, dest_desc, restrictive_ತಾತ್ಕಾಲಿಕ_ಬಹುಲಕ) != 0)
        {
          ಆದರೆ (! owner_failure_ok (x))
            error (0, errno, _("clearing permissions for %s"),
                   quoteaf (dst_name));
          ಹಿಂತಿರುಗಿ -x->require_preserve;
        }
    }

  ಆದರೆ (fchown_or_lchown (dest_desc, dst_dirfd, dst_relname, uid, gid) == 0)
    ಹಿಂತಿರುಗಿ 1;

  /* The ownership change failed.  If the failure merely means we lack
     privileges to change owner+group, try to change just the group
     and ignore any failure of this.  Otherwise, report an error.  */
  ಆದರೆ (chown_failure_ok (x))
    ignore_ಮೌಲ್ಯ (fchown_or_lchown (dest_desc, dst_dirfd, dst_relname,
                                    -1, gid));
  ಇಲ್ಲದಿದ್ದರೆ
    {
      error (0, errno, _("failed to preserve ownership for %s"),
             quoteaf (dst_name));
      ಆದರೆ (x->require_preserve)
        ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Set the st_author field of DEST_DESC to the st_author field of
   SRC_SB. If DEST_DESC is undefined (-1), set the st_author field
   of DST_NAME instead.  DEST_DESC must refer to the same file as
   DST_NAME if defined.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_author (MAYBE_ಬಳಸದ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name, MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ dest_desc,
            MAYBE_ಬಳಸದ ಸ್ಥಿರಾಂಕ ರಚನೆ stat *src_sb)
{
#ಆದರೆ HAVE_STRUCT_STAT_ST_AUTHOR
  /* FIXME: Modify the following code so that it does not
     follow symbolic links.  */

  /* Preserve the st_author field.  */
  file_t file = (dest_desc < 0
                 ? file_name_lookup (dst_name, 0, 0)
                 : getdport (dest_desc));
  ಆದರೆ (file == MACH_ಪೋರ್ಟ್_NULL)
    error (0, errno, _("failed to lookup file %s"), quoteaf (dst_name));
  ಇಲ್ಲದಿದ್ದರೆ
    {
      error_t err = file_chauthor (file, src_sb->st_author);
      ಆದರೆ (err)
        error (0, err, _("failed to preserve authorship for %s"),
               quoteaf (dst_name));
      mach_ಪೋರ್ಟ್_deallocate (mach_task_self (), file);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

/* Set the default security context for the process.  New files will
   have this security context set.  Also existing files can have their
   context adjusted based on this process context, by
   set_file_security_ctx() called with PROCESS_LOCAL=true.
   This should be called before files are created so there is no race
   where a file may be present without an appropriate security context.
   Based on CP_OPTIONS, diagnose warnings and fail when appropriate.
   Return FALSE on failure, TRUE on success.  */

bool
ಗಣ_process_security_ctx (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
                          mode_t ಬಹುಲಕ_2, bool new_dst, ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ಆದರೆ (x->preserve_security_context)
    {
      /* Set the default context for the process to match the source.  */
      bool all_errors = !x->ದತ್ತಾಂಶ_copy_required || x->require_preserve_context;
      bool some_errors = !all_errors && !x->reduce_diagnostics;
      ಅಕ್ಷರ *con_raw;

      ಆದರೆ (0 <= lgetfilecon_raw (src_name, &con_raw))
        {
          ಆದರೆ (setfscreatecon_raw (con_raw) < 0)
            {
              ಆದರೆ (all_errors || (some_errors && !errno_unsupported (errno)))
                error (0, errno,
                       _("failed to set default file creation context to %s"),
                       quote (con_raw));
              ಆದರೆ (x->require_preserve_context)
                {
                  freecon (con_raw);
                  ಹಿಂತಿರುಗಿ false;
                }
            }
          freecon (con_raw);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (all_errors || (some_errors && !errno_unsupported (errno)))
            {
              error (0, errno,
                     _("failed to get security context of %s"),
                     quoteaf (src_name));
            }
          ಆದರೆ (x->require_preserve_context)
            ಹಿಂತಿರುಗಿ false;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->ಗಣ_security_context)
    {
      /* With -Z, adjust the default context for the process
         to have the type component adjusted as per the destination path.  */
      ಆದರೆ (new_dst && defaultcon (x->ಗಣ_security_context, dst_name, ಬಹುಲಕ_2) < 0
          && ! ignorable_ctx_err (errno))
        {
          error (0, errno,
                 _("failed to set default file creation context for %s"),
                 quoteaf (dst_name));
        }
    }

  ಹಿಂತಿರುಗಿ true;
}

/* Reset the security context of DST_NAME, to that already set
   as the process default if !X->set_security_context.  Otherwise
   adjust the type component of DST_NAME's security context as
   per the system default for that path.  Issue warnings upon
   failure, when allowed by various settings in X.
   Return false on failure, true on success.  */

bool
ಗಣ_file_security_ctx (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
                       bool recurse, ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  bool all_errors = (!x->ದತ್ತಾಂಶ_copy_required
                     || x->require_preserve_context);
  bool some_errors = !all_errors && !x->reduce_diagnostics;

  ಆದರೆ (! restorecon (x->ಗಣ_security_context, dst_name, recurse))
    {
      ಆದರೆ (all_errors || (some_errors && !errno_unsupported (errno)))
        error (0, errno, _("failed to set the security context of %s"),
               quoteaf_n (0, dst_name));
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

#ಆದರೆ HAVE_FCLONEFILEAT && !USE_XATTR
# include <sys/acl.h>
/* Return true if FD has a nontrivial ACL.  */
ಸ್ಥಿರ bool
fd_has_acl (ಪೂರ್ಣಾಂಕ fd)
{
  /* Every platform with fclonefileat (macOS 10.12 or later) also has
     acl_get_fd_np.  */
  bool has_acl = false;
  acl_t acl = acl_get_fd_np (fd, ACL_TYPE_EXTENDED);
  ಆದರೆ (acl)
    {
      acl_ನಮೂದು_t ace;
      has_acl = 0 <= acl_get_ನಮೂದು (acl, ACL_ಮೊದಲ_ನಮೂದು, &ace);
      acl_free (acl);
    }
  ಹಿಂತಿರುಗಿ has_acl;
}
#ಆದರೆ_ಅಂತ್ಯ

/* Handle failure from FICLONE or fclonefileat.
   Return FALSE if it's a terminal failure for this file.  */

ಸ್ಥಿರ bool
handle_clone_fail (ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
                   ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
                   bool new_dst, ಎಣಿಕೆ Reflink_type reflink_ಬಹುಲಕ)
{
  /* Record errno for debugging, but diagnose the failure and return
     false only if either --reflink=always, or errno is so serious
     that we should not fall back on read+write copying,
     which can succeed even if cloning fails due to ENOSPC etc.
     E.g., XFS has Allocation Groups where a clone may fail but a copy
     (to other groups) may succeed.  The only known potentially
     serious errno value is EIO, due to Linux kernel bugs reported
     in 2023 <https://bugs.gnu.org/60489>.  */

  copy_debug.reflink = errno;
  ಆದರೆ (! (reflink_ಬಹುಲಕ == REFLINK_ALWAYS || errno == EIO))
    ಹಿಂತಿರುಗಿ true;

  error (0, errno, _("failed to clone %s from %s"),
         quoteaf_n (0, dst_name), quoteaf_n (1, src_name));

  /* Remove the destination if cloning created it but later failed,
     either due to EIO, or to any error if --reflink=always.  The need
     for removal is a Linux kernel bug, as ioctl with FICLONE is
     supposed to be atomic.  We don't know whether currently supported
     kernels have the problem, so play it safe.  */
  ಆದರೆ (new_dst /* currently not for fclonefileat().  */
      && unlinkat (dst_dirfd, dst_relname, 0) < 0 && errno != ENOENT)
    error (0, errno, _("cannot remove %s"), quoteaf (dst_name));

  ಹಿಂತಿರುಗಿ false;
}

/* Copy a regular file from SRC_NAME to DST_NAME aka DST_DIRFD+DST_RELNAME.
   If the source file contains holes, copies holes and blocks of zeros
   in the source file as holes in the destination file.
   (Holes are read as zeroes by the 'read' system call.)
   When creating the destination, use DST_MODE & ~OMITTED_PERMISSIONS
   as the third argument in the call to open, adding
   OMITTED_PERMISSIONS after copying as needed.
   X provides many option settings.
   Return true if successful.
   *NEW_DST is initially as in copy_internal.
   If successful, set *NEW_DST to true if the destination file was created and
   to false otherwise; if unsuccessful, perhaps set *NEW_DST to some value.
   SRC_SB is the result of calling follow_fstatat on SRC_NAME;
   it might be updated by calling fstat again on the same file,
   to give it slightly more up-to-date contents.  */

ಸ್ಥಿರ bool
copy_reg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
          ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
          ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x,
          mode_t dst_ಬಹುಲಕ, mode_t omitted_permissions, bool *new_dst,
          ರಚನೆ stat *src_sb)
{
  ಪೂರ್ಣಾಂಕ dest_desc;
  ಪೂರ್ಣಾಂಕ dest_errno;
  ಪೂರ್ಣಾಂಕ ಮೂಲ_desc;
  mode_t extra_permissions;
  ರಚನೆ stat sb;
  ರಚನೆ stat src_open_sb;
  bool return_val = true;
  bool ದತ್ತಾಂಶ_copy_required = x->ದತ್ತಾಂಶ_copy_required;
  bool preserve_xattr = USE_XATTR & x->preserve_xattr;

  copy_debug.offload = COPY_DEBUG_UNKNOWN;
  copy_debug.reflink = x->reflink_ಬಹುಲಕ ? COPY_DEBUG_UNKNOWN : COPY_DEBUG_NO;
  copy_debug.sparse_detection = COPY_DEBUG_UNKNOWN;

  ಮೂಲ_desc = open (src_name,
                      (O_RDONLY | O_ದ್ವಿಮಾನ
                       | (x->dereference == DEREF_NEVER ? O_NOFOLLOW : 0)));
  ಆದರೆ (ಮೂಲ_desc < 0)
    {
      error (0, errno, _("cannot open %s for reading"), quoteaf (src_name));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (fstat (ಮೂಲ_desc, &src_open_sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (src_name));
      return_val = false;
      ಹೋಗು close_src_desc;
    }

  /* Compare the source dev/ino from the open file to the incoming,
     saved ones obtained via a previous call to stat.  */
  ಆದರೆ (! psame_inode (src_sb, &src_open_sb))
    {
      error (0, 0,
             _("skipping file %s, as it was replaced while being copied"),
             quoteaf (src_name));
      return_val = false;
      ಹೋಗು close_src_desc;
    }

  /* Might as well tell the caller about the latest version of the
     source file status, since we have it already.  */
  *src_sb = src_open_sb;
  mode_t src_ಬಹುಲಕ = src_sb->st_ಬಹುಲಕ;

  /* The semantics of the following open calls are mandated
     by the specs for both cp and mv.  */
  ಆದರೆ (! *new_dst)
    {
      ಪೂರ್ಣಾಂಕ open_flags =
        O_WRONLY | O_ದ್ವಿಮಾನ | (ದತ್ತಾಂಶ_copy_required ? O_TRUNC : 0);
      dest_desc = openat (dst_dirfd, dst_relname, open_flags);
      dest_errno = errno;

      /* When using cp --preserve=context to copy to an existing destination,
         reset the context as per the default context, which has already been
         set according to the src.
         When using the mutually exclusive -Z option, then adjust the type of
         the existing context according to the system default for the dest.
         Note we set the context here, _after_ the file is opened, lest the
         new context disallow that.  */
      ಆದರೆ (0 <= dest_desc
          && (x->ಗಣ_security_context || x->preserve_security_context))
        {
          ಆದರೆ (! ಗಣ_file_security_ctx (dst_name, false, x))
            {
              ಆದರೆ (x->require_preserve_context)
                {
                  return_val = false;
                  ಹೋಗು close_src_and_dst_desc;
                }
            }
        }

      ಆದರೆ (dest_desc < 0 && dest_errno != ENOENT
          && x->unlink_dest_after_failed_open)
        {
          ಆದರೆ (unlinkat (dst_dirfd, dst_relname, 0) == 0)
            {
              ಆದರೆ (x->verbose)
                ಬರೆ (_("removed %s\n"), quoteaf (dst_name));
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != ENOENT)
            {
              error (0, errno, _("cannot remove %s"), quoteaf (dst_name));
              return_val = false;
              ಹೋಗು close_src_desc;
            }

          dest_errno = ENOENT;
        }

      ಆದರೆ (dest_desc < 0 && dest_errno == ENOENT)
        {
          /* Ensure there is no race where a file may be left without
             an appropriate security context.  */
          ಆದರೆ (x->ಗಣ_security_context)
            {
              ಆದರೆ (! ಗಣ_process_security_ctx (src_name, dst_name, dst_ಬಹುಲಕ,
                                              true, x))
                {
                  return_val = false;
                  ಹೋಗು close_src_desc;
                }
            }

          /* Tell caller that the destination file is created.  */
          *new_dst = true;
        }
    }

  ಆದರೆ (*new_dst)
    {
#ಆದರೆ HAVE_FCLONEFILEAT && !USE_XATTR
# ifndef CLONE_ACL
#  define CLONE_ACL 0 /* Added in macOS 12.6.  */
# endif
# ifndef CLONE_NOOWNERCOPY
#  define CLONE_NOOWNERCOPY 0 /* Added in macOS 10.13.  */
# endif
      /* Try fclonefileat if copying data in reflink mode.
         Use CLONE_NOFOLLOW to avoid security issues that could occur
         if writing through dangling symlinks.  Although the circa
         2023 macOS documentation doesn't say so, CLONE_NOFOLLOW
         affects the destination file too.  */
      ಆದರೆ (ದತ್ತಾಂಶ_copy_required && x->reflink_ಬಹುಲಕ
          && (CLONE_NOOWNERCOPY || x->preserve_ownership))
        {
          /* Try fclonefileat so long as it won't create the
             destination with unwanted permissions, which could lead
             to a security race.  */
          mode_t cloned_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು = S_ISVTX | S_IRWXUGO;
          mode_t cloned_ಬಹುಲಕ = src_ಬಹುಲಕ & cloned_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು;
          mode_t desired_ಬಹುಲಕ
            = (x->preserve_ಬಹುಲಕ ? src_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು
               : x->ಗಣ_ಬಹುಲಕ ? x->ಬಹುಲಕ_2
               : ((x->explicit_no_preserve_ಬಹುಲಕ ? ಬಹುಲಕ_RW_UGO : dst_ಬಹುಲಕ)
                  & ~ cached_umask ()));
          ಆದರೆ (! (cloned_ಬಹುಲಕ & ~desired_ಬಹುಲಕ))
            {
              ಪೂರ್ಣಾಂಕ fc_flags
                = (CLONE_NOFOLLOW
                   | (x->preserve_ಬಹುಲಕ ? CLONE_ACL : 0)
                   | (x->preserve_ownership ? 0 : CLONE_NOOWNERCOPY));
              ಪೂರ್ಣಾಂಕ s = fclonefileat (ಮೂಲ_desc, dst_dirfd, dst_relname,
                                    fc_flags);
              ಆದರೆ (s != 0 && (fc_flags & CLONE_ACL) && errno == EINVAL)
                {
                  fc_flags &= ~CLONE_ACL;
                  s = fclonefileat (ಮೂಲ_desc, dst_dirfd, dst_relname,
                                    fc_flags);
                }
              ಆದರೆ (s == 0)
                {
                  copy_debug.reflink = COPY_DEBUG_YES;

                  /* Update the clone's timestamps and permissions
                     as needed.  */

                  ಆದರೆ (!x->preserve_timestamps)
                    {
                      ರಚನೆ timespec timespec[2];
                      timespec[0].tv_nsec = timespec[1].tv_nsec = UTIME_NOW;
                      ಆದರೆ (utimensat (dst_dirfd, dst_relname, timespec,
                                     AT_SYMLINK_NOFOLLOW)
                          != 0)
                        {
                          error (0, errno, _("updating times for %s"),
                                 quoteaf (dst_name));
                          return_val = false;
                          ಹೋಗು close_src_desc;
                        }
                    }

                  extra_permissions = desired_ಬಹುಲಕ & ~cloned_ಬಹುಲಕ;
                  ಆದರೆ (!extra_permissions
                      && (!x->preserve_ಬಹುಲಕ || (fc_flags & CLONE_ACL)
                          || !fd_has_acl (ಮೂಲ_desc)))
                    {
                      ಹೋಗು close_src_desc;
                    }

                  /* Either some desired permissions were not cloned,
                     or ACLs were not cloned despite that being requested.  */
                  omitted_permissions = 0;
                  dest_desc = -1;
                  ಹೋಗು ಗಣ_dest_ಬಹುಲಕ;
                }
              ಆದರೆ (! handle_clone_fail (dst_dirfd, dst_relname, src_name,
                                       dst_name,
                                       false /* We didn't create dst  */,
                                       x->reflink_ಬಹುಲಕ))
                {
                  return_val = false;
                  ಹೋಗು close_src_desc;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            copy_debug.reflink = COPY_DEBUG_AVOIDED;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದತ್ತಾಂಶ_copy_required && x->reflink_ಬಹುಲಕ)
        {
          ಆದರೆ (! CLONE_NOOWNERCOPY)
            copy_debug.reflink = COPY_DEBUG_AVOIDED;
        }
#ಆದರೆ_ಅಂತ್ಯ

      /* To allow copying xattrs on read-only files, create with u+w.
         This satisfies an inode permission check done by
         xattr_permission in fs/xattr.c of the GNU/Linux kernel.  */
      mode_t open_ಬಹುಲಕ =
        ((dst_ಬಹುಲಕ & ~omitted_permissions)
         | (preserve_xattr && !x->owner_privileges ? S_IWUSR : 0));
      extra_permissions = open_ಬಹುಲಕ & ~dst_ಬಹುಲಕ; /* either 0 or S_IWUSR */

      ಪೂರ್ಣಾಂಕ open_flags = O_WRONLY | O_CREAT | O_ದ್ವಿಮಾನ;
      dest_desc = openat (dst_dirfd, dst_relname, open_flags | O_EXCL,
                          open_ಬಹುಲಕ);
      dest_errno = errno;

      /* When trying to copy through a dangling destination symlink,
         the above open fails with EEXIST.  If that happens, and
         issymlinkat shows that it is a symlink, then we
         have a problem: trying to resolve this dangling symlink to
         a directory/destination-entry pair is fundamentally racy,
         so punt.  If x->open_dangling_dest_symlink is set (cp sets
         that when POSIXLY_CORRECT is set in the environment), simply
         call open again, but without O_EXCL (potentially dangerous).
         If not, fail with a diagnostic.  These shenanigans are necessary
         only when copying, i.e., not in move_mode.  */
      ಆದರೆ (dest_desc < 0 && dest_errno == EEXIST && ! x->move_ಬಹುಲಕ)
        {
          ಆದರೆ (issymlinkat (dst_dirfd, dst_relname) == 1)
            {
              ಆದರೆ (x->open_dangling_dest_symlink)
                {
                  dest_desc = openat (dst_dirfd, dst_relname,
                                      open_flags, open_ಬಹುಲಕ);
                  dest_errno = errno;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  error (0, 0, _("not writing through dangling symlink %s"),
                         quoteaf (dst_name));
                  return_val = false;
                  ಹೋಗು close_src_desc;
                }
            }
        }

      /* Improve quality of diagnostic when a nonexistent dst_name
         ends in a slash and open fails with errno == EISDIR.  */
      ಆದರೆ (dest_desc < 0 && dest_errno == EISDIR
          && *dst_name && dst_name[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (dst_name) - 1] == '/')
        dest_errno = ENOTDIR;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      omitted_permissions = extra_permissions = 0;
    }

  ಆದರೆ (dest_desc < 0)
    {
      error (0, dest_errno, _("cannot create regular file %s"),
             quoteaf (dst_name));
      return_val = false;
      ಹೋಗು close_src_desc;
    }

  /* --attributes-only overrides --reflink.  */
  ಆದರೆ (ದತ್ತಾಂಶ_copy_required && x->reflink_ಬಹುಲಕ)
    {
      ಆದರೆ (clone_file (dest_desc, ಮೂಲ_desc) == 0)
        {
          ದತ್ತಾಂಶ_copy_required = false;
          copy_debug.reflink = COPY_DEBUG_YES;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (! handle_clone_fail (dst_dirfd, dst_relname, src_name, dst_name,
                                   *new_dst, x->reflink_ಬಹುಲಕ))
           {
             return_val = false;
             ಹೋಗು close_src_and_dst_desc;
           }
        }
    }

  ಆದರೆ (! (ದತ್ತಾಂಶ_copy_required | x->preserve_ownership | extra_permissions))
    sb.st_ಬಹುಲಕ = 0;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fstat (dest_desc, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (dst_name));
      return_val = false;
      ಹೋಗು close_src_and_dst_desc;
    }

  /* If extra permissions needed for copy_xattr didn't happen (e.g.,
     due to umask) chmod to add them temporarily; if that fails give
     up with extra permissions, letting copy_attr fail later.  */
  mode_t temporary_ಬಹುಲಕ = sb.st_ಬಹುಲಕ | extra_permissions;
  ಆದರೆ (temporary_ಬಹುಲಕ != sb.st_ಬಹುಲಕ
      && (fchmod_or_lchmod (dest_desc, dst_dirfd, dst_relname, temporary_ಬಹುಲಕ)
          != 0))
    extra_permissions = 0;

  ಆದರೆ (ದತ್ತಾಂಶ_copy_required
      && (copy_file_ದತ್ತಾಂಶ (ಮೂಲ_desc, &src_open_sb, 0, src_name,
                          dest_desc, &sb, 0, dst_name,
                          ಎಣಿಕೆ_MAX, x, &copy_debug)
          < 0))
    {
      return_val = false;
      ಹೋಗು close_src_and_dst_desc;
    }

  ಆದರೆ (x->preserve_timestamps)
    {
      ರಚನೆ timespec timespec[2];
      timespec[0] = get_stat_atime (src_sb);
      timespec[1] = get_stat_mtime (src_sb);

      ಆದರೆ (fdutimensat (dest_desc, dst_dirfd, dst_relname, timespec, 0) != 0)
        {
          error (0, errno, _("preserving times for %s"), quoteaf (dst_name));
          ಆದರೆ (x->require_preserve)
            {
              return_val = false;
              ಹೋಗು close_src_and_dst_desc;
            }
        }
    }

  /* Set ownership before xattrs as changing owners will
     clear capabilities.  */
  ಆದರೆ (x->preserve_ownership && ! SAME_OWNER_AND_ಗುಂಪು (*src_sb, sb))
    {
      ಆಯ್ಕೆ (ಗಣ_owner (x, dst_name, dst_dirfd, dst_relname, dest_desc,
                         src_sb, *new_dst, &sb))
        {
        ಸ್ಥಿತಿ -1:
          return_val = false;
          ಹೋಗು close_src_and_dst_desc;

        ಸ್ಥಿತಿ 0:
          src_ಬಹುಲಕ &= ~ (S_ISUID | S_ISGID | S_ISVTX);
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (preserve_xattr)
    {
      ಆದರೆ (!copy_attr (src_name, ಮೂಲ_desc, dst_name, dest_desc, x)
          && x->require_preserve_xattr && ! x->move_ಬಹುಲಕ)
        return_val = false;
    }

  ಗಣ_author (dst_name, dest_desc, src_sb);

#ಆದರೆ HAVE_FCLONEFILEAT && !USE_XATTR
ಗಣ_dest_ಬಹುಲಕ:
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (x->preserve_ಬಹುಲಕ || x->move_ಬಹುಲಕ)
    {
      ಆದರೆ (xcopy_acl (src_name, ಮೂಲ_desc, dst_name, dest_desc, src_ಬಹುಲಕ) != 0
          && x->require_preserve)
        return_val = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->ಗಣ_ಬಹುಲಕ)
    {
      ಆದರೆ (xset_acl (dst_name, dest_desc, x->ಬಹುಲಕ_2) != 0)
        return_val = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->explicit_no_preserve_ಬಹುಲಕ && *new_dst)
    {
      ಆದರೆ (xset_acl (dst_name, dest_desc, ಬಹುಲಕ_RW_UGO & ~cached_umask ()) != 0)
        return_val = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (omitted_permissions | extra_permissions)
    {
      omitted_permissions &= ~ cached_umask ();
      ಆದರೆ ((omitted_permissions | extra_permissions)
          && (fchmod_or_lchmod (dest_desc, dst_dirfd, dst_relname,
                                dst_ಬಹುಲಕ & ~ cached_umask ())
              != 0))
        {
          error (0, errno, _("preserving permissions for %s"),
                 quoteaf (dst_name));
          ಆದರೆ (x->require_preserve)
            return_val = false;
        }
    }

  ಆದರೆ (dest_desc < 0)
    ಹೋಗು close_src_desc;

close_src_and_dst_desc:
  ಆದರೆ (close (dest_desc) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (dst_name));
      return_val = false;
    }
close_src_desc:
  ಆದರೆ (close (ಮೂಲ_desc) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (src_name));
      return_val = false;
    }

  /* Output debug info for data copying operations.  */
  ಆದರೆ (x->debug)
    emit_debug (x);

  ಹಿಂತಿರುಗಿ return_val;
}

/* Return whether it's OK that two files are the "same" by some measure.
   The first file is SRC_NAME and has status SRC_SB.
   The second is DST_DIRFD+DST_RELNAME and has status DST_SB.
   The copying options are X.  The goal is to avoid
   making the 'copy' operation remove both copies of the file
   in that case, while still allowing the user to e.g., move or
   copy a regular file onto a symlink that points to it.
   Try to minimize the cost of this function in the common case.
   Set *RETURN_NOW if we've determined that the caller has no more
   work to do and should return successfully, right away.  */

ಸ್ಥಿರ bool
same_file_ok (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *src_sb,
              ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *dst_sb,
              ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x, bool *return_now)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ stat *src_sb_link;
  ಸ್ಥಿರಾಂಕ ರಚನೆ stat *dst_sb_link;
  ರಚನೆ stat tmp_dst_sb;
  ರಚನೆ stat tmp_src_sb;

  bool same_link;
  bool same = psame_inode (src_sb, dst_sb);

  *return_now = false;

  /* FIXME: this should (at the very least) be moved into the following
     if-block.  More likely, it should be removed, because it inhibits
     making backups.  But removing it will result in a change in behavior
     that will probably have to be documented -- and tests will have to
     be updated.  */
  ಆದರೆ (same && x->hard_link)
    {
      *return_now = true;
      ಹಿಂತಿರುಗಿ true;
    }

  ಆದರೆ (x->dereference == DEREF_NEVER)
    {
      same_link = same;

      /* If both the source and destination files are symlinks (and we'll
         know this here IFF preserving symlinks), then it's usually ok
         when they are distinct.  */
      ಆದರೆ (S_ISLNK (src_sb->st_ಬಹುಲಕ) && S_ISLNK (dst_sb->st_ಬಹುಲಕ))
        {
          bool sn = same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname);
          ಆದರೆ ( ! sn)
            {
              /* It's fine when we're making any type of backup.  */
              ಆದರೆ (x->backup_type != no_backups)
                ಹಿಂತಿರುಗಿ true;

              /* Here we have two symlinks that are hard-linked together,
                 and we're not making backups.  In this unusual case, simply
                 returning true would lead to mv calling "rename(A,B)",
                 which would do nothing and return 0.  */
              ಆದರೆ (same_link)
                {
                  *return_now = true;
                  ಹಿಂತಿರುಗಿ ! x->move_ಬಹುಲಕ;
                }
            }

          ಹಿಂತಿರುಗಿ ! sn;
        }

      src_sb_link = src_sb;
      dst_sb_link = dst_sb;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (!same)
        ಹಿಂತಿರುಗಿ true;

      ಆದರೆ (fstatat (dst_dirfd, dst_relname, &tmp_dst_sb,
                   AT_SYMLINK_NOFOLLOW) != 0
          || lstat (src_name, &tmp_src_sb) != 0)
        ಹಿಂತಿರುಗಿ true;

      src_sb_link = &tmp_src_sb;
      dst_sb_link = &tmp_dst_sb;

      same_link = psame_inode (src_sb_link, dst_sb_link);

      /* If both are symlinks, then it's ok, but only if the destination
         will be unlinked before being opened.  This is like the test
         above, but with the addition of the unlink_dest_before_opening
         conjunct because otherwise, with two symlinks to the same target,
         we'd end up truncating the source file.  */
      ಆದರೆ (S_ISLNK (src_sb_link->st_ಬಹುಲಕ) && S_ISLNK (dst_sb_link->st_ಬಹುಲಕ)
          && x->unlink_dest_before_opening)
        ಹಿಂತಿರುಗಿ true;
    }

  /* The backup code ensures there's a copy, so it's usually ok to
     remove any destination file.  One exception is when both
     source and destination are the same directory entry.  In that
     case, moving the destination file aside (in making the backup)
     would also rename the source file and result in an error.  */
  ಆದರೆ (x->backup_type != no_backups)
    {
      ಆದರೆ (!same_link)
        {
          /* In copy mode when dereferencing symlinks, if the source is a
             symlink and the dest is not, then backing up the destination
             (moving it aside) would make it a dangling symlink, and the
             subsequent attempt to open it in copy_reg would fail with
             a misleading diagnostic.  Avoid that by returning zero in
             that case so the caller can make cp (or mv when it has to
             resort to reading the source file) fail now.  */

          /* FIXME-note: even with the following kludge, we can still provoke
             the offending diagnostic.  It's just a little harder to do :-)
             $ rm -f a b c; touch c; ln -s c b; ln -s b a; cp -b a b
             cp: cannot open 'a' for reading: No such file or directory
             That's misleading, since a subsequent 'ls' shows that 'a'
             is still there.
             One solution would be to open the source file *before* moving
             aside the destination, but that'd involve a big rewrite. */
          ಆದರೆ ( ! x->move_ಬಹುಲಕ
               && x->dereference != DEREF_NEVER
               && S_ISLNK (src_sb_link->st_ಬಹುಲಕ)
               && ! S_ISLNK (dst_sb_link->st_ಬಹುಲಕ))
            ಹಿಂತಿರುಗಿ false;

          ಹಿಂತಿರುಗಿ true;
        }

      /* FIXME: What about case insensitive file systems ?  */
      ಹಿಂತಿರುಗಿ ! same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname);
    }

#ಆದರೆ 0
  /* FIXME: use or remove */

  /* If we're making a backup, we'll detect the problem case in
     copy_reg because SRC_NAME will no longer exist.  Allowing
     the test to be deferred lets cp do some useful things.
     But when creating hardlinks and SRC_NAME is a symlink
     but DST_RELNAME is not we must test anyway.  */
  ಆದರೆ (x->hard_link
      || !S_ISLNK (src_sb_link->st_ಬಹುಲಕ)
      || S_ISLNK (dst_sb_link->st_ಬಹುಲಕ))
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (x->dereference != DEREF_NEVER)
    ಹಿಂತಿರುಗಿ true;
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (x->move_ಬಹುಲಕ || x->unlink_dest_before_opening)
    {
      /* They may refer to the same file if we're in move mode and the
         target is a symlink.  That is ok, since we remove any existing
         destination file before opening it -- via 'rename' if they're on
         the same file system, via unlinkat otherwise.  */
      ಆದರೆ (S_ISLNK (dst_sb_link->st_ಬಹುಲಕ))
        ಹಿಂತಿರುಗಿ true;

      /* It's not ok if they're distinct hard links to the same file as
         this causes a race condition and we may lose data in this case.  */
      ಆದರೆ (same_link
          && 1 < dst_sb_link->st_nlink
          && ! same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname))
        ಹಿಂತಿರುಗಿ ! x->move_ಬಹುಲಕ;
    }

  /* If neither is a symlink, then it's ok as long as they aren't
     hard links to the same file.  */
  ಆದರೆ (!S_ISLNK (src_sb_link->st_ಬಹುಲಕ) && !S_ISLNK (dst_sb_link->st_ಬಹುಲಕ))
    {
      ಆದರೆ (!psame_inode (src_sb_link, dst_sb_link))
        ಹಿಂತಿರುಗಿ true;

      /* If they are the same file, it's ok if we're making hard links.  */
      ಆದರೆ (x->hard_link)
        {
          *return_now = true;
          ಹಿಂತಿರುಗಿ true;
        }
    }

  /* At this point, it is normally an error (data loss) to move a symlink
     onto its referent, but in at least one narrow case, it is not:
     In move mode, when
     1) src is a symlink,
     2) dest has a link count of 2 or more and
     3) dest and the referent of src are not the same directory entry,
     then it's ok, since while we'll lose one of those hard links,
     src will still point to a remaining link.
     Note that technically, condition #3 obviates condition #2, but we
     retain the 1 < st_nlink condition because that means fewer invocations
     of the more expensive #3.

     Given this,
       $ touch f && ln f l && ln -s f s
       $ ls -og f l s
       -rw-------. 2  0 Jan  4 22:46 f
       -rw-------. 2  0 Jan  4 22:46 l
       lrwxrwxrwx. 1  1 Jan  4 22:46 s -> f
     this must fail: mv s f
     this must succeed: mv s l */
  ಆದರೆ (x->move_ಬಹುಲಕ
      && S_ISLNK (src_sb->st_ಬಹುಲಕ)
      && 1 < dst_sb_link->st_nlink)
    {
      ಅಕ್ಷರ *abs_src = canonicalize_file_name (src_name);
      ಆದರೆ (abs_src)
        {
          bool ಫಲಿತಾಂಶ = ! same_nameat (AT_FDCWD, abs_src,
                                       dst_dirfd, dst_relname);
          ಬಿಡುಗಡೆ (abs_src);
          ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
        }
    }

  /* It's ok to recreate a destination symlink. */
  ಆದರೆ (x->symbolic_link && S_ISLNK (dst_sb_link->st_ಬಹುಲಕ))
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (x->dereference == DEREF_NEVER)
    {
      ಆದರೆ ( ! S_ISLNK (src_sb_link->st_ಬಹುಲಕ))
        tmp_src_sb = *src_sb_link;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stat (src_name, &tmp_src_sb) != 0)
        ಹಿಂತಿರುಗಿ true;

      ಆದರೆ ( ! S_ISLNK (dst_sb_link->st_ಬಹುಲಕ))
        tmp_dst_sb = *dst_sb_link;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fstatat (dst_dirfd, dst_relname, &tmp_dst_sb, 0) != 0)
        ಹಿಂತಿರುಗಿ true;

      ಆದರೆ (!psame_inode (&tmp_src_sb, &tmp_dst_sb))
        ಹಿಂತಿರುಗಿ true;

      ಆದರೆ (x->hard_link)
        {
          /* It's ok to attempt to hardlink the same file,
            and return early if not replacing a symlink.
            Note we need to return early to avoid a later
            unlink() of DST (when SRC is a symlink).  */
          *return_now = ! S_ISLNK (dst_sb_link->st_ಬಹುಲಕ);
          ಹಿಂತಿರುಗಿ true;
        }
    }

  ಹಿಂತಿರುಗಿ false;
}

/* Return whether DST_DIRFD+DST_RELNAME, with mode MODE,
   is writable in the sense of 'mv'.
   Always consider a symbolic link to be writable.  */
ಸ್ಥಿರ bool
writable_destination (ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname, mode_t ಬಹುಲಕ_2)
{
  ಹಿಂತಿರುಗಿ (S_ISLNK (ಬಹುಲಕ_2)
          || can_write_any_file ()
          || faccessat (dst_dirfd, dst_relname, W_OK, AT_EACCESS) == 0);
}

ಸ್ಥಿರ bool
overwrite_ok (ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
              ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
              ರಚನೆ stat ಸ್ಥಿರಾಂಕ *dst_sb)
{
  ಆದರೆ (! writable_destination (dst_dirfd, dst_relname, dst_sb->st_ಬಹುಲಕ))
    {
      ಅಕ್ಷರ perms[12];		/* "-rwxrwxrwx " ls-style modes. */
      strmode (dst_sb->st_ಬಹುಲಕ, perms);
      perms[10] = '\0';
      fprintf (stderr,
               (x->move_ಬಹುಲಕ || x->unlink_dest_before_opening
                || x->unlink_dest_after_failed_open)
               ? _("%s: replace %s, overriding mode %04lo (%s)? ")
               : _("%s: unwritable %s (mode %04lo, %s); try anyway? "),
               ಕಾರ್ಯಕ್ರಮ_name, quoteaf (dst_name),
               (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) (dst_sb->st_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು),
               &perms[1]);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      fprintf (stderr, _("%s: overwrite %s? "),
               ಕಾರ್ಯಕ್ರಮ_name, quoteaf (dst_name));
    }

  ಹಿಂತಿರುಗಿ yesno ();
}

/* Initialize the hash table implementing a set of F_triple entries
   corresponding to destination files.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
dest_info_init (ರಚನೆ cp_options *x)
{
  x->dest_info
    = hash_initialize (DEST_INFO_INITIAL_ಸಾಮರ್ಥ್ಯ,
                       ಖಾಲಿ,
                       triple_hash,
                       triple_compare,
                       triple_free);
  ಆದರೆ (! x->dest_info)
    xalloc_die ();
}

/* Initialize the hash table implementing a set of F_triple entries
   corresponding to source files listed on the command line.  */
ಬಾಹ್ಯ ಶೂನ್ಯ
src_info_init (ರಚನೆ cp_options *x)
{

  /* Note that we use triple_hash_no_name here.
     Contrast with the use of triple_hash above.
     That is necessary because a source file may be specified
     in many different ways.  We want to warn about this
       cp a a d/
     as well as this:
       cp a ./a d/
  */
  x->src_info
    = hash_initialize (DEST_INFO_INITIAL_ಸಾಮರ್ಥ್ಯ,
                       ಖಾಲಿ,
                       triple_hash_no_name,
                       triple_compare,
                       triple_free);
  ಆದರೆ (! x->src_info)
    xalloc_die ();
}

/* When effecting a move (e.g., for mv(1)), and given the name DST_NAME
   aka DST_DIRFD+DST_RELNAME
   of the destination and a corresponding stat buffer, DST_SB, return
   true if the logical 'move' operation should _not_ proceed.
   Otherwise, return false.
   Depending on options specified in X, this code may issue an
   interactive prompt asking whether it's ok to overwrite DST_NAME.  */
ಸ್ಥಿರ bool
abandon_move (ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x,
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
              ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
              ರಚನೆ stat ಸ್ಥಿರಾಂಕ *dst_sb)
{
  affirm (x->move_ಬಹುಲಕ);
  ಹಿಂತಿರುಗಿ (x->update == UPDATE_NONE
          || x->update == UPDATE_NONE_FAIL
          || ((x->interactive == I_ASK_USER
               || (x->interactive == I_UNSPECIFIED
                   && x->stdin_tty
                   && ! writable_destination (dst_dirfd, dst_relname,
                                              dst_sb->st_ಬಹುಲಕ)))
              && ! overwrite_ok (x, dst_name, dst_dirfd, dst_relname, dst_sb)));
}

/* Print --verbose output on standard output, e.g. 'new' -> 'old'.
   If BACKUP_DST_NAME is non-null, then also indicate that it is
   the name of a backup file.  */
ಸ್ಥಿರ ಶೂನ್ಯ
emit_verbose (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst,
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *backup_dst_name)
{
  ಬರೆ (format, quoteaf_n (0, src), quoteaf_n (1, dst));
  ಆದರೆ (backup_dst_name)
    ಬರೆ (_(" (backup: %s)"), quoteaf (backup_dst_name));
  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* A wrapper around "setfscreatecon (NULL)" that exits upon failure.  */
ಸ್ಥಿರ ಶೂನ್ಯ
restore_default_fscreatecon_or_die (ಶೂನ್ಯ)
{
  ಆದರೆ (setfscreatecon (ಖಾಲಿ) != 0)
    error (EXIT_FAILURE, errno,
           _("failed to restore the default file creation context"));
}

/* Return a newly-allocated string that is like STR
   except replace its suffix SUFFIX with NEWSUFFIX.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
subst_suffix (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *newsuffix)
{
  idx_t prefixlen = suffix - str;
  idx_t newsuffixsize = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (newsuffix) + 1;
  ಅಕ್ಷರ *r = ximalloc (prefixlen + newsuffixsize);
  ಸ್ಮರಣೆ_ನಕಲು (r + prefixlen, newsuffix, newsuffixsize);
  ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_ನಕಲು (r, str, prefixlen);
}

/* Create a hard link to SRC_NAME aka SRC_DIRFD+SRC_RELNAME;
   the new link is at DST_NAME aka DST_DIRFD+DST_RELNAME.
   A null SRC_NAME stands for the file whose name is like DST_NAME
   except with DST_RELNAME replaced with SRC_RELNAME.
   Honor the REPLACE, VERBOSE and DEREFERENCE settings.
   Return true upon success.  Otherwise, diagnose the
   failure and return false.  If SRC_NAME is a symbolic link, then it will not
   be followed unless DEREFERENCE is true.
   If the system doesn't support hard links to symbolic links, then DST_NAME
   will be created as a symbolic link to SRC_NAME.  */
ಸ್ಥಿರ bool
create_hard_link (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಪೂರ್ಣಾಂಕ src_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_relname,
                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name, ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
                  bool replace, bool verbose, bool dereference)
{
  ಪೂರ್ಣಾಂಕ err = force_linkat (src_dirfd, src_relname, dst_dirfd, dst_relname,
                          dereference ? AT_SYMLINK_FOLLOW : 0,
                          replace, -1);
  ಆದರೆ (0 < err)
    {

      ಅಕ್ಷರ *a_src_name = ಖಾಲಿ;
      ಆದರೆ (!src_name)
        src_name = a_src_name = subst_suffix (dst_name, dst_relname,
                                              src_relname);
      error (0, err, _("cannot create hard link %s to %s"),
             quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
      ಬಿಡುಗಡೆ (a_src_name);
      ಹಿಂತಿರುಗಿ false;
    }
  ಆದರೆ (err < 0 && verbose)
    ಬರೆ (_("removed %s\n"), quoteaf (dst_name));
  ಹಿಂತಿರುಗಿ true;
}

/* Return true if the current file should be (tried to be) dereferenced:
   either for DEREF_ALWAYS or for DEREF_COMMAND_LINE_ARGUMENTS in the case
   where the current file is a COMMAND_LINE_ARG; otherwise return false.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
should_dereference (ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x, bool command_ಸಾಲು_arg)
{
  ಹಿಂತಿರುಗಿ x->dereference == DEREF_ALWAYS
         || (x->dereference == DEREF_COMMAND_ಸಾಲು_ARGUMENTS
             && command_ಸಾಲು_arg);
}

/* Return true if the source file with basename SRCBASE and status SRC_ST
   is likely to be the simple backup file for DST_DIRFD+DST_RELNAME.  */
ಸ್ಥಿರ bool
ಮೂಲ_is_dst_backup (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srcbase, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *src_st,
                      ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname)
{
  size_t srcbaselen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (srcbase);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dstbase = ಕೊನೆಯ_component (dst_relname);
  size_t dstbaselen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (dstbase);
  size_t suffixlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (simple_backup_suffix);
  ಆದರೆ (! (srcbaselen == dstbaselen + suffixlen
         && memeq (srcbase, dstbase, dstbaselen)
         && streq (srcbase + dstbaselen, simple_backup_suffix)))
    ಹಿಂತಿರುಗಿ false;
  ಅಕ್ಷರ *dst_ಹಿಂಭಾಗ = subst_suffix (dst_relname,
                                 dst_relname + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (dst_relname),
                                 simple_backup_suffix);
  ರಚನೆ stat dst_ಹಿಂಭಾಗ_sb;
  ಪೂರ್ಣಾಂಕ dst_ಹಿಂಭಾಗ_ಸ್ಥಿತಿ = fstatat (dst_dirfd, dst_ಹಿಂಭಾಗ, &dst_ಹಿಂಭಾಗ_sb, 0);
  ಬಿಡುಗಡೆ (dst_ಹಿಂಭಾಗ);
  ಹಿಂತಿರುಗಿ dst_ಹಿಂಭಾಗ_ಸ್ಥಿತಿ == 0 && psame_inode (src_st, &dst_ಹಿಂಭಾಗ_sb);
}

/* Copy the file SRC_NAME to the file DST_NAME aka DST_DIRFD+DST_RELNAME.
   If NONEXISTENT_DST is positive, DST_NAME does not exist even as a
   dangling symlink; if negative, it does not exist except possibly
   as a dangling symlink; if zero, its existence status is unknown.
   A non-null PARENT describes the parent directory.
   ANCESTORS points to a linked, null terminated list of
   devices and inodes of parent directories of SRC_NAME.
   X summarizes the command-line options.
   COMMAND_LINE_ARG means SRC_NAME was specified on the command line.
   FIRST_DIR_CREATED_PER_COMMAND_LINE_ARG is both input and output.
   Set *COPY_INTO_SELF if SRC_NAME is a parent of (or the
   same as) DST_NAME; otherwise, clear it.
   If X->move_mode, set *RENAME_SUCCEEDED according to whether
   the source was simply renamed to the destination.
   Return true if successful.  */
ಸ್ಥಿರ bool
copy_ಆಂತರಿಕ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
               ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
               ಪೂರ್ಣಾಂಕ nonexistent_dst,
               ರಚನೆ stat ಸ್ಥಿರಾಂಕ *ಪೋಷಕ,
               ರಚನೆ dir_list *ancestors,
               ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x,
               bool command_ಸಾಲು_arg,
               bool *ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg,
               bool *copy_into_self,
               bool *rename_succeeded)
{
  ರಚನೆ stat src_sb;
  ರಚನೆ stat dst_sb;
  mode_t src_ಬಹುಲಕ IF_LINT ( = 0);
  mode_t dst_ಬಹುಲಕ IF_LINT ( = 0);
  mode_t dst_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು;
  mode_t omitted_permissions;
  bool restore_dst_ಬಹುಲಕ = false;
  ಅಕ್ಷರ *earlier_file = ಖಾಲಿ;
  ಅಕ್ಷರ *dst_backup = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *drelname = *dst_relname ? dst_relname : ".";
  bool delayed_ok;
  bool copied_as_regular = false;
  bool dest_is_symlink = false;
  bool have_dst_lstat = false;

  *copy_into_self = false;

  ಪೂರ್ಣಾಂಕ rename_errno = x->rename_errno;
  ಆದರೆ (x->move_ಬಹುಲಕ && !x->exchange)
    {
      ಆದರೆ (rename_errno < 0)
        rename_errno = (renameatu (AT_FDCWD, src_name, dst_dirfd, drelname,
                                   RENAME_NOREPLACE)
                        ? errno : 0);
      nonexistent_dst = *rename_succeeded = rename_errno == 0;
    }

  ಆದರೆ (rename_errno == 0
      ? !x->ಕೊನೆಯ_file
      : rename_errno != EEXIST
        || (x->update != UPDATE_NONE && x->update != UPDATE_NONE_FAIL))
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = rename_errno == 0 ? dst_name : src_name;
      ಪೂರ್ಣಾಂಕ dirfd = rename_errno == 0 ? dst_dirfd : AT_FDCWD;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relname = rename_errno == 0 ? drelname : src_name;
      ಪೂರ್ಣಾಂಕ fstatat_flags
        = x->dereference == DEREF_NEVER ? AT_SYMLINK_NOFOLLOW : 0;
      ಆದರೆ (follow_fstatat (dirfd, relname, &src_sb, fstatat_flags) != 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (name));
          ಹಿಂತಿರುಗಿ false;
        }

      src_ಬಹುಲಕ = src_sb.st_ಬಹುಲಕ;

      ಆದರೆ (S_ISDIR (src_ಬಹುಲಕ) && !x->recursive)
        {
          error (0, 0, ! x->install_ಬಹುಲಕ /* cp */
                 ? _("-r not specified; omitting directory %s")
                 : _("omitting directory %s"),
                 quoteaf (src_name));
          ಹಿಂತಿರುಗಿ false;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
#ಆದರೆ defined lint && (defined __clang__ || defined __COVERITY__)
      affirm (x->move_ಬಹುಲಕ);
      memset (&src_sb, 0, ಗಾತ್ರ src_sb);
#ಆದರೆ_ಅಂತ್ಯ
    }

  /* Detect the case in which the same source file appears more than
     once on the command line and no backup option has been selected.
     If so, simply warn and don't copy it the second time.
     This check is enabled only if x->src_info is non-null.  */
  ಆದರೆ (command_ಸಾಲು_arg && x->src_info)
    {
      ಆದರೆ ( ! S_ISDIR (src_ಬಹುಲಕ)
           && x->backup_type == no_backups
           && ನೋಡಿದ_file (x->src_info, src_name, &src_sb))
        {
          error (0, 0, _("warning: source file %s specified more than once"),
                 quoteaf (src_name));
          ಹಿಂತಿರುಗಿ true;
        }

      ದಾಖಲೆ_file (x->src_info, src_name, &src_sb);
    }

  bool dereference = should_dereference (x, command_ಸಾಲು_arg);

  /* Whether the destination is (or was) known to be new, updated as
     more info comes in.  This may become true if the destination is a
     dangling symlink, in contexts where dangling symlinks should be
     treated the same as nonexistent files.  */
  bool new_dst = 0 < nonexistent_dst;

  ಆದರೆ (! new_dst)
    {
      /* Normally, fill in DST_SB or set NEW_DST so that later code
         can use DST_SB if NEW_DST is false.  However, don't bother
         doing this when rename_errno == EEXIST and not updating,
         which means that even if !NEW_DST the move will be abandoned
         without looking at DST_SB.  */
      ಆದರೆ (! (rename_errno == EEXIST
             && (x->update == UPDATE_NONE
                 || x->update == UPDATE_NONE_FAIL)))
        {
          /* Regular files can be created by writing through symbolic
             links, but other files cannot.  So use stat on the
             destination when copying a regular file, and lstat otherwise.
             However, if we intend to unlink or remove the destination
             first, use lstat, since a copy won't actually be made to the
             destination in that case.  */
          bool use_lstat
            = ((! S_ISREG (src_ಬಹುಲಕ)
                && (! x->copy_as_regular
                    || (S_ISDIR (src_ಬಹುಲಕ) && !x->keep_directory_symlink)
                    || S_ISLNK (src_ಬಹುಲಕ)))
               || x->move_ಬಹುಲಕ || x->symbolic_link || x->hard_link
               || x->backup_type != no_backups
               || x->unlink_dest_before_opening);
          ಆದರೆ (!use_lstat && nonexistent_dst < 0)
            new_dst = true;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 <= follow_fstatat (dst_dirfd, drelname, &dst_sb,
                                        use_lstat ? AT_SYMLINK_NOFOLLOW : 0))
            {
              have_dst_lstat = use_lstat;
              rename_errno = EEXIST;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == ENOENT)
            new_dst = true;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == ELOOP && !use_lstat
                   && x->unlink_dest_after_failed_open)
            {
              /* cp -f's destination might be a symlink loop.
                 Leave new_dst=false so that we try to unlink later.  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              error (0, errno, _("cannot stat %s"), quoteaf (dst_name));
              ಹಿಂತಿರುಗಿ false;
            }
        }

      ಆದರೆ (rename_errno == EEXIST)
        {
          bool return_now = false;
          bool return_val = true;
          bool skipped = false;

          ಆದರೆ ((x->update != UPDATE_NONE && x->update != UPDATE_NONE_FAIL)
              && ! same_file_ok (src_name, &src_sb, dst_dirfd, drelname,
                                 &dst_sb, x, &return_now))
            {
              error (0, 0, _("%s and %s are the same file"),
                     quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
              ಹಿಂತಿರುಗಿ false;
            }

          ಆದರೆ (x->update == UPDATE_OLDER && !S_ISDIR (src_ಬಹುಲಕ))
            {
              /* When preserving timestamps (but not moving within a file
                 system), don't worry if the destination timestamp is
                 less than the source merely because of timestamp
                 truncation.  */
              ಪೂರ್ಣಾಂಕ options = ((x->preserve_timestamps
                              && ! (x->move_ಬಹುಲಕ
                                    && dst_sb.st_dev == src_sb.st_dev))
                             ? UTIMECMP_TRUNCATE_ಮೂಲ
                             : 0);

              ಆದರೆ (0 <= utimecmpat (dst_dirfd, dst_relname, &dst_sb,
                                   &src_sb, options))
                {
                  /* We're using --update and the destination is not older
                     than the source, so do not copy or move.  Pretend the
                     rename succeeded, so the caller (if it's mv) doesn't
                     end up removing the source file.  */
                  ಆದರೆ (rename_succeeded)
                    *rename_succeeded = true;

                  /* However, we still must record that we've processed
                     this src/dest pair, in case this source file is
                     hard-linked to another one.  In that case, we'll use
                     the mapping information to link the corresponding
                     destination names.  */
                  earlier_file = remember_copied (dst_relname, src_sb.st_ino,
                                                  src_sb.st_dev);
                  ಆದರೆ (earlier_file)
                    {
                      /* Note we currently replace DST_NAME unconditionally,
                         even if it was a newer separate file.  */
                      ಆದರೆ (! create_hard_link (ಖಾಲಿ, dst_dirfd, earlier_file,
                                              dst_name, dst_dirfd, dst_relname,
                                              true,
                                              x->verbose, dereference))
                        {
                          ಹೋಗು un_backup;
                        }
                    }

                  skipped = true;
                  ಹೋಗು skip;
                }
            }

          /* When there is an existing destination file, we may end up
             returning early, and hence not copying/moving the file.
             This may be due to an interactive 'negative' reply to the
             prompt about the existing file.  It may also be due to the
             use of the --no-clobber option.

             cp and mv treat -i and -f differently.  */
          ಆದರೆ (x->move_ಬಹುಲಕ)
            {
              ಆದರೆ (abandon_move (x, dst_name, dst_dirfd, drelname, &dst_sb))
                {
                  /* Pretend the rename succeeded, so the caller (mv)
                     doesn't end up removing the source file.  */
                  ಆದರೆ (rename_succeeded)
                    *rename_succeeded = true;

                  skipped = true;
                  return_val = x->update == UPDATE_NONE;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (! S_ISDIR (src_ಬಹುಲಕ)
                  && (x->update == UPDATE_NONE
                      || x->update == UPDATE_NONE_FAIL
                      || (x->interactive == I_ASK_USER
                          && ! overwrite_ok (x, dst_name, dst_dirfd,
                                             dst_relname, &dst_sb))))
                {
                  skipped = true;
                  return_val = x->update == UPDATE_NONE;
                }
            }

skip:
          ಆದರೆ (skipped)
            {
              ಆದರೆ (x->update == UPDATE_NONE_FAIL)
                error (0, 0, _("not replacing %s"), quoteaf (dst_name));
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->debug)
                ಬರೆ (_("skipped %s\n"), quoteaf (dst_name));

              return_now = true;
            }

          ಆದರೆ (return_now)
            ಹಿಂತಿರುಗಿ return_val;

          /* Copying a directory onto a non-directory, or vice versa,
             is ok only with --backup or --exchange.  */
          ಆದರೆ (!S_ISDIR (src_ಬಹುಲಕ) != !S_ISDIR (dst_sb.st_ಬಹುಲಕ)
              && x->backup_type == no_backups && !x->exchange)
            {
              error (0, 0,
                     _(S_ISDIR (src_ಬಹುಲಕ)
                       ? ("cannot overwrite non-directory %s "
                          "with directory %s")
                       : ("cannot overwrite directory %s "
                          "with non-directory %s")),
                     quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
              ಹಿಂತಿರುಗಿ false;
            }

          /* Don't let the user destroy their data, even if they try hard:
             This mv command must fail (likewise for cp):
             rm -rf a b c; mkdir a b c; touch a/f b/f; mv a/f b/f c
             Otherwise, the contents of b/f would be lost.
             In the case of 'cp', b/f would be lost if the user simulated
             a move using cp and rm.
             Nothing is lost if you use --backup=numbered or --exchange.  */
          ಆದರೆ (!S_ISDIR (dst_sb.st_ಬಹುಲಕ) && command_ಸಾಲು_arg
              && x->backup_type != numbered_backups && !x->exchange
              && ನೋಡಿದ_file (x->dest_info, dst_relname, &dst_sb))
            {
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
              ಹಿಂತಿರುಗಿ false;
            }

          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srcbase;
          ಆದರೆ (x->backup_type != no_backups
              /* Don't try to back up a destination if the last
                 component of src_name is "." or "..".  */
              && ! dot_or_dotdot (srcbase = ಕೊನೆಯ_component (src_name))
              /* Create a backup of each destination directory in move mode,
                 but not in copy mode.  FIXME: it might make sense to add an
                 option to suppress backup creation also for move mode.
                 That would let one use mv to merge new content into an
                 existing hierarchy.  */
              && (x->move_ಬಹುಲಕ || ! S_ISDIR (dst_sb.st_ಬಹುಲಕ)))
            {
              /* Fail if creating the backup file would likely destroy
                 the source file.  Otherwise, the commands:
                 cd /tmp; rm -f a a~; : > a; echo A > a~; cp --b=simple a~ a
                 would leave two zero-length files: a and a~.  */
              ಆದರೆ (x->backup_type != numbered_backups
                  && ಮೂಲ_is_dst_backup (srcbase, &src_sb,
                                           dst_dirfd, dst_relname))
                {
                  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fmt;
                  fmt = (x->move_ಬಹುಲಕ
                 ? _("backing up %s might destroy source;  %s not moved")
                 : _("backing up %s might destroy source;  %s not copied"));
                  error (0, 0, fmt,
                         quoteaf_n (0, dst_name),
                         quoteaf_n (1, src_name));
                  ಹಿಂತಿರುಗಿ false;
                }

              ಅಕ್ಷರ *tmp_backup = backup_file_rename (dst_dirfd, dst_relname,
                                                     x->backup_type);

              /* FIXME: use fts:
                 Using alloca for a file name that may be arbitrarily
                 long is not recommended.  In fact, even forming such a name
                 should be discouraged.  Eventually, this code will be rewritten
                 to use fts, so using alloca here will be less of a problem.  */
              ಆದರೆ (tmp_backup)
                {
                  idx_t dirlen = dst_relname - dst_name;
                  idx_t backupsize = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (tmp_backup) + 1;
                  dst_backup = alloca (dirlen + backupsize);
                  ಸ್ಮರಣೆ_ನಕಲು (mempcpy (dst_backup, dst_name, dirlen),
                          tmp_backup, backupsize);
                  ಬಿಡುಗಡೆ (tmp_backup);
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != ENOENT)
                {
                  error (0, errno, _("cannot backup %s"), quoteaf (dst_name));
                  ಹಿಂತಿರುಗಿ false;
                }
              new_dst = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! S_ISDIR (dst_sb.st_ಬಹುಲಕ)
                   /* Never unlink dst_name when in move mode.  */
                   && ! x->move_ಬಹುಲಕ
                   && (x->unlink_dest_before_opening
                       || (x->ದತ್ತಾಂಶ_copy_required
                           && ((x->preserve_links && 1 < dst_sb.st_nlink)
                               || (x->dereference == DEREF_NEVER
                                   && ! S_ISREG (src_sb.st_ಬಹುಲಕ))))
                      ))
            {
              ಆದರೆ (unlinkat (dst_dirfd, dst_relname, 0) != 0 && errno != ENOENT)
                {
                  error (0, errno, _("cannot remove %s"), quoteaf (dst_name));
                  ಹಿಂತಿರುಗಿ false;
                }
              new_dst = true;
              ಆದರೆ (x->verbose)
                ಬರೆ (_("removed %s\n"), quoteaf (dst_name));
            }
        }
    }

  /* Ensure we don't try to copy through a symlink that was
     created by a prior call to this function.  */
  ಆದರೆ (command_ಸಾಲು_arg
      && x->dest_info
      && ! x->move_ಬಹುಲಕ
      && x->backup_type == no_backups)
    {
      /* If we did not follow symlinks above, good: use that data.
         Otherwise, use AT_SYMLINK_NOFOLLOW, in case dst_name is a symlink.  */
      ರಚನೆ stat tmp_buf;
      ರಚನೆ stat *dst_lstat_sb
        = (have_dst_lstat ? &dst_sb
           : fstatat (dst_dirfd, drelname, &tmp_buf, AT_SYMLINK_NOFOLLOW) < 0
           ? ಖಾಲಿ : &tmp_buf);

      /* Never copy through a symlink we've just created.  */
      ಆದರೆ (dst_lstat_sb
          && S_ISLNK (dst_lstat_sb->st_ಬಹುಲಕ)
          && ನೋಡಿದ_file (x->dest_info, dst_relname, dst_lstat_sb))
        {
          error (0, 0,
                 _("will not copy %s through just-created symlink %s"),
                 quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  /* If the source is a directory, we don't always create the destination
     directory.  So --verbose should not announce anything until we're
     sure we'll create a directory.  Also don't announce yet when moving
     so we can distinguish renames versus copies.  */
  ಆದರೆ (x->verbose && !x->move_ಬಹುಲಕ && !S_ISDIR (src_ಬಹುಲಕ))
    emit_verbose ("%s -> %s", src_name, dst_name, dst_backup);

  /* Associate the destination file name with the source device and inode
     so that if we encounter a matching dev/ino pair in the source tree
     we can arrange to create a hard link between the corresponding names
     in the destination tree.

     When using the --link (-l) option, there is no need to take special
     measures, because (barring race conditions) files that are hard-linked
     in the source tree will also be hard-linked in the destination tree.

     Sometimes, when preserving links, we have to record dev/ino even
     though st_nlink == 1:
     - when in move_mode, since we may be moving a group of N hard-linked
        files (via two or more command line arguments) to a different
        partition; the links may be distributed among the command line
        arguments (possibly hierarchies) so that the link count of
        the final, once-linked source file is reduced to 1 when it is
        considered below.  But in this case (for mv) we don't need to
        incur the expense of recording the dev/ino => name mapping; all we
        really need is a lookup, to see if the dev/ino pair has already
        been copied.
     - when using -H and processing a command line argument;
        that command line argument could be a symlink pointing to another
        command line argument.  With 'cp -H --preserve=link', we hard-link
        those two destination files.
     - likewise for -L except that it applies to all files, not just
        command line arguments.

     Also, with --recursive, record dev/ino of each command-line directory.
     We'll use that info to detect this problem: cp -R dir dir.  */

  ಆದರೆ (rename_errno == 0 || x->exchange)
    earlier_file = ಖಾಲಿ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->recursive && S_ISDIR (src_ಬಹುಲಕ))
    {
      ಆದರೆ (command_ಸಾಲು_arg)
        earlier_file = remember_copied (dst_relname,
                                        src_sb.st_ino, src_sb.st_dev);
      ಇಲ್ಲದಿದ್ದರೆ
        earlier_file = src_ಗಮ್ಯ_dest_lookup (src_sb.st_ino, src_sb.st_dev);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->move_ಬಹುಲಕ && src_sb.st_nlink == 1)
    {
      earlier_file = src_ಗಮ್ಯ_dest_lookup (src_sb.st_ino, src_sb.st_dev);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->preserve_links
           && !x->hard_link
           && (1 < src_sb.st_nlink
               || (command_ಸಾಲು_arg
                   && x->dereference == DEREF_COMMAND_ಸಾಲು_ARGUMENTS)
               || x->dereference == DEREF_ALWAYS))
    {
      earlier_file = remember_copied (dst_relname,
                                      src_sb.st_ino, src_sb.st_dev);
    }

  /* Did we copy this inode somewhere else (in this command line argument)
     and therefore this is a second hard link to the inode?  */

  ಆದರೆ (earlier_file)
    {
      /* Avoid damaging the destination file system by refusing to preserve
         hard-linked directories (which are found at least in Netapp snapshot
         directories).  */
      ಆದರೆ (S_ISDIR (src_ಬಹುಲಕ))
        {
          /* If src_name and earlier_file refer to the same directory entry,
             then warn about copying a directory into itself.  */
          ಆದರೆ (same_nameat (AT_FDCWD, src_name, dst_dirfd, earlier_file))
            {
              error (0, 0, _("cannot copy a directory, %s, into itself, %s"),
                     quoteaf_n (0, ತುದಿ_level_src_name),
                     quoteaf_n (1, ತುದಿ_level_dst_name));
              *copy_into_self = true;
              ಹೋಗು un_backup;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (same_nameat (dst_dirfd, dst_relname,
                                dst_dirfd, earlier_file))
            {
              error (0, 0, _("warning: source directory %s "
                             "specified more than once"),
                     quoteaf (ತುದಿ_level_src_name));
              /* In move mode, if a previous rename succeeded, then
                 we won't be in this path as the source is missing.  If the
                 rename previously failed, then that has been handled, so
                 pretend this attempt succeeded so the source isn't removed.  */
              ಆದರೆ (x->move_ಬಹುಲಕ && rename_succeeded)
                *rename_succeeded = true;
              /* We only do backups in move mode, and for non directories.
                 So just ignore this repeated entry.  */
              ಹಿಂತಿರುಗಿ true;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->dereference == DEREF_ALWAYS
                   || (command_ಸಾಲು_arg
                       && x->dereference == DEREF_COMMAND_ಸಾಲು_ARGUMENTS))
            {
              /* This happens when e.g., encountering a directory for the
                 second or subsequent time via symlinks when cp is invoked
                 with -R and -L.  E.g.,
                 rm -rf a b c d; mkdir a b c d; ln -s ../c a; ln -s ../c b;
                 cp -RL a b d
              */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ *earlier = subst_suffix (dst_name, dst_relname,
                                            earlier_file);
              error (0, 0, _("will not create hard link %s to directory %s"),
                     quoteaf_n (0, dst_name), quoteaf_n (1, earlier));
              ಬಿಡುಗಡೆ (earlier);
              ಹೋಗು un_backup;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (! create_hard_link (ಖಾಲಿ, dst_dirfd, earlier_file,
                                  dst_name, dst_dirfd, dst_relname,
                                  true, x->verbose, dereference))
            ಹೋಗು un_backup;

          ಹಿಂತಿರುಗಿ true;
        }
    }

  ಆದರೆ (x->move_ಬಹುಲಕ)
    {
      ಆದರೆ (rename_errno == EEXIST)
        rename_errno = ((renameatu (AT_FDCWD, src_name, dst_dirfd, drelname,
                                    x->exchange ? RENAME_EXCHANGE : 0)
                         == 0)
                        ? 0 : errno);

      ಆದರೆ (rename_errno == 0)
        {
          ಆದರೆ (x->verbose)
            emit_verbose (x->exchange
                          ? _("exchanged %s <-> %s")
                          : _("renamed %s -> %s"),
                          src_name, dst_name, dst_backup);

          ಆದರೆ (x->ಗಣ_security_context)
            {
              /* -Z failures are only warnings currently.  */
              (ಶೂನ್ಯ) ಗಣ_file_security_ctx (dst_name, true, x);
            }

          ಆದರೆ (rename_succeeded)
            *rename_succeeded = true;

          ಆದರೆ (command_ಸಾಲು_arg && !x->ಕೊನೆಯ_file)
            {
              /* Record destination dev/ino/name, so that if we are asked
                 to overwrite that file again, we can detect it and fail.  */
              /* It's fine to use the _source_ stat buffer (src_sb) to get the
                 _destination_ dev/ino, since the rename above can't have
                 changed those, and 'mv' always uses lstat.
                 We could limit it further by operating
                 only on non-directories when !x->exchange.  */
              ದಾಖಲೆ_file (x->dest_info, dst_relname, &src_sb);
            }

          ಹಿಂತಿರುಗಿ true;
        }

      /* FIXME: someday, consider what to do when moving a directory into
         itself but when source and destination are on different devices.  */

      /* This happens when attempting to rename a directory to a
         subdirectory of itself.  */
      ಆದರೆ (rename_errno == EINVAL)
        {
          /* FIXME: this is a little fragile in that it relies on rename(2)
             failing with a specific errno value.  Expect problems on
             non-POSIX systems.  */
          error (0, 0, _("cannot move %s to a subdirectory of itself, %s"),
                 quoteaf_n (0, ತುದಿ_level_src_name),
                 quoteaf_n (1, ತುದಿ_level_dst_name));

          /* Note that there is no need to call forget_created here,
             (compare with the other calls in this file) since the
             destination directory didn't exist before.  */

          *copy_into_self = true;
          /* FIXME-cleanup: Don't return true here; adjust mv.c accordingly.
             The only caller that uses this code (mv.c) ends up setting its
             exit status to nonzero when copy_into_self is nonzero.  */
          ಹಿಂತಿರುಗಿ true;
        }

      /* WARNING: there probably exist systems for which an inter-device
         rename fails with a value of errno not handled here.
         If/as those are reported, add them to the condition below.
         If this happens to you, please do the following and send the output
         to the bug-reporting address (e.g., in the output of cp --help):
           touch k; perl -e 'rename "k","/tmp/k" or print "$!(",$!+0,")\n"'
         where your current directory is on one partition and /tmp is the other.
         Also, please try to find the E* errno macro name corresponding to
         the diagnostic and parenthesized integer, and include that in your
         e-mail.  One way to do that is to run a command like this
           find /usr/include/. -type f \
             | xargs grep 'define.*\<E[A-Z]*\>.*\<18\>' /dev/null
         where you'd replace '18' with the integer in parentheses that
         was output from the perl one-liner above.
         If necessary, of course, change '/tmp' to some other directory.  */
      ಆದರೆ (rename_errno != EXDEV || x->no_copy || x->exchange)
        {
          /* There are many ways this can happen due to a race condition.
             When something happens between the initial follow_fstatat and the
             subsequent rename, we can get many different types of errors.
             For example, if the destination is initially a non-directory
             or non-existent, but it is created as a directory, the rename
             fails.  If two 'mv' commands try to rename the same file at
             about the same time, one will succeed and the other will fail.
             If the permissions on the directory containing the source or
             destination file are made too restrictive, the rename will
             fail.  Etc.  */
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *quoted_dst_name = quoteaf_n (1, dst_name);
          ಆದರೆ (x->exchange)
            error (0, rename_errno, _("cannot exchange %s and %s"),
                   quoteaf_n (0, src_name), quoted_dst_name);
          ಇಲ್ಲದಿದ್ದರೆ
            ಆಯ್ಕೆ (rename_errno)
              {
              ಸ್ಥಿತಿ EDQUOT: ಸ್ಥಿತಿ EEXIST: ಸ್ಥಿತಿ EISDIR: ಸ್ಥಿತಿ EMLINK:
              ಸ್ಥಿತಿ ENOSPC: ಸ್ಥಿತಿ ETXTBSY:
#ಆದರೆ ENOTEMPTY != EEXIST
              ಸ್ಥಿತಿ ENOTEMPTY:
#ಆದರೆ_ಅಂತ್ಯ
                /* The destination must be the problem.  Don't mention
                   the source as that is more likely to confuse the user
                   than be helpful.  */
                error (0, rename_errno, _("cannot overwrite %s"),
                       quoted_dst_name);
                ನಿಲ್ಲಿಸು;

              ಪೂರ್ವನಿಯೋಜಿತ:
                error (0, rename_errno, _("cannot move %s to %s"),
                       quoteaf_n (0, src_name), quoted_dst_name);
                ನಿಲ್ಲಿಸು;
              }
          forget_created (src_sb.st_ino, src_sb.st_dev);
          ಹಿಂತಿರುಗಿ false;
        }

      /* The rename attempt has failed.  Remove any existing destination
         file so that a cross-device 'mv' acts as if it were really using
         the rename syscall.  Note both src and dst must both be directories
         or not, and this is enforced above.  Therefore we check the src_mode
         and operate on dst_name here as a tighter constraint and also because
         src_mode is readily available here.  */
      ಆದರೆ ((unlinkat (dst_dirfd, drelname,
                     S_ISDIR (src_ಬಹುಲಕ) ? AT_REMOVEDIR : 0)
           != 0)
          && errno != ENOENT)
        {
          error (0, errno,
             _("inter-device move failed: %s to %s; unable to remove target"),
                 quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
          forget_created (src_sb.st_ino, src_sb.st_dev);
          ಹಿಂತಿರುಗಿ false;
        }

      ಆದರೆ (x->verbose && !S_ISDIR (src_ಬಹುಲಕ))
        emit_verbose (_("copied %s -> %s"), src_name, dst_name, dst_backup);
      new_dst = true;
    }

  /* If the ownership might change, or if it is a directory (whose
     special mode bits may change after the directory is created),
     omit some permissions at first, so unauthorized users cannot nip
     in before the file is ready.  */
  dst_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು = (x->ಗಣ_ಬಹುಲಕ ? x->ಬಹುಲಕ_2 : src_ಬಹುಲಕ) & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು;
  omitted_permissions =
    (dst_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು
     & (x->preserve_ownership ? S_IRWXG | S_IRWXO
        : S_ISDIR (src_ಬಹುಲಕ) ? S_IWGRP | S_IWOTH
        : 0));

  delayed_ok = true;

  /* If required, set the default security context for new files.
     Also for existing files this is used as a reference
     when copying the context with --preserve=context.
     FIXME: Do we need to consider dst_mode_bits here?  */
  ಆದರೆ (! ಗಣ_process_security_ctx (src_name, dst_name, src_ಬಹುಲಕ, new_dst, x))
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (S_ISDIR (src_ಬಹುಲಕ))
    {
      ರಚನೆ dir_list *dir;

      /* If this directory has been copied before during the
         recursion, there is a symbolic link to an ancestor
         directory of the symbolic link.  It is impossible to
         continue to copy this, unless we've got an infinite file system.  */

      ಆದರೆ (is_ancestor (&src_sb, ancestors))
        {
          error (0, 0, _("cannot copy cyclic symbolic link %s"),
                 quoteaf (src_name));
          ಹೋಗು un_backup;
        }

      /* Insert the current directory in the list of parents.  */

      dir = alloca (ಗಾತ್ರ *dir);
      dir->ಪೋಷಕ = ancestors;
      dir->st_ino = src_sb.st_ino;
      dir->st_dev = src_sb.st_dev;

      ಆದರೆ (new_dst || !S_ISDIR (dst_sb.st_ಬಹುಲಕ))
        {
          /* POSIX says mkdir's behavior is implementation-defined when
             (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
             to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
             decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
          mode_t ಬಹುಲಕ_2 = dst_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು & ~omitted_permissions;
          ಆದರೆ (mkdirat (dst_dirfd, drelname, ಬಹುಲಕ_2) != 0)
            {
              error (0, errno, _("cannot create directory %s"),
                     quoteaf (dst_name));
              ಹೋಗು un_backup;
            }

          /* We need search and write permissions to the new directory
             for writing the directory's contents. Check if these
             permissions are there.  */

          ಆದರೆ (fstatat (dst_dirfd, drelname, &dst_sb, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, errno, _("cannot stat %s"), quoteaf (dst_name));
              ಹೋಗು un_backup;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((dst_sb.st_ಬಹುಲಕ & S_IRWXU) != S_IRWXU)
            {
              /* Make the new directory searchable and writable.  */

              dst_ಬಹುಲಕ = dst_sb.st_ಬಹುಲಕ;
              restore_dst_ಬಹುಲಕ = true;

              ಆದರೆ (lchmodat (dst_dirfd, drelname, dst_ಬಹುಲಕ | S_IRWXU) != 0)
                {
                  error (0, errno, _("setting permissions for %s"),
                         quoteaf (dst_name));
                  ಹೋಗು un_backup;
                }
            }

          /* Record the created directory's inode and device numbers into
             the search structure, so that we can avoid copying it again.
             Do this only for the first directory that is created for each
             source command line argument.  */
          ಆದರೆ (!*ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg)
            {
              remember_copied (dst_relname, dst_sb.st_ino, dst_sb.st_dev);
              *ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg = true;
            }

          ಆದರೆ (x->verbose)
            {
              ಆದರೆ (x->move_ಬಹುಲಕ)
                ಬರೆ (_("created directory %s\n"), quoteaf (dst_name));
              ಇಲ್ಲದಿದ್ದರೆ
                emit_verbose ("%s -> %s", src_name, dst_name, ಖಾಲಿ);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          omitted_permissions = 0;

          /* For directories, the process global context could be reset for
             descendants, so use it to set the context for existing dirs here.
             This will also give earlier indication of failure to set ctx.  */
          ಆದರೆ (x->ಗಣ_security_context || x->preserve_security_context)
            ಆದರೆ (! ಗಣ_file_security_ctx (dst_name, false, x))
              {
                ಆದರೆ (x->require_preserve_context)
                  ಹೋಗು un_backup;
              }
        }

      /* Decide whether to copy the contents of the directory.  */
      ಆದರೆ (x->one_file_system && ಪೋಷಕ && ಪೋಷಕ->st_dev != src_sb.st_dev)
        {
          /* Here, we are crossing a file system boundary and cp's -x option
             is in effect: so don't copy the contents of this directory. */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Copy the contents of the directory.  Don't just return if
             this fails -- otherwise, the failure to read a single file
             in a source directory would cause the containing destination
             directory not to have owner/perms set properly.  */
          delayed_ok = copy_dir (src_name, dst_name, dst_dirfd, dst_relname,
                                 new_dst, &src_sb, dir, x,
                                 ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg,
                                 copy_into_self);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->symbolic_link)
    {
      dest_is_symlink = true;
      ಆದರೆ (*src_name != '/')
        {
          /* Check that DST_NAME denotes a file in the current directory.  */
          ರಚನೆ stat dot_sb;
          ರಚನೆ stat dst_ಪೋಷಕ_sb;
          ಅಕ್ಷರ *dst_ಪೋಷಕ;
          bool ಒಳ_current_dir;

          dst_ಪೋಷಕ = dir_name (dst_relname);

          ಒಳ_current_dir = ((dst_dirfd == AT_FDCWD && streq (".", dst_ಪೋಷಕ))
                            /* If either stat call fails, it's ok not to report
                               the failure and say dst_name is in the current
                               directory.  Other things will fail later.  */
                            || stat (".", &dot_sb) != 0
                            || (fstatat (dst_dirfd, dst_ಪೋಷಕ, &dst_ಪೋಷಕ_sb,
                                         0) != 0)
                            || psame_inode (&dot_sb, &dst_ಪೋಷಕ_sb));
          ಬಿಡುಗಡೆ (dst_ಪೋಷಕ);

          ಆದರೆ (! ಒಳ_current_dir)
            {
              error (0, 0,
           _("%s: can make relative symbolic links only in current directory"),
                     quotef (dst_name));
              ಹೋಗು un_backup;
            }
        }

      ಪೂರ್ಣಾಂಕ err = force_symlinkat (src_name, dst_dirfd, dst_relname,
                                 x->unlink_dest_after_failed_open, -1);
      ಆದರೆ (0 < err)
        {
          error (0, err, _("cannot create symbolic link %s to %s"),
                 quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
          ಹೋಗು un_backup;
        }
    }

  /* POSIX 2008 states that it is implementation-defined whether
     link() on a symlink creates a hard-link to the symlink, or only
     to the referent (effectively dereferencing the symlink) (POSIX
     2001 required the latter behavior, although many systems provided
     the former).  Yet cp, invoked with '--link --no-dereference',
     should not follow the link.  We can approximate the desired
     behavior by skipping this hard-link creating block and instead
     copying the symlink, via the 'S_ISLNK'- copying code below.

     Note gnulib's linkat module, guarantees that the symlink is not
     dereferenced.  However its emulation currently doesn't maintain
     timestamps or ownership so we only call it when we know the
     emulation will not be needed.  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->hard_link
           && !(! CAN_HARDLINK_SYMLINKS && S_ISLNK (src_ಬಹುಲಕ)
                && x->dereference == DEREF_NEVER))
    {
      bool replace = (x->unlink_dest_after_failed_open
                      || x->interactive == I_ASK_USER);
      ಆದರೆ (! create_hard_link (src_name, AT_FDCWD, src_name,
                              dst_name, dst_dirfd, dst_relname,
                              replace, false, dereference))
        ಹೋಗು un_backup;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG (src_ಬಹುಲಕ)
           || (x->copy_as_regular && !S_ISLNK (src_ಬಹುಲಕ)))
    {
      copied_as_regular = true;
      /* POSIX says the permission bits of the source file must be
         used as the 3rd argument in the open call.  Historical
         practice passed all the source mode bits to 'open', but the extra
         bits were ignored, so it should be the same either way.

         This call uses DST_MODE_BITS, not SRC_MODE.  These are
         normally the same, and the exception (where x->set_mode) is
         used only by 'install', which POSIX does not specify and
         where DST_MODE_BITS is what's wanted.  */
      ಆದರೆ (! copy_reg (src_name, dst_name, dst_dirfd, dst_relname,
                      x, dst_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು & S_IRWXUGO,
                      omitted_permissions, &new_dst, &src_sb))
        ಹೋಗು un_backup;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISFIFO (src_ಬಹುಲಕ))
    {
      /* Use mknodat, rather than mkfifoat, because the former preserves
         the special mode bits of a fifo on Solaris 10, while mkfifoat
         does not.  But fall back on mkfifoat, because on some BSD systems,
         mknodat always fails when asked to create a FIFO.  */
      mode_t ಬಹುಲಕ_2 = src_ಬಹುಲಕ & ~omitted_permissions;
      ಆದರೆ (mknodat (dst_dirfd, dst_relname, ಬಹುಲಕ_2, 0) != 0)
        ಆದರೆ (mkfifoat (dst_dirfd, dst_relname, ಬಹುಲಕ_2 & ~S_IFIFO) != 0)
          {
            error (0, errno, _("cannot create fifo %s"), quoteaf (dst_name));
            ಹೋಗು un_backup;
          }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISBLK (src_ಬಹುಲಕ) || S_ISCHR (src_ಬಹುಲಕ) || S_ISSOCK (src_ಬಹುಲಕ))
    {
      mode_t ಬಹುಲಕ_2 = src_ಬಹುಲಕ & ~omitted_permissions;
      ಆದರೆ (mknodat (dst_dirfd, dst_relname, ಬಹುಲಕ_2, src_sb.st_rdev) != 0)
        {
          error (0, errno, _("cannot create special file %s"),
                 quoteaf (dst_name));
          ಹೋಗು un_backup;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISLNK (src_ಬಹುಲಕ))
    {
      ಅಕ್ಷರ *src_link_val = areadlink_with_ಗಾತ್ರ (src_name, src_sb.st_ಗಾತ್ರ);
      dest_is_symlink = true;
      ಆದರೆ (src_link_val == ಖಾಲಿ)
        {
          error (0, errno, _("cannot read symbolic link %s"),
                 quoteaf (src_name));
          ಹೋಗು un_backup;
        }

      ಪೂರ್ಣಾಂಕ symlink_err = force_symlinkat (src_link_val, dst_dirfd, dst_relname,
                                         x->unlink_dest_after_failed_open, -1);
      ಆದರೆ (0 < symlink_err && x->update == UPDATE_OLDER
          && !new_dst && S_ISLNK (dst_sb.st_ಬಹುಲಕ)
          && dst_sb.st_ಗಾತ್ರ == ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (src_link_val))
        {
          /* See if the destination is already the desired symlink.
             FIXME: This behavior isn't documented, and seems wrong
             in some cases, e.g., if the destination symlink has the
             wrong ownership, permissions, or timestamps.  */
          ಅಕ್ಷರ *dest_link_val =
            areadlinkat_with_ಗಾತ್ರ (dst_dirfd, dst_relname, dst_sb.st_ಗಾತ್ರ);
          ಆದರೆ (dest_link_val)
            {
              ಆದರೆ (streq (dest_link_val, src_link_val))
                symlink_err = 0;
              ಬಿಡುಗಡೆ (dest_link_val);
            }
        }
      ಬಿಡುಗಡೆ (src_link_val);
      ಆದರೆ (0 < symlink_err)
        {
          error (0, symlink_err, _("cannot create symbolic link %s"),
                 quoteaf (dst_name));
          ಹೋಗು un_backup;
        }

      ಆದರೆ (x->preserve_security_context)
        restore_default_fscreatecon_or_die ();

      ಆದರೆ (x->preserve_ownership)
        {
          /* Preserve the owner and group of the just-'copied'
             symbolic link, if possible.  */
          ಆದರೆ (HAVE_LCHOWN
              && (lchownat (dst_dirfd, dst_relname,
                            src_sb.st_uid, src_sb.st_gid)
                  != 0)
              && ! chown_failure_ok (x))
            {
              error (0, errno, _("failed to preserve ownership for %s"),
                     dst_name);
              ಆದರೆ (x->require_preserve)
                ಹೋಗು un_backup;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Can't preserve ownership of symlinks.
                 FIXME: maybe give a warning or even error for symlinks
                 in directories with the sticky bit set -- there, not
                 preserving owner/group is a potential security problem.  */
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      error (0, 0, _("%s has unknown file type"), quoteaf (src_name));
      ಹೋಗು un_backup;
    }

  /* With -Z or --preserve=context, set the context for existing files.
     Note this is done already for copy_reg() for reasons described therein.  */
  ಆದರೆ (!new_dst && !x->copy_as_regular && !S_ISDIR (src_ಬಹುಲಕ)
      && (x->ಗಣ_security_context || x->preserve_security_context))
    {
      ಆದರೆ (! ಗಣ_file_security_ctx (dst_name, false, x))
        {
           ಆದರೆ (x->require_preserve_context)
             ಹೋಗು un_backup;
        }
    }

  ಆದರೆ (command_ಸಾಲು_arg && x->dest_info)
    {
      /* Now that the destination file is very likely to exist,
         add its info to the set.  */
      ರಚನೆ stat sb;
      ಆದರೆ (fstatat (dst_dirfd, drelname, &sb, AT_SYMLINK_NOFOLLOW) == 0)
        ದಾಖಲೆ_file (x->dest_info, dst_relname, &sb);
    }

  /* If we've just created a hard-link due to cp's --link option,
     we're done.  */
  ಆದರೆ (x->hard_link && ! S_ISDIR (src_ಬಹುಲಕ)
      && !(! CAN_HARDLINK_SYMLINKS && S_ISLNK (src_ಬಹುಲಕ)
           && x->dereference == DEREF_NEVER))
    ಹಿಂತಿರುಗಿ delayed_ok;

  ಆದರೆ (copied_as_regular)
    ಹಿಂತಿರುಗಿ delayed_ok;

  /* POSIX says that 'cp -p' must restore the following:
     - permission bits
     - setuid, setgid bits
     - owner and group
     If it fails to restore any of those, we may give a warning but
     the destination must not be removed.
     FIXME: implement the above. */

  /* Adjust the times (and if possible, ownership) for the copy.
     chown turns off set[ug]id bits for non-root,
     so do the chmod last.  */

  ಆದರೆ (x->preserve_timestamps)
    {
      ರಚನೆ timespec timespec[2];
      timespec[0] = get_stat_atime (&src_sb);
      timespec[1] = get_stat_mtime (&src_sb);

      ಪೂರ್ಣಾಂಕ utimensat_flags = dest_is_symlink ? AT_SYMLINK_NOFOLLOW : 0;
      ಆದರೆ (utimensat (dst_dirfd, drelname, timespec, utimensat_flags) != 0)
        {
          error (0, errno, _("preserving times for %s"), quoteaf (dst_name));
          ಆದರೆ (x->require_preserve)
            ಹಿಂತಿರುಗಿ false;
        }
    }

  /* Avoid calling chown if we know it's not necessary.  */
  ಆದರೆ (!dest_is_symlink && x->preserve_ownership
      && (new_dst || !SAME_OWNER_AND_ಗುಂಪು (src_sb, dst_sb)))
    {
      ಆಯ್ಕೆ (ಗಣ_owner (x, dst_name, dst_dirfd, drelname, -1,
                         &src_sb, new_dst, &dst_sb))
        {
        ಸ್ಥಿತಿ -1:
          ಹಿಂತಿರುಗಿ false;

        ಸ್ಥಿತಿ 0:
          src_ಬಹುಲಕ &= ~ (S_ISUID | S_ISGID | S_ISVTX);
          ನಿಲ್ಲಿಸು;
        }
    }

  /* Set xattrs after ownership as changing owners will clear capabilities.  */
  ಆದರೆ (x->preserve_xattr && ! copy_attr (src_name, -1, dst_name, -1, x)
      && x->require_preserve_xattr && ! x->move_ಬಹುಲಕ)
    ಹಿಂತಿರುಗಿ false;

  /* The operations beyond this point may dereference a symlink.  */
  ಆದರೆ (dest_is_symlink)
    ಹಿಂತಿರುಗಿ delayed_ok;

  ಗಣ_author (dst_name, -1, &src_sb);

  ಆದರೆ (x->preserve_ಬಹುಲಕ || x->move_ಬಹುಲಕ)
    {
      ಆದರೆ (xcopy_acl (src_name, -1, dst_name, -1, src_ಬಹುಲಕ) != 0
          && x->require_preserve)
        ಹಿಂತಿರುಗಿ false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->ಗಣ_ಬಹುಲಕ)
    {
      ಆದರೆ (xset_acl (dst_name, -1, x->ಬಹುಲಕ_2) != 0)
        ಹಿಂತಿರುಗಿ false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->explicit_no_preserve_ಬಹುಲಕ && new_dst)
    {
      ಪೂರ್ಣಾಂಕ default_permissions = S_ISDIR (src_ಬಹುಲಕ) || S_ISSOCK (src_ಬಹುಲಕ)
                                ? S_IRWXUGO : ಬಹುಲಕ_RW_UGO;
      dst_ಬಹುಲಕ = dst_sb.st_ಬಹುಲಕ;
      ಆದರೆ (S_ISDIR (src_ಬಹುಲಕ))  /* Keep set-group-ID for directories.  */
        default_permissions |= (dst_ಬಹುಲಕ & S_ISGID);
      ಆದರೆ (xset_acl (dst_name, -1, default_permissions & ~cached_umask ()) != 0)
        ಹಿಂತಿರುಗಿ false;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (omitted_permissions)
        {
          omitted_permissions &= ~ cached_umask ();

          ಆದರೆ (omitted_permissions && !restore_dst_ಬಹುಲಕ)
            {
              /* Permissions were deliberately omitted when the file
                 was created due to security concerns.  See whether
                 they need to be re-added now.  It'd be faster to omit
                 the lstat, but deducing the current destination mode
                 is tricky in the presence of implementation-defined
                 rules for special mode bits.  */
              ಆದರೆ (new_dst && (fstatat (dst_dirfd, drelname, &dst_sb,
                                       AT_SYMLINK_NOFOLLOW)
                              != 0))
                {
                  error (0, errno, _("cannot stat %s"), quoteaf (dst_name));
                  ಹಿಂತಿರುಗಿ false;
                }
              dst_ಬಹುಲಕ = dst_sb.st_ಬಹುಲಕ;
              ಆದರೆ (omitted_permissions & ~dst_ಬಹುಲಕ)
                restore_dst_ಬಹುಲಕ = true;
            }
        }

      ಆದರೆ (restore_dst_ಬಹುಲಕ)
        {
          ಆದರೆ (lchmodat (dst_dirfd, drelname, dst_ಬಹುಲಕ | omitted_permissions)
              != 0)
            {
              error (0, errno, _("preserving permissions for %s"),
                     quoteaf (dst_name));
              ಆದರೆ (x->require_preserve)
                ಹಿಂತಿರುಗಿ false;
            }
        }
    }

  ಹಿಂತಿರುಗಿ delayed_ok;

un_backup:

  ಆದರೆ (x->preserve_security_context)
    restore_default_fscreatecon_or_die ();

  /* We have failed to create the destination file.
     If we've just added a dev/ino entry via the remember_copied
     call above (i.e., unless we've just failed to create a hard link),
     remove the entry associating the source dev/ino with the
     destination file name, so we don't try to 'preserve' a link
     to a file we didn't create.  */
  ಆದರೆ (earlier_file == ಖಾಲಿ)
    forget_created (src_sb.st_ino, src_sb.st_dev);

  ಆದರೆ (dst_backup)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relbackup = &dst_backup[dst_relname - dst_name];
      ಆದರೆ (renameat (dst_dirfd, dst_relbackup, dst_dirfd, drelname) != 0)
        error (0, errno, _("cannot un-backup %s"), quoteaf (dst_name));
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (x->verbose)
            ಬರೆ (_("%s -> %s (unbackup)\n"),
                    quoteaf_n (0, dst_backup), quoteaf_n (1, dst_name));
        }
    }
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
valid_options (ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *co)
{
  affirm (VALID_BACKUP_TYPE (co->backup_type));
  affirm (VALID_SPARSE_ಬಹುಲಕ (co->sparse_ಬಹುಲಕ));
  affirm (VALID_REFLINK_ಬಹುಲಕ (co->reflink_ಬಹುಲಕ));
  affirm (!(co->hard_link && co->symbolic_link));
  affirm (!
          (co->reflink_ಬಹುಲಕ == REFLINK_ALWAYS
           && co->sparse_ಬಹುಲಕ != SPARSE_AUTO));
}

/* Copy the file SRC_NAME to the file DST_NAME aka DST_DIRFD+DST_RELNAME.
   If NONEXISTENT_DST is positive, DST_NAME does not exist even as a
   dangling symlink; if negative, it does not exist except possibly
   as a dangling symlink; if zero, its existence status is unknown.
   OPTIONS summarizes the command-line options.
   Set *COPY_INTO_SELF if SRC_NAME is a parent of (or the
   same as) DST_NAME; otherwise, set clear it.
   If X->move_mode, set *RENAME_SUCCEEDED according to whether
   the source was simply renamed to the destination.
   Return true if successful.  */

ಬಾಹ್ಯ bool
copy (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_name,
      ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
      ಪೂರ್ಣಾಂಕ nonexistent_dst, ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *options,
      bool *copy_into_self, bool *rename_succeeded)
{
  valid_options (options);

  /* Record the file names: they're used in case of error, when copying
     a directory into itself.  I don't like to make these tools do *any*
     extra work in the common case when that work is solely to handle
     exceptional cases, but in this case, I don't see a way to derive the
     top level source and destination directory names where they're used.
     An alternative is to use COPY_INTO_SELF and print the diagnostic
     from every caller -- but I don't want to do that.  */
  ತುದಿ_level_src_name = src_name;
  ತುದಿ_level_dst_name = dst_name;

  bool ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg = false;
  ಹಿಂತಿರುಗಿ copy_ಆಂತರಿಕ (src_name, dst_name, dst_dirfd, dst_relname,
                        nonexistent_dst, ಖಾಲಿ, ಖಾಲಿ,
                        options, true,
                        &ಮೊದಲ_dir_created_ಪ್ರತಿ_command_ಸಾಲು_arg,
                        copy_into_self, rename_succeeded);
}

/* Set *X to the default options for a value of type struct cp_options.  */

ಬಾಹ್ಯ ಶೂನ್ಯ
cp_options_default (ರಚನೆ cp_options *x)
{
  memset (x, 0, ಗಾತ್ರ *x);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PRIV_FILE_CHOWN
  {
    priv_ಗಣ_t *pset = priv_allocset ();
    ಆದರೆ (!pset)
      xalloc_die ();
    ಆದರೆ (getppriv (PRIV_EFFECTIVE, pset) == 0)
      {
        x->chown_privileges = priv_ismember (pset, PRIV_FILE_CHOWN);
        x->owner_privileges = priv_ismember (pset, PRIV_FILE_OWNER);
      }
    priv_freeset (pset);
  }
#ಇಲ್ಲದಿದ್ದರೆ
  x->chown_privileges = x->owner_privileges = (geteuid () == ಬೇರು_UID);
#ಆದರೆ_ಅಂತ್ಯ
  x->rename_errno = -1;
}

/* Return true if it's OK for chown to fail, where errno is
   the error number that chown failed with and X is the copying
   option set.  */

ಬಾಹ್ಯ bool
chown_failure_ok (ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x)
{
  /* If non-root uses -p, it's ok if we can't preserve ownership.
     But root probably wants to know, e.g. if NFS disallows it,
     or if the target system doesn't support file ownership.

     Treat EACCES like EPERM and EINVAL to work around a bug in Linux
     CIFS <https://bugs.gnu.org/65599>.  Although this means coreutils
     will ignore EACCES errors that it should report, problems should
     occur only when some other process is racing with coreutils and
     coreutils is not immune to races anyway.  */

  ಹಿಂತಿರುಗಿ ((errno == EPERM || errno == EINVAL || errno == EACCES)
          && !x->chown_privileges);
}

/* Similarly, return true if it's OK for chmod and similar operations
   to fail, where errno is the error number that chmod failed with and
   X is the copying option set.  */

ಸ್ಥಿರ bool
owner_failure_ok (ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x)
{
  ಹಿಂತಿರುಗಿ ((errno == EPERM || errno == EINVAL || errno == EACCES)
          && !x->owner_privileges);
}

/* Return the user's umask, caching the result.

   FIXME: If the destination's parent directory has has a default ACL,
   some operating systems (e.g., GNU/Linux's "POSIX" ACLs) use that
   ACL's mask rather than the process umask.  Currently, the callers
   of cached_umask incorrectly assume that this situation cannot occur.  */
ಬಾಹ್ಯ mode_t
cached_umask (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ mode_t ಮುಖವಾಡ;
  ಸ್ಥಿರ bool cached;
  ಆದರೆ (!cached)
    {
      cached = true;
      ಮುಖವಾಡ = umask (0);
      umask (ಮುಖವಾಡ);
    }
  ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}
