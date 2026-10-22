/* cp.c  -- file copying (main routines)
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Torbjörn Granlund, David MacKenzie, and Jim Meyering. */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "backupfile.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "cp-hash.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "targetdir.h"
#ಅನ್ತರ್ಭಾವ "utimens.h"
#ಅನ್ತರ್ಭಾವ "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cp"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
ಸಂರಚನಾ dir_attr
{
  ಸಂರಚನಾ stat st;
  bool restore_mode;
  size_t slash_offset;
  ಸಂರಚನಾ dir_attr *next;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  ATTRIBUTES_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_ATTRIBUTES_OPTION,
  PARENTS_OPTION,
  PRESERVE_ATTRIBUTES_OPTION,
  REFLINK_OPTION,
  SPARSE_OPTION,
  STRIP_TRAILING_SLASHES_OPTION,
  UNLINK_DEST_BEFORE_OPENING,
  KEEP_DIRECTORY_SYMLINK_OPTION
};

/* True if the kernel is SELinux enabled.  */
ಸ್ಥಿರ bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
ಸ್ಥಿರ bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
ಸ್ಥಿರ bool remove_trailing_slashes;

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ sparse_type_string[] =
{
  "never", "auto", "always", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ Sparse_type ಧ್ರುವ sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ reflink_type_string[] =
{
  "auto", "always", "never", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ Reflink_type ಧ್ರುವ reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ update_type_string[] =
{
  "all", "none", "none-fail", "older", ರಿಕ್ತ
};
ಸ್ಥಿರ ಗಣನಾ Update_type ಧ್ರುವ update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_opts[] =
{
  {"archive", no_argument, ರಿಕ್ತ, 'a'},
  {"attributes-only", no_argument, ರಿಕ್ತ, ATTRIBUTES_ONLY_OPTION},
  {"backup", optional_argument, ರಿಕ್ತ, 'b'},
  {"copy-contents", no_argument, ರಿಕ್ತ, COPY_CONTENTS_OPTION},
  {"debug", no_argument, ರಿಕ್ತ, DEBUG_OPTION},
  {"dereference", no_argument, ರಿಕ್ತ, 'L'},
  {"force", no_argument, ರಿಕ್ತ, 'f'},
  {"interactive", no_argument, ರಿಕ್ತ, 'i'},
  {"link", no_argument, ರಿಕ್ತ, 'l'},
  {"no-clobber", no_argument, ರಿಕ್ತ, 'n'},   /* Deprecated.  */
  {"no-dereference", no_argument, ರಿಕ್ತ, 'P'},
  {"no-preserve", required_argument, ರಿಕ್ತ, NO_PRESERVE_ATTRIBUTES_OPTION},
  {"no-target-directory", no_argument, ರಿಕ್ತ, 'T'},
  {"one-file-system", no_argument, ರಿಕ್ತ, 'x'},
  {"parents", no_argument, ರಿಕ್ತ, PARENTS_OPTION},
  {"path", no_argument, ರಿಕ್ತ, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_argument, ರಿಕ್ತ, PRESERVE_ATTRIBUTES_OPTION},
  {"recursive", no_argument, ರಿಕ್ತ, 'R'},
  {"remove-destination", no_argument, ರಿಕ್ತ, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_argument, ರಿಕ್ತ, SPARSE_OPTION},
  {"reflink", optional_argument, ರಿಕ್ತ, REFLINK_OPTION},
  {"strip-trailing-slashes", no_argument, ರಿಕ್ತ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_argument, ರಿಕ್ತ, 'S'},
  {"symbolic-link", no_argument, ರಿಕ್ತ, 's'},
  {"target-directory", required_argument, ರಿಕ್ತ, 't'},
  {"update", optional_argument, ರಿಕ್ತ, 'u'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {"keep-directory-symlink", no_argument, ರಿಕ್ತ,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --attributes-only\n\
         don't copy the file data, just the attributes\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup of each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an argument\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents of special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  Implies -v\n\
"));
      oputs (_("\
  -f, --force\n\
         ಯದಿ an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also used)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a previous -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-ಸಾಲು symbolic links in SOURCE\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links in SOURCE\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links in SOURCE\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks to directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead of copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=mode,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified attributes\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified attributes\n\
"));
      oputs (_("\
      --parents\n\
         use full source file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See below\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting to open it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation of sparse files. See below\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each SOURCE argument\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead of copying\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --target-directory=DIRECTORY\n\
         copy all SOURCE arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-target-directory\n\
         treat DEST as a normal file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(ಸ್ವಭಾವ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent to --update[=older].  See below\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being done\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of destination file to ಸ್ವಭಾವ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಯದಿ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list of attributes. Attributes are 'mode' ಪ್ರತಿ\n\
permissions (including any ACL and xattr permissions), 'ownership' ಪ್ರತಿ user\n\
and group, 'timestamps' ಪ್ರತಿ file timestamps, 'links' ಪ್ರತಿ hard links, 'context'\
\nfor security context, 'xattr' ಪ್ರತಿ extended attributes, and 'all' ಪ್ರತಿ all\n\
attributes.\n\
"), stdout);
      fputs (_("\
\n\
By ಸ್ವಭಾವ, sparse SOURCE files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=auto.  Specify --sparse=always to create a sparse DEST\n\
file whenever the SOURCE file contains a long enough sequence of zero bytes.\n\
Use --sparse=never to inhibit creation of sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By ಸ್ವಭಾವ or with --reflink=auto, cp will try a lightweight copy, where the\n\
data blocks are copied only when modified, falling back to a standard copy\n\
ಯದಿ this is not possible.  With --reflink[=always] cp will fail ಯದಿ CoW is not\n\
supported, ಯಾವತ್ --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special ಪಕ್ಷ, cp makes a backup of SOURCE when the force and backup\n\
options are given and SOURCE and DEST are the same name ಪ್ರತಿ an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Ensure that parents of CONST_DST_NAME have correct protections, for
   the --parents option.  This is done after all copying has been
   completed, to allow permissions that don't include user write/execute.

   DST_SRC_NAME is the suffix of CONST_DST_NAME that is the source file name,
   DST_DIRFD+DST_RELNAME is equivalent to CONST_DST_NAME, and
   DST_RELNAME equals DST_SRC_NAME after skipping any leading '/'s.

   ATTR_LIST is a null-terminated linked list of structures that
   indicates the end of the filename of each intermediate directory
   in CONST_DST_NAME that may need to have its attributes changed.
   The command 'cp --parents --preserve a/b/c d/e_dir' changes the
   attributes of the directories d/e_dir/a and d/e_dir/a/b to match
   the corresponding source directories regardless of whether they
   existed before the 'cp' command was given.

   Return true if the parent of CONST_DST_NAME and any intermediate
   directories specified by ATTR_LIST have the proper permissions
   when done.  */

ಸ್ಥಿರ bool
re_protect (ವರ್ಣ ಧ್ರುವ *const_dst_name, ವರ್ಣ ಧ್ರುವ *dst_src_name,
            ಪೂರ್ಣಾಙ್ಕ dst_dirfd, ವರ್ಣ ಧ್ರುವ *dst_relname,
            ಸಂರಚನಾ dir_attr *attr_list, ಧ್ರುವ ಸಂರಚನಾ cp_options *x)
{
  ವರ್ಣ *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  ASSIGN_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  ವರ್ಣ ಧ್ರುವ *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  ವರ್ಣ ಧ್ರುವ *relname = dst_name + (dst_relname - const_dst_name);

  ಪ್ರತಿ (ಸಂರಚನಾ dir_attr *p = attr_list; p; p = p->next)
    {
      dst_name[p->slash_offset] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      ಯದಿ (x->preserve_timestamps)
        {
          ಸಂರಚನಾ timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          ಯದಿ (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              ಪ್ರತ್ಯಾಗಮ false;
            }
        }

      ಯದಿ (x->preserve_ownership)
        {
          ಯದಿ (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              ಯದಿ (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  ಪ್ರತ್ಯಾಗಮ false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_ಮೌಲ್ಯ (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      ಯದಿ (x->preserve_mode)
        {
          ಯದಿ (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_mode) != 0)
            ಪ್ರತ್ಯಾಗಮ false;
        }
      ಅನ್ಯಥಾ ಯದಿ (p->restore_mode)
        {
          ಯದಿ (lchmodat (dst_dirfd, relname, p->st.st_mode) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              ಪ್ರತ್ಯಾಗಮ false;
            }
        }

      dst_name[p->slash_offset] = '/';
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Ensure that the parent directory of CONST_DIR exists, for
   the --parents option.

   SRC_OFFSET is the index in CONST_DIR (which is a destination
   directory) of the beginning of the source directory name.
   Create any leading directories that don't already exist.
   DST_DIRFD is a file descriptor for the target directory.
   If VERBOSE_FMT_STRING is nonzero, use it as a printf format
   string for printing a message after successfully making a directory.
   The format should take two string arguments: the names of the
   source and destination directories.
   Creates a linked list of attributes of intermediate directories,
   *ATTR_LIST, for re_protect to use after calling copy.
   Sets *NEW_DST if this function creates parent of CONST_DIR.

   Return true if parent of CONST_DIR exists as a directory with the proper
   permissions when done.  */

/* FIXME: Synch this function with the one in ../lib/mkdir-p.c.  */

ಸ್ಥಿರ bool
make_dir_parents_private (ವರ್ಣ ಧ್ರುವ *const_dir, size_t src_offset,
                          ಪೂರ್ಣಾಙ್ಕ dst_dirfd,
                          ವರ್ಣ ಧ್ರುವ *verbose_fmt_string,
                          ಸಂರಚನಾ dir_attr **attr_list, bool *new_dst,
                          ಧ್ರುವ ಸಂರಚನಾ cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = ರಿಕ್ತ;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  ಯದಿ (dirlen <= src_offset)
    ಪ್ರತ್ಯಾಗಮ true;

  /* A copy of CONST_DIR we can change.  */
  ವರ್ಣ *dir;
  ASSIGN_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  ವರ್ಣ *src = dir + src_offset;

  /* Leading directory of DIR.  */
  ವರ್ಣ *dst_dir = alloca (dirlen + 1);
  memcpy (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  ವರ್ಣ ಧ್ರುವ *dst_reldir = dst_dir + src_offset;
  ಯಾವತ್ (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  ಸಂರಚನಾ stat stats;
  ಯದಿ (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      ವರ್ಣ *slash = src;

      ಯಾವತ್ (*slash == '/')
        slash++;
      dst_reldir = slash;

      ಯಾವತ್ ((slash = strchr (slash, '/')))
        {
          ಸಂರಚನಾ dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          ಯದಿ (missing_dir || x->preserve_ownership || x->preserve_mode
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              ಸಂರಚನಾ stat src_st;
              ಪೂರ್ಣಾಙ್ಕ src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_mode)
                               ? 0
                               : ENOTDIR);
              ಯದಿ (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  ಪ್ರತ್ಯಾಗಮ false;
                }

              new = xmalloc (ಪರಿಮಾಣ *new);
              new->st = src_st;
              new->slash_offset = slash - dir;
              new->restore_mode = false;
              new->next = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          ಯದಿ (! set_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_mode : 0,
                                          missing_dir, x))
            ಪ್ರತ್ಯಾಗಮ false;

          ಯದಿ (missing_dir)
            {
              /* This component does not exist.  We must set
                 *new_dst and new->st.st_mode inside this loop because,
                 for example, in the command 'cp --parents ../a/../b/c e_dir',
                 make_dir_parents_private creates only e_dir/../a if
                 ./b already exists. */
              *new_dst = true;
              mode_t src_mode = new->st.st_mode;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_mode
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_mode
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_mode = (x->explicit_no_preserve_mode
                                   ? S_IRWXUGO : src_mode);
              mkdir_mode &= CHMOD_MODE_BITS & ~omitted_permissions;
              ಯದಿ (mkdirat (dst_dirfd, dst_reldir, mkdir_mode) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  ಪ್ರತ್ಯಾಗಮ false;
                }
              ಅನ್ಯಥಾ
                {
                  ಯದಿ (verbose_fmt_string != ರಿಕ್ತ)
                    printf (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              ಯದಿ (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  ಪ್ರತ್ಯಾಗಮ false;
                }


              ಯದಿ (! x->preserve_mode)
                {
                  ಯದಿ (omitted_permissions & ~stats.st_mode)
                    omitted_permissions &= ~ cached_umask ();
                  ಯದಿ (omitted_permissions & ~stats.st_mode
                      || (stats.st_mode & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_mode = stats.st_mode | omitted_permissions;
                      new->restore_mode = true;
                    }
                }

              mode_t accessible = stats.st_mode | S_IRWXU;
              ಯದಿ (stats.st_mode != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  ಯದಿ (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      ಪ್ರತ್ಯಾಗಮ false;
                    }
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (!S_ISDIR (stats.st_mode))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              ಪ್ರತ್ಯಾಗಮ false;
            }
          ಅನ್ಯಥಾ
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          ಯದಿ (! *new_dst
              && (x->set_security_context || x->preserve_security_context))
            {
              ಯದಿ (! set_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                ಪ್ರತ್ಯಾಗಮ false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          ಯಾವತ್ (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  ಅನ್ಯಥಾ ಯದಿ (!S_ISDIR (stats.st_mode))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಅನ್ಯಥಾ
    {
      *new_dst = false;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

ಸ್ಥಿರ bool
do_copy (ಪೂರ್ಣಾಙ್ಕ n_files, ವರ್ಣ **file, ವರ್ಣ ಧ್ರುವ *target_directory,
         bool no_target_directory, ಸಂರಚನಾ cp_options *x)
{
  ಯದಿ (n_files <= !target_directory)
    {
      ಯದಿ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ಸಂರಚನಾ stat sb;
  sb.st_mode = 0;
  ಪೂರ್ಣಾಙ್ಕ target_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  ಯದಿ (no_target_directory)
    {
      ಯದಿ (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ಯದಿ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      ಯದಿ (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ ಧ್ರುವ *lastfile = file[n_files - 1];
      ಪೂರ್ಣಾಙ್ಕ fd = target_directory_operand (lastfile, &sb);
      ಯದಿ (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ err = errno;
          ಯದಿ (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          ಯದಿ (2 < n_files
              || (O_PATHSEARCH == O_ಹುಡುಕು && err == EACCES
                  && (sb.st_mode || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_mode)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  ಯದಿ (target_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      ಯದಿ (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_files; i++)
        {
          ವರ್ಣ *dst_name;
          bool parent_exists = true;  /* True if dir_name (dst_name) exists. */
          ಸಂರಚನಾ dir_attr *attr_list;
          ವರ್ಣ *arg_in_concat;
          ವರ್ಣ *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          ಯದಿ (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          ಯದಿ (parents_option)
            {
              ವರ್ಣ *arg_no_trailing_slash;

              /* Use 'arg' without trailing slashes in constructing destination
                 file names.  Otherwise, we can end up trying to create a
                 directory using a name with trailing slash, which fails on
                 NetBSD 1.[34] systems.  */
              ASSIGN_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (target_directory,
                                           arg_no_trailing_slash,
                                           &arg_in_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              parent_exists =
                (make_dir_parents_private
                 (dst_name, arg_in_concat - dst_name, target_dirfd,
                  (x->verbose ? "%s -> %s\n" : ರಿಕ್ತ),
                  &attr_list, &new_dst, x));
            }
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ *arg_base;
              /* Append the last component of 'arg' to 'target_directory'.  */
              ASSIGN_STRDUPA (arg_base, last_component (arg));
              strip_trailing_slashes (arg_base);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_base += streq (arg_base, "..");
              dst_name = file_name_concat (target_directory, arg_base,
                                           &arg_in_concat);
            }

          ಯದಿ (!parent_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ ಧ್ರುವ *dst_relname = arg_in_concat;
              ಯಾವತ್ (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, target_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, ರಿಕ್ತ);

              ಯದಿ (parents_option)
                ok &= re_protect (dst_name, arg_in_concat, target_dirfd,
                                  dst_relname, attr_list, x);
            }

          ಯದಿ (parents_option)
            {
              ಯಾವತ್ (attr_list)
                {
                  ಸಂರಚನಾ dir_attr *p = attr_list;
                  attr_list = attr_list->next;
                  ಮುಕ್ತ (p);
                }
            }

          ಮುಕ್ತ (dst_name);
        }
    }
  ಅನ್ಯಥಾ /* !target_directory */
    {
      ಯದಿ (parents_option)
        {
          error (0, 0,
                 _("with --parents, the destination must be a directory"));
          usage (EXIT_FAILURE);
        }

      /* When the force and backup options have been specified and
         the source and destination are the same name for an existing
         regular file, convert the user's command, e.g.,
         'cp --force --backup foo foo' to 'cp --force foo fooSUFFIX'
         where SUFFIX is determined by any version control options used.  */

      ವರ್ಣ ಧ್ರುವ *source = file[0];
      ವರ್ಣ ಧ್ರುವ *dest = file[1];
      ಯದಿ (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (source, dest)
          && !new_dst
          && (sb.st_mode != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_mode))
        {
          ಸ್ಥಿರ ಸಂರಚನಾ cp_options x_tmp;

          dest = find_backup_file_name (AT_FDCWD, dest, x->backup_type);
          /* Set x->backup_type to 'no_backups' so that the normal backup
             mechanism is not used when performing the actual copy.
             backup_type must be set to 'no_backups' only *after* the above
             call to find_backup_file_name -- that function uses
             backup_type to determine the suffix it applies.  */
          x_tmp = *x;
          x_tmp.backup_type = no_backups;
          x = &x_tmp;
        }

      bool unused;
      ok = copy (source, dest, AT_FDCWD, dest, -new_dst, x, &unused, ರಿಕ್ತ);
    }

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cp_option_init (ಸಂರಚನಾ cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->dereference = DEREF_UNDEFINED;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_mode = false;
  x->install_mode = false;
  x->one_file_system = false;
  x->reflink_mode = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_mode = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_mode = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->set_security_context = ರಿಕ್ತ;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->data_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_mode = SPARSE_AUTO;
  x->symbolic_link = false;
  x->set_mode = false;
  x->mode = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != ರಿಕ್ತ;

  x->dest_info = ರಿಕ್ತ;
  x->src_info = ರಿಕ್ತ;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
ಸ್ಥಿರ ಶೂನ್ಯ
decode_preserve_arg (ವರ್ಣ ಧ್ರುವ *arg, ಸಂರಚನಾ cp_options *x, bool on_off)
{
  ಗಣನಾ File_attribute
    {
      PRESERVE_MODE,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  ಸ್ಥಿರ ಗಣನಾ File_attribute ಧ್ರುವ preserve_vals[] =
    {
      PRESERVE_MODE, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ preserve_args[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", ರಿಕ್ತ
    };
  ARGMATCH_VERIFY (preserve_args, preserve_vals);

  ವರ್ಣ *arg_writable = xstrdup (arg);
  ವರ್ಣ *s = arg_writable;
  ಕುರು
    {
      /* find next comma */
      ವರ್ಣ *comma = strchr (s, ',');
      ಗಣನಾ File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      ಯದಿ (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_args, preserve_vals);
      ವಿಕಲ್ಪ (val)
        {
        ಪಕ್ಷ PRESERVE_MODE:
          x->preserve_mode = on_off;
          x->explicit_no_preserve_mode = !on_off;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_LINK:
          x->preserve_links = on_off;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_ALL:
          x->preserve_mode = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_mode = !on_off;
          ಯದಿ (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          ಭಙ್ಗ;

        ಸ್ವಭಾವ:
          affirm (false);
        }
      s = comma;
    }
  ಯಾವತ್ (s);

  ಮುಕ್ತ (arg_writable);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool make_backups = false;
  ವರ್ಣ ಧ್ರುವ *backup_suffix = ರಿಕ್ತ;
  ವರ್ಣ *version_control_string = ರಿಕ್ತ;
  ಸಂರಚನಾ cp_options x;
  bool copy_contents = false;
  ವರ್ಣ *target_directory = ರಿಕ್ತ;
  bool no_target_directory = false;
  ವರ್ಣ ಧ್ರುವ *scontext = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  ಪೂರ್ಣಾಙ್ಕ c;
  ಯಾವತ್ ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ SPARSE_OPTION:
          x.sparse_mode = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          ಭಙ್ಗ;

        ಪಕ್ಷ REFLINK_OPTION:
          ಯದಿ (optarg == ರಿಕ್ತ)
            x.reflink_mode = REFLINK_ALWAYS;
          ಅನ್ಯಥಾ
            x.reflink_mode = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_mode = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ಯದಿ (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'b':
          make_backups = true;
          ಯದಿ (optarg)
            version_control_string = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ ATTRIBUTES_ONLY_OPTION:
          x.data_copy_required = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ COPY_CONTENTS_OPTION:
          copy_contents = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          x.unlink_dest_after_failed_open = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'H':
          x.dereference = DEREF_COMMAND_ಸಾಲು_ARGUMENTS;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'i':
          x.interactive = I_ASK_USER;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'l':
          x.hard_link = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'L':
          x.dereference = DEREF_ALWAYS;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'n':
          x.interactive = I_ALWAYS_SKIP;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'P':
          x.dereference = DEREF_NEVER;
          ಭಙ್ಗ;

        ಪಕ್ಷ NO_PRESERVE_ATTRIBUTES_OPTION:
          decode_preserve_arg (optarg, &x, false);
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_ATTRIBUTES_OPTION:
          ಯದಿ (optarg == ರಿಕ್ತ)
            {
              /* Fall through to the case for 'p' below.  */
            }
          ಅನ್ಯಥಾ
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              ಭಙ್ಗ;
            }
          FALLTHROUGH;

        ಪಕ್ಷ 'p':
          x.preserve_ownership = true;
          x.preserve_mode = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ PARENTS_OPTION:
          parents_option = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'r':
        ಪಕ್ಷ 'R':
          x.recursive = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          x.symbolic_link = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          ಯದಿ (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'T':
          no_target_directory = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          x.update = UPDATE_OLDER;
          ಯದಿ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':
          x.verbose = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'x':
          x.one_file_system = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          ಯದಿ (selinux_enabled)
            {
              ಯದಿ (optarg)
                scontext = optarg;
              ಅನ್ಯಥಾ
                {
                  x.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         ರಿಕ್ತ, 0);
                  ಯದಿ (! x.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  ಯದಿ (x.reflink_mode == REFLINK_AUTO && x.sparse_mode == SPARSE_NEVER)
    x.reflink_mode = REFLINK_NEVER;

  ಯದಿ (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ಯದಿ (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (x.reflink_mode == REFLINK_ALWAYS && x.sparse_mode != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  ಯದಿ (x.dereference == DEREF_UNDEFINED)
    {
      ಯದಿ (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      ಅನ್ಯಥಾ
        x.dereference = DEREF_ALWAYS;
    }

  ಯದಿ (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  ಯದಿ ((x.set_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  ಯದಿ (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ಯದಿ (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  ಯದಿ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#ಯದಿ !USE_XATTR
  ಯದಿ (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#endif

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     target_directory, no_target_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
