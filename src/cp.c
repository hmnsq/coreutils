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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include <selinux/label.h>

#include "system.h"
#include "argmatch.h"
#include "assure.h"
#include "backupfile.h"
#include "copy.h"
#include "cp-hash.h"
#include "filenamecat.h"
#include "ignore-value.h"
#include "quote.h"
#include "stat-time.h"
#include "targetdir.h"
#include "utimens.h"
#include "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cp"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
sa_nracanaa dir_attr
{
  sa_nracanaa stat st;
  bool restore_mode;
  size_t slash_offset;
  sa_nracanaa dir_attr *next;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
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
static bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
static bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
static bool remove_trailing_slashes;

static var_na const *const sparse_type_string[] =
{
  "never", "auto", "always", NULL
};
static ga_nanaa Sparse_type const sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

static var_na const *const reflink_type_string[] =
{
  "auto", "always", "never", NULL
};
static ga_nanaa Reflink_type const reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

static var_na const *const update_type_string[] =
{
  "all", "none", "none-fail", "older", NULL
};
static ga_nanaa Update_type const update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

static sa_nracanaa option const long_opts[] =
{
  {"archive", no_argument, NULL, 'a'},
  {"attributes-only", no_argument, NULL, ATTRIBUTES_ONLY_OPTION},
  {"backup", optional_argument, NULL, 'b'},
  {"copy-contents", no_argument, NULL, COPY_CONTENTS_OPTION},
  {"debug", no_argument, NULL, DEBUG_OPTION},
  {"dereference", no_argument, NULL, 'L'},
  {"force", no_argument, NULL, 'f'},
  {"interactive", no_argument, NULL, 'i'},
  {"link", no_argument, NULL, 'l'},
  {"no-clobber", no_argument, NULL, 'n'},   /* Deprecated.  */
  {"no-dereference", no_argument, NULL, 'P'},
  {"no-preserve", required_argument, NULL, NO_PRESERVE_ATTRIBUTES_OPTION},
  {"no-target-directory", no_argument, NULL, 'T'},
  {"one-file-system", no_argument, NULL, 'x'},
  {"parents", no_argument, NULL, PARENTS_OPTION},
  {"path", no_argument, NULL, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_argument, NULL, PRESERVE_ATTRIBUTES_OPTION},
  {"recursive", no_argument, NULL, 'R'},
  {"remove-destination", no_argument, NULL, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_argument, NULL, SPARSE_OPTION},
  {"reflink", optional_argument, NULL, REFLINK_OPTION},
  {"strip-trailing-slashes", no_argument, NULL,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_argument, NULL, 'S'},
  {"symbolic-link", no_argument, NULL, 's'},
  {"target-directory", required_argument, NULL, 't'},
  {"update", optional_argument, NULL, 'u'},
  {"verbose", no_argument, NULL, 'v'},
  {"keep-directory-symlink", no_argument, NULL,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
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
         ถ้า an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also used)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a previous -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-line symbolic links in SOURCE\n\
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
         UPDATE={all,none,none-fail,older(default)}\n\
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
         set SELinux security context of destination file to default type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ถ้า CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list of attributes. Attributes are 'mode' สำหรับ\n\
permissions (including any ACL and xattr permissions), 'ownership' สำหรับ user\n\
and group, 'timestamps' สำหรับ file timestamps, 'links' สำหรับ hard links, 'context'\
\nfor security context, 'xattr' สำหรับ extended attributes, and 'all' สำหรับ all\n\
attributes.\n\
"), stdout);
      fputs (_("\
\n\
By default, sparse SOURCE files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=auto.  Specify --sparse=always to create a sparse DEST\n\
file whenever the SOURCE file contains a long enough sequence of zero bytes.\n\
Use --sparse=never to inhibit creation of sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By default or with --reflink=auto, cp will try a lightweight copy, where the\n\
data blocks are copied only when modified, falling back to a standard copy\n\
ถ้า this is not possible.  With --reflink[=always] cp will fail ถ้า CoW is not\n\
supported, ขณะ --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special pak_sa, cp makes a backup of SOURCE when the force and backup\n\
options are given and SOURCE and DEST are the same name สำหรับ an existing,\n\
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

static bool
re_protect (var_na const *const_dst_name, var_na const *dst_src_name,
            จำนวนเต็ม dst_dirfd, var_na const *dst_relname,
            sa_nracanaa dir_attr *attr_list, const sa_nracanaa cp_options *x)
{
  var_na *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  ASSIGN_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  var_na const *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  var_na const *relname = dst_name + (dst_relname - const_dst_name);

  สำหรับ (sa_nracanaa dir_attr *p = attr_list; p; p = p->next)
    {
      dst_name[p->slash_offset] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      ถ้า (x->preserve_timestamps)
        {
          sa_nracanaa timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          ถ้า (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              คืน false;
            }
        }

      ถ้า (x->preserve_ownership)
        {
          ถ้า (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              ถ้า (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  คืน false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_value (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      ถ้า (x->preserve_mode)
        {
          ถ้า (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_mode) != 0)
            คืน false;
        }
      มิฉะนั้น ถ้า (p->restore_mode)
        {
          ถ้า (lchmodat (dst_dirfd, relname, p->st.st_mode) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              คืน false;
            }
        }

      dst_name[p->slash_offset] = '/';
    }
  คืน true;
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

static bool
make_dir_parents_private (var_na const *const_dir, size_t src_offset,
                          จำนวนเต็ม dst_dirfd,
                          var_na const *verbose_fmt_string,
                          sa_nracanaa dir_attr **attr_list, bool *new_dst,
                          const sa_nracanaa cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = NULL;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  ถ้า (dirlen <= src_offset)
    คืน true;

  /* A copy of CONST_DIR we can change.  */
  var_na *dir;
  ASSIGN_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  var_na *src = dir + src_offset;

  /* Leading directory of DIR.  */
  var_na *dst_dir = alloca (dirlen + 1);
  memcpy (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  var_na const *dst_reldir = dst_dir + src_offset;
  ขณะ (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  sa_nracanaa stat stats;
  ถ้า (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      var_na *slash = src;

      ขณะ (*slash == '/')
        slash++;
      dst_reldir = slash;

      ขณะ ((slash = strchr (slash, '/')))
        {
          sa_nracanaa dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          ถ้า (missing_dir || x->preserve_ownership || x->preserve_mode
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              sa_nracanaa stat src_st;
              จำนวนเต็ม src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_mode)
                               ? 0
                               : ENOTDIR);
              ถ้า (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  คืน false;
                }

              new = xmalloc (parimaa_na *new);
              new->st = src_st;
              new->slash_offset = slash - dir;
              new->restore_mode = false;
              new->next = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          ถ้า (! set_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_mode : 0,
                                          missing_dir, x))
            คืน false;

          ถ้า (missing_dir)
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
              ถ้า (mkdirat (dst_dirfd, dst_reldir, mkdir_mode) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  คืน false;
                }
              มิฉะนั้น
                {
                  ถ้า (verbose_fmt_string != NULL)
                    printf (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              ถ้า (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  คืน false;
                }


              ถ้า (! x->preserve_mode)
                {
                  ถ้า (omitted_permissions & ~stats.st_mode)
                    omitted_permissions &= ~ cached_umask ();
                  ถ้า (omitted_permissions & ~stats.st_mode
                      || (stats.st_mode & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_mode = stats.st_mode | omitted_permissions;
                      new->restore_mode = true;
                    }
                }

              mode_t accessible = stats.st_mode | S_IRWXU;
              ถ้า (stats.st_mode != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  ถ้า (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      คืน false;
                    }
                }
            }
          มิฉะนั้น ถ้า (!S_ISDIR (stats.st_mode))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              คืน false;
            }
          มิฉะนั้น
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          ถ้า (! *new_dst
              && (x->set_security_context || x->preserve_security_context))
            {
              ถ้า (! set_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                คืน false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          ขณะ (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  มิฉะนั้น ถ้า (!S_ISDIR (stats.st_mode))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      คืน false;
    }
  มิฉะนั้น
    {
      *new_dst = false;
    }
  คืน true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

static bool
do_copy (จำนวนเต็ม n_files, var_na **file, var_na const *target_directory,
         bool no_target_directory, sa_nracanaa cp_options *x)
{
  ถ้า (n_files <= !target_directory)
    {
      ถ้า (n_files <= 0)
        error (0, 0, _("missing file operand"));
      มิฉะนั้น
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  sa_nracanaa stat sb;
  sb.st_mode = 0;
  จำนวนเต็ม target_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  ถ้า (no_target_directory)
    {
      ถ้า (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ถ้า (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  มิฉะนั้น ถ้า (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      ถ้า (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  มิฉะนั้น
    {
      var_na const *lastfile = file[n_files - 1];
      จำนวนเต็ม fd = target_directory_operand (lastfile, &sb);
      ถ้า (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      มิฉะนั้น
        {
          จำนวนเต็ม err = errno;
          ถ้า (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          ถ้า (2 < n_files
              || (O_PATHSEARCH == O_SEARCH && err == EACCES
                  && (sb.st_mode || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_mode)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  ถ้า (target_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      ถ้า (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      สำหรับ (จำนวนเต็ม i = 0; i < n_files; i++)
        {
          var_na *dst_name;
          bool parent_exists = true;  /* True if dir_name (dst_name) exists. */
          sa_nracanaa dir_attr *attr_list;
          var_na *arg_in_concat;
          var_na *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          ถ้า (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          ถ้า (parents_option)
            {
              var_na *arg_no_trailing_slash;

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
                  (x->verbose ? "%s -> %s\n" : NULL),
                  &attr_list, &new_dst, x));
            }
          มิฉะนั้น
            {
              var_na *arg_base;
              /* Append the last component of 'arg' to 'target_directory'.  */
              ASSIGN_STRDUPA (arg_base, last_component (arg));
              strip_trailing_slashes (arg_base);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_base += streq (arg_base, "..");
              dst_name = file_name_concat (target_directory, arg_base,
                                           &arg_in_concat);
            }

          ถ้า (!parent_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          มิฉะนั้น
            {
              var_na const *dst_relname = arg_in_concat;
              ขณะ (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, target_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, NULL);

              ถ้า (parents_option)
                ok &= re_protect (dst_name, arg_in_concat, target_dirfd,
                                  dst_relname, attr_list, x);
            }

          ถ้า (parents_option)
            {
              ขณะ (attr_list)
                {
                  sa_nracanaa dir_attr *p = attr_list;
                  attr_list = attr_list->next;
                  free (p);
                }
            }

          free (dst_name);
        }
    }
  มิฉะนั้น /* !target_directory */
    {
      ถ้า (parents_option)
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

      var_na const *source = file[0];
      var_na const *dest = file[1];
      ถ้า (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (source, dest)
          && !new_dst
          && (sb.st_mode != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_mode))
        {
          static sa_nracanaa cp_options x_tmp;

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
      ok = copy (source, dest, AT_FDCWD, dest, -new_dst, x, &unused, NULL);
    }

  คืน ok;
}

static void
cp_option_init (sa_nracanaa cp_options *x)
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
  x->set_security_context = NULL;       /* -Z, set sys default context. */
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
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != NULL;

  x->dest_info = NULL;
  x->src_info = NULL;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
static void
decode_preserve_arg (var_na const *arg, sa_nracanaa cp_options *x, bool on_off)
{
  ga_nanaa File_attribute
    {
      PRESERVE_MODE,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  static ga_nanaa File_attribute const preserve_vals[] =
    {
      PRESERVE_MODE, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  static var_na const *const preserve_args[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", NULL
    };
  ARGMATCH_VERIFY (preserve_args, preserve_vals);

  var_na *arg_writable = xstrdup (arg);
  var_na *s = arg_writable;
  do
    {
      /* find next comma */
      var_na *comma = strchr (s, ',');
      ga_nanaa File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      ถ้า (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_args, preserve_vals);
      switch (val)
        {
        pak_sa PRESERVE_MODE:
          x->preserve_mode = on_off;
          x->explicit_no_preserve_mode = !on_off;
          bha_nga;

        pak_sa PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          bha_nga;

        pak_sa PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          bha_nga;

        pak_sa PRESERVE_LINK:
          x->preserve_links = on_off;
          bha_nga;

        pak_sa PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          bha_nga;

        pak_sa PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          bha_nga;

        pak_sa PRESERVE_ALL:
          x->preserve_mode = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_mode = !on_off;
          ถ้า (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          bha_nga;

        default:
          affirm (false);
        }
      s = comma;
    }
  ขณะ (s);

  free (arg_writable);
}

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na **argv)
{
  bool make_backups = false;
  var_na const *backup_suffix = NULL;
  var_na *version_control_string = NULL;
  sa_nracanaa cp_options x;
  bool copy_contents = false;
  var_na *target_directory = NULL;
  bool no_target_directory = false;
  var_na const *scontext = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  จำนวนเต็ม c;
  ขณะ ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, NULL))
         != -1)
    {
      switch (c)
        {
        pak_sa SPARSE_OPTION:
          x.sparse_mode = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          bha_nga;

        pak_sa REFLINK_OPTION:
          ถ้า (optarg == NULL)
            x.reflink_mode = REFLINK_ALWAYS;
          มิฉะนั้น
            x.reflink_mode = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          bha_nga;

        pak_sa 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_mode = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ถ้า (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          bha_nga;

        pak_sa 'b':
          make_backups = true;
          ถ้า (optarg)
            version_control_string = optarg;
          bha_nga;

        pak_sa ATTRIBUTES_ONLY_OPTION:
          x.data_copy_required = false;
          bha_nga;

        pak_sa DEBUG_OPTION:
          x.debug = x.verbose = true;
          bha_nga;

        pak_sa COPY_CONTENTS_OPTION:
          copy_contents = true;
          bha_nga;

        pak_sa 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          bha_nga;

        pak_sa 'f':
          x.unlink_dest_after_failed_open = true;
          bha_nga;

        pak_sa 'H':
          x.dereference = DEREF_COMMAND_LINE_ARGUMENTS;
          bha_nga;

        pak_sa 'i':
          x.interactive = I_ASK_USER;
          bha_nga;

        pak_sa 'l':
          x.hard_link = true;
          bha_nga;

        pak_sa 'L':
          x.dereference = DEREF_ALWAYS;
          bha_nga;

        pak_sa 'n':
          x.interactive = I_ALWAYS_SKIP;
          bha_nga;

        pak_sa 'P':
          x.dereference = DEREF_NEVER;
          bha_nga;

        pak_sa NO_PRESERVE_ATTRIBUTES_OPTION:
          decode_preserve_arg (optarg, &x, false);
          bha_nga;

        pak_sa PRESERVE_ATTRIBUTES_OPTION:
          ถ้า (optarg == NULL)
            {
              /* Fall through to the case for 'p' below.  */
            }
          มิฉะนั้น
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              bha_nga;
            }
          FALLTHROUGH;

        pak_sa 'p':
          x.preserve_ownership = true;
          x.preserve_mode = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          bha_nga;

        pak_sa PARENTS_OPTION:
          parents_option = true;
          bha_nga;

        pak_sa 'r':
        pak_sa 'R':
          x.recursive = true;
          bha_nga;

        pak_sa UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          bha_nga;

        pak_sa STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          bha_nga;

        pak_sa 's':
          x.symbolic_link = true;
          bha_nga;

        pak_sa 't':
          ถ้า (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          bha_nga;

        pak_sa 'T':
          no_target_directory = true;
          bha_nga;

        pak_sa 'u':
          x.update = UPDATE_OLDER;
          ถ้า (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          bha_nga;

        pak_sa 'v':
          x.verbose = true;
          bha_nga;

        pak_sa KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          bha_nga;

        pak_sa 'x':
          x.one_file_system = true;
          bha_nga;

        pak_sa 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          ถ้า (selinux_enabled)
            {
              ถ้า (optarg)
                scontext = optarg;
              มิฉะนั้น
                {
                  x.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         NULL, 0);
                  ถ้า (! x.set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          มิฉะนั้น ถ้า (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          bha_nga;

        pak_sa 'S':
          make_backups = true;
          backup_suffix = optarg;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  ถ้า (x.reflink_mode == REFLINK_AUTO && x.sparse_mode == SPARSE_NEVER)
    x.reflink_mode = REFLINK_NEVER;

  ถ้า (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  ถ้า (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ถ้า (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  ถ้า (x.reflink_mode == REFLINK_ALWAYS && x.sparse_mode != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  ถ้า (x.dereference == DEREF_UNDEFINED)
    {
      ถ้า (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      มิฉะนั้น
        x.dereference = DEREF_ALWAYS;
    }

  ถ้า (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  ถ้า ((x.set_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  ถ้า (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ถ้า (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  ถ้า (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#ถ้า !USE_XATTR
  ถ้า (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#endif

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     target_directory, no_target_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
