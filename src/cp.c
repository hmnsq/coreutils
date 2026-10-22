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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>
#ضمن <selinux/label.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "backupfile.h"
#ضمن "copy.h"
#ضمن "cp-hash.h"
#ضمن "filenamecat.h"
#ضمن "ignore-value.h"
#ضمن "quote.h"
#ضمن "stat-time.h"
#ضمن "targetdir.h"
#ضمن "utimens.h"
#ضمن "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "cp"

#تعریف_کن AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
بنية dir_attr
{
  بنية stat st;
  bool restore_نما;
  size_t slash_جابه_جایی;
  بنية dir_attr *بعدی;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  صفات_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_صفات_OPTION,
  PARENTS_OPTION,
  PRESERVE_صفات_OPTION,
  REFLINK_OPTION,
  SPARSE_OPTION,
  STRIP_TRAILING_SLASHES_OPTION,
  UNLINK_DEST_BEFORE_OPENING,
  KEEP_DIRECTORY_SYMLINK_OPTION
};

/* True if the kernel is SELinux enabled.  */
ساكن bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
ساكن bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
ساكن bool remove_trailing_slashes;

ساكن حرف ثابت *ثابت sparse_type_string[] =
{
  "never", "auto", "always", عدم
};
ساكن تعداد Sparse_type ثابت sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

ساكن حرف ثابت *ثابت reflink_type_string[] =
{
  "auto", "always", "never", عدم
};
ساكن تعداد Reflink_type ثابت reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

ساكن حرف ثابت *ثابت update_type_string[] =
{
  "all", "none", "none-fail", "older", عدم
};
ساكن تعداد Update_type ثابت update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ساكن بنية option ثابت long_opts[] =
{
  {"archive", no_آرگومان, عدم, 'a'},
  {"attributes-only", no_آرگومان, عدم, صفات_ONLY_OPTION},
  {"backup", optional_آرگومان, عدم, 'b'},
  {"copy-contents", no_آرگومان, عدم, COPY_CONTENTS_OPTION},
  {"debug", no_آرگومان, عدم, DEBUG_OPTION},
  {"dereference", no_آرگومان, عدم, 'L'},
  {"force", no_آرگومان, عدم, 'f'},
  {"interactive", no_آرگومان, عدم, 'i'},
  {"link", no_آرگومان, عدم, 'l'},
  {"no-clobber", no_آرگومان, عدم, 'n'},   /* Deprecated.  */
  {"no-dereference", no_آرگومان, عدم, 'P'},
  {"no-preserve", required_آرگومان, عدم, NO_PRESERVE_صفات_OPTION},
  {"no-target-directory", no_آرگومان, عدم, 'T'},
  {"one-file-system", no_آرگومان, عدم, 'x'},
  {"parents", no_آرگومان, عدم, PARENTS_OPTION},
  {"path", no_آرگومان, عدم, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_آرگومان, عدم, PRESERVE_صفات_OPTION},
  {"recursive", no_آرگومان, عدم, 'R'},
  {"remove-destination", no_آرگومان, عدم, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_آرگومان, عدم, SPARSE_OPTION},
  {"reflink", optional_آرگومان, عدم, REFLINK_OPTION},
  {"strip-trailing-slashes", no_آرگومان, عدم,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_آرگومان, عدم, 'S'},
  {"symbolic-link", no_آرگومان, عدم, 's'},
  {"target-directory", required_آرگومان, عدم, 't'},
  {"update", optional_آرگومان, عدم, 'u'},
  {"verbose", no_آرگومان, عدم, 'v'},
  {"keep-directory-symlink", no_آرگومان, عدم,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [-T] منبع DEST\n\
  or:  %s [OPTION]... منبع... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY منبع...\n\
"),
              برنامه_name, برنامه_name, برنامه_name);
      fputs (_("\
Copy منبع مقصد DEST, or مضرب منبع(s) مقصد DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --صفات-only\n\
         don't copy the file داده, just the صفات\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup از each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an آرگومان\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents از special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  نتیجه_میدهد_2 -v\n\
"));
      oputs (_("\
  -f, --force\n\
         إذا an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also استفاده_شده)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a قبلی -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-سطر symbolic links درون منبع\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links درون منبع\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links درون منبع\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks مقصد directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead از copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=نما_2,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified صفات\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified صفات\n\
"));
      oputs (_("\
      --parents\n\
         use پر منبع_2 file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See زیر\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting مقصد open it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation از sparse files. See زیر\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each منبع آرگومان\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead از copying\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --هدف-directory=DIRECTORY\n\
         copy all منبع arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-هدف-directory\n\
         treat DEST as a نرمال file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(افتراضي)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent مقصد --update[=older].  See زیر\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being انجامشده\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         مجموعه_2 SELinux security context از destination file مقصد افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then مجموعه_2 the\n\
         SELinux or SMACK security context مقصد CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list از صفات. صفات_2 are 'mode' لكل\n\
permissions (including any ACL and xattr permissions), 'ownership' لكل user\n\
and گروه, 'timestamps' لكل file timestamps, 'links' لكل hard links, 'context'\
\nfor security context, 'xattr' لكل extended صفات, and 'all' لكل all\n\
صفات.\n\
"), stdout);
      fputs (_("\
\n\
By افتراضي, sparse منبع files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=خودکار.  Specify --sparse=always مقصد create a sparse DEST\n\
file whenever the منبع file شامل_است a دراز enough sequence از zero بایتها.\n\
Use --sparse=never مقصد inhibit creation از sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By افتراضي or with --reflink=خودکار, cp will try a lightweight copy, where the\n\
داده بلوکها are copied only when modified, falling پشت مقصد a standard copy\n\
إذا this is not possible.  With --reflink[=always] cp will fail إذا CoW is not\n\
supported, طالما --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special حالة, cp makes a backup از منبع when the force and backup\n\
options are given and منبع and DEST are the same name لكل an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
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

ساكن bool
re_protect (حرف ثابت *const_dst_name, حرف ثابت *dst_src_name,
            صحيح dst_dirfd, حرف ثابت *dst_relname,
            بنية dir_attr *attr_list, ثابت بنية cp_options *x)
{
  حرف *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  تخصیص_بده_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  حرف ثابت *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  حرف ثابت *relname = dst_name + (dst_relname - const_dst_name);

  لكل (بنية dir_attr *p = attr_list; p; p = p->بعدی)
    {
      dst_name[p->slash_جابه_جایی] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      إذا (x->preserve_timestamps)
        {
          بنية timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          إذا (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              أرجع false;
            }
        }

      إذا (x->preserve_ownership)
        {
          إذا (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              إذا (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  أرجع false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_مقدار (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      إذا (x->preserve_نما)
        {
          إذا (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_نما) != 0)
            أرجع false;
        }
      وإلا إذا (p->restore_نما)
        {
          إذا (lchmodat (dst_dirfd, relname, p->st.st_نما) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              أرجع false;
            }
        }

      dst_name[p->slash_جابه_جایی] = '/';
    }
  أرجع true;
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

ساكن bool
make_dir_parents_خصوصی (حرف ثابت *const_dir, size_t src_جابه_جایی,
                          صحيح dst_dirfd,
                          حرف ثابت *verbose_fmt_string,
                          بنية dir_attr **attr_list, bool *new_dst,
                          ثابت بنية cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = عدم;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  إذا (dirlen <= src_جابه_جایی)
    أرجع true;

  /* A copy of CONST_DIR we can change.  */
  حرف *dir;
  تخصیص_بده_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  حرف *src = dir + src_جابه_جایی;

  /* Leading directory of DIR.  */
  حرف *dst_dir = alloca (dirlen + 1);
  کپی_حافظه (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  حرف ثابت *dst_reldir = dst_dir + src_جابه_جایی;
  طالما (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  بنية stat stats;
  إذا (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      حرف *slash = src;

      طالما (*slash == '/')
        slash++;
      dst_reldir = slash;

      طالما ((slash = strchr (slash, '/')))
        {
          بنية dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          إذا (missing_dir || x->preserve_ownership || x->preserve_نما
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              بنية stat src_st;
              صحيح src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_نما)
                               ? 0
                               : ENOTDIR);
              إذا (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  أرجع false;
                }

              new = xmalloc (حجم *new);
              new->st = src_st;
              new->slash_جابه_جایی = slash - dir;
              new->restore_نما = false;
              new->بعدی = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          إذا (! مجموعه_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_نما : 0,
                                          missing_dir, x))
            أرجع false;

          إذا (missing_dir)
            {
              /* This component does not exist.  We must set
                 *new_dst and new->st.st_mode inside this loop because,
                 for example, in the command 'cp --parents ../a/../b/c e_dir',
                 make_dir_parents_private creates only e_dir/../a if
                 ./b already exists. */
              *new_dst = true;
              mode_t src_نما = new->st.st_نما;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_نما
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_نما
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_نما = (x->explicit_no_preserve_نما
                                   ? S_IRWXUGO : src_نما);
              mkdir_نما &= CHMOD_نما_بیتها & ~omitted_permissions;
              إذا (mkdirat (dst_dirfd, dst_reldir, mkdir_نما) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  أرجع false;
                }
              وإلا
                {
                  إذا (verbose_fmt_string != عدم)
                    چاپ_کن (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              إذا (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  أرجع false;
                }


              إذا (! x->preserve_نما)
                {
                  إذا (omitted_permissions & ~stats.st_نما)
                    omitted_permissions &= ~ cached_umask ();
                  إذا (omitted_permissions & ~stats.st_نما
                      || (stats.st_نما & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_نما = stats.st_نما | omitted_permissions;
                      new->restore_نما = true;
                    }
                }

              mode_t accessible = stats.st_نما | S_IRWXU;
              إذا (stats.st_نما != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  إذا (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      أرجع false;
                    }
                }
            }
          وإلا إذا (!S_ISDIR (stats.st_نما))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              أرجع false;
            }
          وإلا
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          إذا (! *new_dst
              && (x->مجموعه_security_context || x->preserve_security_context))
            {
              إذا (! مجموعه_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                أرجع false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          طالما (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  وإلا إذا (!S_ISDIR (stats.st_نما))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      أرجع false;
    }
  وإلا
    {
      *new_dst = false;
    }
  أرجع true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

ساكن bool
do_copy (صحيح n_files, حرف **file, حرف ثابت *هدف_directory,
         bool no_هدف_directory, بنية cp_options *x)
{
  إذا (n_files <= !هدف_directory)
    {
      إذا (n_files <= 0)
        error (0, 0, _("missing file operand"));
      وإلا
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  بنية stat sb;
  sb.st_نما = 0;
  صحيح هدف_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  إذا (no_هدف_directory)
    {
      إذا (هدف_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      إذا (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  وإلا إذا (هدف_directory)
    {
      هدف_dirfd = هدف_directory_عملوند (هدف_directory, &sb);
      إذا (! هدف_dirfd_valid (هدف_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (هدف_directory));
    }
  وإلا
    {
      حرف ثابت *lastfile = file[n_files - 1];
      صحيح fd = هدف_directory_عملوند (lastfile, &sb);
      إذا (هدف_dirfd_valid (fd))
        {
          هدف_dirfd = fd;
          هدف_directory = lastfile;
          n_files--;
        }
      وإلا
        {
          صحيح err = errno;
          إذا (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          إذا (2 < n_files
              || (O_PATHSEARCH == O_جستجو && err == EACCES
                  && (sb.st_نما || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_نما)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  إذا (هدف_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      إذا (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      لكل (صحيح i = 0; i < n_files; i++)
        {
          حرف *dst_name;
          bool والد_exists = true;  /* True if dir_name (dst_name) exists. */
          بنية dir_attr *attr_list;
          حرف *arg_درون_concat;
          حرف *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          إذا (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          إذا (parents_option)
            {
              حرف *arg_no_trailing_slash;

              /* Use 'arg' without trailing slashes in constructing destination
                 file names.  Otherwise, we can end up trying to create a
                 directory using a name with trailing slash, which fails on
                 NetBSD 1.[34] systems.  */
              تخصیص_بده_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (هدف_directory,
                                           arg_no_trailing_slash,
                                           &arg_درون_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              والد_exists =
                (make_dir_parents_خصوصی
                 (dst_name, arg_درون_concat - dst_name, هدف_dirfd,
                  (x->verbose ? "%s -> %s\n" : عدم),
                  &attr_list, &new_dst, x));
            }
          وإلا
            {
              حرف *arg_پایه;
              /* Append the last component of 'arg' to 'target_directory'.  */
              تخصیص_بده_STRDUPA (arg_پایه, آخرین_component (arg));
              strip_trailing_slashes (arg_پایه);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_پایه += streq (arg_پایه, "..");
              dst_name = file_name_concat (هدف_directory, arg_پایه,
                                           &arg_درون_concat);
            }

          إذا (!والد_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          وإلا
            {
              حرف ثابت *dst_relname = arg_درون_concat;
              طالما (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, هدف_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, عدم);

              إذا (parents_option)
                ok &= re_protect (dst_name, arg_درون_concat, هدف_dirfd,
                                  dst_relname, attr_list, x);
            }

          إذا (parents_option)
            {
              طالما (attr_list)
                {
                  بنية dir_attr *p = attr_list;
                  attr_list = attr_list->بعدی;
                  حرر (p);
                }
            }

          حرر (dst_name);
        }
    }
  وإلا /* !target_directory */
    {
      إذا (parents_option)
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

      حرف ثابت *منبع_2 = file[0];
      حرف ثابت *dest = file[1];
      إذا (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (منبع_2, dest)
          && !new_dst
          && (sb.st_نما != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_نما))
        {
          ساكن بنية cp_options x_tmp;

          dest = پیدا_کن_backup_file_name (AT_FDCWD, dest, x->backup_type);
          /* Set x->backup_type to 'no_backups' so that the normal backup
             mechanism is not used when performing the actual copy.
             backup_type must be set to 'no_backups' only *after* the above
             call to find_backup_file_name -- that function uses
             backup_type to determine the suffix it applies.  */
          x_tmp = *x;
          x_tmp.backup_type = no_backups;
          x = &x_tmp;
        }

      bool استفادهنشده;
      ok = copy (منبع_2, dest, AT_FDCWD, dest, -new_dst, x, &استفادهنشده, عدم);
    }

  أرجع ok;
}

ساكن فراغ
cp_option_init (بنية cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->dereference = DEREF_UNDEFINED;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_نما = false;
  x->install_نما = false;
  x->one_file_system = false;
  x->reflink_نما = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_نما = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_نما = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->مجموعه_security_context = عدم;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->داده_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_نما = SPARSE_AUTO;
  x->symbolic_link = false;
  x->مجموعه_نما = false;
  x->نما_2 = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != عدم;

  x->dest_info = عدم;
  x->src_info = عدم;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
ساكن فراغ
decode_preserve_arg (حرف ثابت *arg, بنية cp_options *x, bool on_off)
{
  تعداد File_attribute
    {
      PRESERVE_نما,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  ساكن تعداد File_attribute ثابت preserve_vals[] =
    {
      PRESERVE_نما, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  ساكن حرف ثابت *ثابت preserve_آرگومانها[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", عدم
    };
  ARGMATCH_VERIFY (preserve_آرگومانها, preserve_vals);

  حرف *arg_writable = xstrdup (arg);
  حرف *s = arg_writable;
  افعل
    {
      /* find next comma */
      حرف *comma = strchr (s, ',');
      تعداد File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      إذا (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_آرگومانها, preserve_vals);
      بدل (val)
        {
        حالة PRESERVE_نما:
          x->preserve_نما = on_off;
          x->explicit_no_preserve_نما = !on_off;
          اكسر;

        حالة PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          اكسر;

        حالة PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          اكسر;

        حالة PRESERVE_LINK:
          x->preserve_links = on_off;
          اكسر;

        حالة PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          اكسر;

        حالة PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          اكسر;

        حالة PRESERVE_ALL:
          x->preserve_نما = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_نما = !on_off;
          إذا (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          اكسر;

        افتراضي:
          affirm (false);
        }
      s = comma;
    }
  طالما (s);

  حرر (arg_writable);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  bool make_backups = false;
  حرف ثابت *backup_suffix = عدم;
  حرف *نسخه_control_string = عدم;
  بنية cp_options x;
  bool copy_contents = false;
  حرف *هدف_directory = عدم;
  bool no_هدف_directory = false;
  حرف ثابت *scontext = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  صحيح c;
  طالما ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة SPARSE_OPTION:
          x.sparse_نما = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          اكسر;

        حالة REFLINK_OPTION:
          إذا (optarg == عدم)
            x.reflink_نما = REFLINK_ALWAYS;
          وإلا
            x.reflink_نما = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          اكسر;

        حالة 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_نما = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          إذا (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          اكسر;

        حالة 'b':
          make_backups = true;
          إذا (optarg)
            نسخه_control_string = optarg;
          اكسر;

        حالة صفات_ONLY_OPTION:
          x.داده_copy_required = false;
          اكسر;

        حالة DEBUG_OPTION:
          x.debug = x.verbose = true;
          اكسر;

        حالة COPY_CONTENTS_OPTION:
          copy_contents = true;
          اكسر;

        حالة 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          اكسر;

        حالة 'f':
          x.unlink_dest_after_failed_open = true;
          اكسر;

        حالة 'H':
          x.dereference = DEREF_COMMAND_سطر_ARGUMENTS;
          اكسر;

        حالة 'i':
          x.interactive = I_ASK_USER;
          اكسر;

        حالة 'l':
          x.hard_link = true;
          اكسر;

        حالة 'L':
          x.dereference = DEREF_ALWAYS;
          اكسر;

        حالة 'n':
          x.interactive = I_ALWAYS_SKIP;
          اكسر;

        حالة 'P':
          x.dereference = DEREF_NEVER;
          اكسر;

        حالة NO_PRESERVE_صفات_OPTION:
          decode_preserve_arg (optarg, &x, false);
          اكسر;

        حالة PRESERVE_صفات_OPTION:
          إذا (optarg == عدم)
            {
              /* Fall through to the case for 'p' below.  */
            }
          وإلا
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              اكسر;
            }
          FALLTHROUGH;

        حالة 'p':
          x.preserve_ownership = true;
          x.preserve_نما = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          اكسر;

        حالة PARENTS_OPTION:
          parents_option = true;
          اكسر;

        حالة 'r':
        حالة 'R':
          x.recursive = true;
          اكسر;

        حالة UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          اكسر;

        حالة STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          اكسر;

        حالة 's':
          x.symbolic_link = true;
          اكسر;

        حالة 't':
          إذا (هدف_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          هدف_directory = optarg;
          اكسر;

        حالة 'T':
          no_هدف_directory = true;
          اكسر;

        حالة 'u':
          x.update = UPDATE_OLDER;
          إذا (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          اكسر;

        حالة 'v':
          x.verbose = true;
          اكسر;

        حالة KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          اكسر;

        حالة 'x':
          x.one_file_system = true;
          اكسر;

        حالة 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          إذا (selinux_enabled)
            {
              إذا (optarg)
                scontext = optarg;
              وإلا
                {
                  x.مجموعه_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         عدم, 0);
                  إذا (! x.مجموعه_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وإلا إذا (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          اكسر;

        حالة 'S':
          make_backups = true;
          backup_suffix = optarg;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  إذا (x.reflink_نما == REFLINK_AUTO && x.sparse_نما == SPARSE_NEVER)
    x.reflink_نما = REFLINK_NEVER;

  إذا (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  إذا (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  إذا (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  إذا (x.reflink_نما == REFLINK_ALWAYS && x.sparse_نما != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_نسخه (_("backup type"),
                                   نسخه_control_string)
                   : no_backups);
  مجموعه_simple_backup_suffix (backup_suffix);

  إذا (x.dereference == DEREF_UNDEFINED)
    {
      إذا (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      وإلا
        x.dereference = DEREF_ALWAYS;
    }

  إذا (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  إذا ((x.مجموعه_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  إذا (x.preserve_security_context && (x.مجموعه_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  إذا (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  إذا (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#إذا !USE_XATTR
  إذا (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#پایان_اگر

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     هدف_directory, no_هدف_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
