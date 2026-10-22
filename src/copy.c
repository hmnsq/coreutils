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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/ioctl.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <selinux/selinux.h>

#ఒకవేళ HAVE_HURD_H
# include <hurd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_PRIV_H
# include <priv.h>
#అయితే_ముగింపు

#అన్తర్భావ "system.h"
#అన్తర్భావ "acl.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "backupfile.h"
#అన్తర్భావ "canonicalize.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "cp-hash.h"
#అన్తర్భావ "fcntl--.h"
#అన్తర్భావ "file-set.h"
#అన్తర్భావ "filemode.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "force-link.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hashcode-file.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "issymlinkat.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "renameatu.h"
#అన్తర్భావ "root-uid.h"
#అన్తర్భావ "same.h"
#అన్తర్భావ "savedir.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "utimecmp.h"
#అన్తర్భావ "utimens.h"
#అన్తర్భావ "write-any-file.h"
#అన్తర్భావ "areadlink.h"
#అన్తర్భావ "yesno.h"
#అన్తర్భావ "selinux.h"

#నిర్వచితం_కాకపోతే USE_XATTR
# define USE_XATTR false
#అయితే_ముగింపు

#ఒకవేళ USE_XATTR
# include <attr/error_context.h>
# include <attr/libattr.h>
# include <stdarg.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_LINUX_FALLOC_H
# include <linux/falloc.h>
#అయితే_ముగింపు

/* See HAVE_FALLOCATE workaround when including this file.  */
#నిర్వచితమైతే HAVE_LINUX_FS_H
# include <linux/fs.h>
#అయితే_ముగింపు

#ఒకవేళ !defined FICLONE && defined __linux__
# define FICLONE _IOW (0x94, 9, పూర్ణాంకం)
#అయితే_ముగింపు

#ఒకవేళ HAVE_FCLONEFILEAT && !USE_XATTR
# include <sys/clonefile.h>
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే USE_ACL
# define USE_ACL 0
#అయితే_ముగింపు

#నిర్వచించు SAME_OWNER(A, B) ((A).st_uid == (B).st_uid)
#నిర్వచించు SAME_సమూహం(A, B) ((A).st_gid == (B).st_gid)
#నిర్వచించు SAME_OWNER_AND_సమూహం(A, B) (SAME_OWNER (A, B) && SAME_సమూహం (A, B))

/* LINK_FOLLOWS_SYMLINKS is tri-state; if it is -1, we don't know
   how link() behaves, so assume we can't hardlink symlinks in that case.  */
#ఒకవేళ (defined HAVE_LINKAT && ! LINKAT_SYMLINK_NOTSUP) || ! LINK_FOLLOWS_SYMLINKS
# define CAN_HARDLINK_SYMLINKS 1
#లేకపోతే
# define CAN_HARDLINK_SYMLINKS 0
#అయితే_ముగింపు

నిర్మాణం dir_list
{
  నిర్మాణం dir_list *మాతృ;
  ino_t st_ino;
  dev_t st_dev;
};

/* Initial size of the cp.dest_info hash table.  */
#నిర్వచించు DEST_INFO_INITIAL_సామర్థ్యం 61

స్థిర bool copy_అంతర్గత (అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
                           పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
                           పూర్ణాంకం nonexistent_dst, నిర్మాణం stat స్థిరరాశి *మాతృ,
                           నిర్మాణం dir_list *ancestors,
                           స్థిరరాశి నిర్మాణం cp_options *x,
                           bool command_పంక్తి_arg,
                           bool *మొదటి_dir_created_చొప్పున_command_పంక్తి_arg,
                           bool *copy_into_self,
                           bool *rename_succeeded);
స్థిర bool owner_failure_ok (నిర్మాణం cp_options స్థిరరాశి *x);

/* Pointers to the file names:  they're used in the diagnostic that is issued
   when we detect the user is trying to copy a directory into itself.  */
స్థిర అక్షరం స్థిరరాశి *శిఖరం_level_src_name;
స్థిర అక్షరం స్థిరరాశి *శిఖరం_level_dst_name;

/* debug info about the last file copy.  */
స్థిర నిర్మాణం copy_debug copy_debug;

స్థిర అక్షరం స్థిరరాశి *
copy_debug_string (గణన copy_debug_val debug_val)
{
  ఎంపిక (debug_val)
    {
    స్థితి COPY_DEBUG_NO: తిరిగి "no";
    స్థితి COPY_DEBUG_YES: తిరిగి "yes";
    స్థితి COPY_DEBUG_AVOIDED: తిరిగి "avoided";
    స్థితి COPY_DEBUG_UNKNOWN: తిరిగి "unknown";

    స్థితి COPY_DEBUG_EXTERNAL:
    స్థితి COPY_DEBUG_EXTERNAL_అంతర్గత:
    స్థితి COPY_DEBUG_MAX:
    అప్రమేయ:
      ఒకవేళ (0 < debug_val)
        తిరిగి strerror (debug_val);
      affirm (false);
    }
}

స్థిర అక్షరం స్థిరరాశి *
copy_debug_sparse_string (గణన copy_debug_val debug_val)
{
  ఎంపిక (debug_val)
    {
    స్థితి COPY_DEBUG_NO: తిరిగి "no";
    స్థితి COPY_DEBUG_YES: తిరిగి "zeros";
    స్థితి COPY_DEBUG_EXTERNAL: తిరిగి "SEEK_HOLE";
    స్థితి COPY_DEBUG_EXTERNAL_అంతర్గత: తిరిగి "SEEK_HOLE + zeros";
    స్థితి COPY_DEBUG_UNKNOWN: తిరిగి "unknown";

    స్థితి COPY_DEBUG_AVOIDED:
    స్థితి COPY_DEBUG_MAX:
    అప్రమేయ: affirm (false);
    }
}

/* Print --debug output on standard output.  */
స్థిర శూన్య
emit_debug (స్థిరరాశి నిర్మాణం cp_options *x)
{
  ఒకవేళ (! x->hard_link && ! x->symbolic_link && x->దత్తాంశం_copy_required)
    రాయి ("copy offload: %s, reflink: %s, sparse detection: %s\n",
            copy_debug_string (copy_debug.offload),
            copy_debug_string (copy_debug.reflink),
            copy_debug_sparse_string (copy_debug.sparse_detection));
}

#నిర్వచితం_కాకపోతే DEV_FD_MIGHT_BE_CHR
# define DEV_FD_MIGHT_BE_CHR false
#అయితే_ముగింపు

/* Act like fstat (DIRFD, FILENAME, ST, FLAGS), except when following
   symbolic links on Solaris-like systems, treat any character-special
   device like /dev/fd/0 as if it were the file it is open on.  */
స్థిర పూర్ణాంకం
follow_fstatat (పూర్ణాంకం dirfd, అక్షరం స్థిరరాశి *filename, నిర్మాణం stat *st, పూర్ణాంకం flags)
{
  పూర్ణాంకం ఫలితం = fstatat (dirfd, filename, st, flags);

  ఒకవేళ (DEV_FD_MIGHT_BE_CHR && ఫలితం == 0 && !(flags & AT_SYMLINK_NOFOLLOW)
      && S_ISCHR (st->st_బహుళకం))
    {
      స్థిర dev_t stdin_rdev;
      స్థిర గుర్తుగల అక్షరం stdin_rdev_స్థితి;
      ఒకవేళ (stdin_rdev_స్థితి == 0)
        {
          నిర్మాణం stat stdin_st;
          ఒకవేళ (stat ("/dev/stdin", &stdin_st) == 0 && S_ISCHR (stdin_st.st_బహుళకం)
              && minor (stdin_st.st_rdev) == STDIN_FILENO)
            {
              stdin_rdev = stdin_st.st_rdev;
              stdin_rdev_స్థితి = 1;
            }
          లేకపోతే
            stdin_rdev_స్థితి = -1;
        }
      ఒకవేళ (0 < stdin_rdev_స్థితి && major (stdin_rdev) == major (st->st_rdev))
        ఫలితం = fstat (minor (st->st_rdev), st);
    }

  తిరిగి ఫలితం;
}

/* Perform the O(1) btrfs clone operation, if possible.
   Upon success, return 0.  Otherwise, return -1 and set errno.  */
స్థిర అంతఃవరుస పూర్ణాంకం
clone_file (MAYBE_వాడని పూర్ణాంకం dest_fd, MAYBE_వాడని పూర్ణాంకం src_fd)
{
#నిర్వచితమైతే FICLONE
  తిరిగి ioctl (dest_fd, FICLONE, src_fd);
#లేకపోతే
  errno = ENOTSUP;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* FIXME: describe */
/* FIXME: rewrite this to use a hash table so we avoid the quadratic
   performance hit that's probably noticeable only on trees deeper
   than a few hundred levels.  See use of active_dir_map in remove.c  */

ATTRIBUTE_శుద్ధ
స్థిర bool
is_ancestor (స్థిరరాశి నిర్మాణం stat *sb, స్థిరరాశి నిర్మాణం dir_list *ancestors)
{
  అయితే (ancestors)
    {
      ఒకవేళ (PSAME_INODE (ancestors, sb))
        తిరిగి true;
      ancestors = ancestors->మాతృ;
    }
  తిరిగి false;
}

స్థిర bool
errno_unsupported (పూర్ణాంకం err)
{
  తిరిగి err == ENOTSUP || err == ENODATA;
}

#ఒకవేళ USE_XATTR
ATTRIBUTE_FORMAT ((రాయి, 2, 3))
స్థిర శూన్య
copy_attr_error (MAYBE_వాడని నిర్మాణం error_context *ctx,
                 అక్షరం స్థిరరాశి *fmt, ...)
{
  ఒకవేళ (!errno_unsupported (errno))
    {
      పూర్ణాంకం err = errno;
      va_list ap;

      /* use verror module to print error message */
      va_ప్రారంభం (ap, fmt);
      verror (0, err, fmt, ap);
      va_చివర (ap);
    }
}

ATTRIBUTE_FORMAT ((రాయి, 2, 3))
స్థిర శూన్య
copy_attr_allerror (MAYBE_వాడని నిర్మాణం error_context *ctx,
                    అక్షరం స్థిరరాశి *fmt, ...)
{
  పూర్ణాంకం err = errno;
  va_list ap;

  /* use verror module to print error message */
  va_ప్రారంభం (ap, fmt);
  verror (0, err, fmt, ap);
  va_చివర (ap);
}

స్థిర అక్షరం స్థిరరాశి *
copy_attr_quote (MAYBE_వాడని నిర్మాణం error_context *ctx, అక్షరం స్థిరరాశి *str)
{
  తిరిగి quoteaf (str);
}

స్థిర శూన్య
copy_attr_free (MAYBE_వాడని నిర్మాణం error_context *ctx,
                MAYBE_వాడని అక్షరం స్థిరరాశి *str)
{
}

/* Exclude SELinux extended attributes that are otherwise handled,
   and are problematic to copy again.  Also honor attributes
   configured for exclusion in /etc/xattr.conf.
   FIXME: Should we handle POSIX ACLs similarly?
   Return zero to skip.  */
స్థిర పూర్ణాంకం
check_selinux_attr (అక్షరం స్థిరరాశి *name, నిర్మాణం error_context *ctx)
{
  తిరిగి STRNCMP_LIT (name, "security.selinux")
         && attr_copy_check_permissions (name, ctx);
}

/* If positive SRC_FD and DST_FD descriptors are passed,
   then copy by fd, otherwise copy by name.  */

స్థిర bool
copy_attr (అక్షరం స్థిరరాశి *src_మార్గం, పూర్ణాంకం src_fd,
           అక్షరం స్థిరరాశి *dst_మార్గం, పూర్ణాంకం dst_fd, నిర్మాణం cp_options స్థిరరాశి *x)
{
  bool all_errors = (!x->దత్తాంశం_copy_required || x->require_preserve_xattr);
  bool some_errors = (!all_errors && !x->reduce_diagnostics);
  పూర్ణాంకం (*check) (అక్షరం స్థిరరాశి *, నిర్మాణం error_context *)
    = (x->preserve_security_context || x->సమితి_security_context
       ? check_selinux_attr : ఖాళీ);

# ఒకవేళ 4 < __GNUC__ + (8 <= __GNUC_MINOR__)
  /* Pacify gcc -Wsuggest-attribute=format through at least GCC 13.2.1.  */
#  pragma GCC diagnostic నెట్టు
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
# endif
  నిర్మాణం error_context *ctx
    = (all_errors || some_errors
       ? (&(నిర్మాణం error_context) {
           .error = all_errors ? copy_attr_allerror : copy_attr_error,
           .quote = copy_attr_quote,
           .quote_free = copy_attr_free
         })
       : ఖాళీ);
# ఒకవేళ 4 < __GNUC__ + (8 <= __GNUC_MINOR__)
#  pragma GCC diagnostic తీయి
# endif

  తిరిగి ! (0 <= src_fd && 0 <= dst_fd
            ? attr_copy_fd (src_మార్గం, src_fd, dst_మార్గం, dst_fd, check, ctx)
            : attr_copy_file (src_మార్గం, dst_మార్గం, check, ctx));
}
#లేకపోతే /* USE_XATTR */

స్థిర bool
copy_attr (MAYBE_వాడని అక్షరం స్థిరరాశి *src_మార్గం,
           MAYBE_వాడని పూర్ణాంకం src_fd,
           MAYBE_వాడని అక్షరం స్థిరరాశి *dst_మార్గం,
           MAYBE_వాడని పూర్ణాంకం dst_fd,
           MAYBE_వాడని నిర్మాణం cp_options స్థిరరాశి *x)
{
  తిరిగి true;
}
#అయితే_ముగింపు /* USE_XATTR */

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

స్థిర bool
copy_dir (అక్షరం స్థిరరాశి *src_name_లోపల, అక్షరం స్థిరరాశి *dst_name_లోపల,
          పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname_లోపల, bool new_dst,
          స్థిరరాశి నిర్మాణం stat *src_sb, నిర్మాణం dir_list *ancestors,
          స్థిరరాశి నిర్మాణం cp_options *x,
          bool *మొదటి_dir_created_చొప్పున_command_పంక్తి_arg,
          bool *copy_into_self)
{
  అక్షరం *name_space;
  అక్షరం *namep;
  నిర్మాణం cp_options non_command_పంక్తి_options = *x;
  bool ok = true;

  name_space = savedir (src_name_లోపల, SAVEDIR_క్రమీకరించు_FASTREAD);
  ఒకవేళ (name_space == ఖాళీ)
    {
      /* This diagnostic is a bit vague because savedir can fail in
         several different ways.  */
      error (0, errno, _("cannot access %s"), quoteaf (src_name_లోపల));
      తిరిగి false;
    }

  /* For cp's -H option, dereference command line arguments, but do not
     dereference symlinks that are found via recursive traversal.  */
  ఒకవేళ (x->dereference == DEREF_COMMAND_పంక్తి_ARGUMENTS)
    non_command_పంక్తి_options.dereference = DEREF_NEVER;

  bool new_మొదటి_dir_created = false;
  namep = name_space;
  అయితే (*namep != '\0')
    {
      bool local_copy_into_self;
      అక్షరం *src_name = file_name_concat (src_name_లోపల, namep, ఖాళీ);
      అక్షరం *dst_name = file_name_concat (dst_name_లోపల, namep, ఖాళీ);
      bool మొదటి_dir_created = *మొదటి_dir_created_చొప్పున_command_పంక్తి_arg;
      bool rename_succeeded;

      ok &= copy_అంతర్గత (src_name, dst_name, dst_dirfd,
                           dst_name + (dst_relname_లోపల - dst_name_లోపల),
                           new_dst, src_sb,
                           ancestors, &non_command_పంక్తి_options, false,
                           &మొదటి_dir_created,
                           &local_copy_into_self, &rename_succeeded);
      *copy_into_self |= local_copy_into_self;

      విడుదల (dst_name);
      విడుదల (src_name);

      /* If we're copying into self, there's no point in continuing,
         and in fact, that would even infloop, now that we record only
         the first created directory per command line argument.  */
      ఒకవేళ (local_copy_into_self)
        ఆపు;

      new_మొదటి_dir_created |= మొదటి_dir_created;
      namep += అక్షరశ్రేణి_పొడవు (namep) + 1;
    }
  విడుదల (name_space);
  *మొదటి_dir_created_చొప్పున_command_పంక్తి_arg = new_మొదటి_dir_created;

  తిరిగి ok;
}

/* Change the file mode bits of the file identified by DESC or
   DIRFD+NAME to MODE.  Use DESC if DESC is valid and fchmod is
   available, DIRFD+NAME otherwise.  */

స్థిర పూర్ణాంకం
fchmod_or_lchmod (పూర్ణాంకం desc, పూర్ణాంకం dirfd, అక్షరం స్థిరరాశి *name, mode_t బహుళకం_2)
{
#ఒకవేళ HAVE_FCHMOD
  ఒకవేళ (0 <= desc)
    తిరిగి fchmod (desc, బహుళకం_2);
#అయితే_ముగింపు
  తిరిగి lchmodat (dirfd, name, బహుళకం_2);
}

/* Change the ownership of the file identified by DESC or
   DIRFD+NAME to UID+GID.  Use DESC if DESC is valid and fchown is
   available, DIRFD+NAME otherwise.  */

స్థిర పూర్ణాంకం
fchown_or_lchown (పూర్ణాంకం desc, పూర్ణాంకం dirfd, అక్షరం స్థిరరాశి *name, uid_t uid, gid_t gid)
{
#ఒకవేళ HAVE_FCHOWN
  ఒకవేళ (0 <= desc)
    తిరిగి fchown (desc, uid, gid);
#అయితే_ముగింపు
  తిరిగి lchownat (dirfd, name, uid, gid);
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

స్థిర పూర్ణాంకం
సమితి_owner (స్థిరరాశి నిర్మాణం cp_options *x, అక్షరం స్థిరరాశి *dst_name,
           పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname, పూర్ణాంకం dest_desc,
           నిర్మాణం stat స్థిరరాశి *src_sb, bool new_dst,
           నిర్మాణం stat స్థిరరాశి *dst_sb)
{
  uid_t uid = src_sb->st_uid;
  gid_t gid = src_sb->st_gid;

  /* Naively changing the ownership of an already-existing file before
     changing its permissions would create a window of vulnerability if
     the file's old permissions are too generous for the new owner and
     group.  Avoid the window by first changing to a restrictive
     temporary mode if necessary.  */

  ఒకవేళ (!new_dst && (x->preserve_బహుళకం || x->move_బహుళకం || x->సమితి_బహుళకం))
    {
      mode_t old_బహుళకం = dst_sb->st_బహుళకం;
      mode_t new_బహుళకం =
        (x->preserve_బహుళకం || x->move_బహుళకం ? src_sb->st_బహుళకం : x->బహుళకం_2);
      mode_t restrictive_తాత్కాలిక_బహుళకం = old_బహుళకం & new_బహుళకం & S_IRWXU;

      ఒకవేళ ((USE_ACL
           || (old_బహుళకం & CHMOD_బహుళకం_బిట్లు
               & (~new_బహుళకం | S_ISUID | S_ISGID | S_ISVTX)))
          && qset_acl (dst_name, dest_desc, restrictive_తాత్కాలిక_బహుళకం) != 0)
        {
          ఒకవేళ (! owner_failure_ok (x))
            error (0, errno, _("clearing permissions for %s"),
                   quoteaf (dst_name));
          తిరిగి -x->require_preserve;
        }
    }

  ఒకవేళ (fchown_or_lchown (dest_desc, dst_dirfd, dst_relname, uid, gid) == 0)
    తిరిగి 1;

  /* The ownership change failed.  If the failure merely means we lack
     privileges to change owner+group, try to change just the group
     and ignore any failure of this.  Otherwise, report an error.  */
  ఒకవేళ (chown_failure_ok (x))
    ignore_విలువ (fchown_or_lchown (dest_desc, dst_dirfd, dst_relname,
                                    -1, gid));
  లేకపోతే
    {
      error (0, errno, _("failed to preserve ownership for %s"),
             quoteaf (dst_name));
      ఒకవేళ (x->require_preserve)
        తిరిగి -1;
    }

  తిరిగి 0;
}

/* Set the st_author field of DEST_DESC to the st_author field of
   SRC_SB. If DEST_DESC is undefined (-1), set the st_author field
   of DST_NAME instead.  DEST_DESC must refer to the same file as
   DST_NAME if defined.  */

స్థిర శూన్య
సమితి_author (MAYBE_వాడని అక్షరం స్థిరరాశి *dst_name, MAYBE_వాడని పూర్ణాంకం dest_desc,
            MAYBE_వాడని స్థిరరాశి నిర్మాణం stat *src_sb)
{
#ఒకవేళ HAVE_STRUCT_STAT_ST_AUTHOR
  /* FIXME: Modify the following code so that it does not
     follow symbolic links.  */

  /* Preserve the st_author field.  */
  file_t file = (dest_desc < 0
                 ? file_name_lookup (dst_name, 0, 0)
                 : getdport (dest_desc));
  ఒకవేళ (file == MACH_పోర్టు_NULL)
    error (0, errno, _("failed to lookup file %s"), quoteaf (dst_name));
  లేకపోతే
    {
      error_t err = file_chauthor (file, src_sb->st_author);
      ఒకవేళ (err)
        error (0, err, _("failed to preserve authorship for %s"),
               quoteaf (dst_name));
      mach_పోర్టు_deallocate (mach_task_self (), file);
    }
#అయితే_ముగింపు
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
సమితి_process_security_ctx (అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
                          mode_t బహుళకం_2, bool new_dst, స్థిరరాశి నిర్మాణం cp_options *x)
{
  ఒకవేళ (x->preserve_security_context)
    {
      /* Set the default context for the process to match the source.  */
      bool all_errors = !x->దత్తాంశం_copy_required || x->require_preserve_context;
      bool some_errors = !all_errors && !x->reduce_diagnostics;
      అక్షరం *con_raw;

      ఒకవేళ (0 <= lgetfilecon_raw (src_name, &con_raw))
        {
          ఒకవేళ (setfscreatecon_raw (con_raw) < 0)
            {
              ఒకవేళ (all_errors || (some_errors && !errno_unsupported (errno)))
                error (0, errno,
                       _("failed to set default file creation context to %s"),
                       quote (con_raw));
              ఒకవేళ (x->require_preserve_context)
                {
                  freecon (con_raw);
                  తిరిగి false;
                }
            }
          freecon (con_raw);
        }
      లేకపోతే
        {
          ఒకవేళ (all_errors || (some_errors && !errno_unsupported (errno)))
            {
              error (0, errno,
                     _("failed to get security context of %s"),
                     quoteaf (src_name));
            }
          ఒకవేళ (x->require_preserve_context)
            తిరిగి false;
        }
    }
  లేకపోతే ఒకవేళ (x->సమితి_security_context)
    {
      /* With -Z, adjust the default context for the process
         to have the type component adjusted as per the destination path.  */
      ఒకవేళ (new_dst && defaultcon (x->సమితి_security_context, dst_name, బహుళకం_2) < 0
          && ! ignorable_ctx_err (errno))
        {
          error (0, errno,
                 _("failed to set default file creation context for %s"),
                 quoteaf (dst_name));
        }
    }

  తిరిగి true;
}

/* Reset the security context of DST_NAME, to that already set
   as the process default if !X->set_security_context.  Otherwise
   adjust the type component of DST_NAME's security context as
   per the system default for that path.  Issue warnings upon
   failure, when allowed by various settings in X.
   Return false on failure, true on success.  */

bool
సమితి_file_security_ctx (అక్షరం స్థిరరాశి *dst_name,
                       bool recurse, స్థిరరాశి నిర్మాణం cp_options *x)
{
  bool all_errors = (!x->దత్తాంశం_copy_required
                     || x->require_preserve_context);
  bool some_errors = !all_errors && !x->reduce_diagnostics;

  ఒకవేళ (! restorecon (x->సమితి_security_context, dst_name, recurse))
    {
      ఒకవేళ (all_errors || (some_errors && !errno_unsupported (errno)))
        error (0, errno, _("failed to set the security context of %s"),
               quoteaf_n (0, dst_name));
      తిరిగి false;
    }

  తిరిగి true;
}

#ఒకవేళ HAVE_FCLONEFILEAT && !USE_XATTR
# include <sys/acl.h>
/* Return true if FD has a nontrivial ACL.  */
స్థిర bool
fd_has_acl (పూర్ణాంకం fd)
{
  /* Every platform with fclonefileat (macOS 10.12 or later) also has
     acl_get_fd_np.  */
  bool has_acl = false;
  acl_t acl = acl_get_fd_np (fd, ACL_TYPE_EXTENDED);
  ఒకవేళ (acl)
    {
      acl_నమోదు_t ace;
      has_acl = 0 <= acl_get_నమోదు (acl, ACL_మొదటి_నమోదు, &ace);
      acl_free (acl);
    }
  తిరిగి has_acl;
}
#అయితే_ముగింపు

/* Handle failure from FICLONE or fclonefileat.
   Return FALSE if it's a terminal failure for this file.  */

స్థిర bool
handle_clone_fail (పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
                   అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
                   bool new_dst, గణన Reflink_type reflink_బహుళకం)
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
  ఒకవేళ (! (reflink_బహుళకం == REFLINK_ALWAYS || errno == EIO))
    తిరిగి true;

  error (0, errno, _("failed to clone %s from %s"),
         quoteaf_n (0, dst_name), quoteaf_n (1, src_name));

  /* Remove the destination if cloning created it but later failed,
     either due to EIO, or to any error if --reflink=always.  The need
     for removal is a Linux kernel bug, as ioctl with FICLONE is
     supposed to be atomic.  We don't know whether currently supported
     kernels have the problem, so play it safe.  */
  ఒకవేళ (new_dst /* currently not for fclonefileat().  */
      && unlinkat (dst_dirfd, dst_relname, 0) < 0 && errno != ENOENT)
    error (0, errno, _("cannot remove %s"), quoteaf (dst_name));

  తిరిగి false;
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

స్థిర bool
copy_reg (అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
          పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
          స్థిరరాశి నిర్మాణం cp_options *x,
          mode_t dst_బహుళకం, mode_t omitted_permissions, bool *new_dst,
          నిర్మాణం stat *src_sb)
{
  పూర్ణాంకం dest_desc;
  పూర్ణాంకం dest_errno;
  పూర్ణాంకం ఆధారం_desc;
  mode_t extra_permissions;
  నిర్మాణం stat sb;
  నిర్మాణం stat src_open_sb;
  bool return_val = true;
  bool దత్తాంశం_copy_required = x->దత్తాంశం_copy_required;
  bool preserve_xattr = USE_XATTR & x->preserve_xattr;

  copy_debug.offload = COPY_DEBUG_UNKNOWN;
  copy_debug.reflink = x->reflink_బహుళకం ? COPY_DEBUG_UNKNOWN : COPY_DEBUG_NO;
  copy_debug.sparse_detection = COPY_DEBUG_UNKNOWN;

  ఆధారం_desc = open (src_name,
                      (O_RDONLY | O_ద్విమాన
                       | (x->dereference == DEREF_NEVER ? O_NOFOLLOW : 0)));
  ఒకవేళ (ఆధారం_desc < 0)
    {
      error (0, errno, _("cannot open %s for reading"), quoteaf (src_name));
      తిరిగి false;
    }

  ఒకవేళ (fstat (ఆధారం_desc, &src_open_sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (src_name));
      return_val = false;
      వెళ్ళు close_src_desc;
    }

  /* Compare the source dev/ino from the open file to the incoming,
     saved ones obtained via a previous call to stat.  */
  ఒకవేళ (! psame_inode (src_sb, &src_open_sb))
    {
      error (0, 0,
             _("skipping file %s, as it was replaced while being copied"),
             quoteaf (src_name));
      return_val = false;
      వెళ్ళు close_src_desc;
    }

  /* Might as well tell the caller about the latest version of the
     source file status, since we have it already.  */
  *src_sb = src_open_sb;
  mode_t src_బహుళకం = src_sb->st_బహుళకం;

  /* The semantics of the following open calls are mandated
     by the specs for both cp and mv.  */
  ఒకవేళ (! *new_dst)
    {
      పూర్ణాంకం open_flags =
        O_WRONLY | O_ద్విమాన | (దత్తాంశం_copy_required ? O_TRUNC : 0);
      dest_desc = openat (dst_dirfd, dst_relname, open_flags);
      dest_errno = errno;

      /* When using cp --preserve=context to copy to an existing destination,
         reset the context as per the default context, which has already been
         set according to the src.
         When using the mutually exclusive -Z option, then adjust the type of
         the existing context according to the system default for the dest.
         Note we set the context here, _after_ the file is opened, lest the
         new context disallow that.  */
      ఒకవేళ (0 <= dest_desc
          && (x->సమితి_security_context || x->preserve_security_context))
        {
          ఒకవేళ (! సమితి_file_security_ctx (dst_name, false, x))
            {
              ఒకవేళ (x->require_preserve_context)
                {
                  return_val = false;
                  వెళ్ళు close_src_and_dst_desc;
                }
            }
        }

      ఒకవేళ (dest_desc < 0 && dest_errno != ENOENT
          && x->unlink_dest_after_failed_open)
        {
          ఒకవేళ (unlinkat (dst_dirfd, dst_relname, 0) == 0)
            {
              ఒకవేళ (x->verbose)
                రాయి (_("removed %s\n"), quoteaf (dst_name));
            }
          లేకపోతే ఒకవేళ (errno != ENOENT)
            {
              error (0, errno, _("cannot remove %s"), quoteaf (dst_name));
              return_val = false;
              వెళ్ళు close_src_desc;
            }

          dest_errno = ENOENT;
        }

      ఒకవేళ (dest_desc < 0 && dest_errno == ENOENT)
        {
          /* Ensure there is no race where a file may be left without
             an appropriate security context.  */
          ఒకవేళ (x->సమితి_security_context)
            {
              ఒకవేళ (! సమితి_process_security_ctx (src_name, dst_name, dst_బహుళకం,
                                              true, x))
                {
                  return_val = false;
                  వెళ్ళు close_src_desc;
                }
            }

          /* Tell caller that the destination file is created.  */
          *new_dst = true;
        }
    }

  ఒకవేళ (*new_dst)
    {
#ఒకవేళ HAVE_FCLONEFILEAT && !USE_XATTR
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
      ఒకవేళ (దత్తాంశం_copy_required && x->reflink_బహుళకం
          && (CLONE_NOOWNERCOPY || x->preserve_ownership))
        {
          /* Try fclonefileat so long as it won't create the
             destination with unwanted permissions, which could lead
             to a security race.  */
          mode_t cloned_బహుళకం_బిట్లు = S_ISVTX | S_IRWXUGO;
          mode_t cloned_బహుళకం = src_బహుళకం & cloned_బహుళకం_బిట్లు;
          mode_t desired_బహుళకం
            = (x->preserve_బహుళకం ? src_బహుళకం & CHMOD_బహుళకం_బిట్లు
               : x->సమితి_బహుళకం ? x->బహుళకం_2
               : ((x->explicit_no_preserve_బహుళకం ? బహుళకం_RW_UGO : dst_బహుళకం)
                  & ~ cached_umask ()));
          ఒకవేళ (! (cloned_బహుళకం & ~desired_బహుళకం))
            {
              పూర్ణాంకం fc_flags
                = (CLONE_NOFOLLOW
                   | (x->preserve_బహుళకం ? CLONE_ACL : 0)
                   | (x->preserve_ownership ? 0 : CLONE_NOOWNERCOPY));
              పూర్ణాంకం s = fclonefileat (ఆధారం_desc, dst_dirfd, dst_relname,
                                    fc_flags);
              ఒకవేళ (s != 0 && (fc_flags & CLONE_ACL) && errno == EINVAL)
                {
                  fc_flags &= ~CLONE_ACL;
                  s = fclonefileat (ఆధారం_desc, dst_dirfd, dst_relname,
                                    fc_flags);
                }
              ఒకవేళ (s == 0)
                {
                  copy_debug.reflink = COPY_DEBUG_YES;

                  /* Update the clone's timestamps and permissions
                     as needed.  */

                  ఒకవేళ (!x->preserve_timestamps)
                    {
                      నిర్మాణం timespec timespec[2];
                      timespec[0].tv_nsec = timespec[1].tv_nsec = UTIME_NOW;
                      ఒకవేళ (utimensat (dst_dirfd, dst_relname, timespec,
                                     AT_SYMLINK_NOFOLLOW)
                          != 0)
                        {
                          error (0, errno, _("updating times for %s"),
                                 quoteaf (dst_name));
                          return_val = false;
                          వెళ్ళు close_src_desc;
                        }
                    }

                  extra_permissions = desired_బహుళకం & ~cloned_బహుళకం;
                  ఒకవేళ (!extra_permissions
                      && (!x->preserve_బహుళకం || (fc_flags & CLONE_ACL)
                          || !fd_has_acl (ఆధారం_desc)))
                    {
                      వెళ్ళు close_src_desc;
                    }

                  /* Either some desired permissions were not cloned,
                     or ACLs were not cloned despite that being requested.  */
                  omitted_permissions = 0;
                  dest_desc = -1;
                  వెళ్ళు సమితి_dest_బహుళకం;
                }
              ఒకవేళ (! handle_clone_fail (dst_dirfd, dst_relname, src_name,
                                       dst_name,
                                       false /* We didn't create dst  */,
                                       x->reflink_బహుళకం))
                {
                  return_val = false;
                  వెళ్ళు close_src_desc;
                }
            }
          లేకపోతే
            copy_debug.reflink = COPY_DEBUG_AVOIDED;
        }
      లేకపోతే ఒకవేళ (దత్తాంశం_copy_required && x->reflink_బహుళకం)
        {
          ఒకవేళ (! CLONE_NOOWNERCOPY)
            copy_debug.reflink = COPY_DEBUG_AVOIDED;
        }
#అయితే_ముగింపు

      /* To allow copying xattrs on read-only files, create with u+w.
         This satisfies an inode permission check done by
         xattr_permission in fs/xattr.c of the GNU/Linux kernel.  */
      mode_t open_బహుళకం =
        ((dst_బహుళకం & ~omitted_permissions)
         | (preserve_xattr && !x->owner_privileges ? S_IWUSR : 0));
      extra_permissions = open_బహుళకం & ~dst_బహుళకం; /* either 0 or S_IWUSR */

      పూర్ణాంకం open_flags = O_WRONLY | O_CREAT | O_ద్విమాన;
      dest_desc = openat (dst_dirfd, dst_relname, open_flags | O_EXCL,
                          open_బహుళకం);
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
      ఒకవేళ (dest_desc < 0 && dest_errno == EEXIST && ! x->move_బహుళకం)
        {
          ఒకవేళ (issymlinkat (dst_dirfd, dst_relname) == 1)
            {
              ఒకవేళ (x->open_dangling_dest_symlink)
                {
                  dest_desc = openat (dst_dirfd, dst_relname,
                                      open_flags, open_బహుళకం);
                  dest_errno = errno;
                }
              లేకపోతే
                {
                  error (0, 0, _("not writing through dangling symlink %s"),
                         quoteaf (dst_name));
                  return_val = false;
                  వెళ్ళు close_src_desc;
                }
            }
        }

      /* Improve quality of diagnostic when a nonexistent dst_name
         ends in a slash and open fails with errno == EISDIR.  */
      ఒకవేళ (dest_desc < 0 && dest_errno == EISDIR
          && *dst_name && dst_name[అక్షరశ్రేణి_పొడవు (dst_name) - 1] == '/')
        dest_errno = ENOTDIR;
    }
  లేకపోతే
    {
      omitted_permissions = extra_permissions = 0;
    }

  ఒకవేళ (dest_desc < 0)
    {
      error (0, dest_errno, _("cannot create regular file %s"),
             quoteaf (dst_name));
      return_val = false;
      వెళ్ళు close_src_desc;
    }

  /* --attributes-only overrides --reflink.  */
  ఒకవేళ (దత్తాంశం_copy_required && x->reflink_బహుళకం)
    {
      ఒకవేళ (clone_file (dest_desc, ఆధారం_desc) == 0)
        {
          దత్తాంశం_copy_required = false;
          copy_debug.reflink = COPY_DEBUG_YES;
        }
      లేకపోతే
        {
          ఒకవేళ (! handle_clone_fail (dst_dirfd, dst_relname, src_name, dst_name,
                                   *new_dst, x->reflink_బహుళకం))
           {
             return_val = false;
             వెళ్ళు close_src_and_dst_desc;
           }
        }
    }

  ఒకవేళ (! (దత్తాంశం_copy_required | x->preserve_ownership | extra_permissions))
    sb.st_బహుళకం = 0;
  లేకపోతే ఒకవేళ (fstat (dest_desc, &sb) != 0)
    {
      error (0, errno, _("cannot fstat %s"), quoteaf (dst_name));
      return_val = false;
      వెళ్ళు close_src_and_dst_desc;
    }

  /* If extra permissions needed for copy_xattr didn't happen (e.g.,
     due to umask) chmod to add them temporarily; if that fails give
     up with extra permissions, letting copy_attr fail later.  */
  mode_t temporary_బహుళకం = sb.st_బహుళకం | extra_permissions;
  ఒకవేళ (temporary_బహుళకం != sb.st_బహుళకం
      && (fchmod_or_lchmod (dest_desc, dst_dirfd, dst_relname, temporary_బహుళకం)
          != 0))
    extra_permissions = 0;

  ఒకవేళ (దత్తాంశం_copy_required
      && (copy_file_దత్తాంశం (ఆధారం_desc, &src_open_sb, 0, src_name,
                          dest_desc, &sb, 0, dst_name,
                          లెక్క_MAX, x, &copy_debug)
          < 0))
    {
      return_val = false;
      వెళ్ళు close_src_and_dst_desc;
    }

  ఒకవేళ (x->preserve_timestamps)
    {
      నిర్మాణం timespec timespec[2];
      timespec[0] = get_stat_atime (src_sb);
      timespec[1] = get_stat_mtime (src_sb);

      ఒకవేళ (fdutimensat (dest_desc, dst_dirfd, dst_relname, timespec, 0) != 0)
        {
          error (0, errno, _("preserving times for %s"), quoteaf (dst_name));
          ఒకవేళ (x->require_preserve)
            {
              return_val = false;
              వెళ్ళు close_src_and_dst_desc;
            }
        }
    }

  /* Set ownership before xattrs as changing owners will
     clear capabilities.  */
  ఒకవేళ (x->preserve_ownership && ! SAME_OWNER_AND_సమూహం (*src_sb, sb))
    {
      ఎంపిక (సమితి_owner (x, dst_name, dst_dirfd, dst_relname, dest_desc,
                         src_sb, *new_dst, &sb))
        {
        స్థితి -1:
          return_val = false;
          వెళ్ళు close_src_and_dst_desc;

        స్థితి 0:
          src_బహుళకం &= ~ (S_ISUID | S_ISGID | S_ISVTX);
          ఆపు;
        }
    }

  ఒకవేళ (preserve_xattr)
    {
      ఒకవేళ (!copy_attr (src_name, ఆధారం_desc, dst_name, dest_desc, x)
          && x->require_preserve_xattr && ! x->move_బహుళకం)
        return_val = false;
    }

  సమితి_author (dst_name, dest_desc, src_sb);

#ఒకవేళ HAVE_FCLONEFILEAT && !USE_XATTR
సమితి_dest_బహుళకం:
#అయితే_ముగింపు
  ఒకవేళ (x->preserve_బహుళకం || x->move_బహుళకం)
    {
      ఒకవేళ (xcopy_acl (src_name, ఆధారం_desc, dst_name, dest_desc, src_బహుళకం) != 0
          && x->require_preserve)
        return_val = false;
    }
  లేకపోతే ఒకవేళ (x->సమితి_బహుళకం)
    {
      ఒకవేళ (xset_acl (dst_name, dest_desc, x->బహుళకం_2) != 0)
        return_val = false;
    }
  లేకపోతే ఒకవేళ (x->explicit_no_preserve_బహుళకం && *new_dst)
    {
      ఒకవేళ (xset_acl (dst_name, dest_desc, బహుళకం_RW_UGO & ~cached_umask ()) != 0)
        return_val = false;
    }
  లేకపోతే ఒకవేళ (omitted_permissions | extra_permissions)
    {
      omitted_permissions &= ~ cached_umask ();
      ఒకవేళ ((omitted_permissions | extra_permissions)
          && (fchmod_or_lchmod (dest_desc, dst_dirfd, dst_relname,
                                dst_బహుళకం & ~ cached_umask ())
              != 0))
        {
          error (0, errno, _("preserving permissions for %s"),
                 quoteaf (dst_name));
          ఒకవేళ (x->require_preserve)
            return_val = false;
        }
    }

  ఒకవేళ (dest_desc < 0)
    వెళ్ళు close_src_desc;

close_src_and_dst_desc:
  ఒకవేళ (close (dest_desc) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (dst_name));
      return_val = false;
    }
close_src_desc:
  ఒకవేళ (close (ఆధారం_desc) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (src_name));
      return_val = false;
    }

  /* Output debug info for data copying operations.  */
  ఒకవేళ (x->debug)
    emit_debug (x);

  తిరిగి return_val;
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

స్థిర bool
same_file_ok (అక్షరం స్థిరరాశి *src_name, నిర్మాణం stat స్థిరరాశి *src_sb,
              పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname, నిర్మాణం stat స్థిరరాశి *dst_sb,
              స్థిరరాశి నిర్మాణం cp_options *x, bool *return_now)
{
  స్థిరరాశి నిర్మాణం stat *src_sb_link;
  స్థిరరాశి నిర్మాణం stat *dst_sb_link;
  నిర్మాణం stat tmp_dst_sb;
  నిర్మాణం stat tmp_src_sb;

  bool same_link;
  bool same = psame_inode (src_sb, dst_sb);

  *return_now = false;

  /* FIXME: this should (at the very least) be moved into the following
     if-block.  More likely, it should be removed, because it inhibits
     making backups.  But removing it will result in a change in behavior
     that will probably have to be documented -- and tests will have to
     be updated.  */
  ఒకవేళ (same && x->hard_link)
    {
      *return_now = true;
      తిరిగి true;
    }

  ఒకవేళ (x->dereference == DEREF_NEVER)
    {
      same_link = same;

      /* If both the source and destination files are symlinks (and we'll
         know this here IFF preserving symlinks), then it's usually ok
         when they are distinct.  */
      ఒకవేళ (S_ISLNK (src_sb->st_బహుళకం) && S_ISLNK (dst_sb->st_బహుళకం))
        {
          bool sn = same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname);
          ఒకవేళ ( ! sn)
            {
              /* It's fine when we're making any type of backup.  */
              ఒకవేళ (x->backup_type != no_backups)
                తిరిగి true;

              /* Here we have two symlinks that are hard-linked together,
                 and we're not making backups.  In this unusual case, simply
                 returning true would lead to mv calling "rename(A,B)",
                 which would do nothing and return 0.  */
              ఒకవేళ (same_link)
                {
                  *return_now = true;
                  తిరిగి ! x->move_బహుళకం;
                }
            }

          తిరిగి ! sn;
        }

      src_sb_link = src_sb;
      dst_sb_link = dst_sb;
    }
  లేకపోతే
    {
      ఒకవేళ (!same)
        తిరిగి true;

      ఒకవేళ (fstatat (dst_dirfd, dst_relname, &tmp_dst_sb,
                   AT_SYMLINK_NOFOLLOW) != 0
          || lstat (src_name, &tmp_src_sb) != 0)
        తిరిగి true;

      src_sb_link = &tmp_src_sb;
      dst_sb_link = &tmp_dst_sb;

      same_link = psame_inode (src_sb_link, dst_sb_link);

      /* If both are symlinks, then it's ok, but only if the destination
         will be unlinked before being opened.  This is like the test
         above, but with the addition of the unlink_dest_before_opening
         conjunct because otherwise, with two symlinks to the same target,
         we'd end up truncating the source file.  */
      ఒకవేళ (S_ISLNK (src_sb_link->st_బహుళకం) && S_ISLNK (dst_sb_link->st_బహుళకం)
          && x->unlink_dest_before_opening)
        తిరిగి true;
    }

  /* The backup code ensures there's a copy, so it's usually ok to
     remove any destination file.  One exception is when both
     source and destination are the same directory entry.  In that
     case, moving the destination file aside (in making the backup)
     would also rename the source file and result in an error.  */
  ఒకవేళ (x->backup_type != no_backups)
    {
      ఒకవేళ (!same_link)
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
          ఒకవేళ ( ! x->move_బహుళకం
               && x->dereference != DEREF_NEVER
               && S_ISLNK (src_sb_link->st_బహుళకం)
               && ! S_ISLNK (dst_sb_link->st_బహుళకం))
            తిరిగి false;

          తిరిగి true;
        }

      /* FIXME: What about case insensitive file systems ?  */
      తిరిగి ! same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname);
    }

#ఒకవేళ 0
  /* FIXME: use or remove */

  /* If we're making a backup, we'll detect the problem case in
     copy_reg because SRC_NAME will no longer exist.  Allowing
     the test to be deferred lets cp do some useful things.
     But when creating hardlinks and SRC_NAME is a symlink
     but DST_RELNAME is not we must test anyway.  */
  ఒకవేళ (x->hard_link
      || !S_ISLNK (src_sb_link->st_బహుళకం)
      || S_ISLNK (dst_sb_link->st_బహుళకం))
    తిరిగి true;

  ఒకవేళ (x->dereference != DEREF_NEVER)
    తిరిగి true;
#అయితే_ముగింపు

  ఒకవేళ (x->move_బహుళకం || x->unlink_dest_before_opening)
    {
      /* They may refer to the same file if we're in move mode and the
         target is a symlink.  That is ok, since we remove any existing
         destination file before opening it -- via 'rename' if they're on
         the same file system, via unlinkat otherwise.  */
      ఒకవేళ (S_ISLNK (dst_sb_link->st_బహుళకం))
        తిరిగి true;

      /* It's not ok if they're distinct hard links to the same file as
         this causes a race condition and we may lose data in this case.  */
      ఒకవేళ (same_link
          && 1 < dst_sb_link->st_nlink
          && ! same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname))
        తిరిగి ! x->move_బహుళకం;
    }

  /* If neither is a symlink, then it's ok as long as they aren't
     hard links to the same file.  */
  ఒకవేళ (!S_ISLNK (src_sb_link->st_బహుళకం) && !S_ISLNK (dst_sb_link->st_బహుళకం))
    {
      ఒకవేళ (!psame_inode (src_sb_link, dst_sb_link))
        తిరిగి true;

      /* If they are the same file, it's ok if we're making hard links.  */
      ఒకవేళ (x->hard_link)
        {
          *return_now = true;
          తిరిగి true;
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
  ఒకవేళ (x->move_బహుళకం
      && S_ISLNK (src_sb->st_బహుళకం)
      && 1 < dst_sb_link->st_nlink)
    {
      అక్షరం *abs_src = canonicalize_file_name (src_name);
      ఒకవేళ (abs_src)
        {
          bool ఫలితం = ! same_nameat (AT_FDCWD, abs_src,
                                       dst_dirfd, dst_relname);
          విడుదల (abs_src);
          తిరిగి ఫలితం;
        }
    }

  /* It's ok to recreate a destination symlink. */
  ఒకవేళ (x->symbolic_link && S_ISLNK (dst_sb_link->st_బహుళకం))
    తిరిగి true;

  ఒకవేళ (x->dereference == DEREF_NEVER)
    {
      ఒకవేళ ( ! S_ISLNK (src_sb_link->st_బహుళకం))
        tmp_src_sb = *src_sb_link;
      లేకపోతే ఒకవేళ (stat (src_name, &tmp_src_sb) != 0)
        తిరిగి true;

      ఒకవేళ ( ! S_ISLNK (dst_sb_link->st_బహుళకం))
        tmp_dst_sb = *dst_sb_link;
      లేకపోతే ఒకవేళ (fstatat (dst_dirfd, dst_relname, &tmp_dst_sb, 0) != 0)
        తిరిగి true;

      ఒకవేళ (!psame_inode (&tmp_src_sb, &tmp_dst_sb))
        తిరిగి true;

      ఒకవేళ (x->hard_link)
        {
          /* It's ok to attempt to hardlink the same file,
            and return early if not replacing a symlink.
            Note we need to return early to avoid a later
            unlink() of DST (when SRC is a symlink).  */
          *return_now = ! S_ISLNK (dst_sb_link->st_బహుళకం);
          తిరిగి true;
        }
    }

  తిరిగి false;
}

/* Return whether DST_DIRFD+DST_RELNAME, with mode MODE,
   is writable in the sense of 'mv'.
   Always consider a symbolic link to be writable.  */
స్థిర bool
writable_destination (పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname, mode_t బహుళకం_2)
{
  తిరిగి (S_ISLNK (బహుళకం_2)
          || can_write_any_file ()
          || faccessat (dst_dirfd, dst_relname, W_OK, AT_EACCESS) == 0);
}

స్థిర bool
overwrite_ok (నిర్మాణం cp_options స్థిరరాశి *x, అక్షరం స్థిరరాశి *dst_name,
              పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
              నిర్మాణం stat స్థిరరాశి *dst_sb)
{
  ఒకవేళ (! writable_destination (dst_dirfd, dst_relname, dst_sb->st_బహుళకం))
    {
      అక్షరం perms[12];		/* "-rwxrwxrwx " ls-style modes. */
      strmode (dst_sb->st_బహుళకం, perms);
      perms[10] = '\0';
      fprintf (stderr,
               (x->move_బహుళకం || x->unlink_dest_before_opening
                || x->unlink_dest_after_failed_open)
               ? _("%s: replace %s, overriding mode %04lo (%s)? ")
               : _("%s: unwritable %s (mode %04lo, %s); try anyway? "),
               కార్యక్రమం_name, quoteaf (dst_name),
               (గుర్తులేని దీర్ఘ పూర్ణాంకం) (dst_sb->st_బహుళకం & CHMOD_బహుళకం_బిట్లు),
               &perms[1]);
    }
  లేకపోతే
    {
      fprintf (stderr, _("%s: overwrite %s? "),
               కార్యక్రమం_name, quoteaf (dst_name));
    }

  తిరిగి yesno ();
}

/* Initialize the hash table implementing a set of F_triple entries
   corresponding to destination files.  */
బాహ్య శూన్య
dest_info_init (నిర్మాణం cp_options *x)
{
  x->dest_info
    = hash_initialize (DEST_INFO_INITIAL_సామర్థ్యం,
                       ఖాళీ,
                       triple_hash,
                       triple_compare,
                       triple_free);
  ఒకవేళ (! x->dest_info)
    xalloc_die ();
}

/* Initialize the hash table implementing a set of F_triple entries
   corresponding to source files listed on the command line.  */
బాహ్య శూన్య
src_info_init (నిర్మాణం cp_options *x)
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
    = hash_initialize (DEST_INFO_INITIAL_సామర్థ్యం,
                       ఖాళీ,
                       triple_hash_no_name,
                       triple_compare,
                       triple_free);
  ఒకవేళ (! x->src_info)
    xalloc_die ();
}

/* When effecting a move (e.g., for mv(1)), and given the name DST_NAME
   aka DST_DIRFD+DST_RELNAME
   of the destination and a corresponding stat buffer, DST_SB, return
   true if the logical 'move' operation should _not_ proceed.
   Otherwise, return false.
   Depending on options specified in X, this code may issue an
   interactive prompt asking whether it's ok to overwrite DST_NAME.  */
స్థిర bool
abandon_move (స్థిరరాశి నిర్మాణం cp_options *x,
              అక్షరం స్థిరరాశి *dst_name,
              పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
              నిర్మాణం stat స్థిరరాశి *dst_sb)
{
  affirm (x->move_బహుళకం);
  తిరిగి (x->update == UPDATE_NONE
          || x->update == UPDATE_NONE_FAIL
          || ((x->interactive == I_ASK_USER
               || (x->interactive == I_UNSPECIFIED
                   && x->stdin_tty
                   && ! writable_destination (dst_dirfd, dst_relname,
                                              dst_sb->st_బహుళకం)))
              && ! overwrite_ok (x, dst_name, dst_dirfd, dst_relname, dst_sb)));
}

/* Print --verbose output on standard output, e.g. 'new' -> 'old'.
   If BACKUP_DST_NAME is non-null, then also indicate that it is
   the name of a backup file.  */
స్థిర శూన్య
emit_verbose (అక్షరం స్థిరరాశి *format, అక్షరం స్థిరరాశి *src, అక్షరం స్థిరరాశి *dst,
              అక్షరం స్థిరరాశి *backup_dst_name)
{
  రాయి (format, quoteaf_n (0, src), quoteaf_n (1, dst));
  ఒకవేళ (backup_dst_name)
    రాయి (_(" (backup: %s)"), quoteaf (backup_dst_name));
  అక్షరం_రాయి ('\n');
}

/* A wrapper around "setfscreatecon (NULL)" that exits upon failure.  */
స్థిర శూన్య
restore_default_fscreatecon_or_die (శూన్య)
{
  ఒకవేళ (setfscreatecon (ఖాళీ) != 0)
    error (EXIT_FAILURE, errno,
           _("failed to restore the default file creation context"));
}

/* Return a newly-allocated string that is like STR
   except replace its suffix SUFFIX with NEWSUFFIX.  */
స్థిర అక్షరం *
subst_suffix (అక్షరం స్థిరరాశి *str, అక్షరం స్థిరరాశి *suffix, అక్షరం స్థిరరాశి *newsuffix)
{
  idx_t prefixlen = suffix - str;
  idx_t newsuffixsize = అక్షరశ్రేణి_పొడవు (newsuffix) + 1;
  అక్షరం *r = ximalloc (prefixlen + newsuffixsize);
  జ్ఞాపకం_నకలు (r + prefixlen, newsuffix, newsuffixsize);
  తిరిగి జ్ఞాపకం_నకలు (r, str, prefixlen);
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
స్థిర bool
create_hard_link (అక్షరం స్థిరరాశి *src_name, పూర్ణాంకం src_dirfd, అక్షరం స్థిరరాశి *src_relname,
                  అక్షరం స్థిరరాశి *dst_name, పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
                  bool replace, bool verbose, bool dereference)
{
  పూర్ణాంకం err = force_linkat (src_dirfd, src_relname, dst_dirfd, dst_relname,
                          dereference ? AT_SYMLINK_FOLLOW : 0,
                          replace, -1);
  ఒకవేళ (0 < err)
    {

      అక్షరం *a_src_name = ఖాళీ;
      ఒకవేళ (!src_name)
        src_name = a_src_name = subst_suffix (dst_name, dst_relname,
                                              src_relname);
      error (0, err, _("cannot create hard link %s to %s"),
             quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
      విడుదల (a_src_name);
      తిరిగి false;
    }
  ఒకవేళ (err < 0 && verbose)
    రాయి (_("removed %s\n"), quoteaf (dst_name));
  తిరిగి true;
}

/* Return true if the current file should be (tried to be) dereferenced:
   either for DEREF_ALWAYS or for DEREF_COMMAND_LINE_ARGUMENTS in the case
   where the current file is a COMMAND_LINE_ARG; otherwise return false.  */
ATTRIBUTE_శుద్ధ
స్థిర అంతఃవరుస bool
should_dereference (స్థిరరాశి నిర్మాణం cp_options *x, bool command_పంక్తి_arg)
{
  తిరిగి x->dereference == DEREF_ALWAYS
         || (x->dereference == DEREF_COMMAND_పంక్తి_ARGUMENTS
             && command_పంక్తి_arg);
}

/* Return true if the source file with basename SRCBASE and status SRC_ST
   is likely to be the simple backup file for DST_DIRFD+DST_RELNAME.  */
స్థిర bool
ఆధారం_is_dst_backup (అక్షరం స్థిరరాశి *srcbase, నిర్మాణం stat స్థిరరాశి *src_st,
                      పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname)
{
  size_t srcbaselen = అక్షరశ్రేణి_పొడవు (srcbase);
  అక్షరం స్థిరరాశి *dstbase = చివరి_component (dst_relname);
  size_t dstbaselen = అక్షరశ్రేణి_పొడవు (dstbase);
  size_t suffixlen = అక్షరశ్రేణి_పొడవు (simple_backup_suffix);
  ఒకవేళ (! (srcbaselen == dstbaselen + suffixlen
         && memeq (srcbase, dstbase, dstbaselen)
         && streq (srcbase + dstbaselen, simple_backup_suffix)))
    తిరిగి false;
  అక్షరం *dst_వెనుక = subst_suffix (dst_relname,
                                 dst_relname + అక్షరశ్రేణి_పొడవు (dst_relname),
                                 simple_backup_suffix);
  నిర్మాణం stat dst_వెనుక_sb;
  పూర్ణాంకం dst_వెనుక_స్థితి = fstatat (dst_dirfd, dst_వెనుక, &dst_వెనుక_sb, 0);
  విడుదల (dst_వెనుక);
  తిరిగి dst_వెనుక_స్థితి == 0 && psame_inode (src_st, &dst_వెనుక_sb);
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
స్థిర bool
copy_అంతర్గత (అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
               పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
               పూర్ణాంకం nonexistent_dst,
               నిర్మాణం stat స్థిరరాశి *మాతృ,
               నిర్మాణం dir_list *ancestors,
               స్థిరరాశి నిర్మాణం cp_options *x,
               bool command_పంక్తి_arg,
               bool *మొదటి_dir_created_చొప్పున_command_పంక్తి_arg,
               bool *copy_into_self,
               bool *rename_succeeded)
{
  నిర్మాణం stat src_sb;
  నిర్మాణం stat dst_sb;
  mode_t src_బహుళకం IF_LINT ( = 0);
  mode_t dst_బహుళకం IF_LINT ( = 0);
  mode_t dst_బహుళకం_బిట్లు;
  mode_t omitted_permissions;
  bool restore_dst_బహుళకం = false;
  అక్షరం *earlier_file = ఖాళీ;
  అక్షరం *dst_backup = ఖాళీ;
  అక్షరం స్థిరరాశి *drelname = *dst_relname ? dst_relname : ".";
  bool delayed_ok;
  bool copied_as_regular = false;
  bool dest_is_symlink = false;
  bool have_dst_lstat = false;

  *copy_into_self = false;

  పూర్ణాంకం rename_errno = x->rename_errno;
  ఒకవేళ (x->move_బహుళకం && !x->exchange)
    {
      ఒకవేళ (rename_errno < 0)
        rename_errno = (renameatu (AT_FDCWD, src_name, dst_dirfd, drelname,
                                   RENAME_NOREPLACE)
                        ? errno : 0);
      nonexistent_dst = *rename_succeeded = rename_errno == 0;
    }

  ఒకవేళ (rename_errno == 0
      ? !x->చివరి_file
      : rename_errno != EEXIST
        || (x->update != UPDATE_NONE && x->update != UPDATE_NONE_FAIL))
    {
      అక్షరం స్థిరరాశి *name = rename_errno == 0 ? dst_name : src_name;
      పూర్ణాంకం dirfd = rename_errno == 0 ? dst_dirfd : AT_FDCWD;
      అక్షరం స్థిరరాశి *relname = rename_errno == 0 ? drelname : src_name;
      పూర్ణాంకం fstatat_flags
        = x->dereference == DEREF_NEVER ? AT_SYMLINK_NOFOLLOW : 0;
      ఒకవేళ (follow_fstatat (dirfd, relname, &src_sb, fstatat_flags) != 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (name));
          తిరిగి false;
        }

      src_బహుళకం = src_sb.st_బహుళకం;

      ఒకవేళ (S_ISDIR (src_బహుళకం) && !x->recursive)
        {
          error (0, 0, ! x->install_బహుళకం /* cp */
                 ? _("-r not specified; omitting directory %s")
                 : _("omitting directory %s"),
                 quoteaf (src_name));
          తిరిగి false;
        }
    }
  లేకపోతే
    {
#ఒకవేళ defined lint && (defined __clang__ || defined __COVERITY__)
      affirm (x->move_బహుళకం);
      memset (&src_sb, 0, పరిమాణం src_sb);
#అయితే_ముగింపు
    }

  /* Detect the case in which the same source file appears more than
     once on the command line and no backup option has been selected.
     If so, simply warn and don't copy it the second time.
     This check is enabled only if x->src_info is non-null.  */
  ఒకవేళ (command_పంక్తి_arg && x->src_info)
    {
      ఒకవేళ ( ! S_ISDIR (src_బహుళకం)
           && x->backup_type == no_backups
           && చూసిన_file (x->src_info, src_name, &src_sb))
        {
          error (0, 0, _("warning: source file %s specified more than once"),
                 quoteaf (src_name));
          తిరిగి true;
        }

      రికార్డు_file (x->src_info, src_name, &src_sb);
    }

  bool dereference = should_dereference (x, command_పంక్తి_arg);

  /* Whether the destination is (or was) known to be new, updated as
     more info comes in.  This may become true if the destination is a
     dangling symlink, in contexts where dangling symlinks should be
     treated the same as nonexistent files.  */
  bool new_dst = 0 < nonexistent_dst;

  ఒకవేళ (! new_dst)
    {
      /* Normally, fill in DST_SB or set NEW_DST so that later code
         can use DST_SB if NEW_DST is false.  However, don't bother
         doing this when rename_errno == EEXIST and not updating,
         which means that even if !NEW_DST the move will be abandoned
         without looking at DST_SB.  */
      ఒకవేళ (! (rename_errno == EEXIST
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
            = ((! S_ISREG (src_బహుళకం)
                && (! x->copy_as_regular
                    || (S_ISDIR (src_బహుళకం) && !x->keep_directory_symlink)
                    || S_ISLNK (src_బహుళకం)))
               || x->move_బహుళకం || x->symbolic_link || x->hard_link
               || x->backup_type != no_backups
               || x->unlink_dest_before_opening);
          ఒకవేళ (!use_lstat && nonexistent_dst < 0)
            new_dst = true;
          లేకపోతే ఒకవేళ (0 <= follow_fstatat (dst_dirfd, drelname, &dst_sb,
                                        use_lstat ? AT_SYMLINK_NOFOLLOW : 0))
            {
              have_dst_lstat = use_lstat;
              rename_errno = EEXIST;
            }
          లేకపోతే ఒకవేళ (errno == ENOENT)
            new_dst = true;
          లేకపోతే ఒకవేళ (errno == ELOOP && !use_lstat
                   && x->unlink_dest_after_failed_open)
            {
              /* cp -f's destination might be a symlink loop.
                 Leave new_dst=false so that we try to unlink later.  */
            }
          లేకపోతే
            {
              error (0, errno, _("cannot stat %s"), quoteaf (dst_name));
              తిరిగి false;
            }
        }

      ఒకవేళ (rename_errno == EEXIST)
        {
          bool return_now = false;
          bool return_val = true;
          bool skipped = false;

          ఒకవేళ ((x->update != UPDATE_NONE && x->update != UPDATE_NONE_FAIL)
              && ! same_file_ok (src_name, &src_sb, dst_dirfd, drelname,
                                 &dst_sb, x, &return_now))
            {
              error (0, 0, _("%s and %s are the same file"),
                     quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
              తిరిగి false;
            }

          ఒకవేళ (x->update == UPDATE_OLDER && !S_ISDIR (src_బహుళకం))
            {
              /* When preserving timestamps (but not moving within a file
                 system), don't worry if the destination timestamp is
                 less than the source merely because of timestamp
                 truncation.  */
              పూర్ణాంకం options = ((x->preserve_timestamps
                              && ! (x->move_బహుళకం
                                    && dst_sb.st_dev == src_sb.st_dev))
                             ? UTIMECMP_TRUNCATE_ఆధారం
                             : 0);

              ఒకవేళ (0 <= utimecmpat (dst_dirfd, dst_relname, &dst_sb,
                                   &src_sb, options))
                {
                  /* We're using --update and the destination is not older
                     than the source, so do not copy or move.  Pretend the
                     rename succeeded, so the caller (if it's mv) doesn't
                     end up removing the source file.  */
                  ఒకవేళ (rename_succeeded)
                    *rename_succeeded = true;

                  /* However, we still must record that we've processed
                     this src/dest pair, in case this source file is
                     hard-linked to another one.  In that case, we'll use
                     the mapping information to link the corresponding
                     destination names.  */
                  earlier_file = remember_copied (dst_relname, src_sb.st_ino,
                                                  src_sb.st_dev);
                  ఒకవేళ (earlier_file)
                    {
                      /* Note we currently replace DST_NAME unconditionally,
                         even if it was a newer separate file.  */
                      ఒకవేళ (! create_hard_link (ఖాళీ, dst_dirfd, earlier_file,
                                              dst_name, dst_dirfd, dst_relname,
                                              true,
                                              x->verbose, dereference))
                        {
                          వెళ్ళు un_backup;
                        }
                    }

                  skipped = true;
                  వెళ్ళు skip;
                }
            }

          /* When there is an existing destination file, we may end up
             returning early, and hence not copying/moving the file.
             This may be due to an interactive 'negative' reply to the
             prompt about the existing file.  It may also be due to the
             use of the --no-clobber option.

             cp and mv treat -i and -f differently.  */
          ఒకవేళ (x->move_బహుళకం)
            {
              ఒకవేళ (abandon_move (x, dst_name, dst_dirfd, drelname, &dst_sb))
                {
                  /* Pretend the rename succeeded, so the caller (mv)
                     doesn't end up removing the source file.  */
                  ఒకవేళ (rename_succeeded)
                    *rename_succeeded = true;

                  skipped = true;
                  return_val = x->update == UPDATE_NONE;
                }
            }
          లేకపోతే
            {
              ఒకవేళ (! S_ISDIR (src_బహుళకం)
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
          ఒకవేళ (skipped)
            {
              ఒకవేళ (x->update == UPDATE_NONE_FAIL)
                error (0, 0, _("not replacing %s"), quoteaf (dst_name));
              లేకపోతే ఒకవేళ (x->debug)
                రాయి (_("skipped %s\n"), quoteaf (dst_name));

              return_now = true;
            }

          ఒకవేళ (return_now)
            తిరిగి return_val;

          /* Copying a directory onto a non-directory, or vice versa,
             is ok only with --backup or --exchange.  */
          ఒకవేళ (!S_ISDIR (src_బహుళకం) != !S_ISDIR (dst_sb.st_బహుళకం)
              && x->backup_type == no_backups && !x->exchange)
            {
              error (0, 0,
                     _(S_ISDIR (src_బహుళకం)
                       ? ("cannot overwrite non-directory %s "
                          "with directory %s")
                       : ("cannot overwrite directory %s "
                          "with non-directory %s")),
                     quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
              తిరిగి false;
            }

          /* Don't let the user destroy their data, even if they try hard:
             This mv command must fail (likewise for cp):
             rm -rf a b c; mkdir a b c; touch a/f b/f; mv a/f b/f c
             Otherwise, the contents of b/f would be lost.
             In the case of 'cp', b/f would be lost if the user simulated
             a move using cp and rm.
             Nothing is lost if you use --backup=numbered or --exchange.  */
          ఒకవేళ (!S_ISDIR (dst_sb.st_బహుళకం) && command_పంక్తి_arg
              && x->backup_type != numbered_backups && !x->exchange
              && చూసిన_file (x->dest_info, dst_relname, &dst_sb))
            {
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
              తిరిగి false;
            }

          అక్షరం స్థిరరాశి *srcbase;
          ఒకవేళ (x->backup_type != no_backups
              /* Don't try to back up a destination if the last
                 component of src_name is "." or "..".  */
              && ! dot_or_dotdot (srcbase = చివరి_component (src_name))
              /* Create a backup of each destination directory in move mode,
                 but not in copy mode.  FIXME: it might make sense to add an
                 option to suppress backup creation also for move mode.
                 That would let one use mv to merge new content into an
                 existing hierarchy.  */
              && (x->move_బహుళకం || ! S_ISDIR (dst_sb.st_బహుళకం)))
            {
              /* Fail if creating the backup file would likely destroy
                 the source file.  Otherwise, the commands:
                 cd /tmp; rm -f a a~; : > a; echo A > a~; cp --b=simple a~ a
                 would leave two zero-length files: a and a~.  */
              ఒకవేళ (x->backup_type != numbered_backups
                  && ఆధారం_is_dst_backup (srcbase, &src_sb,
                                           dst_dirfd, dst_relname))
                {
                  అక్షరం స్థిరరాశి *fmt;
                  fmt = (x->move_బహుళకం
                 ? _("backing up %s might destroy source;  %s not moved")
                 : _("backing up %s might destroy source;  %s not copied"));
                  error (0, 0, fmt,
                         quoteaf_n (0, dst_name),
                         quoteaf_n (1, src_name));
                  తిరిగి false;
                }

              అక్షరం *tmp_backup = backup_file_rename (dst_dirfd, dst_relname,
                                                     x->backup_type);

              /* FIXME: use fts:
                 Using alloca for a file name that may be arbitrarily
                 long is not recommended.  In fact, even forming such a name
                 should be discouraged.  Eventually, this code will be rewritten
                 to use fts, so using alloca here will be less of a problem.  */
              ఒకవేళ (tmp_backup)
                {
                  idx_t dirlen = dst_relname - dst_name;
                  idx_t backupsize = అక్షరశ్రేణి_పొడవు (tmp_backup) + 1;
                  dst_backup = alloca (dirlen + backupsize);
                  జ్ఞాపకం_నకలు (mempcpy (dst_backup, dst_name, dirlen),
                          tmp_backup, backupsize);
                  విడుదల (tmp_backup);
                }
              లేకపోతే ఒకవేళ (errno != ENOENT)
                {
                  error (0, errno, _("cannot backup %s"), quoteaf (dst_name));
                  తిరిగి false;
                }
              new_dst = true;
            }
          లేకపోతే ఒకవేళ (! S_ISDIR (dst_sb.st_బహుళకం)
                   /* Never unlink dst_name when in move mode.  */
                   && ! x->move_బహుళకం
                   && (x->unlink_dest_before_opening
                       || (x->దత్తాంశం_copy_required
                           && ((x->preserve_links && 1 < dst_sb.st_nlink)
                               || (x->dereference == DEREF_NEVER
                                   && ! S_ISREG (src_sb.st_బహుళకం))))
                      ))
            {
              ఒకవేళ (unlinkat (dst_dirfd, dst_relname, 0) != 0 && errno != ENOENT)
                {
                  error (0, errno, _("cannot remove %s"), quoteaf (dst_name));
                  తిరిగి false;
                }
              new_dst = true;
              ఒకవేళ (x->verbose)
                రాయి (_("removed %s\n"), quoteaf (dst_name));
            }
        }
    }

  /* Ensure we don't try to copy through a symlink that was
     created by a prior call to this function.  */
  ఒకవేళ (command_పంక్తి_arg
      && x->dest_info
      && ! x->move_బహుళకం
      && x->backup_type == no_backups)
    {
      /* If we did not follow symlinks above, good: use that data.
         Otherwise, use AT_SYMLINK_NOFOLLOW, in case dst_name is a symlink.  */
      నిర్మాణం stat tmp_buf;
      నిర్మాణం stat *dst_lstat_sb
        = (have_dst_lstat ? &dst_sb
           : fstatat (dst_dirfd, drelname, &tmp_buf, AT_SYMLINK_NOFOLLOW) < 0
           ? ఖాళీ : &tmp_buf);

      /* Never copy through a symlink we've just created.  */
      ఒకవేళ (dst_lstat_sb
          && S_ISLNK (dst_lstat_sb->st_బహుళకం)
          && చూసిన_file (x->dest_info, dst_relname, dst_lstat_sb))
        {
          error (0, 0,
                 _("will not copy %s through just-created symlink %s"),
                 quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
          తిరిగి false;
        }
    }

  /* If the source is a directory, we don't always create the destination
     directory.  So --verbose should not announce anything until we're
     sure we'll create a directory.  Also don't announce yet when moving
     so we can distinguish renames versus copies.  */
  ఒకవేళ (x->verbose && !x->move_బహుళకం && !S_ISDIR (src_బహుళకం))
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

  ఒకవేళ (rename_errno == 0 || x->exchange)
    earlier_file = ఖాళీ;
  లేకపోతే ఒకవేళ (x->recursive && S_ISDIR (src_బహుళకం))
    {
      ఒకవేళ (command_పంక్తి_arg)
        earlier_file = remember_copied (dst_relname,
                                        src_sb.st_ino, src_sb.st_dev);
      లేకపోతే
        earlier_file = src_గమ్యం_dest_lookup (src_sb.st_ino, src_sb.st_dev);
    }
  లేకపోతే ఒకవేళ (x->move_బహుళకం && src_sb.st_nlink == 1)
    {
      earlier_file = src_గమ్యం_dest_lookup (src_sb.st_ino, src_sb.st_dev);
    }
  లేకపోతే ఒకవేళ (x->preserve_links
           && !x->hard_link
           && (1 < src_sb.st_nlink
               || (command_పంక్తి_arg
                   && x->dereference == DEREF_COMMAND_పంక్తి_ARGUMENTS)
               || x->dereference == DEREF_ALWAYS))
    {
      earlier_file = remember_copied (dst_relname,
                                      src_sb.st_ino, src_sb.st_dev);
    }

  /* Did we copy this inode somewhere else (in this command line argument)
     and therefore this is a second hard link to the inode?  */

  ఒకవేళ (earlier_file)
    {
      /* Avoid damaging the destination file system by refusing to preserve
         hard-linked directories (which are found at least in Netapp snapshot
         directories).  */
      ఒకవేళ (S_ISDIR (src_బహుళకం))
        {
          /* If src_name and earlier_file refer to the same directory entry,
             then warn about copying a directory into itself.  */
          ఒకవేళ (same_nameat (AT_FDCWD, src_name, dst_dirfd, earlier_file))
            {
              error (0, 0, _("cannot copy a directory, %s, into itself, %s"),
                     quoteaf_n (0, శిఖరం_level_src_name),
                     quoteaf_n (1, శిఖరం_level_dst_name));
              *copy_into_self = true;
              వెళ్ళు un_backup;
            }
          లేకపోతే ఒకవేళ (same_nameat (dst_dirfd, dst_relname,
                                dst_dirfd, earlier_file))
            {
              error (0, 0, _("warning: source directory %s "
                             "specified more than once"),
                     quoteaf (శిఖరం_level_src_name));
              /* In move mode, if a previous rename succeeded, then
                 we won't be in this path as the source is missing.  If the
                 rename previously failed, then that has been handled, so
                 pretend this attempt succeeded so the source isn't removed.  */
              ఒకవేళ (x->move_బహుళకం && rename_succeeded)
                *rename_succeeded = true;
              /* We only do backups in move mode, and for non directories.
                 So just ignore this repeated entry.  */
              తిరిగి true;
            }
          లేకపోతే ఒకవేళ (x->dereference == DEREF_ALWAYS
                   || (command_పంక్తి_arg
                       && x->dereference == DEREF_COMMAND_పంక్తి_ARGUMENTS))
            {
              /* This happens when e.g., encountering a directory for the
                 second or subsequent time via symlinks when cp is invoked
                 with -R and -L.  E.g.,
                 rm -rf a b c d; mkdir a b c d; ln -s ../c a; ln -s ../c b;
                 cp -RL a b d
              */
            }
          లేకపోతే
            {
              అక్షరం *earlier = subst_suffix (dst_name, dst_relname,
                                            earlier_file);
              error (0, 0, _("will not create hard link %s to directory %s"),
                     quoteaf_n (0, dst_name), quoteaf_n (1, earlier));
              విడుదల (earlier);
              వెళ్ళు un_backup;
            }
        }
      లేకపోతే
        {
          ఒకవేళ (! create_hard_link (ఖాళీ, dst_dirfd, earlier_file,
                                  dst_name, dst_dirfd, dst_relname,
                                  true, x->verbose, dereference))
            వెళ్ళు un_backup;

          తిరిగి true;
        }
    }

  ఒకవేళ (x->move_బహుళకం)
    {
      ఒకవేళ (rename_errno == EEXIST)
        rename_errno = ((renameatu (AT_FDCWD, src_name, dst_dirfd, drelname,
                                    x->exchange ? RENAME_EXCHANGE : 0)
                         == 0)
                        ? 0 : errno);

      ఒకవేళ (rename_errno == 0)
        {
          ఒకవేళ (x->verbose)
            emit_verbose (x->exchange
                          ? _("exchanged %s <-> %s")
                          : _("renamed %s -> %s"),
                          src_name, dst_name, dst_backup);

          ఒకవేళ (x->సమితి_security_context)
            {
              /* -Z failures are only warnings currently.  */
              (శూన్య) సమితి_file_security_ctx (dst_name, true, x);
            }

          ఒకవేళ (rename_succeeded)
            *rename_succeeded = true;

          ఒకవేళ (command_పంక్తి_arg && !x->చివరి_file)
            {
              /* Record destination dev/ino/name, so that if we are asked
                 to overwrite that file again, we can detect it and fail.  */
              /* It's fine to use the _source_ stat buffer (src_sb) to get the
                 _destination_ dev/ino, since the rename above can't have
                 changed those, and 'mv' always uses lstat.
                 We could limit it further by operating
                 only on non-directories when !x->exchange.  */
              రికార్డు_file (x->dest_info, dst_relname, &src_sb);
            }

          తిరిగి true;
        }

      /* FIXME: someday, consider what to do when moving a directory into
         itself but when source and destination are on different devices.  */

      /* This happens when attempting to rename a directory to a
         subdirectory of itself.  */
      ఒకవేళ (rename_errno == EINVAL)
        {
          /* FIXME: this is a little fragile in that it relies on rename(2)
             failing with a specific errno value.  Expect problems on
             non-POSIX systems.  */
          error (0, 0, _("cannot move %s to a subdirectory of itself, %s"),
                 quoteaf_n (0, శిఖరం_level_src_name),
                 quoteaf_n (1, శిఖరం_level_dst_name));

          /* Note that there is no need to call forget_created here,
             (compare with the other calls in this file) since the
             destination directory didn't exist before.  */

          *copy_into_self = true;
          /* FIXME-cleanup: Don't return true here; adjust mv.c accordingly.
             The only caller that uses this code (mv.c) ends up setting its
             exit status to nonzero when copy_into_self is nonzero.  */
          తిరిగి true;
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
      ఒకవేళ (rename_errno != EXDEV || x->no_copy || x->exchange)
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
          అక్షరం స్థిరరాశి *quoted_dst_name = quoteaf_n (1, dst_name);
          ఒకవేళ (x->exchange)
            error (0, rename_errno, _("cannot exchange %s and %s"),
                   quoteaf_n (0, src_name), quoted_dst_name);
          లేకపోతే
            ఎంపిక (rename_errno)
              {
              స్థితి EDQUOT: స్థితి EEXIST: స్థితి EISDIR: స్థితి EMLINK:
              స్థితి ENOSPC: స్థితి ETXTBSY:
#ఒకవేళ ENOTEMPTY != EEXIST
              స్థితి ENOTEMPTY:
#అయితే_ముగింపు
                /* The destination must be the problem.  Don't mention
                   the source as that is more likely to confuse the user
                   than be helpful.  */
                error (0, rename_errno, _("cannot overwrite %s"),
                       quoted_dst_name);
                ఆపు;

              అప్రమేయ:
                error (0, rename_errno, _("cannot move %s to %s"),
                       quoteaf_n (0, src_name), quoted_dst_name);
                ఆపు;
              }
          forget_created (src_sb.st_ino, src_sb.st_dev);
          తిరిగి false;
        }

      /* The rename attempt has failed.  Remove any existing destination
         file so that a cross-device 'mv' acts as if it were really using
         the rename syscall.  Note both src and dst must both be directories
         or not, and this is enforced above.  Therefore we check the src_mode
         and operate on dst_name here as a tighter constraint and also because
         src_mode is readily available here.  */
      ఒకవేళ ((unlinkat (dst_dirfd, drelname,
                     S_ISDIR (src_బహుళకం) ? AT_REMOVEDIR : 0)
           != 0)
          && errno != ENOENT)
        {
          error (0, errno,
             _("inter-device move failed: %s to %s; unable to remove target"),
                 quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
          forget_created (src_sb.st_ino, src_sb.st_dev);
          తిరిగి false;
        }

      ఒకవేళ (x->verbose && !S_ISDIR (src_బహుళకం))
        emit_verbose (_("copied %s -> %s"), src_name, dst_name, dst_backup);
      new_dst = true;
    }

  /* If the ownership might change, or if it is a directory (whose
     special mode bits may change after the directory is created),
     omit some permissions at first, so unauthorized users cannot nip
     in before the file is ready.  */
  dst_బహుళకం_బిట్లు = (x->సమితి_బహుళకం ? x->బహుళకం_2 : src_బహుళకం) & CHMOD_బహుళకం_బిట్లు;
  omitted_permissions =
    (dst_బహుళకం_బిట్లు
     & (x->preserve_ownership ? S_IRWXG | S_IRWXO
        : S_ISDIR (src_బహుళకం) ? S_IWGRP | S_IWOTH
        : 0));

  delayed_ok = true;

  /* If required, set the default security context for new files.
     Also for existing files this is used as a reference
     when copying the context with --preserve=context.
     FIXME: Do we need to consider dst_mode_bits here?  */
  ఒకవేళ (! సమితి_process_security_ctx (src_name, dst_name, src_బహుళకం, new_dst, x))
    తిరిగి false;

  ఒకవేళ (S_ISDIR (src_బహుళకం))
    {
      నిర్మాణం dir_list *dir;

      /* If this directory has been copied before during the
         recursion, there is a symbolic link to an ancestor
         directory of the symbolic link.  It is impossible to
         continue to copy this, unless we've got an infinite file system.  */

      ఒకవేళ (is_ancestor (&src_sb, ancestors))
        {
          error (0, 0, _("cannot copy cyclic symbolic link %s"),
                 quoteaf (src_name));
          వెళ్ళు un_backup;
        }

      /* Insert the current directory in the list of parents.  */

      dir = alloca (పరిమాణం *dir);
      dir->మాతృ = ancestors;
      dir->st_ino = src_sb.st_ino;
      dir->st_dev = src_sb.st_dev;

      ఒకవేళ (new_dst || !S_ISDIR (dst_sb.st_బహుళకం))
        {
          /* POSIX says mkdir's behavior is implementation-defined when
             (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
             to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
             decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
          mode_t బహుళకం_2 = dst_బహుళకం_బిట్లు & ~omitted_permissions;
          ఒకవేళ (mkdirat (dst_dirfd, drelname, బహుళకం_2) != 0)
            {
              error (0, errno, _("cannot create directory %s"),
                     quoteaf (dst_name));
              వెళ్ళు un_backup;
            }

          /* We need search and write permissions to the new directory
             for writing the directory's contents. Check if these
             permissions are there.  */

          ఒకవేళ (fstatat (dst_dirfd, drelname, &dst_sb, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, errno, _("cannot stat %s"), quoteaf (dst_name));
              వెళ్ళు un_backup;
            }
          లేకపోతే ఒకవేళ ((dst_sb.st_బహుళకం & S_IRWXU) != S_IRWXU)
            {
              /* Make the new directory searchable and writable.  */

              dst_బహుళకం = dst_sb.st_బహుళకం;
              restore_dst_బహుళకం = true;

              ఒకవేళ (lchmodat (dst_dirfd, drelname, dst_బహుళకం | S_IRWXU) != 0)
                {
                  error (0, errno, _("setting permissions for %s"),
                         quoteaf (dst_name));
                  వెళ్ళు un_backup;
                }
            }

          /* Record the created directory's inode and device numbers into
             the search structure, so that we can avoid copying it again.
             Do this only for the first directory that is created for each
             source command line argument.  */
          ఒకవేళ (!*మొదటి_dir_created_చొప్పున_command_పంక్తి_arg)
            {
              remember_copied (dst_relname, dst_sb.st_ino, dst_sb.st_dev);
              *మొదటి_dir_created_చొప్పున_command_పంక్తి_arg = true;
            }

          ఒకవేళ (x->verbose)
            {
              ఒకవేళ (x->move_బహుళకం)
                రాయి (_("created directory %s\n"), quoteaf (dst_name));
              లేకపోతే
                emit_verbose ("%s -> %s", src_name, dst_name, ఖాళీ);
            }
        }
      లేకపోతే
        {
          omitted_permissions = 0;

          /* For directories, the process global context could be reset for
             descendants, so use it to set the context for existing dirs here.
             This will also give earlier indication of failure to set ctx.  */
          ఒకవేళ (x->సమితి_security_context || x->preserve_security_context)
            ఒకవేళ (! సమితి_file_security_ctx (dst_name, false, x))
              {
                ఒకవేళ (x->require_preserve_context)
                  వెళ్ళు un_backup;
              }
        }

      /* Decide whether to copy the contents of the directory.  */
      ఒకవేళ (x->one_file_system && మాతృ && మాతృ->st_dev != src_sb.st_dev)
        {
          /* Here, we are crossing a file system boundary and cp's -x option
             is in effect: so don't copy the contents of this directory. */
        }
      లేకపోతే
        {
          /* Copy the contents of the directory.  Don't just return if
             this fails -- otherwise, the failure to read a single file
             in a source directory would cause the containing destination
             directory not to have owner/perms set properly.  */
          delayed_ok = copy_dir (src_name, dst_name, dst_dirfd, dst_relname,
                                 new_dst, &src_sb, dir, x,
                                 మొదటి_dir_created_చొప్పున_command_పంక్తి_arg,
                                 copy_into_self);
        }
    }
  లేకపోతే ఒకవేళ (x->symbolic_link)
    {
      dest_is_symlink = true;
      ఒకవేళ (*src_name != '/')
        {
          /* Check that DST_NAME denotes a file in the current directory.  */
          నిర్మాణం stat dot_sb;
          నిర్మాణం stat dst_మాతృ_sb;
          అక్షరం *dst_మాతృ;
          bool లోపల_current_dir;

          dst_మాతృ = dir_name (dst_relname);

          లోపల_current_dir = ((dst_dirfd == AT_FDCWD && streq (".", dst_మాతృ))
                            /* If either stat call fails, it's ok not to report
                               the failure and say dst_name is in the current
                               directory.  Other things will fail later.  */
                            || stat (".", &dot_sb) != 0
                            || (fstatat (dst_dirfd, dst_మాతృ, &dst_మాతృ_sb,
                                         0) != 0)
                            || psame_inode (&dot_sb, &dst_మాతృ_sb));
          విడుదల (dst_మాతృ);

          ఒకవేళ (! లోపల_current_dir)
            {
              error (0, 0,
           _("%s: can make relative symbolic links only in current directory"),
                     quotef (dst_name));
              వెళ్ళు un_backup;
            }
        }

      పూర్ణాంకం err = force_symlinkat (src_name, dst_dirfd, dst_relname,
                                 x->unlink_dest_after_failed_open, -1);
      ఒకవేళ (0 < err)
        {
          error (0, err, _("cannot create symbolic link %s to %s"),
                 quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
          వెళ్ళు un_backup;
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
  లేకపోతే ఒకవేళ (x->hard_link
           && !(! CAN_HARDLINK_SYMLINKS && S_ISLNK (src_బహుళకం)
                && x->dereference == DEREF_NEVER))
    {
      bool replace = (x->unlink_dest_after_failed_open
                      || x->interactive == I_ASK_USER);
      ఒకవేళ (! create_hard_link (src_name, AT_FDCWD, src_name,
                              dst_name, dst_dirfd, dst_relname,
                              replace, false, dereference))
        వెళ్ళు un_backup;
    }
  లేకపోతే ఒకవేళ (S_ISREG (src_బహుళకం)
           || (x->copy_as_regular && !S_ISLNK (src_బహుళకం)))
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
      ఒకవేళ (! copy_reg (src_name, dst_name, dst_dirfd, dst_relname,
                      x, dst_బహుళకం_బిట్లు & S_IRWXUGO,
                      omitted_permissions, &new_dst, &src_sb))
        వెళ్ళు un_backup;
    }
  లేకపోతే ఒకవేళ (S_ISFIFO (src_బహుళకం))
    {
      /* Use mknodat, rather than mkfifoat, because the former preserves
         the special mode bits of a fifo on Solaris 10, while mkfifoat
         does not.  But fall back on mkfifoat, because on some BSD systems,
         mknodat always fails when asked to create a FIFO.  */
      mode_t బహుళకం_2 = src_బహుళకం & ~omitted_permissions;
      ఒకవేళ (mknodat (dst_dirfd, dst_relname, బహుళకం_2, 0) != 0)
        ఒకవేళ (mkfifoat (dst_dirfd, dst_relname, బహుళకం_2 & ~S_IFIFO) != 0)
          {
            error (0, errno, _("cannot create fifo %s"), quoteaf (dst_name));
            వెళ్ళు un_backup;
          }
    }
  లేకపోతే ఒకవేళ (S_ISBLK (src_బహుళకం) || S_ISCHR (src_బహుళకం) || S_ISSOCK (src_బహుళకం))
    {
      mode_t బహుళకం_2 = src_బహుళకం & ~omitted_permissions;
      ఒకవేళ (mknodat (dst_dirfd, dst_relname, బహుళకం_2, src_sb.st_rdev) != 0)
        {
          error (0, errno, _("cannot create special file %s"),
                 quoteaf (dst_name));
          వెళ్ళు un_backup;
        }
    }
  లేకపోతే ఒకవేళ (S_ISLNK (src_బహుళకం))
    {
      అక్షరం *src_link_val = areadlink_with_కొలత (src_name, src_sb.st_కొలత);
      dest_is_symlink = true;
      ఒకవేళ (src_link_val == ఖాళీ)
        {
          error (0, errno, _("cannot read symbolic link %s"),
                 quoteaf (src_name));
          వెళ్ళు un_backup;
        }

      పూర్ణాంకం symlink_err = force_symlinkat (src_link_val, dst_dirfd, dst_relname,
                                         x->unlink_dest_after_failed_open, -1);
      ఒకవేళ (0 < symlink_err && x->update == UPDATE_OLDER
          && !new_dst && S_ISLNK (dst_sb.st_బహుళకం)
          && dst_sb.st_కొలత == అక్షరశ్రేణి_పొడవు (src_link_val))
        {
          /* See if the destination is already the desired symlink.
             FIXME: This behavior isn't documented, and seems wrong
             in some cases, e.g., if the destination symlink has the
             wrong ownership, permissions, or timestamps.  */
          అక్షరం *dest_link_val =
            areadlinkat_with_కొలత (dst_dirfd, dst_relname, dst_sb.st_కొలత);
          ఒకవేళ (dest_link_val)
            {
              ఒకవేళ (streq (dest_link_val, src_link_val))
                symlink_err = 0;
              విడుదల (dest_link_val);
            }
        }
      విడుదల (src_link_val);
      ఒకవేళ (0 < symlink_err)
        {
          error (0, symlink_err, _("cannot create symbolic link %s"),
                 quoteaf (dst_name));
          వెళ్ళు un_backup;
        }

      ఒకవేళ (x->preserve_security_context)
        restore_default_fscreatecon_or_die ();

      ఒకవేళ (x->preserve_ownership)
        {
          /* Preserve the owner and group of the just-'copied'
             symbolic link, if possible.  */
          ఒకవేళ (HAVE_LCHOWN
              && (lchownat (dst_dirfd, dst_relname,
                            src_sb.st_uid, src_sb.st_gid)
                  != 0)
              && ! chown_failure_ok (x))
            {
              error (0, errno, _("failed to preserve ownership for %s"),
                     dst_name);
              ఒకవేళ (x->require_preserve)
                వెళ్ళు un_backup;
            }
          లేకపోతే
            {
              /* Can't preserve ownership of symlinks.
                 FIXME: maybe give a warning or even error for symlinks
                 in directories with the sticky bit set -- there, not
                 preserving owner/group is a potential security problem.  */
            }
        }
    }
  లేకపోతే
    {
      error (0, 0, _("%s has unknown file type"), quoteaf (src_name));
      వెళ్ళు un_backup;
    }

  /* With -Z or --preserve=context, set the context for existing files.
     Note this is done already for copy_reg() for reasons described therein.  */
  ఒకవేళ (!new_dst && !x->copy_as_regular && !S_ISDIR (src_బహుళకం)
      && (x->సమితి_security_context || x->preserve_security_context))
    {
      ఒకవేళ (! సమితి_file_security_ctx (dst_name, false, x))
        {
           ఒకవేళ (x->require_preserve_context)
             వెళ్ళు un_backup;
        }
    }

  ఒకవేళ (command_పంక్తి_arg && x->dest_info)
    {
      /* Now that the destination file is very likely to exist,
         add its info to the set.  */
      నిర్మాణం stat sb;
      ఒకవేళ (fstatat (dst_dirfd, drelname, &sb, AT_SYMLINK_NOFOLLOW) == 0)
        రికార్డు_file (x->dest_info, dst_relname, &sb);
    }

  /* If we've just created a hard-link due to cp's --link option,
     we're done.  */
  ఒకవేళ (x->hard_link && ! S_ISDIR (src_బహుళకం)
      && !(! CAN_HARDLINK_SYMLINKS && S_ISLNK (src_బహుళకం)
           && x->dereference == DEREF_NEVER))
    తిరిగి delayed_ok;

  ఒకవేళ (copied_as_regular)
    తిరిగి delayed_ok;

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

  ఒకవేళ (x->preserve_timestamps)
    {
      నిర్మాణం timespec timespec[2];
      timespec[0] = get_stat_atime (&src_sb);
      timespec[1] = get_stat_mtime (&src_sb);

      పూర్ణాంకం utimensat_flags = dest_is_symlink ? AT_SYMLINK_NOFOLLOW : 0;
      ఒకవేళ (utimensat (dst_dirfd, drelname, timespec, utimensat_flags) != 0)
        {
          error (0, errno, _("preserving times for %s"), quoteaf (dst_name));
          ఒకవేళ (x->require_preserve)
            తిరిగి false;
        }
    }

  /* Avoid calling chown if we know it's not necessary.  */
  ఒకవేళ (!dest_is_symlink && x->preserve_ownership
      && (new_dst || !SAME_OWNER_AND_సమూహం (src_sb, dst_sb)))
    {
      ఎంపిక (సమితి_owner (x, dst_name, dst_dirfd, drelname, -1,
                         &src_sb, new_dst, &dst_sb))
        {
        స్థితి -1:
          తిరిగి false;

        స్థితి 0:
          src_బహుళకం &= ~ (S_ISUID | S_ISGID | S_ISVTX);
          ఆపు;
        }
    }

  /* Set xattrs after ownership as changing owners will clear capabilities.  */
  ఒకవేళ (x->preserve_xattr && ! copy_attr (src_name, -1, dst_name, -1, x)
      && x->require_preserve_xattr && ! x->move_బహుళకం)
    తిరిగి false;

  /* The operations beyond this point may dereference a symlink.  */
  ఒకవేళ (dest_is_symlink)
    తిరిగి delayed_ok;

  సమితి_author (dst_name, -1, &src_sb);

  ఒకవేళ (x->preserve_బహుళకం || x->move_బహుళకం)
    {
      ఒకవేళ (xcopy_acl (src_name, -1, dst_name, -1, src_బహుళకం) != 0
          && x->require_preserve)
        తిరిగి false;
    }
  లేకపోతే ఒకవేళ (x->సమితి_బహుళకం)
    {
      ఒకవేళ (xset_acl (dst_name, -1, x->బహుళకం_2) != 0)
        తిరిగి false;
    }
  లేకపోతే ఒకవేళ (x->explicit_no_preserve_బహుళకం && new_dst)
    {
      పూర్ణాంకం default_permissions = S_ISDIR (src_బహుళకం) || S_ISSOCK (src_బహుళకం)
                                ? S_IRWXUGO : బహుళకం_RW_UGO;
      dst_బహుళకం = dst_sb.st_బహుళకం;
      ఒకవేళ (S_ISDIR (src_బహుళకం))  /* Keep set-group-ID for directories.  */
        default_permissions |= (dst_బహుళకం & S_ISGID);
      ఒకవేళ (xset_acl (dst_name, -1, default_permissions & ~cached_umask ()) != 0)
        తిరిగి false;
    }
  లేకపోతే
    {
      ఒకవేళ (omitted_permissions)
        {
          omitted_permissions &= ~ cached_umask ();

          ఒకవేళ (omitted_permissions && !restore_dst_బహుళకం)
            {
              /* Permissions were deliberately omitted when the file
                 was created due to security concerns.  See whether
                 they need to be re-added now.  It'd be faster to omit
                 the lstat, but deducing the current destination mode
                 is tricky in the presence of implementation-defined
                 rules for special mode bits.  */
              ఒకవేళ (new_dst && (fstatat (dst_dirfd, drelname, &dst_sb,
                                       AT_SYMLINK_NOFOLLOW)
                              != 0))
                {
                  error (0, errno, _("cannot stat %s"), quoteaf (dst_name));
                  తిరిగి false;
                }
              dst_బహుళకం = dst_sb.st_బహుళకం;
              ఒకవేళ (omitted_permissions & ~dst_బహుళకం)
                restore_dst_బహుళకం = true;
            }
        }

      ఒకవేళ (restore_dst_బహుళకం)
        {
          ఒకవేళ (lchmodat (dst_dirfd, drelname, dst_బహుళకం | omitted_permissions)
              != 0)
            {
              error (0, errno, _("preserving permissions for %s"),
                     quoteaf (dst_name));
              ఒకవేళ (x->require_preserve)
                తిరిగి false;
            }
        }
    }

  తిరిగి delayed_ok;

un_backup:

  ఒకవేళ (x->preserve_security_context)
    restore_default_fscreatecon_or_die ();

  /* We have failed to create the destination file.
     If we've just added a dev/ino entry via the remember_copied
     call above (i.e., unless we've just failed to create a hard link),
     remove the entry associating the source dev/ino with the
     destination file name, so we don't try to 'preserve' a link
     to a file we didn't create.  */
  ఒకవేళ (earlier_file == ఖాళీ)
    forget_created (src_sb.st_ino, src_sb.st_dev);

  ఒకవేళ (dst_backup)
    {
      అక్షరం స్థిరరాశి *dst_relbackup = &dst_backup[dst_relname - dst_name];
      ఒకవేళ (renameat (dst_dirfd, dst_relbackup, dst_dirfd, drelname) != 0)
        error (0, errno, _("cannot un-backup %s"), quoteaf (dst_name));
      లేకపోతే
        {
          ఒకవేళ (x->verbose)
            రాయి (_("%s -> %s (unbackup)\n"),
                    quoteaf_n (0, dst_backup), quoteaf_n (1, dst_name));
        }
    }
  తిరిగి false;
}

స్థిర శూన్య
valid_options (స్థిరరాశి నిర్మాణం cp_options *co)
{
  affirm (VALID_BACKUP_TYPE (co->backup_type));
  affirm (VALID_SPARSE_బహుళకం (co->sparse_బహుళకం));
  affirm (VALID_REFLINK_బహుళకం (co->reflink_బహుళకం));
  affirm (!(co->hard_link && co->symbolic_link));
  affirm (!
          (co->reflink_బహుళకం == REFLINK_ALWAYS
           && co->sparse_బహుళకం != SPARSE_AUTO));
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

బాహ్య bool
copy (అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dst_name,
      పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
      పూర్ణాంకం nonexistent_dst, స్థిరరాశి నిర్మాణం cp_options *options,
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
  శిఖరం_level_src_name = src_name;
  శిఖరం_level_dst_name = dst_name;

  bool మొదటి_dir_created_చొప్పున_command_పంక్తి_arg = false;
  తిరిగి copy_అంతర్గత (src_name, dst_name, dst_dirfd, dst_relname,
                        nonexistent_dst, ఖాళీ, ఖాళీ,
                        options, true,
                        &మొదటి_dir_created_చొప్పున_command_పంక్తి_arg,
                        copy_into_self, rename_succeeded);
}

/* Set *X to the default options for a value of type struct cp_options.  */

బాహ్య శూన్య
cp_options_default (నిర్మాణం cp_options *x)
{
  memset (x, 0, పరిమాణం *x);
#నిర్వచితమైతే PRIV_FILE_CHOWN
  {
    priv_సమితి_t *pset = priv_allocset ();
    ఒకవేళ (!pset)
      xalloc_die ();
    ఒకవేళ (getppriv (PRIV_EFFECTIVE, pset) == 0)
      {
        x->chown_privileges = priv_ismember (pset, PRIV_FILE_CHOWN);
        x->owner_privileges = priv_ismember (pset, PRIV_FILE_OWNER);
      }
    priv_freeset (pset);
  }
#లేకపోతే
  x->chown_privileges = x->owner_privileges = (geteuid () == మూలం_UID);
#అయితే_ముగింపు
  x->rename_errno = -1;
}

/* Return true if it's OK for chown to fail, where errno is
   the error number that chown failed with and X is the copying
   option set.  */

బాహ్య bool
chown_failure_ok (నిర్మాణం cp_options స్థిరరాశి *x)
{
  /* If non-root uses -p, it's ok if we can't preserve ownership.
     But root probably wants to know, e.g. if NFS disallows it,
     or if the target system doesn't support file ownership.

     Treat EACCES like EPERM and EINVAL to work around a bug in Linux
     CIFS <https://bugs.gnu.org/65599>.  Although this means coreutils
     will ignore EACCES errors that it should report, problems should
     occur only when some other process is racing with coreutils and
     coreutils is not immune to races anyway.  */

  తిరిగి ((errno == EPERM || errno == EINVAL || errno == EACCES)
          && !x->chown_privileges);
}

/* Similarly, return true if it's OK for chmod and similar operations
   to fail, where errno is the error number that chmod failed with and
   X is the copying option set.  */

స్థిర bool
owner_failure_ok (నిర్మాణం cp_options స్థిరరాశి *x)
{
  తిరిగి ((errno == EPERM || errno == EINVAL || errno == EACCES)
          && !x->owner_privileges);
}

/* Return the user's umask, caching the result.

   FIXME: If the destination's parent directory has has a default ACL,
   some operating systems (e.g., GNU/Linux's "POSIX" ACLs) use that
   ACL's mask rather than the process umask.  Currently, the callers
   of cached_umask incorrectly assume that this situation cannot occur.  */
బాహ్య mode_t
cached_umask (శూన్య)
{
  స్థిర mode_t ముసుగు;
  స్థిర bool cached;
  ఒకవేళ (!cached)
    {
      cached = true;
      ముసుగు = umask (0);
      umask (ముసుగు);
    }
  తిరిగి ముసుగు;
}
