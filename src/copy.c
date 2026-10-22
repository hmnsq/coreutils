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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/ioctl.h>
#समावेश <sys/types.h>
#समावेश <selinux/selinux.h>

#अगर HAVE_HURD_H
# include <hurd.h>
#पूर्ण_अगर
#अगर HAVE_PRIV_H
# include <priv.h>
#पूर्ण_अगर

#समावेश "system.h"
#समावेश "acl.h"
#समावेश "assure.h"
#समावेश "backupfile.h"
#समावेश "canonicalize.h"
#समावेश "copy.h"
#समावेश "cp-hash.h"
#समावेश "fcntl--.h"
#समावेश "file-set.h"
#समावेश "filemode.h"
#समावेश "filenamecat.h"
#समावेश "force-link.h"
#समावेश "hash.h"
#समावेश "hashcode-file.h"
#समावेश "ignore-value.h"
#समावेश "issymlinkat.h"
#समावेश "quote.h"
#समावेश "renameatu.h"
#समावेश "root-uid.h"
#समावेश "same.h"
#समावेश "savedir.h"
#समावेश "stat-size.h"
#समावेश "stat-time.h"
#समावेश "utimecmp.h"
#समावेश "utimens.h"
#समावेश "write-any-file.h"
#समावेश "areadlink.h"
#समावेश "yesno.h"
#समावेश "selinux.h"

#अगर_अघोषित USE_XATTR
# define USE_XATTR false
#पूर्ण_अगर

#अगर USE_XATTR
# include <attr/error_context.h>
# include <attr/libattr.h>
# include <stdarg.h>
#पूर्ण_अगर

#अगर HAVE_LINUX_FALLOC_H
# include <linux/falloc.h>
#पूर्ण_अगर

/* See HAVE_FALLOCATE workaround when including this file.  */
#अगर_घोषित HAVE_LINUX_FS_H
# include <linux/fs.h>
#पूर्ण_अगर

#अगर !defined FICLONE && defined __linux__
# define FICLONE _IOW (0x94, 9, पूर्णांक)
#पूर्ण_अगर

#अगर HAVE_FCLONEFILEAT && !USE_XATTR
# include <sys/clonefile.h>
#पूर्ण_अगर

#अगर_अघोषित USE_ACL
# define USE_ACL 0
#पूर्ण_अगर

#घोषणा SAME_OWNER(A, B) ((A).st_uid == (B).st_uid)
#घोषणा SAME_GROUP(A, B) ((A).st_gid == (B).st_gid)
#घोषणा SAME_OWNER_AND_GROUP(A, B) (SAME_OWNER (A, B) && SAME_GROUP (A, B))

/* LINK_FOLLOWS_SYMLINKS is tri-state; if it is -1, we don't know
   how link() behaves, so assume we can't hardlink symlinks in that case.  */
#अगर (defined HAVE_LINKAT && ! LINKAT_SYMLINK_NOTSUP) || ! LINK_FOLLOWS_SYMLINKS
# define CAN_HARDLINK_SYMLINKS 1
#अन्यथा
# define CAN_HARDLINK_SYMLINKS 0
#पूर्ण_अगर

काष्ठा dir_list
{
  काष्ठा dir_list *parent;
  ino_t st_ino;
  dev_t st_dev;
};

/* Initial size of the cp.dest_info hash table.  */
#घोषणा DEST_INFO_INITIAL_CAPACITY 61

अटल bool copy_internal (अक्षर स्थिर *src_name, अक्षर स्थिर *dst_name,
                           पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
                           पूर्णांक nonexistent_dst, काष्ठा stat स्थिर *parent,
                           काष्ठा dir_list *ancestors,
                           स्थिर काष्ठा cp_options *x,
                           bool command_पंक्ति_arg,
                           bool *first_dir_created_per_command_पंक्ति_arg,
                           bool *copy_into_self,
                           bool *rename_succeeded);
अटल bool owner_failure_ok (काष्ठा cp_options स्थिर *x);

/* Pointers to the file names:  they're used in the diagnostic that is issued
   when we detect the user is trying to copy a directory into itself.  */
अटल अक्षर स्थिर *शीर्ष_level_src_name;
अटल अक्षर स्थिर *शीर्ष_level_dst_name;

/* debug info about the last file copy.  */
अटल काष्ठा copy_debug copy_debug;

अटल अक्षर स्थिर *
copy_debug_string (क्रमागत copy_debug_val debug_val)
{
  चयन (debug_val)
    {
    हाल COPY_DEBUG_NO: वापस "no";
    हाल COPY_DEBUG_YES: वापस "yes";
    हाल COPY_DEBUG_AVOIDED: वापस "avoided";
    हाल COPY_DEBUG_UNKNOWN: वापस "unknown";

    हाल COPY_DEBUG_EXTERNAL:
    हाल COPY_DEBUG_EXTERNAL_INTERNAL:
    हाल COPY_DEBUG_MAX:
    शेष:
      अगर (0 < debug_val)
        वापस म_त्रुटि (debug_val);
      affirm (false);
    }
}

अटल अक्षर स्थिर *
copy_debug_sparse_string (क्रमागत copy_debug_val debug_val)
{
  चयन (debug_val)
    {
    हाल COPY_DEBUG_NO: वापस "no";
    हाल COPY_DEBUG_YES: वापस "zeros";
    हाल COPY_DEBUG_EXTERNAL: वापस "SEEK_HOLE";
    हाल COPY_DEBUG_EXTERNAL_INTERNAL: वापस "SEEK_HOLE + zeros";
    हाल COPY_DEBUG_UNKNOWN: वापस "unknown";

    हाल COPY_DEBUG_AVOIDED:
    हाल COPY_DEBUG_MAX:
    शेष: affirm (false);
    }
}

/* Print --debug output on standard output.  */
अटल व्योम
emit_debug (स्थिर काष्ठा cp_options *x)
{
  अगर (! x->hard_link && ! x->symbolic_link && x->तथ्य_copy_required)
    म_लिखो ("copy offload: %s, reflink: %s, sparse detection: %s\n",
            copy_debug_string (copy_debug.offload),
            copy_debug_string (copy_debug.reflink),
            copy_debug_sparse_string (copy_debug.sparse_detection));
}

#अगर_अघोषित DEV_FD_MIGHT_BE_CHR
# define DEV_FD_MIGHT_BE_CHR false
#पूर्ण_अगर

/* Act like fstat (DIRFD, FILENAME, ST, FLAGS), except when following
   symbolic links on Solaris-like systems, treat any character-special
   device like /dev/fd/0 as if it were the file it is open on.  */
अटल पूर्णांक
follow_fstatat (पूर्णांक dirfd, अक्षर स्थिर *filename, काष्ठा stat *st, पूर्णांक flags)
{
  पूर्णांक परिणाम = fstatat (dirfd, filename, st, flags);

  अगर (DEV_FD_MIGHT_BE_CHR && परिणाम == 0 && !(flags & AT_SYMLINK_NOFOLLOW)
      && S_ISCHR (st->st_mode))
    {
      अटल dev_t stdin_rdev;
      अटल चिन्हित अक्षर stdin_rdev_status;
      अगर (stdin_rdev_status == 0)
        {
          काष्ठा stat stdin_st;
          अगर (stat ("/dev/stdin", &stdin_st) == 0 && S_ISCHR (stdin_st.st_mode)
              && minor (stdin_st.st_rdev) == STDIN_FILENO)
            {
              stdin_rdev = stdin_st.st_rdev;
              stdin_rdev_status = 1;
            }
          अन्यथा
            stdin_rdev_status = -1;
        }
      अगर (0 < stdin_rdev_status && major (stdin_rdev) == major (st->st_rdev))
        परिणाम = ख_स्थिति (minor (st->st_rdev), st);
    }

  वापस परिणाम;
}

/* Perform the O(1) btrfs clone operation, if possible.
   Upon success, return 0.  Otherwise, return -1 and set errno.  */
अटल अंतरभूत पूर्णांक
clone_file (MAYBE_UNUSED पूर्णांक dest_fd, MAYBE_UNUSED पूर्णांक src_fd)
{
#अगर_घोषित FICLONE
  वापस ioctl (dest_fd, FICLONE, src_fd);
#अन्यथा
  त्रुटि_सं = ENOTSUP;
  वापस -1;
#पूर्ण_अगर
}

/* FIXME: describe */
/* FIXME: rewrite this to use a hash table so we avoid the quadratic
   performance hit that's probably noticeable only on trees deeper
   than a few hundred levels.  See use of active_dir_map in remove.c  */

ATTRIBUTE_PURE
अटल bool
is_ancestor (स्थिर काष्ठा stat *sb, स्थिर काष्ठा dir_list *ancestors)
{
  जबतक (ancestors)
    {
      अगर (PSAME_INODE (ancestors, sb))
        वापस true;
      ancestors = ancestors->parent;
    }
  वापस false;
}

अटल bool
errno_unsupported (पूर्णांक err)
{
  वापस err == ENOTSUP || err == ENODATA;
}

#अगर USE_XATTR
ATTRIBUTE_FORMAT ((म_लिखो, 2, 3))
अटल व्योम
copy_attr_error (MAYBE_UNUSED काष्ठा error_context *ctx,
                 अक्षर स्थिर *fmt, ...)
{
  अगर (!errno_unsupported (त्रुटि_सं))
    {
      पूर्णांक err = त्रुटि_सं;
      बहु_सूची ap;

      /* use verror module to print error message */
      बहु_शुरू (ap, fmt);
      verror (0, err, fmt, ap);
      बहु_पूर्ण (ap);
    }
}

ATTRIBUTE_FORMAT ((म_लिखो, 2, 3))
अटल व्योम
copy_attr_allerror (MAYBE_UNUSED काष्ठा error_context *ctx,
                    अक्षर स्थिर *fmt, ...)
{
  पूर्णांक err = त्रुटि_सं;
  बहु_सूची ap;

  /* use verror module to print error message */
  बहु_शुरू (ap, fmt);
  verror (0, err, fmt, ap);
  बहु_पूर्ण (ap);
}

अटल अक्षर स्थिर *
copy_attr_quote (MAYBE_UNUSED काष्ठा error_context *ctx, अक्षर स्थिर *str)
{
  वापस quoteaf (str);
}

अटल व्योम
copy_attr_free (MAYBE_UNUSED काष्ठा error_context *ctx,
                MAYBE_UNUSED अक्षर स्थिर *str)
{
}

/* Exclude SELinux extended attributes that are otherwise handled,
   and are problematic to copy again.  Also honor attributes
   configured for exclusion in /etc/xattr.conf.
   FIXME: Should we handle POSIX ACLs similarly?
   Return zero to skip.  */
अटल पूर्णांक
check_selinux_attr (अक्षर स्थिर *name, काष्ठा error_context *ctx)
{
  वापस STRNCMP_LIT (name, "security.selinux")
         && attr_copy_check_permissions (name, ctx);
}

/* If positive SRC_FD and DST_FD descriptors are passed,
   then copy by fd, otherwise copy by name.  */

अटल bool
copy_attr (अक्षर स्थिर *src_path, पूर्णांक src_fd,
           अक्षर स्थिर *dst_path, पूर्णांक dst_fd, काष्ठा cp_options स्थिर *x)
{
  bool all_errors = (!x->तथ्य_copy_required || x->require_preserve_xattr);
  bool some_errors = (!all_errors && !x->reduce_diagnostics);
  पूर्णांक (*check) (अक्षर स्थिर *, काष्ठा error_context *)
    = (x->preserve_security_context || x->set_security_context
       ? check_selinux_attr : शून्य);

# अगर 4 < __GNUC__ + (8 <= __GNUC_MINOR__)
  /* Pacify gcc -Wsuggest-attribute=format through at least GCC 13.2.1.  */
#  pragma GCC diagnostic धकेलो
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
# endif
  काष्ठा error_context *ctx
    = (all_errors || some_errors
       ? (&(काष्ठा error_context) {
           .error = all_errors ? copy_attr_allerror : copy_attr_error,
           .quote = copy_attr_quote,
           .quote_free = copy_attr_free
         })
       : शून्य);
# अगर 4 < __GNUC__ + (8 <= __GNUC_MINOR__)
#  pragma GCC diagnostic निकालो
# endif

  वापस ! (0 <= src_fd && 0 <= dst_fd
            ? attr_copy_fd (src_path, src_fd, dst_path, dst_fd, check, ctx)
            : attr_copy_file (src_path, dst_path, check, ctx));
}
#अन्यथा /* USE_XATTR */

अटल bool
copy_attr (MAYBE_UNUSED अक्षर स्थिर *src_path,
           MAYBE_UNUSED पूर्णांक src_fd,
           MAYBE_UNUSED अक्षर स्थिर *dst_path,
           MAYBE_UNUSED पूर्णांक dst_fd,
           MAYBE_UNUSED काष्ठा cp_options स्थिर *x)
{
  वापस true;
}
#पूर्ण_अगर /* USE_XATTR */

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

अटल bool
copy_dir (अक्षर स्थिर *src_name_in, अक्षर स्थिर *dst_name_in,
          पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname_in, bool new_dst,
          स्थिर काष्ठा stat *src_sb, काष्ठा dir_list *ancestors,
          स्थिर काष्ठा cp_options *x,
          bool *first_dir_created_per_command_पंक्ति_arg,
          bool *copy_into_self)
{
  अक्षर *name_space;
  अक्षर *namep;
  काष्ठा cp_options non_command_पंक्ति_options = *x;
  bool ok = true;

  name_space = savedir (src_name_in, SAVEDIR_SORT_FASTREAD);
  अगर (name_space == शून्य)
    {
      /* This diagnostic is a bit vague because savedir can fail in
         several different ways.  */
      error (0, त्रुटि_सं, _("cannot access %s"), quoteaf (src_name_in));
      वापस false;
    }

  /* For cp's -H option, dereference command line arguments, but do not
     dereference symlinks that are found via recursive traversal.  */
  अगर (x->dereference == DEREF_COMMAND_पंक्ति_ARGUMENTS)
    non_command_पंक्ति_options.dereference = DEREF_NEVER;

  bool new_first_dir_created = false;
  namep = name_space;
  जबतक (*namep != '\0')
    {
      bool local_copy_into_self;
      अक्षर *src_name = file_name_concat (src_name_in, namep, शून्य);
      अक्षर *dst_name = file_name_concat (dst_name_in, namep, शून्य);
      bool first_dir_created = *first_dir_created_per_command_पंक्ति_arg;
      bool rename_succeeded;

      ok &= copy_internal (src_name, dst_name, dst_dirfd,
                           dst_name + (dst_relname_in - dst_name_in),
                           new_dst, src_sb,
                           ancestors, &non_command_पंक्ति_options, false,
                           &first_dir_created,
                           &local_copy_into_self, &rename_succeeded);
      *copy_into_self |= local_copy_into_self;

      मुक्त (dst_name);
      मुक्त (src_name);

      /* If we're copying into self, there's no point in continuing,
         and in fact, that would even infloop, now that we record only
         the first created directory per command line argument.  */
      अगर (local_copy_into_self)
        अवरोध;

      new_first_dir_created |= first_dir_created;
      namep += म_माप (namep) + 1;
    }
  मुक्त (name_space);
  *first_dir_created_per_command_पंक्ति_arg = new_first_dir_created;

  वापस ok;
}

/* Change the file mode bits of the file identified by DESC or
   DIRFD+NAME to MODE.  Use DESC if DESC is valid and fchmod is
   available, DIRFD+NAME otherwise.  */

अटल पूर्णांक
fchmod_or_lchmod (पूर्णांक desc, पूर्णांक dirfd, अक्षर स्थिर *name, mode_t mode)
{
#अगर HAVE_FCHMOD
  अगर (0 <= desc)
    वापस fchmod (desc, mode);
#पूर्ण_अगर
  वापस lchmodat (dirfd, name, mode);
}

/* Change the ownership of the file identified by DESC or
   DIRFD+NAME to UID+GID.  Use DESC if DESC is valid and fchown is
   available, DIRFD+NAME otherwise.  */

अटल पूर्णांक
fchown_or_lchown (पूर्णांक desc, पूर्णांक dirfd, अक्षर स्थिर *name, uid_t uid, gid_t gid)
{
#अगर HAVE_FCHOWN
  अगर (0 <= desc)
    वापस fchown (desc, uid, gid);
#पूर्ण_अगर
  वापस lchownat (dirfd, name, uid, gid);
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

अटल पूर्णांक
set_owner (स्थिर काष्ठा cp_options *x, अक्षर स्थिर *dst_name,
           पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname, पूर्णांक dest_desc,
           काष्ठा stat स्थिर *src_sb, bool new_dst,
           काष्ठा stat स्थिर *dst_sb)
{
  uid_t uid = src_sb->st_uid;
  gid_t gid = src_sb->st_gid;

  /* Naively changing the ownership of an already-existing file before
     changing its permissions would create a window of vulnerability if
     the file's old permissions are too generous for the new owner and
     group.  Avoid the window by first changing to a restrictive
     temporary mode if necessary.  */

  अगर (!new_dst && (x->preserve_mode || x->move_mode || x->set_mode))
    {
      mode_t old_mode = dst_sb->st_mode;
      mode_t new_mode =
        (x->preserve_mode || x->move_mode ? src_sb->st_mode : x->mode);
      mode_t restrictive_अस्थायी_mode = old_mode & new_mode & S_IRWXU;

      अगर ((USE_ACL
           || (old_mode & CHMOD_MODE_BITS
               & (~new_mode | S_ISUID | S_ISGID | S_ISVTX)))
          && qset_acl (dst_name, dest_desc, restrictive_अस्थायी_mode) != 0)
        {
          अगर (! owner_failure_ok (x))
            error (0, त्रुटि_सं, _("clearing permissions for %s"),
                   quoteaf (dst_name));
          वापस -x->require_preserve;
        }
    }

  अगर (fchown_or_lchown (dest_desc, dst_dirfd, dst_relname, uid, gid) == 0)
    वापस 1;

  /* The ownership change failed.  If the failure merely means we lack
     privileges to change owner+group, try to change just the group
     and ignore any failure of this.  Otherwise, report an error.  */
  अगर (chown_failure_ok (x))
    ignore_मूल्य (fchown_or_lchown (dest_desc, dst_dirfd, dst_relname,
                                    -1, gid));
  अन्यथा
    {
      error (0, त्रुटि_सं, _("failed to preserve ownership for %s"),
             quoteaf (dst_name));
      अगर (x->require_preserve)
        वापस -1;
    }

  वापस 0;
}

/* Set the st_author field of DEST_DESC to the st_author field of
   SRC_SB. If DEST_DESC is undefined (-1), set the st_author field
   of DST_NAME instead.  DEST_DESC must refer to the same file as
   DST_NAME if defined.  */

अटल व्योम
set_author (MAYBE_UNUSED अक्षर स्थिर *dst_name, MAYBE_UNUSED पूर्णांक dest_desc,
            MAYBE_UNUSED स्थिर काष्ठा stat *src_sb)
{
#अगर HAVE_STRUCT_STAT_ST_AUTHOR
  /* FIXME: Modify the following code so that it does not
     follow symbolic links.  */

  /* Preserve the st_author field.  */
  file_t file = (dest_desc < 0
                 ? file_name_lookup (dst_name, 0, 0)
                 : getdport (dest_desc));
  अगर (file == MACH_PORT_NULL)
    error (0, त्रुटि_सं, _("failed to lookup file %s"), quoteaf (dst_name));
  अन्यथा
    {
      error_t err = file_chauthor (file, src_sb->st_author);
      अगर (err)
        error (0, err, _("failed to preserve authorship for %s"),
               quoteaf (dst_name));
      mach_port_deallocate (mach_task_self (), file);
    }
#पूर्ण_अगर
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
set_process_security_ctx (अक्षर स्थिर *src_name, अक्षर स्थिर *dst_name,
                          mode_t mode, bool new_dst, स्थिर काष्ठा cp_options *x)
{
  अगर (x->preserve_security_context)
    {
      /* Set the default context for the process to match the source.  */
      bool all_errors = !x->तथ्य_copy_required || x->require_preserve_context;
      bool some_errors = !all_errors && !x->reduce_diagnostics;
      अक्षर *con_raw;

      अगर (0 <= lgetfilecon_raw (src_name, &con_raw))
        {
          अगर (setfscreatecon_raw (con_raw) < 0)
            {
              अगर (all_errors || (some_errors && !errno_unsupported (त्रुटि_सं)))
                error (0, त्रुटि_सं,
                       _("failed to set default file creation context to %s"),
                       quote (con_raw));
              अगर (x->require_preserve_context)
                {
                  freecon (con_raw);
                  वापस false;
                }
            }
          freecon (con_raw);
        }
      अन्यथा
        {
          अगर (all_errors || (some_errors && !errno_unsupported (त्रुटि_सं)))
            {
              error (0, त्रुटि_सं,
                     _("failed to get security context of %s"),
                     quoteaf (src_name));
            }
          अगर (x->require_preserve_context)
            वापस false;
        }
    }
  अन्यथा अगर (x->set_security_context)
    {
      /* With -Z, adjust the default context for the process
         to have the type component adjusted as per the destination path.  */
      अगर (new_dst && defaultcon (x->set_security_context, dst_name, mode) < 0
          && ! ignorable_ctx_err (त्रुटि_सं))
        {
          error (0, त्रुटि_सं,
                 _("failed to set default file creation context for %s"),
                 quoteaf (dst_name));
        }
    }

  वापस true;
}

/* Reset the security context of DST_NAME, to that already set
   as the process default if !X->set_security_context.  Otherwise
   adjust the type component of DST_NAME's security context as
   per the system default for that path.  Issue warnings upon
   failure, when allowed by various settings in X.
   Return false on failure, true on success.  */

bool
set_file_security_ctx (अक्षर स्थिर *dst_name,
                       bool recurse, स्थिर काष्ठा cp_options *x)
{
  bool all_errors = (!x->तथ्य_copy_required
                     || x->require_preserve_context);
  bool some_errors = !all_errors && !x->reduce_diagnostics;

  अगर (! restorecon (x->set_security_context, dst_name, recurse))
    {
      अगर (all_errors || (some_errors && !errno_unsupported (त्रुटि_सं)))
        error (0, त्रुटि_सं, _("failed to set the security context of %s"),
               quoteaf_n (0, dst_name));
      वापस false;
    }

  वापस true;
}

#अगर HAVE_FCLONEFILEAT && !USE_XATTR
# include <sys/acl.h>
/* Return true if FD has a nontrivial ACL.  */
अटल bool
fd_has_acl (पूर्णांक fd)
{
  /* Every platform with fclonefileat (macOS 10.12 or later) also has
     acl_get_fd_np.  */
  bool has_acl = false;
  acl_t acl = acl_get_fd_np (fd, ACL_TYPE_EXTENDED);
  अगर (acl)
    {
      acl_entry_t ace;
      has_acl = 0 <= acl_get_entry (acl, ACL_FIRST_ENTRY, &ace);
      acl_free (acl);
    }
  वापस has_acl;
}
#पूर्ण_अगर

/* Handle failure from FICLONE or fclonefileat.
   Return FALSE if it's a terminal failure for this file.  */

अटल bool
handle_clone_fail (पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
                   अक्षर स्थिर *src_name, अक्षर स्थिर *dst_name,
                   bool new_dst, क्रमागत Reflink_type reflink_mode)
{
  /* Record errno for debugging, but diagnose the failure and return
     false only if either --reflink=always, or errno is so serious
     that we should not fall back on read+write copying,
     which can succeed even if cloning fails due to ENOSPC etc.
     E.g., XFS has Allocation Groups where a clone may fail but a copy
     (to other groups) may succeed.  The only known potentially
     serious errno value is EIO, due to Linux kernel bugs reported
     in 2023 <https://bugs.gnu.org/60489>.  */

  copy_debug.reflink = त्रुटि_सं;
  अगर (! (reflink_mode == REFLINK_ALWAYS || त्रुटि_सं == EIO))
    वापस true;

  error (0, त्रुटि_सं, _("failed to clone %s from %s"),
         quoteaf_n (0, dst_name), quoteaf_n (1, src_name));

  /* Remove the destination if cloning created it but later failed,
     either due to EIO, or to any error if --reflink=always.  The need
     for removal is a Linux kernel bug, as ioctl with FICLONE is
     supposed to be atomic.  We don't know whether currently supported
     kernels have the problem, so play it safe.  */
  अगर (new_dst /* currently not for fclonefileat().  */
      && unlinkat (dst_dirfd, dst_relname, 0) < 0 && त्रुटि_सं != ENOENT)
    error (0, त्रुटि_सं, _("cannot remove %s"), quoteaf (dst_name));

  वापस false;
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

अटल bool
copy_reg (अक्षर स्थिर *src_name, अक्षर स्थिर *dst_name,
          पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
          स्थिर काष्ठा cp_options *x,
          mode_t dst_mode, mode_t omitted_permissions, bool *new_dst,
          काष्ठा stat *src_sb)
{
  पूर्णांक dest_desc;
  पूर्णांक dest_errno;
  पूर्णांक source_desc;
  mode_t extra_permissions;
  काष्ठा stat sb;
  काष्ठा stat src_open_sb;
  bool return_val = true;
  bool तथ्य_copy_required = x->तथ्य_copy_required;
  bool preserve_xattr = USE_XATTR & x->preserve_xattr;

  copy_debug.offload = COPY_DEBUG_UNKNOWN;
  copy_debug.reflink = x->reflink_mode ? COPY_DEBUG_UNKNOWN : COPY_DEBUG_NO;
  copy_debug.sparse_detection = COPY_DEBUG_UNKNOWN;

  source_desc = खोलो (src_name,
                      (O_RDONLY | O_द्विआधारी
                       | (x->dereference == DEREF_NEVER ? O_NOFOLLOW : 0)));
  अगर (source_desc < 0)
    {
      error (0, त्रुटि_सं, _("cannot open %s for reading"), quoteaf (src_name));
      वापस false;
    }

  अगर (ख_स्थिति (source_desc, &src_open_sb) != 0)
    {
      error (0, त्रुटि_सं, _("cannot fstat %s"), quoteaf (src_name));
      return_val = false;
      जाओ close_src_desc;
    }

  /* Compare the source dev/ino from the open file to the incoming,
     saved ones obtained via a previous call to stat.  */
  अगर (! psame_inode (src_sb, &src_open_sb))
    {
      error (0, 0,
             _("skipping file %s, as it was replaced while being copied"),
             quoteaf (src_name));
      return_val = false;
      जाओ close_src_desc;
    }

  /* Might as well tell the caller about the latest version of the
     source file status, since we have it already.  */
  *src_sb = src_open_sb;
  mode_t src_mode = src_sb->st_mode;

  /* The semantics of the following open calls are mandated
     by the specs for both cp and mv.  */
  अगर (! *new_dst)
    {
      पूर्णांक open_flags =
        O_WRONLY | O_द्विआधारी | (तथ्य_copy_required ? O_TRUNC : 0);
      dest_desc = openat (dst_dirfd, dst_relname, open_flags);
      dest_errno = त्रुटि_सं;

      /* When using cp --preserve=context to copy to an existing destination,
         reset the context as per the default context, which has already been
         set according to the src.
         When using the mutually exclusive -Z option, then adjust the type of
         the existing context according to the system default for the dest.
         Note we set the context here, _after_ the file is opened, lest the
         new context disallow that.  */
      अगर (0 <= dest_desc
          && (x->set_security_context || x->preserve_security_context))
        {
          अगर (! set_file_security_ctx (dst_name, false, x))
            {
              अगर (x->require_preserve_context)
                {
                  return_val = false;
                  जाओ close_src_and_dst_desc;
                }
            }
        }

      अगर (dest_desc < 0 && dest_errno != ENOENT
          && x->unlink_dest_after_failed_open)
        {
          अगर (unlinkat (dst_dirfd, dst_relname, 0) == 0)
            {
              अगर (x->verbose)
                म_लिखो (_("removed %s\n"), quoteaf (dst_name));
            }
          अन्यथा अगर (त्रुटि_सं != ENOENT)
            {
              error (0, त्रुटि_सं, _("cannot remove %s"), quoteaf (dst_name));
              return_val = false;
              जाओ close_src_desc;
            }

          dest_errno = ENOENT;
        }

      अगर (dest_desc < 0 && dest_errno == ENOENT)
        {
          /* Ensure there is no race where a file may be left without
             an appropriate security context.  */
          अगर (x->set_security_context)
            {
              अगर (! set_process_security_ctx (src_name, dst_name, dst_mode,
                                              true, x))
                {
                  return_val = false;
                  जाओ close_src_desc;
                }
            }

          /* Tell caller that the destination file is created.  */
          *new_dst = true;
        }
    }

  अगर (*new_dst)
    {
#अगर HAVE_FCLONEFILEAT && !USE_XATTR
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
      अगर (तथ्य_copy_required && x->reflink_mode
          && (CLONE_NOOWNERCOPY || x->preserve_ownership))
        {
          /* Try fclonefileat so long as it won't create the
             destination with unwanted permissions, which could lead
             to a security race.  */
          mode_t cloned_mode_bits = S_ISVTX | S_IRWXUGO;
          mode_t cloned_mode = src_mode & cloned_mode_bits;
          mode_t desired_mode
            = (x->preserve_mode ? src_mode & CHMOD_MODE_BITS
               : x->set_mode ? x->mode
               : ((x->explicit_no_preserve_mode ? MODE_RW_UGO : dst_mode)
                  & ~ cached_umask ()));
          अगर (! (cloned_mode & ~desired_mode))
            {
              पूर्णांक fc_flags
                = (CLONE_NOFOLLOW
                   | (x->preserve_mode ? CLONE_ACL : 0)
                   | (x->preserve_ownership ? 0 : CLONE_NOOWNERCOPY));
              पूर्णांक s = fclonefileat (source_desc, dst_dirfd, dst_relname,
                                    fc_flags);
              अगर (s != 0 && (fc_flags & CLONE_ACL) && त्रुटि_सं == EINVAL)
                {
                  fc_flags &= ~CLONE_ACL;
                  s = fclonefileat (source_desc, dst_dirfd, dst_relname,
                                    fc_flags);
                }
              अगर (s == 0)
                {
                  copy_debug.reflink = COPY_DEBUG_YES;

                  /* Update the clone's timestamps and permissions
                     as needed.  */

                  अगर (!x->preserve_timestamps)
                    {
                      काष्ठा timespec timespec[2];
                      timespec[0].tv_nsec = timespec[1].tv_nsec = UTIME_NOW;
                      अगर (utimensat (dst_dirfd, dst_relname, timespec,
                                     AT_SYMLINK_NOFOLLOW)
                          != 0)
                        {
                          error (0, त्रुटि_सं, _("updating times for %s"),
                                 quoteaf (dst_name));
                          return_val = false;
                          जाओ close_src_desc;
                        }
                    }

                  extra_permissions = desired_mode & ~cloned_mode;
                  अगर (!extra_permissions
                      && (!x->preserve_mode || (fc_flags & CLONE_ACL)
                          || !fd_has_acl (source_desc)))
                    {
                      जाओ close_src_desc;
                    }

                  /* Either some desired permissions were not cloned,
                     or ACLs were not cloned despite that being requested.  */
                  omitted_permissions = 0;
                  dest_desc = -1;
                  जाओ set_dest_mode;
                }
              अगर (! handle_clone_fail (dst_dirfd, dst_relname, src_name,
                                       dst_name,
                                       false /* We didn't create dst  */,
                                       x->reflink_mode))
                {
                  return_val = false;
                  जाओ close_src_desc;
                }
            }
          अन्यथा
            copy_debug.reflink = COPY_DEBUG_AVOIDED;
        }
      अन्यथा अगर (तथ्य_copy_required && x->reflink_mode)
        {
          अगर (! CLONE_NOOWNERCOPY)
            copy_debug.reflink = COPY_DEBUG_AVOIDED;
        }
#पूर्ण_अगर

      /* To allow copying xattrs on read-only files, create with u+w.
         This satisfies an inode permission check done by
         xattr_permission in fs/xattr.c of the GNU/Linux kernel.  */
      mode_t open_mode =
        ((dst_mode & ~omitted_permissions)
         | (preserve_xattr && !x->owner_privileges ? S_IWUSR : 0));
      extra_permissions = open_mode & ~dst_mode; /* either 0 or S_IWUSR */

      पूर्णांक open_flags = O_WRONLY | O_CREAT | O_द्विआधारी;
      dest_desc = openat (dst_dirfd, dst_relname, open_flags | O_EXCL,
                          open_mode);
      dest_errno = त्रुटि_सं;

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
      अगर (dest_desc < 0 && dest_errno == EEXIST && ! x->move_mode)
        {
          अगर (issymlinkat (dst_dirfd, dst_relname) == 1)
            {
              अगर (x->open_dangling_dest_symlink)
                {
                  dest_desc = openat (dst_dirfd, dst_relname,
                                      open_flags, open_mode);
                  dest_errno = त्रुटि_सं;
                }
              अन्यथा
                {
                  error (0, 0, _("not writing through dangling symlink %s"),
                         quoteaf (dst_name));
                  return_val = false;
                  जाओ close_src_desc;
                }
            }
        }

      /* Improve quality of diagnostic when a nonexistent dst_name
         ends in a slash and open fails with errno == EISDIR.  */
      अगर (dest_desc < 0 && dest_errno == EISDIR
          && *dst_name && dst_name[म_माप (dst_name) - 1] == '/')
        dest_errno = ENOTDIR;
    }
  अन्यथा
    {
      omitted_permissions = extra_permissions = 0;
    }

  अगर (dest_desc < 0)
    {
      error (0, dest_errno, _("cannot create regular file %s"),
             quoteaf (dst_name));
      return_val = false;
      जाओ close_src_desc;
    }

  /* --attributes-only overrides --reflink.  */
  अगर (तथ्य_copy_required && x->reflink_mode)
    {
      अगर (clone_file (dest_desc, source_desc) == 0)
        {
          तथ्य_copy_required = false;
          copy_debug.reflink = COPY_DEBUG_YES;
        }
      अन्यथा
        {
          अगर (! handle_clone_fail (dst_dirfd, dst_relname, src_name, dst_name,
                                   *new_dst, x->reflink_mode))
           {
             return_val = false;
             जाओ close_src_and_dst_desc;
           }
        }
    }

  अगर (! (तथ्य_copy_required | x->preserve_ownership | extra_permissions))
    sb.st_mode = 0;
  अन्यथा अगर (ख_स्थिति (dest_desc, &sb) != 0)
    {
      error (0, त्रुटि_सं, _("cannot fstat %s"), quoteaf (dst_name));
      return_val = false;
      जाओ close_src_and_dst_desc;
    }

  /* If extra permissions needed for copy_xattr didn't happen (e.g.,
     due to umask) chmod to add them temporarily; if that fails give
     up with extra permissions, letting copy_attr fail later.  */
  mode_t temporary_mode = sb.st_mode | extra_permissions;
  अगर (temporary_mode != sb.st_mode
      && (fchmod_or_lchmod (dest_desc, dst_dirfd, dst_relname, temporary_mode)
          != 0))
    extra_permissions = 0;

  अगर (तथ्य_copy_required
      && (copy_file_तथ्य (source_desc, &src_open_sb, 0, src_name,
                          dest_desc, &sb, 0, dst_name,
                          गिनती_MAX, x, &copy_debug)
          < 0))
    {
      return_val = false;
      जाओ close_src_and_dst_desc;
    }

  अगर (x->preserve_timestamps)
    {
      काष्ठा timespec timespec[2];
      timespec[0] = get_stat_atime (src_sb);
      timespec[1] = get_stat_mtime (src_sb);

      अगर (fdutimensat (dest_desc, dst_dirfd, dst_relname, timespec, 0) != 0)
        {
          error (0, त्रुटि_सं, _("preserving times for %s"), quoteaf (dst_name));
          अगर (x->require_preserve)
            {
              return_val = false;
              जाओ close_src_and_dst_desc;
            }
        }
    }

  /* Set ownership before xattrs as changing owners will
     clear capabilities.  */
  अगर (x->preserve_ownership && ! SAME_OWNER_AND_GROUP (*src_sb, sb))
    {
      चयन (set_owner (x, dst_name, dst_dirfd, dst_relname, dest_desc,
                         src_sb, *new_dst, &sb))
        {
        हाल -1:
          return_val = false;
          जाओ close_src_and_dst_desc;

        हाल 0:
          src_mode &= ~ (S_ISUID | S_ISGID | S_ISVTX);
          अवरोध;
        }
    }

  अगर (preserve_xattr)
    {
      अगर (!copy_attr (src_name, source_desc, dst_name, dest_desc, x)
          && x->require_preserve_xattr && ! x->move_mode)
        return_val = false;
    }

  set_author (dst_name, dest_desc, src_sb);

#अगर HAVE_FCLONEFILEAT && !USE_XATTR
set_dest_mode:
#पूर्ण_अगर
  अगर (x->preserve_mode || x->move_mode)
    {
      अगर (xcopy_acl (src_name, source_desc, dst_name, dest_desc, src_mode) != 0
          && x->require_preserve)
        return_val = false;
    }
  अन्यथा अगर (x->set_mode)
    {
      अगर (xset_acl (dst_name, dest_desc, x->mode) != 0)
        return_val = false;
    }
  अन्यथा अगर (x->explicit_no_preserve_mode && *new_dst)
    {
      अगर (xset_acl (dst_name, dest_desc, MODE_RW_UGO & ~cached_umask ()) != 0)
        return_val = false;
    }
  अन्यथा अगर (omitted_permissions | extra_permissions)
    {
      omitted_permissions &= ~ cached_umask ();
      अगर ((omitted_permissions | extra_permissions)
          && (fchmod_or_lchmod (dest_desc, dst_dirfd, dst_relname,
                                dst_mode & ~ cached_umask ())
              != 0))
        {
          error (0, त्रुटि_सं, _("preserving permissions for %s"),
                 quoteaf (dst_name));
          अगर (x->require_preserve)
            return_val = false;
        }
    }

  अगर (dest_desc < 0)
    जाओ close_src_desc;

close_src_and_dst_desc:
  अगर (बंद (dest_desc) < 0)
    {
      error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (dst_name));
      return_val = false;
    }
close_src_desc:
  अगर (बंद (source_desc) < 0)
    {
      error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (src_name));
      return_val = false;
    }

  /* Output debug info for data copying operations.  */
  अगर (x->debug)
    emit_debug (x);

  वापस return_val;
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

अटल bool
same_file_ok (अक्षर स्थिर *src_name, काष्ठा stat स्थिर *src_sb,
              पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname, काष्ठा stat स्थिर *dst_sb,
              स्थिर काष्ठा cp_options *x, bool *return_now)
{
  स्थिर काष्ठा stat *src_sb_link;
  स्थिर काष्ठा stat *dst_sb_link;
  काष्ठा stat tmp_dst_sb;
  काष्ठा stat tmp_src_sb;

  bool same_link;
  bool same = psame_inode (src_sb, dst_sb);

  *return_now = false;

  /* FIXME: this should (at the very least) be moved into the following
     if-block.  More likely, it should be removed, because it inhibits
     making backups.  But removing it will result in a change in behavior
     that will probably have to be documented -- and tests will have to
     be updated.  */
  अगर (same && x->hard_link)
    {
      *return_now = true;
      वापस true;
    }

  अगर (x->dereference == DEREF_NEVER)
    {
      same_link = same;

      /* If both the source and destination files are symlinks (and we'll
         know this here IFF preserving symlinks), then it's usually ok
         when they are distinct.  */
      अगर (S_ISLNK (src_sb->st_mode) && S_ISLNK (dst_sb->st_mode))
        {
          bool sn = same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname);
          अगर ( ! sn)
            {
              /* It's fine when we're making any type of backup.  */
              अगर (x->backup_type != no_backups)
                वापस true;

              /* Here we have two symlinks that are hard-linked together,
                 and we're not making backups.  In this unusual case, simply
                 returning true would lead to mv calling "rename(A,B)",
                 which would do nothing and return 0.  */
              अगर (same_link)
                {
                  *return_now = true;
                  वापस ! x->move_mode;
                }
            }

          वापस ! sn;
        }

      src_sb_link = src_sb;
      dst_sb_link = dst_sb;
    }
  अन्यथा
    {
      अगर (!same)
        वापस true;

      अगर (fstatat (dst_dirfd, dst_relname, &tmp_dst_sb,
                   AT_SYMLINK_NOFOLLOW) != 0
          || lstat (src_name, &tmp_src_sb) != 0)
        वापस true;

      src_sb_link = &tmp_src_sb;
      dst_sb_link = &tmp_dst_sb;

      same_link = psame_inode (src_sb_link, dst_sb_link);

      /* If both are symlinks, then it's ok, but only if the destination
         will be unlinked before being opened.  This is like the test
         above, but with the addition of the unlink_dest_before_opening
         conjunct because otherwise, with two symlinks to the same target,
         we'd end up truncating the source file.  */
      अगर (S_ISLNK (src_sb_link->st_mode) && S_ISLNK (dst_sb_link->st_mode)
          && x->unlink_dest_before_opening)
        वापस true;
    }

  /* The backup code ensures there's a copy, so it's usually ok to
     remove any destination file.  One exception is when both
     source and destination are the same directory entry.  In that
     case, moving the destination file aside (in making the backup)
     would also rename the source file and result in an error.  */
  अगर (x->backup_type != no_backups)
    {
      अगर (!same_link)
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
          अगर ( ! x->move_mode
               && x->dereference != DEREF_NEVER
               && S_ISLNK (src_sb_link->st_mode)
               && ! S_ISLNK (dst_sb_link->st_mode))
            वापस false;

          वापस true;
        }

      /* FIXME: What about case insensitive file systems ?  */
      वापस ! same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname);
    }

#अगर 0
  /* FIXME: use or remove */

  /* If we're making a backup, we'll detect the problem case in
     copy_reg because SRC_NAME will no longer exist.  Allowing
     the test to be deferred lets cp do some useful things.
     But when creating hardlinks and SRC_NAME is a symlink
     but DST_RELNAME is not we must test anyway.  */
  अगर (x->hard_link
      || !S_ISLNK (src_sb_link->st_mode)
      || S_ISLNK (dst_sb_link->st_mode))
    वापस true;

  अगर (x->dereference != DEREF_NEVER)
    वापस true;
#पूर्ण_अगर

  अगर (x->move_mode || x->unlink_dest_before_opening)
    {
      /* They may refer to the same file if we're in move mode and the
         target is a symlink.  That is ok, since we remove any existing
         destination file before opening it -- via 'rename' if they're on
         the same file system, via unlinkat otherwise.  */
      अगर (S_ISLNK (dst_sb_link->st_mode))
        वापस true;

      /* It's not ok if they're distinct hard links to the same file as
         this causes a race condition and we may lose data in this case.  */
      अगर (same_link
          && 1 < dst_sb_link->st_nlink
          && ! same_nameat (AT_FDCWD, src_name, dst_dirfd, dst_relname))
        वापस ! x->move_mode;
    }

  /* If neither is a symlink, then it's ok as long as they aren't
     hard links to the same file.  */
  अगर (!S_ISLNK (src_sb_link->st_mode) && !S_ISLNK (dst_sb_link->st_mode))
    {
      अगर (!psame_inode (src_sb_link, dst_sb_link))
        वापस true;

      /* If they are the same file, it's ok if we're making hard links.  */
      अगर (x->hard_link)
        {
          *return_now = true;
          वापस true;
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
  अगर (x->move_mode
      && S_ISLNK (src_sb->st_mode)
      && 1 < dst_sb_link->st_nlink)
    {
      अक्षर *abs_src = canonicalize_file_name (src_name);
      अगर (abs_src)
        {
          bool परिणाम = ! same_nameat (AT_FDCWD, abs_src,
                                       dst_dirfd, dst_relname);
          मुक्त (abs_src);
          वापस परिणाम;
        }
    }

  /* It's ok to recreate a destination symlink. */
  अगर (x->symbolic_link && S_ISLNK (dst_sb_link->st_mode))
    वापस true;

  अगर (x->dereference == DEREF_NEVER)
    {
      अगर ( ! S_ISLNK (src_sb_link->st_mode))
        tmp_src_sb = *src_sb_link;
      अन्यथा अगर (stat (src_name, &tmp_src_sb) != 0)
        वापस true;

      अगर ( ! S_ISLNK (dst_sb_link->st_mode))
        tmp_dst_sb = *dst_sb_link;
      अन्यथा अगर (fstatat (dst_dirfd, dst_relname, &tmp_dst_sb, 0) != 0)
        वापस true;

      अगर (!psame_inode (&tmp_src_sb, &tmp_dst_sb))
        वापस true;

      अगर (x->hard_link)
        {
          /* It's ok to attempt to hardlink the same file,
            and return early if not replacing a symlink.
            Note we need to return early to avoid a later
            unlink() of DST (when SRC is a symlink).  */
          *return_now = ! S_ISLNK (dst_sb_link->st_mode);
          वापस true;
        }
    }

  वापस false;
}

/* Return whether DST_DIRFD+DST_RELNAME, with mode MODE,
   is writable in the sense of 'mv'.
   Always consider a symbolic link to be writable.  */
अटल bool
writable_destination (पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname, mode_t mode)
{
  वापस (S_ISLNK (mode)
          || can_write_any_file ()
          || faccessat (dst_dirfd, dst_relname, W_OK, AT_EACCESS) == 0);
}

अटल bool
overwrite_ok (काष्ठा cp_options स्थिर *x, अक्षर स्थिर *dst_name,
              पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
              काष्ठा stat स्थिर *dst_sb)
{
  अगर (! writable_destination (dst_dirfd, dst_relname, dst_sb->st_mode))
    {
      अक्षर perms[12];		/* "-rwxrwxrwx " ls-style modes. */
      strmode (dst_sb->st_mode, perms);
      perms[10] = '\0';
      ख_लिखो (मानक_त्रुटि,
               (x->move_mode || x->unlink_dest_before_opening
                || x->unlink_dest_after_failed_open)
               ? _("%s: replace %s, overriding mode %04lo (%s)? ")
               : _("%s: unwritable %s (mode %04lo, %s); try anyway? "),
               program_name, quoteaf (dst_name),
               (अचिन्हित दीर्घ पूर्णांक) (dst_sb->st_mode & CHMOD_MODE_BITS),
               &perms[1]);
    }
  अन्यथा
    {
      ख_लिखो (मानक_त्रुटि, _("%s: overwrite %s? "),
               program_name, quoteaf (dst_name));
    }

  वापस yesno ();
}

/* Initialize the hash table implementing a set of F_triple entries
   corresponding to destination files.  */
बाह्य व्योम
dest_info_init (काष्ठा cp_options *x)
{
  x->dest_info
    = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                       शून्य,
                       triple_hash,
                       triple_compare,
                       triple_free);
  अगर (! x->dest_info)
    xalloc_die ();
}

/* Initialize the hash table implementing a set of F_triple entries
   corresponding to source files listed on the command line.  */
बाह्य व्योम
src_info_init (काष्ठा cp_options *x)
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
    = hash_initialize (DEST_INFO_INITIAL_CAPACITY,
                       शून्य,
                       triple_hash_no_name,
                       triple_compare,
                       triple_free);
  अगर (! x->src_info)
    xalloc_die ();
}

/* When effecting a move (e.g., for mv(1)), and given the name DST_NAME
   aka DST_DIRFD+DST_RELNAME
   of the destination and a corresponding stat buffer, DST_SB, return
   true if the logical 'move' operation should _not_ proceed.
   Otherwise, return false.
   Depending on options specified in X, this code may issue an
   interactive prompt asking whether it's ok to overwrite DST_NAME.  */
अटल bool
abandon_move (स्थिर काष्ठा cp_options *x,
              अक्षर स्थिर *dst_name,
              पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
              काष्ठा stat स्थिर *dst_sb)
{
  affirm (x->move_mode);
  वापस (x->update == UPDATE_NONE
          || x->update == UPDATE_NONE_FAIL
          || ((x->interactive == I_ASK_USER
               || (x->interactive == I_UNSPECIFIED
                   && x->stdin_tty
                   && ! writable_destination (dst_dirfd, dst_relname,
                                              dst_sb->st_mode)))
              && ! overwrite_ok (x, dst_name, dst_dirfd, dst_relname, dst_sb)));
}

/* Print --verbose output on standard output, e.g. 'new' -> 'old'.
   If BACKUP_DST_NAME is non-null, then also indicate that it is
   the name of a backup file.  */
अटल व्योम
emit_verbose (अक्षर स्थिर *format, अक्षर स्थिर *src, अक्षर स्थिर *dst,
              अक्षर स्थिर *backup_dst_name)
{
  म_लिखो (format, quoteaf_n (0, src), quoteaf_n (1, dst));
  अगर (backup_dst_name)
    म_लिखो (_(" (backup: %s)"), quoteaf (backup_dst_name));
  अक्षर_दो ('\n');
}

/* A wrapper around "setfscreatecon (NULL)" that exits upon failure.  */
अटल व्योम
restore_default_fscreatecon_or_die (व्योम)
{
  अगर (setfscreatecon (शून्य) != 0)
    error (निकास_त्रुटि, त्रुटि_सं,
           _("failed to restore the default file creation context"));
}

/* Return a newly-allocated string that is like STR
   except replace its suffix SUFFIX with NEWSUFFIX.  */
अटल अक्षर *
subst_suffix (अक्षर स्थिर *str, अक्षर स्थिर *suffix, अक्षर स्थिर *newsuffix)
{
  idx_t prefixlen = suffix - str;
  idx_t newsuffixsize = म_माप (newsuffix) + 1;
  अक्षर *r = ximalloc (prefixlen + newsuffixsize);
  स_नकल (r + prefixlen, newsuffix, newsuffixsize);
  वापस स_नकल (r, str, prefixlen);
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
अटल bool
create_hard_link (अक्षर स्थिर *src_name, पूर्णांक src_dirfd, अक्षर स्थिर *src_relname,
                  अक्षर स्थिर *dst_name, पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
                  bool replace, bool verbose, bool dereference)
{
  पूर्णांक err = force_linkat (src_dirfd, src_relname, dst_dirfd, dst_relname,
                          dereference ? AT_SYMLINK_FOLLOW : 0,
                          replace, -1);
  अगर (0 < err)
    {

      अक्षर *a_src_name = शून्य;
      अगर (!src_name)
        src_name = a_src_name = subst_suffix (dst_name, dst_relname,
                                              src_relname);
      error (0, err, _("cannot create hard link %s to %s"),
             quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
      मुक्त (a_src_name);
      वापस false;
    }
  अगर (err < 0 && verbose)
    म_लिखो (_("removed %s\n"), quoteaf (dst_name));
  वापस true;
}

/* Return true if the current file should be (tried to be) dereferenced:
   either for DEREF_ALWAYS or for DEREF_COMMAND_LINE_ARGUMENTS in the case
   where the current file is a COMMAND_LINE_ARG; otherwise return false.  */
ATTRIBUTE_PURE
अटल अंतरभूत bool
should_dereference (स्थिर काष्ठा cp_options *x, bool command_पंक्ति_arg)
{
  वापस x->dereference == DEREF_ALWAYS
         || (x->dereference == DEREF_COMMAND_पंक्ति_ARGUMENTS
             && command_पंक्ति_arg);
}

/* Return true if the source file with basename SRCBASE and status SRC_ST
   is likely to be the simple backup file for DST_DIRFD+DST_RELNAME.  */
अटल bool
source_is_dst_backup (अक्षर स्थिर *srcbase, काष्ठा stat स्थिर *src_st,
                      पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname)
{
  माप_प्रकार srcbaselen = म_माप (srcbase);
  अक्षर स्थिर *dstbase = last_component (dst_relname);
  माप_प्रकार dstbaselen = म_माप (dstbase);
  माप_प्रकार suffixlen = म_माप (simple_backup_suffix);
  अगर (! (srcbaselen == dstbaselen + suffixlen
         && memeq (srcbase, dstbase, dstbaselen)
         && streq (srcbase + dstbaselen, simple_backup_suffix)))
    वापस false;
  अक्षर *dst_back = subst_suffix (dst_relname,
                                 dst_relname + म_माप (dst_relname),
                                 simple_backup_suffix);
  काष्ठा stat dst_back_sb;
  पूर्णांक dst_back_status = fstatat (dst_dirfd, dst_back, &dst_back_sb, 0);
  मुक्त (dst_back);
  वापस dst_back_status == 0 && psame_inode (src_st, &dst_back_sb);
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
अटल bool
copy_internal (अक्षर स्थिर *src_name, अक्षर स्थिर *dst_name,
               पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
               पूर्णांक nonexistent_dst,
               काष्ठा stat स्थिर *parent,
               काष्ठा dir_list *ancestors,
               स्थिर काष्ठा cp_options *x,
               bool command_पंक्ति_arg,
               bool *first_dir_created_per_command_पंक्ति_arg,
               bool *copy_into_self,
               bool *rename_succeeded)
{
  काष्ठा stat src_sb;
  काष्ठा stat dst_sb;
  mode_t src_mode IF_LINT ( = 0);
  mode_t dst_mode IF_LINT ( = 0);
  mode_t dst_mode_bits;
  mode_t omitted_permissions;
  bool restore_dst_mode = false;
  अक्षर *earlier_file = शून्य;
  अक्षर *dst_backup = शून्य;
  अक्षर स्थिर *drelname = *dst_relname ? dst_relname : ".";
  bool delayed_ok;
  bool copied_as_regular = false;
  bool dest_is_symlink = false;
  bool have_dst_lstat = false;

  *copy_into_self = false;

  पूर्णांक rename_errno = x->rename_errno;
  अगर (x->move_mode && !x->exchange)
    {
      अगर (rename_errno < 0)
        rename_errno = (renameatu (AT_FDCWD, src_name, dst_dirfd, drelname,
                                   RENAME_NOREPLACE)
                        ? त्रुटि_सं : 0);
      nonexistent_dst = *rename_succeeded = rename_errno == 0;
    }

  अगर (rename_errno == 0
      ? !x->last_file
      : rename_errno != EEXIST
        || (x->update != UPDATE_NONE && x->update != UPDATE_NONE_FAIL))
    {
      अक्षर स्थिर *name = rename_errno == 0 ? dst_name : src_name;
      पूर्णांक dirfd = rename_errno == 0 ? dst_dirfd : AT_FDCWD;
      अक्षर स्थिर *relname = rename_errno == 0 ? drelname : src_name;
      पूर्णांक fstatat_flags
        = x->dereference == DEREF_NEVER ? AT_SYMLINK_NOFOLLOW : 0;
      अगर (follow_fstatat (dirfd, relname, &src_sb, fstatat_flags) != 0)
        {
          error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (name));
          वापस false;
        }

      src_mode = src_sb.st_mode;

      अगर (S_ISDIR (src_mode) && !x->recursive)
        {
          error (0, 0, ! x->install_mode /* cp */
                 ? _("-r not specified; omitting directory %s")
                 : _("omitting directory %s"),
                 quoteaf (src_name));
          वापस false;
        }
    }
  अन्यथा
    {
#अगर defined lint && (defined __clang__ || defined __COVERITY__)
      affirm (x->move_mode);
      स_रखो (&src_sb, 0, माप src_sb);
#पूर्ण_अगर
    }

  /* Detect the case in which the same source file appears more than
     once on the command line and no backup option has been selected.
     If so, simply warn and don't copy it the second time.
     This check is enabled only if x->src_info is non-null.  */
  अगर (command_पंक्ति_arg && x->src_info)
    {
      अगर ( ! S_ISDIR (src_mode)
           && x->backup_type == no_backups
           && seen_file (x->src_info, src_name, &src_sb))
        {
          error (0, 0, _("warning: source file %s specified more than once"),
                 quoteaf (src_name));
          वापस true;
        }

      record_file (x->src_info, src_name, &src_sb);
    }

  bool dereference = should_dereference (x, command_पंक्ति_arg);

  /* Whether the destination is (or was) known to be new, updated as
     more info comes in.  This may become true if the destination is a
     dangling symlink, in contexts where dangling symlinks should be
     treated the same as nonexistent files.  */
  bool new_dst = 0 < nonexistent_dst;

  अगर (! new_dst)
    {
      /* Normally, fill in DST_SB or set NEW_DST so that later code
         can use DST_SB if NEW_DST is false.  However, don't bother
         doing this when rename_errno == EEXIST and not updating,
         which means that even if !NEW_DST the move will be abandoned
         without looking at DST_SB.  */
      अगर (! (rename_errno == EEXIST
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
            = ((! S_ISREG (src_mode)
                && (! x->copy_as_regular
                    || (S_ISDIR (src_mode) && !x->keep_directory_symlink)
                    || S_ISLNK (src_mode)))
               || x->move_mode || x->symbolic_link || x->hard_link
               || x->backup_type != no_backups
               || x->unlink_dest_before_opening);
          अगर (!use_lstat && nonexistent_dst < 0)
            new_dst = true;
          अन्यथा अगर (0 <= follow_fstatat (dst_dirfd, drelname, &dst_sb,
                                        use_lstat ? AT_SYMLINK_NOFOLLOW : 0))
            {
              have_dst_lstat = use_lstat;
              rename_errno = EEXIST;
            }
          अन्यथा अगर (त्रुटि_सं == ENOENT)
            new_dst = true;
          अन्यथा अगर (त्रुटि_सं == ELOOP && !use_lstat
                   && x->unlink_dest_after_failed_open)
            {
              /* cp -f's destination might be a symlink loop.
                 Leave new_dst=false so that we try to unlink later.  */
            }
          अन्यथा
            {
              error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (dst_name));
              वापस false;
            }
        }

      अगर (rename_errno == EEXIST)
        {
          bool return_now = false;
          bool return_val = true;
          bool skipped = false;

          अगर ((x->update != UPDATE_NONE && x->update != UPDATE_NONE_FAIL)
              && ! same_file_ok (src_name, &src_sb, dst_dirfd, drelname,
                                 &dst_sb, x, &return_now))
            {
              error (0, 0, _("%s and %s are the same file"),
                     quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
              वापस false;
            }

          अगर (x->update == UPDATE_OLDER && !S_ISDIR (src_mode))
            {
              /* When preserving timestamps (but not moving within a file
                 system), don't worry if the destination timestamp is
                 less than the source merely because of timestamp
                 truncation.  */
              पूर्णांक options = ((x->preserve_timestamps
                              && ! (x->move_mode
                                    && dst_sb.st_dev == src_sb.st_dev))
                             ? UTIMECMP_TRUNCATE_SOURCE
                             : 0);

              अगर (0 <= utimecmpat (dst_dirfd, dst_relname, &dst_sb,
                                   &src_sb, options))
                {
                  /* We're using --update and the destination is not older
                     than the source, so do not copy or move.  Pretend the
                     rename succeeded, so the caller (if it's mv) doesn't
                     end up removing the source file.  */
                  अगर (rename_succeeded)
                    *rename_succeeded = true;

                  /* However, we still must record that we've processed
                     this src/dest pair, in case this source file is
                     hard-linked to another one.  In that case, we'll use
                     the mapping information to link the corresponding
                     destination names.  */
                  earlier_file = remember_copied (dst_relname, src_sb.st_ino,
                                                  src_sb.st_dev);
                  अगर (earlier_file)
                    {
                      /* Note we currently replace DST_NAME unconditionally,
                         even if it was a newer separate file.  */
                      अगर (! create_hard_link (शून्य, dst_dirfd, earlier_file,
                                              dst_name, dst_dirfd, dst_relname,
                                              true,
                                              x->verbose, dereference))
                        {
                          जाओ un_backup;
                        }
                    }

                  skipped = true;
                  जाओ skip;
                }
            }

          /* When there is an existing destination file, we may end up
             returning early, and hence not copying/moving the file.
             This may be due to an interactive 'negative' reply to the
             prompt about the existing file.  It may also be due to the
             use of the --no-clobber option.

             cp and mv treat -i and -f differently.  */
          अगर (x->move_mode)
            {
              अगर (abandon_move (x, dst_name, dst_dirfd, drelname, &dst_sb))
                {
                  /* Pretend the rename succeeded, so the caller (mv)
                     doesn't end up removing the source file.  */
                  अगर (rename_succeeded)
                    *rename_succeeded = true;

                  skipped = true;
                  return_val = x->update == UPDATE_NONE;
                }
            }
          अन्यथा
            {
              अगर (! S_ISDIR (src_mode)
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
          अगर (skipped)
            {
              अगर (x->update == UPDATE_NONE_FAIL)
                error (0, 0, _("not replacing %s"), quoteaf (dst_name));
              अन्यथा अगर (x->debug)
                म_लिखो (_("skipped %s\n"), quoteaf (dst_name));

              return_now = true;
            }

          अगर (return_now)
            वापस return_val;

          /* Copying a directory onto a non-directory, or vice versa,
             is ok only with --backup or --exchange.  */
          अगर (!S_ISDIR (src_mode) != !S_ISDIR (dst_sb.st_mode)
              && x->backup_type == no_backups && !x->exchange)
            {
              error (0, 0,
                     _(S_ISDIR (src_mode)
                       ? ("cannot overwrite non-directory %s "
                          "with directory %s")
                       : ("cannot overwrite directory %s "
                          "with non-directory %s")),
                     quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
              वापस false;
            }

          /* Don't let the user destroy their data, even if they try hard:
             This mv command must fail (likewise for cp):
             rm -rf a b c; mkdir a b c; touch a/f b/f; mv a/f b/f c
             Otherwise, the contents of b/f would be lost.
             In the case of 'cp', b/f would be lost if the user simulated
             a move using cp and rm.
             Nothing is lost if you use --backup=numbered or --exchange.  */
          अगर (!S_ISDIR (dst_sb.st_mode) && command_पंक्ति_arg
              && x->backup_type != numbered_backups && !x->exchange
              && seen_file (x->dest_info, dst_relname, &dst_sb))
            {
              error (0, 0,
                     _("will not overwrite just-created %s with %s"),
                     quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
              वापस false;
            }

          अक्षर स्थिर *srcbase;
          अगर (x->backup_type != no_backups
              /* Don't try to back up a destination if the last
                 component of src_name is "." or "..".  */
              && ! dot_or_dotdot (srcbase = last_component (src_name))
              /* Create a backup of each destination directory in move mode,
                 but not in copy mode.  FIXME: it might make sense to add an
                 option to suppress backup creation also for move mode.
                 That would let one use mv to merge new content into an
                 existing hierarchy.  */
              && (x->move_mode || ! S_ISDIR (dst_sb.st_mode)))
            {
              /* Fail if creating the backup file would likely destroy
                 the source file.  Otherwise, the commands:
                 cd /tmp; rm -f a a~; : > a; echo A > a~; cp --b=simple a~ a
                 would leave two zero-length files: a and a~.  */
              अगर (x->backup_type != numbered_backups
                  && source_is_dst_backup (srcbase, &src_sb,
                                           dst_dirfd, dst_relname))
                {
                  अक्षर स्थिर *fmt;
                  fmt = (x->move_mode
                 ? _("backing up %s might destroy source;  %s not moved")
                 : _("backing up %s might destroy source;  %s not copied"));
                  error (0, 0, fmt,
                         quoteaf_n (0, dst_name),
                         quoteaf_n (1, src_name));
                  वापस false;
                }

              अक्षर *tmp_backup = backup_file_rename (dst_dirfd, dst_relname,
                                                     x->backup_type);

              /* FIXME: use fts:
                 Using alloca for a file name that may be arbitrarily
                 long is not recommended.  In fact, even forming such a name
                 should be discouraged.  Eventually, this code will be rewritten
                 to use fts, so using alloca here will be less of a problem.  */
              अगर (tmp_backup)
                {
                  idx_t dirlen = dst_relname - dst_name;
                  idx_t backupsize = म_माप (tmp_backup) + 1;
                  dst_backup = alloca (dirlen + backupsize);
                  स_नकल (mempcpy (dst_backup, dst_name, dirlen),
                          tmp_backup, backupsize);
                  मुक्त (tmp_backup);
                }
              अन्यथा अगर (त्रुटि_सं != ENOENT)
                {
                  error (0, त्रुटि_सं, _("cannot backup %s"), quoteaf (dst_name));
                  वापस false;
                }
              new_dst = true;
            }
          अन्यथा अगर (! S_ISDIR (dst_sb.st_mode)
                   /* Never unlink dst_name when in move mode.  */
                   && ! x->move_mode
                   && (x->unlink_dest_before_opening
                       || (x->तथ्य_copy_required
                           && ((x->preserve_links && 1 < dst_sb.st_nlink)
                               || (x->dereference == DEREF_NEVER
                                   && ! S_ISREG (src_sb.st_mode))))
                      ))
            {
              अगर (unlinkat (dst_dirfd, dst_relname, 0) != 0 && त्रुटि_सं != ENOENT)
                {
                  error (0, त्रुटि_सं, _("cannot remove %s"), quoteaf (dst_name));
                  वापस false;
                }
              new_dst = true;
              अगर (x->verbose)
                म_लिखो (_("removed %s\n"), quoteaf (dst_name));
            }
        }
    }

  /* Ensure we don't try to copy through a symlink that was
     created by a prior call to this function.  */
  अगर (command_पंक्ति_arg
      && x->dest_info
      && ! x->move_mode
      && x->backup_type == no_backups)
    {
      /* If we did not follow symlinks above, good: use that data.
         Otherwise, use AT_SYMLINK_NOFOLLOW, in case dst_name is a symlink.  */
      काष्ठा stat tmp_buf;
      काष्ठा stat *dst_lstat_sb
        = (have_dst_lstat ? &dst_sb
           : fstatat (dst_dirfd, drelname, &tmp_buf, AT_SYMLINK_NOFOLLOW) < 0
           ? शून्य : &tmp_buf);

      /* Never copy through a symlink we've just created.  */
      अगर (dst_lstat_sb
          && S_ISLNK (dst_lstat_sb->st_mode)
          && seen_file (x->dest_info, dst_relname, dst_lstat_sb))
        {
          error (0, 0,
                 _("will not copy %s through just-created symlink %s"),
                 quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
          वापस false;
        }
    }

  /* If the source is a directory, we don't always create the destination
     directory.  So --verbose should not announce anything until we're
     sure we'll create a directory.  Also don't announce yet when moving
     so we can distinguish renames versus copies.  */
  अगर (x->verbose && !x->move_mode && !S_ISDIR (src_mode))
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

  अगर (rename_errno == 0 || x->exchange)
    earlier_file = शून्य;
  अन्यथा अगर (x->recursive && S_ISDIR (src_mode))
    {
      अगर (command_पंक्ति_arg)
        earlier_file = remember_copied (dst_relname,
                                        src_sb.st_ino, src_sb.st_dev);
      अन्यथा
        earlier_file = src_to_dest_lookup (src_sb.st_ino, src_sb.st_dev);
    }
  अन्यथा अगर (x->move_mode && src_sb.st_nlink == 1)
    {
      earlier_file = src_to_dest_lookup (src_sb.st_ino, src_sb.st_dev);
    }
  अन्यथा अगर (x->preserve_links
           && !x->hard_link
           && (1 < src_sb.st_nlink
               || (command_पंक्ति_arg
                   && x->dereference == DEREF_COMMAND_पंक्ति_ARGUMENTS)
               || x->dereference == DEREF_ALWAYS))
    {
      earlier_file = remember_copied (dst_relname,
                                      src_sb.st_ino, src_sb.st_dev);
    }

  /* Did we copy this inode somewhere else (in this command line argument)
     and therefore this is a second hard link to the inode?  */

  अगर (earlier_file)
    {
      /* Avoid damaging the destination file system by refusing to preserve
         hard-linked directories (which are found at least in Netapp snapshot
         directories).  */
      अगर (S_ISDIR (src_mode))
        {
          /* If src_name and earlier_file refer to the same directory entry,
             then warn about copying a directory into itself.  */
          अगर (same_nameat (AT_FDCWD, src_name, dst_dirfd, earlier_file))
            {
              error (0, 0, _("cannot copy a directory, %s, into itself, %s"),
                     quoteaf_n (0, शीर्ष_level_src_name),
                     quoteaf_n (1, शीर्ष_level_dst_name));
              *copy_into_self = true;
              जाओ un_backup;
            }
          अन्यथा अगर (same_nameat (dst_dirfd, dst_relname,
                                dst_dirfd, earlier_file))
            {
              error (0, 0, _("warning: source directory %s "
                             "specified more than once"),
                     quoteaf (शीर्ष_level_src_name));
              /* In move mode, if a previous rename succeeded, then
                 we won't be in this path as the source is missing.  If the
                 rename previously failed, then that has been handled, so
                 pretend this attempt succeeded so the source isn't removed.  */
              अगर (x->move_mode && rename_succeeded)
                *rename_succeeded = true;
              /* We only do backups in move mode, and for non directories.
                 So just ignore this repeated entry.  */
              वापस true;
            }
          अन्यथा अगर (x->dereference == DEREF_ALWAYS
                   || (command_पंक्ति_arg
                       && x->dereference == DEREF_COMMAND_पंक्ति_ARGUMENTS))
            {
              /* This happens when e.g., encountering a directory for the
                 second or subsequent time via symlinks when cp is invoked
                 with -R and -L.  E.g.,
                 rm -rf a b c d; mkdir a b c d; ln -s ../c a; ln -s ../c b;
                 cp -RL a b d
              */
            }
          अन्यथा
            {
              अक्षर *earlier = subst_suffix (dst_name, dst_relname,
                                            earlier_file);
              error (0, 0, _("will not create hard link %s to directory %s"),
                     quoteaf_n (0, dst_name), quoteaf_n (1, earlier));
              मुक्त (earlier);
              जाओ un_backup;
            }
        }
      अन्यथा
        {
          अगर (! create_hard_link (शून्य, dst_dirfd, earlier_file,
                                  dst_name, dst_dirfd, dst_relname,
                                  true, x->verbose, dereference))
            जाओ un_backup;

          वापस true;
        }
    }

  अगर (x->move_mode)
    {
      अगर (rename_errno == EEXIST)
        rename_errno = ((renameatu (AT_FDCWD, src_name, dst_dirfd, drelname,
                                    x->exchange ? RENAME_EXCHANGE : 0)
                         == 0)
                        ? 0 : त्रुटि_सं);

      अगर (rename_errno == 0)
        {
          अगर (x->verbose)
            emit_verbose (x->exchange
                          ? _("exchanged %s <-> %s")
                          : _("renamed %s -> %s"),
                          src_name, dst_name, dst_backup);

          अगर (x->set_security_context)
            {
              /* -Z failures are only warnings currently.  */
              (व्योम) set_file_security_ctx (dst_name, true, x);
            }

          अगर (rename_succeeded)
            *rename_succeeded = true;

          अगर (command_पंक्ति_arg && !x->last_file)
            {
              /* Record destination dev/ino/name, so that if we are asked
                 to overwrite that file again, we can detect it and fail.  */
              /* It's fine to use the _source_ stat buffer (src_sb) to get the
                 _destination_ dev/ino, since the rename above can't have
                 changed those, and 'mv' always uses lstat.
                 We could limit it further by operating
                 only on non-directories when !x->exchange.  */
              record_file (x->dest_info, dst_relname, &src_sb);
            }

          वापस true;
        }

      /* FIXME: someday, consider what to do when moving a directory into
         itself but when source and destination are on different devices.  */

      /* This happens when attempting to rename a directory to a
         subdirectory of itself.  */
      अगर (rename_errno == EINVAL)
        {
          /* FIXME: this is a little fragile in that it relies on rename(2)
             failing with a specific errno value.  Expect problems on
             non-POSIX systems.  */
          error (0, 0, _("cannot move %s to a subdirectory of itself, %s"),
                 quoteaf_n (0, शीर्ष_level_src_name),
                 quoteaf_n (1, शीर्ष_level_dst_name));

          /* Note that there is no need to call forget_created here,
             (compare with the other calls in this file) since the
             destination directory didn't exist before.  */

          *copy_into_self = true;
          /* FIXME-cleanup: Don't return true here; adjust mv.c accordingly.
             The only caller that uses this code (mv.c) ends up setting its
             exit status to nonzero when copy_into_self is nonzero.  */
          वापस true;
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
      अगर (rename_errno != EXDEV || x->no_copy || x->exchange)
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
          अक्षर स्थिर *quoted_dst_name = quoteaf_n (1, dst_name);
          अगर (x->exchange)
            error (0, rename_errno, _("cannot exchange %s and %s"),
                   quoteaf_n (0, src_name), quoted_dst_name);
          अन्यथा
            चयन (rename_errno)
              {
              हाल EDQUOT: हाल EEXIST: हाल EISDIR: हाल EMLINK:
              हाल ENOSPC: हाल ETXTBSY:
#अगर ENOTEMPTY != EEXIST
              हाल ENOTEMPTY:
#पूर्ण_अगर
                /* The destination must be the problem.  Don't mention
                   the source as that is more likely to confuse the user
                   than be helpful.  */
                error (0, rename_errno, _("cannot overwrite %s"),
                       quoted_dst_name);
                अवरोध;

              शेष:
                error (0, rename_errno, _("cannot move %s to %s"),
                       quoteaf_n (0, src_name), quoted_dst_name);
                अवरोध;
              }
          forget_created (src_sb.st_ino, src_sb.st_dev);
          वापस false;
        }

      /* The rename attempt has failed.  Remove any existing destination
         file so that a cross-device 'mv' acts as if it were really using
         the rename syscall.  Note both src and dst must both be directories
         or not, and this is enforced above.  Therefore we check the src_mode
         and operate on dst_name here as a tighter constraint and also because
         src_mode is readily available here.  */
      अगर ((unlinkat (dst_dirfd, drelname,
                     S_ISDIR (src_mode) ? AT_REMOVEDIR : 0)
           != 0)
          && त्रुटि_सं != ENOENT)
        {
          error (0, त्रुटि_सं,
             _("inter-device move failed: %s to %s; unable to remove target"),
                 quoteaf_n (0, src_name), quoteaf_n (1, dst_name));
          forget_created (src_sb.st_ino, src_sb.st_dev);
          वापस false;
        }

      अगर (x->verbose && !S_ISDIR (src_mode))
        emit_verbose (_("copied %s -> %s"), src_name, dst_name, dst_backup);
      new_dst = true;
    }

  /* If the ownership might change, or if it is a directory (whose
     special mode bits may change after the directory is created),
     omit some permissions at first, so unauthorized users cannot nip
     in before the file is ready.  */
  dst_mode_bits = (x->set_mode ? x->mode : src_mode) & CHMOD_MODE_BITS;
  omitted_permissions =
    (dst_mode_bits
     & (x->preserve_ownership ? S_IRWXG | S_IRWXO
        : S_ISDIR (src_mode) ? S_IWGRP | S_IWOTH
        : 0));

  delayed_ok = true;

  /* If required, set the default security context for new files.
     Also for existing files this is used as a reference
     when copying the context with --preserve=context.
     FIXME: Do we need to consider dst_mode_bits here?  */
  अगर (! set_process_security_ctx (src_name, dst_name, src_mode, new_dst, x))
    वापस false;

  अगर (S_ISDIR (src_mode))
    {
      काष्ठा dir_list *dir;

      /* If this directory has been copied before during the
         recursion, there is a symbolic link to an ancestor
         directory of the symbolic link.  It is impossible to
         continue to copy this, unless we've got an infinite file system.  */

      अगर (is_ancestor (&src_sb, ancestors))
        {
          error (0, 0, _("cannot copy cyclic symbolic link %s"),
                 quoteaf (src_name));
          जाओ un_backup;
        }

      /* Insert the current directory in the list of parents.  */

      dir = alloca (माप *dir);
      dir->parent = ancestors;
      dir->st_ino = src_sb.st_ino;
      dir->st_dev = src_sb.st_dev;

      अगर (new_dst || !S_ISDIR (dst_sb.st_mode))
        {
          /* POSIX says mkdir's behavior is implementation-defined when
             (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
             to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
             decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
          mode_t mode = dst_mode_bits & ~omitted_permissions;
          अगर (mkdirat (dst_dirfd, drelname, mode) != 0)
            {
              error (0, त्रुटि_सं, _("cannot create directory %s"),
                     quoteaf (dst_name));
              जाओ un_backup;
            }

          /* We need search and write permissions to the new directory
             for writing the directory's contents. Check if these
             permissions are there.  */

          अगर (fstatat (dst_dirfd, drelname, &dst_sb, AT_SYMLINK_NOFOLLOW) != 0)
            {
              error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (dst_name));
              जाओ un_backup;
            }
          अन्यथा अगर ((dst_sb.st_mode & S_IRWXU) != S_IRWXU)
            {
              /* Make the new directory searchable and writable.  */

              dst_mode = dst_sb.st_mode;
              restore_dst_mode = true;

              अगर (lchmodat (dst_dirfd, drelname, dst_mode | S_IRWXU) != 0)
                {
                  error (0, त्रुटि_सं, _("setting permissions for %s"),
                         quoteaf (dst_name));
                  जाओ un_backup;
                }
            }

          /* Record the created directory's inode and device numbers into
             the search structure, so that we can avoid copying it again.
             Do this only for the first directory that is created for each
             source command line argument.  */
          अगर (!*first_dir_created_per_command_पंक्ति_arg)
            {
              remember_copied (dst_relname, dst_sb.st_ino, dst_sb.st_dev);
              *first_dir_created_per_command_पंक्ति_arg = true;
            }

          अगर (x->verbose)
            {
              अगर (x->move_mode)
                म_लिखो (_("created directory %s\n"), quoteaf (dst_name));
              अन्यथा
                emit_verbose ("%s -> %s", src_name, dst_name, शून्य);
            }
        }
      अन्यथा
        {
          omitted_permissions = 0;

          /* For directories, the process global context could be reset for
             descendants, so use it to set the context for existing dirs here.
             This will also give earlier indication of failure to set ctx.  */
          अगर (x->set_security_context || x->preserve_security_context)
            अगर (! set_file_security_ctx (dst_name, false, x))
              {
                अगर (x->require_preserve_context)
                  जाओ un_backup;
              }
        }

      /* Decide whether to copy the contents of the directory.  */
      अगर (x->one_file_system && parent && parent->st_dev != src_sb.st_dev)
        {
          /* Here, we are crossing a file system boundary and cp's -x option
             is in effect: so don't copy the contents of this directory. */
        }
      अन्यथा
        {
          /* Copy the contents of the directory.  Don't just return if
             this fails -- otherwise, the failure to read a single file
             in a source directory would cause the containing destination
             directory not to have owner/perms set properly.  */
          delayed_ok = copy_dir (src_name, dst_name, dst_dirfd, dst_relname,
                                 new_dst, &src_sb, dir, x,
                                 first_dir_created_per_command_पंक्ति_arg,
                                 copy_into_self);
        }
    }
  अन्यथा अगर (x->symbolic_link)
    {
      dest_is_symlink = true;
      अगर (*src_name != '/')
        {
          /* Check that DST_NAME denotes a file in the current directory.  */
          काष्ठा stat dot_sb;
          काष्ठा stat dst_parent_sb;
          अक्षर *dst_parent;
          bool in_current_dir;

          dst_parent = dir_name (dst_relname);

          in_current_dir = ((dst_dirfd == AT_FDCWD && streq (".", dst_parent))
                            /* If either stat call fails, it's ok not to report
                               the failure and say dst_name is in the current
                               directory.  Other things will fail later.  */
                            || stat (".", &dot_sb) != 0
                            || (fstatat (dst_dirfd, dst_parent, &dst_parent_sb,
                                         0) != 0)
                            || psame_inode (&dot_sb, &dst_parent_sb));
          मुक्त (dst_parent);

          अगर (! in_current_dir)
            {
              error (0, 0,
           _("%s: can make relative symbolic links only in current directory"),
                     quotef (dst_name));
              जाओ un_backup;
            }
        }

      पूर्णांक err = force_symlinkat (src_name, dst_dirfd, dst_relname,
                                 x->unlink_dest_after_failed_open, -1);
      अगर (0 < err)
        {
          error (0, err, _("cannot create symbolic link %s to %s"),
                 quoteaf_n (0, dst_name), quoteaf_n (1, src_name));
          जाओ un_backup;
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
  अन्यथा अगर (x->hard_link
           && !(! CAN_HARDLINK_SYMLINKS && S_ISLNK (src_mode)
                && x->dereference == DEREF_NEVER))
    {
      bool replace = (x->unlink_dest_after_failed_open
                      || x->interactive == I_ASK_USER);
      अगर (! create_hard_link (src_name, AT_FDCWD, src_name,
                              dst_name, dst_dirfd, dst_relname,
                              replace, false, dereference))
        जाओ un_backup;
    }
  अन्यथा अगर (S_ISREG (src_mode)
           || (x->copy_as_regular && !S_ISLNK (src_mode)))
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
      अगर (! copy_reg (src_name, dst_name, dst_dirfd, dst_relname,
                      x, dst_mode_bits & S_IRWXUGO,
                      omitted_permissions, &new_dst, &src_sb))
        जाओ un_backup;
    }
  अन्यथा अगर (S_ISFIFO (src_mode))
    {
      /* Use mknodat, rather than mkfifoat, because the former preserves
         the special mode bits of a fifo on Solaris 10, while mkfifoat
         does not.  But fall back on mkfifoat, because on some BSD systems,
         mknodat always fails when asked to create a FIFO.  */
      mode_t mode = src_mode & ~omitted_permissions;
      अगर (mknodat (dst_dirfd, dst_relname, mode, 0) != 0)
        अगर (mkfifoat (dst_dirfd, dst_relname, mode & ~S_IFIFO) != 0)
          {
            error (0, त्रुटि_सं, _("cannot create fifo %s"), quoteaf (dst_name));
            जाओ un_backup;
          }
    }
  अन्यथा अगर (S_ISBLK (src_mode) || S_ISCHR (src_mode) || S_ISSOCK (src_mode))
    {
      mode_t mode = src_mode & ~omitted_permissions;
      अगर (mknodat (dst_dirfd, dst_relname, mode, src_sb.st_rdev) != 0)
        {
          error (0, त्रुटि_सं, _("cannot create special file %s"),
                 quoteaf (dst_name));
          जाओ un_backup;
        }
    }
  अन्यथा अगर (S_ISLNK (src_mode))
    {
      अक्षर *src_link_val = areadlink_with_आकार (src_name, src_sb.st_आकार);
      dest_is_symlink = true;
      अगर (src_link_val == शून्य)
        {
          error (0, त्रुटि_सं, _("cannot read symbolic link %s"),
                 quoteaf (src_name));
          जाओ un_backup;
        }

      पूर्णांक symlink_err = force_symlinkat (src_link_val, dst_dirfd, dst_relname,
                                         x->unlink_dest_after_failed_open, -1);
      अगर (0 < symlink_err && x->update == UPDATE_OLDER
          && !new_dst && S_ISLNK (dst_sb.st_mode)
          && dst_sb.st_आकार == म_माप (src_link_val))
        {
          /* See if the destination is already the desired symlink.
             FIXME: This behavior isn't documented, and seems wrong
             in some cases, e.g., if the destination symlink has the
             wrong ownership, permissions, or timestamps.  */
          अक्षर *dest_link_val =
            areadlinkat_with_आकार (dst_dirfd, dst_relname, dst_sb.st_आकार);
          अगर (dest_link_val)
            {
              अगर (streq (dest_link_val, src_link_val))
                symlink_err = 0;
              मुक्त (dest_link_val);
            }
        }
      मुक्त (src_link_val);
      अगर (0 < symlink_err)
        {
          error (0, symlink_err, _("cannot create symbolic link %s"),
                 quoteaf (dst_name));
          जाओ un_backup;
        }

      अगर (x->preserve_security_context)
        restore_default_fscreatecon_or_die ();

      अगर (x->preserve_ownership)
        {
          /* Preserve the owner and group of the just-'copied'
             symbolic link, if possible.  */
          अगर (HAVE_LCHOWN
              && (lchownat (dst_dirfd, dst_relname,
                            src_sb.st_uid, src_sb.st_gid)
                  != 0)
              && ! chown_failure_ok (x))
            {
              error (0, त्रुटि_सं, _("failed to preserve ownership for %s"),
                     dst_name);
              अगर (x->require_preserve)
                जाओ un_backup;
            }
          अन्यथा
            {
              /* Can't preserve ownership of symlinks.
                 FIXME: maybe give a warning or even error for symlinks
                 in directories with the sticky bit set -- there, not
                 preserving owner/group is a potential security problem.  */
            }
        }
    }
  अन्यथा
    {
      error (0, 0, _("%s has unknown file type"), quoteaf (src_name));
      जाओ un_backup;
    }

  /* With -Z or --preserve=context, set the context for existing files.
     Note this is done already for copy_reg() for reasons described therein.  */
  अगर (!new_dst && !x->copy_as_regular && !S_ISDIR (src_mode)
      && (x->set_security_context || x->preserve_security_context))
    {
      अगर (! set_file_security_ctx (dst_name, false, x))
        {
           अगर (x->require_preserve_context)
             जाओ un_backup;
        }
    }

  अगर (command_पंक्ति_arg && x->dest_info)
    {
      /* Now that the destination file is very likely to exist,
         add its info to the set.  */
      काष्ठा stat sb;
      अगर (fstatat (dst_dirfd, drelname, &sb, AT_SYMLINK_NOFOLLOW) == 0)
        record_file (x->dest_info, dst_relname, &sb);
    }

  /* If we've just created a hard-link due to cp's --link option,
     we're done.  */
  अगर (x->hard_link && ! S_ISDIR (src_mode)
      && !(! CAN_HARDLINK_SYMLINKS && S_ISLNK (src_mode)
           && x->dereference == DEREF_NEVER))
    वापस delayed_ok;

  अगर (copied_as_regular)
    वापस delayed_ok;

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

  अगर (x->preserve_timestamps)
    {
      काष्ठा timespec timespec[2];
      timespec[0] = get_stat_atime (&src_sb);
      timespec[1] = get_stat_mtime (&src_sb);

      पूर्णांक utimensat_flags = dest_is_symlink ? AT_SYMLINK_NOFOLLOW : 0;
      अगर (utimensat (dst_dirfd, drelname, timespec, utimensat_flags) != 0)
        {
          error (0, त्रुटि_सं, _("preserving times for %s"), quoteaf (dst_name));
          अगर (x->require_preserve)
            वापस false;
        }
    }

  /* Avoid calling chown if we know it's not necessary.  */
  अगर (!dest_is_symlink && x->preserve_ownership
      && (new_dst || !SAME_OWNER_AND_GROUP (src_sb, dst_sb)))
    {
      चयन (set_owner (x, dst_name, dst_dirfd, drelname, -1,
                         &src_sb, new_dst, &dst_sb))
        {
        हाल -1:
          वापस false;

        हाल 0:
          src_mode &= ~ (S_ISUID | S_ISGID | S_ISVTX);
          अवरोध;
        }
    }

  /* Set xattrs after ownership as changing owners will clear capabilities.  */
  अगर (x->preserve_xattr && ! copy_attr (src_name, -1, dst_name, -1, x)
      && x->require_preserve_xattr && ! x->move_mode)
    वापस false;

  /* The operations beyond this point may dereference a symlink.  */
  अगर (dest_is_symlink)
    वापस delayed_ok;

  set_author (dst_name, -1, &src_sb);

  अगर (x->preserve_mode || x->move_mode)
    {
      अगर (xcopy_acl (src_name, -1, dst_name, -1, src_mode) != 0
          && x->require_preserve)
        वापस false;
    }
  अन्यथा अगर (x->set_mode)
    {
      अगर (xset_acl (dst_name, -1, x->mode) != 0)
        वापस false;
    }
  अन्यथा अगर (x->explicit_no_preserve_mode && new_dst)
    {
      पूर्णांक default_permissions = S_ISDIR (src_mode) || S_ISSOCK (src_mode)
                                ? S_IRWXUGO : MODE_RW_UGO;
      dst_mode = dst_sb.st_mode;
      अगर (S_ISDIR (src_mode))  /* Keep set-group-ID for directories.  */
        default_permissions |= (dst_mode & S_ISGID);
      अगर (xset_acl (dst_name, -1, default_permissions & ~cached_umask ()) != 0)
        वापस false;
    }
  अन्यथा
    {
      अगर (omitted_permissions)
        {
          omitted_permissions &= ~ cached_umask ();

          अगर (omitted_permissions && !restore_dst_mode)
            {
              /* Permissions were deliberately omitted when the file
                 was created due to security concerns.  See whether
                 they need to be re-added now.  It'd be faster to omit
                 the lstat, but deducing the current destination mode
                 is tricky in the presence of implementation-defined
                 rules for special mode bits.  */
              अगर (new_dst && (fstatat (dst_dirfd, drelname, &dst_sb,
                                       AT_SYMLINK_NOFOLLOW)
                              != 0))
                {
                  error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (dst_name));
                  वापस false;
                }
              dst_mode = dst_sb.st_mode;
              अगर (omitted_permissions & ~dst_mode)
                restore_dst_mode = true;
            }
        }

      अगर (restore_dst_mode)
        {
          अगर (lchmodat (dst_dirfd, drelname, dst_mode | omitted_permissions)
              != 0)
            {
              error (0, त्रुटि_सं, _("preserving permissions for %s"),
                     quoteaf (dst_name));
              अगर (x->require_preserve)
                वापस false;
            }
        }
    }

  वापस delayed_ok;

un_backup:

  अगर (x->preserve_security_context)
    restore_default_fscreatecon_or_die ();

  /* We have failed to create the destination file.
     If we've just added a dev/ino entry via the remember_copied
     call above (i.e., unless we've just failed to create a hard link),
     remove the entry associating the source dev/ino with the
     destination file name, so we don't try to 'preserve' a link
     to a file we didn't create.  */
  अगर (earlier_file == शून्य)
    forget_created (src_sb.st_ino, src_sb.st_dev);

  अगर (dst_backup)
    {
      अक्षर स्थिर *dst_relbackup = &dst_backup[dst_relname - dst_name];
      अगर (renameat (dst_dirfd, dst_relbackup, dst_dirfd, drelname) != 0)
        error (0, त्रुटि_सं, _("cannot un-backup %s"), quoteaf (dst_name));
      अन्यथा
        {
          अगर (x->verbose)
            म_लिखो (_("%s -> %s (unbackup)\n"),
                    quoteaf_n (0, dst_backup), quoteaf_n (1, dst_name));
        }
    }
  वापस false;
}

अटल व्योम
valid_options (स्थिर काष्ठा cp_options *co)
{
  affirm (VALID_BACKUP_TYPE (co->backup_type));
  affirm (VALID_SPARSE_MODE (co->sparse_mode));
  affirm (VALID_REFLINK_MODE (co->reflink_mode));
  affirm (!(co->hard_link && co->symbolic_link));
  affirm (!
          (co->reflink_mode == REFLINK_ALWAYS
           && co->sparse_mode != SPARSE_AUTO));
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

बाह्य bool
copy (अक्षर स्थिर *src_name, अक्षर स्थिर *dst_name,
      पूर्णांक dst_dirfd, अक्षर स्थिर *dst_relname,
      पूर्णांक nonexistent_dst, स्थिर काष्ठा cp_options *options,
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
  शीर्ष_level_src_name = src_name;
  शीर्ष_level_dst_name = dst_name;

  bool first_dir_created_per_command_पंक्ति_arg = false;
  वापस copy_internal (src_name, dst_name, dst_dirfd, dst_relname,
                        nonexistent_dst, शून्य, शून्य,
                        options, true,
                        &first_dir_created_per_command_पंक्ति_arg,
                        copy_into_self, rename_succeeded);
}

/* Set *X to the default options for a value of type struct cp_options.  */

बाह्य व्योम
cp_options_default (काष्ठा cp_options *x)
{
  स_रखो (x, 0, माप *x);
#अगर_घोषित PRIV_FILE_CHOWN
  {
    priv_set_t *pset = priv_allocset ();
    अगर (!pset)
      xalloc_die ();
    अगर (getppriv (PRIV_EFFECTIVE, pset) == 0)
      {
        x->chown_privileges = priv_ismember (pset, PRIV_FILE_CHOWN);
        x->owner_privileges = priv_ismember (pset, PRIV_FILE_OWNER);
      }
    priv_freeset (pset);
  }
#अन्यथा
  x->chown_privileges = x->owner_privileges = (geteuid () == मूल_UID);
#पूर्ण_अगर
  x->rename_errno = -1;
}

/* Return true if it's OK for chown to fail, where errno is
   the error number that chown failed with and X is the copying
   option set.  */

बाह्य bool
chown_failure_ok (काष्ठा cp_options स्थिर *x)
{
  /* If non-root uses -p, it's ok if we can't preserve ownership.
     But root probably wants to know, e.g. if NFS disallows it,
     or if the target system doesn't support file ownership.

     Treat EACCES like EPERM and EINVAL to work around a bug in Linux
     CIFS <https://bugs.gnu.org/65599>.  Although this means coreutils
     will ignore EACCES errors that it should report, problems should
     occur only when some other process is racing with coreutils and
     coreutils is not immune to races anyway.  */

  वापस ((त्रुटि_सं == EPERM || त्रुटि_सं == EINVAL || त्रुटि_सं == EACCES)
          && !x->chown_privileges);
}

/* Similarly, return true if it's OK for chmod and similar operations
   to fail, where errno is the error number that chmod failed with and
   X is the copying option set.  */

अटल bool
owner_failure_ok (काष्ठा cp_options स्थिर *x)
{
  वापस ((त्रुटि_सं == EPERM || त्रुटि_सं == EINVAL || त्रुटि_सं == EACCES)
          && !x->owner_privileges);
}

/* Return the user's umask, caching the result.

   FIXME: If the destination's parent directory has has a default ACL,
   some operating systems (e.g., GNU/Linux's "POSIX" ACLs) use that
   ACL's mask rather than the process umask.  Currently, the callers
   of cached_umask incorrectly assume that this situation cannot occur.  */
बाह्य mode_t
cached_umask (व्योम)
{
  अटल mode_t mask;
  अटल bool cached;
  अगर (!cached)
    {
      cached = true;
      mask = umask (0);
      umask (mask);
    }
  वापस mask;
}
