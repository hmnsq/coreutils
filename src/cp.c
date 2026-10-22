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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>
#شامل <selinux/label.h>

#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "backupfile.h"
#شامل "copy.h"
#شامل "cp-hash.h"
#شامل "filenamecat.h"
#شامل "ignore-value.h"
#شامل "quote.h"
#شامل "stat-time.h"
#شامل "targetdir.h"
#شامل "utimens.h"
#شامل "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "cp"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
ساخت dir_attr
{
  ساخت stat st;
  bool restore_منوال;
  size_t slash_انحراف;
  ساخت dir_attr *اگلا;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  اوصاف_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_اوصاف_OPTION,
  PARENTS_OPTION,
  PRESERVE_اوصاف_OPTION,
  REFLINK_OPTION,
  SPARSE_OPTION,
  STRIP_TRAILING_SLASHES_OPTION,
  UNLINK_DEST_BEFORE_OPENING,
  KEEP_DIRECTORY_SYMLINK_OPTION
};

/* True if the kernel is SELinux enabled.  */
ساکن bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
ساکن bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
ساکن bool remove_trailing_slashes;

ساکن حرف ثابت *ثابت sparse_type_string[] =
{
  "never", "auto", "always", تہی
};
ساکن شمار Sparse_type ثابت sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

ساکن حرف ثابت *ثابت reflink_type_string[] =
{
  "auto", "always", "never", تہی
};
ساکن شمار Reflink_type ثابت reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

ساکن حرف ثابت *ثابت update_type_string[] =
{
  "all", "none", "none-fail", "older", تہی
};
ساکن شمار Update_type ثابت update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ساکن ساخت option ثابت long_opts[] =
{
  {"archive", no_دلیل, تہی, 'a'},
  {"attributes-only", no_دلیل, تہی, اوصاف_ONLY_OPTION},
  {"backup", optional_دلیل, تہی, 'b'},
  {"copy-contents", no_دلیل, تہی, COPY_CONTENTS_OPTION},
  {"debug", no_دلیل, تہی, DEBUG_OPTION},
  {"dereference", no_دلیل, تہی, 'L'},
  {"force", no_دلیل, تہی, 'f'},
  {"interactive", no_دلیل, تہی, 'i'},
  {"link", no_دلیل, تہی, 'l'},
  {"no-clobber", no_دلیل, تہی, 'n'},   /* Deprecated.  */
  {"no-dereference", no_دلیل, تہی, 'P'},
  {"no-preserve", required_دلیل, تہی, NO_PRESERVE_اوصاف_OPTION},
  {"no-target-directory", no_دلیل, تہی, 'T'},
  {"one-file-system", no_دلیل, تہی, 'x'},
  {"parents", no_دلیل, تہی, PARENTS_OPTION},
  {"path", no_دلیل, تہی, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_دلیل, تہی, PRESERVE_اوصاف_OPTION},
  {"recursive", no_دلیل, تہی, 'R'},
  {"remove-destination", no_دلیل, تہی, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_دلیل, تہی, SPARSE_OPTION},
  {"reflink", optional_دلیل, تہی, REFLINK_OPTION},
  {"strip-trailing-slashes", no_دلیل, تہی,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_دلیل, تہی, 'S'},
  {"symbolic-link", no_دلیل, تہی, 's'},
  {"target-directory", required_دلیل, تہی, 't'},
  {"update", optional_دلیل, تہی, 'u'},
  {"verbose", no_دلیل, تہی, 'v'},
  {"keep-directory-symlink", no_دلیل, تہی,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [-T] ماخذ DEST\n\
  or:  %s [OPTION]... ماخذ... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ماخذ...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Copy ماخذ مقصد DEST, or مضرب ماخذ(s) مقصد DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --اوصاف-only\n\
         don't copy the file ڈیٹا, just the اوصاف\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup کا each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an دلیل\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents کا special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  مستلزم_2 -v\n\
"));
      oputs (_("\
  -f, --force\n\
         اگر an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also مستعمل)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a پچھلا_2 -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-سطر symbolic links اندر ماخذ\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links اندر ماخذ\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links اندر ماخذ\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks مقصد directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead کا copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=منوال_2,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified اوصاف\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified اوصاف\n\
"));
      oputs (_("\
      --parents\n\
         use بھرا ماخذ_2 file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See نیچے\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting مقصد open it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation کا sparse files. See نیچے\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ماخذ دلیل\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead کا copying\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ہدف-directory=DIRECTORY\n\
         copy all ماخذ arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-ہدف-directory\n\
         treat DEST as a معمول file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(طےشدہ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent مقصد --update[=older].  See نیچے\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being مکمل\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 SELinux security context کا destination file مقصد طےشدہ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context مقصد CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list کا اوصاف. اوصاف_2 are 'mode' برائے\n\
permissions (including any ACL and xattr permissions), 'ownership' برائے user\n\
and گروہ, 'timestamps' برائے file timestamps, 'links' برائے hard links, 'context'\
\nfor security context, 'xattr' برائے extended اوصاف, and 'all' برائے all\n\
اوصاف.\n\
"), stdout);
      fputs (_("\
\n\
By طےشدہ, sparse ماخذ files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=خودکار.  Specify --sparse=always مقصد create a sparse DEST\n\
file whenever the ماخذ file شامل_ہے a طویل enough sequence کا zero بائٹیں.\n\
Use --sparse=never مقصد inhibit creation کا sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By طےشدہ or with --reflink=خودکار, cp will try a lightweight copy, where the\n\
ڈیٹا بلاکیں are copied only when modified, falling عقب مقصد a standard copy\n\
اگر this is not possible.  With --reflink[=always] cp will fail اگر CoW is not\n\
supported, جب --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special حالت, cp makes a backup کا ماخذ when the force and backup\n\
options are given and ماخذ and DEST are the same name برائے an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
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

ساکن bool
re_protect (حرف ثابت *const_dst_name, حرف ثابت *dst_src_name,
            صحیح dst_dirfd, حرف ثابت *dst_relname,
            ساخت dir_attr *attr_list, ثابت ساخت cp_options *x)
{
  حرف *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  تفویض_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  حرف ثابت *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  حرف ثابت *relname = dst_name + (dst_relname - const_dst_name);

  برائے (ساخت dir_attr *p = attr_list; p; p = p->اگلا)
    {
      dst_name[p->slash_انحراف] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      اگر (x->preserve_timestamps)
        {
          ساخت timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          اگر (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              واپس false;
            }
        }

      اگر (x->preserve_ownership)
        {
          اگر (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              اگر (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  واپس false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_قدر (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      اگر (x->preserve_منوال)
        {
          اگر (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_منوال) != 0)
            واپس false;
        }
      ورنہ اگر (p->restore_منوال)
        {
          اگر (lchmodat (dst_dirfd, relname, p->st.st_منوال) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              واپس false;
            }
        }

      dst_name[p->slash_انحراف] = '/';
    }
  واپس true;
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

ساکن bool
make_dir_parents_نجی (حرف ثابت *const_dir, size_t src_انحراف,
                          صحیح dst_dirfd,
                          حرف ثابت *verbose_fmt_string,
                          ساخت dir_attr **attr_list, bool *new_dst,
                          ثابت ساخت cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = تہی;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  اگر (dirlen <= src_انحراف)
    واپس true;

  /* A copy of CONST_DIR we can change.  */
  حرف *dir;
  تفویض_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  حرف *src = dir + src_انحراف;

  /* Leading directory of DIR.  */
  حرف *dst_dir = alloca (dirlen + 1);
  حافظہ_نقل (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  حرف ثابت *dst_reldir = dst_dir + src_انحراف;
  جب (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  ساخت stat stats;
  اگر (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      حرف *slash = src;

      جب (*slash == '/')
        slash++;
      dst_reldir = slash;

      جب ((slash = strchr (slash, '/')))
        {
          ساخت dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          اگر (missing_dir || x->preserve_ownership || x->preserve_منوال
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              ساخت stat src_st;
              صحیح src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_منوال)
                               ? 0
                               : ENOTDIR);
              اگر (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  واپس false;
                }

              new = xmalloc (حجم *new);
              new->st = src_st;
              new->slash_انحراف = slash - dir;
              new->restore_منوال = false;
              new->اگلا = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          اگر (! سیٹ_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_منوال : 0,
                                          missing_dir, x))
            واپس false;

          اگر (missing_dir)
            {
              /* This component does not exist.  We must set
                 *new_dst and new->st.st_mode inside this loop because,
                 for example, in the command 'cp --parents ../a/../b/c e_dir',
                 make_dir_parents_private creates only e_dir/../a if
                 ./b already exists. */
              *new_dst = true;
              mode_t src_منوال = new->st.st_منوال;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_منوال
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_منوال
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_منوال = (x->explicit_no_preserve_منوال
                                   ? S_IRWXUGO : src_منوال);
              mkdir_منوال &= CHMOD_منوال_بٹ & ~omitted_permissions;
              اگر (mkdirat (dst_dirfd, dst_reldir, mkdir_منوال) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  واپس false;
                }
              ورنہ
                {
                  اگر (verbose_fmt_string != تہی)
                    لکھو (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              اگر (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  واپس false;
                }


              اگر (! x->preserve_منوال)
                {
                  اگر (omitted_permissions & ~stats.st_منوال)
                    omitted_permissions &= ~ cached_umask ();
                  اگر (omitted_permissions & ~stats.st_منوال
                      || (stats.st_منوال & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_منوال = stats.st_منوال | omitted_permissions;
                      new->restore_منوال = true;
                    }
                }

              mode_t accessible = stats.st_منوال | S_IRWXU;
              اگر (stats.st_منوال != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  اگر (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      واپس false;
                    }
                }
            }
          ورنہ اگر (!S_ISDIR (stats.st_منوال))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              واپس false;
            }
          ورنہ
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          اگر (! *new_dst
              && (x->سیٹ_security_context || x->preserve_security_context))
            {
              اگر (! سیٹ_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                واپس false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          جب (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  ورنہ اگر (!S_ISDIR (stats.st_منوال))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      واپس false;
    }
  ورنہ
    {
      *new_dst = false;
    }
  واپس true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

ساکن bool
do_copy (صحیح n_files, حرف **file, حرف ثابت *ہدف_directory,
         bool no_ہدف_directory, ساخت cp_options *x)
{
  اگر (n_files <= !ہدف_directory)
    {
      اگر (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ورنہ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ساخت stat sb;
  sb.st_منوال = 0;
  صحیح ہدف_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  اگر (no_ہدف_directory)
    {
      اگر (ہدف_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      اگر (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ورنہ اگر (ہدف_directory)
    {
      ہدف_dirfd = ہدف_directory_عملیہ (ہدف_directory, &sb);
      اگر (! ہدف_dirfd_valid (ہدف_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (ہدف_directory));
    }
  ورنہ
    {
      حرف ثابت *lastfile = file[n_files - 1];
      صحیح fd = ہدف_directory_عملیہ (lastfile, &sb);
      اگر (ہدف_dirfd_valid (fd))
        {
          ہدف_dirfd = fd;
          ہدف_directory = lastfile;
          n_files--;
        }
      ورنہ
        {
          صحیح err = errno;
          اگر (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          اگر (2 < n_files
              || (O_PATHSEARCH == O_تلاش && err == EACCES
                  && (sb.st_منوال || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_منوال)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  اگر (ہدف_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      اگر (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      برائے (صحیح i = 0; i < n_files; i++)
        {
          حرف *dst_name;
          bool والد_exists = true;  /* True if dir_name (dst_name) exists. */
          ساخت dir_attr *attr_list;
          حرف *arg_اندر_concat;
          حرف *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          اگر (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          اگر (parents_option)
            {
              حرف *arg_no_trailing_slash;

              /* Use 'arg' without trailing slashes in constructing destination
                 file names.  Otherwise, we can end up trying to create a
                 directory using a name with trailing slash, which fails on
                 NetBSD 1.[34] systems.  */
              تفویض_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (ہدف_directory,
                                           arg_no_trailing_slash,
                                           &arg_اندر_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              والد_exists =
                (make_dir_parents_نجی
                 (dst_name, arg_اندر_concat - dst_name, ہدف_dirfd,
                  (x->verbose ? "%s -> %s\n" : تہی),
                  &attr_list, &new_dst, x));
            }
          ورنہ
            {
              حرف *arg_بنیاد;
              /* Append the last component of 'arg' to 'target_directory'.  */
              تفویض_STRDUPA (arg_بنیاد, آخری_component (arg));
              strip_trailing_slashes (arg_بنیاد);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_بنیاد += streq (arg_بنیاد, "..");
              dst_name = file_name_concat (ہدف_directory, arg_بنیاد,
                                           &arg_اندر_concat);
            }

          اگر (!والد_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          ورنہ
            {
              حرف ثابت *dst_relname = arg_اندر_concat;
              جب (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, ہدف_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, تہی);

              اگر (parents_option)
                ok &= re_protect (dst_name, arg_اندر_concat, ہدف_dirfd,
                                  dst_relname, attr_list, x);
            }

          اگر (parents_option)
            {
              جب (attr_list)
                {
                  ساخت dir_attr *p = attr_list;
                  attr_list = attr_list->اگلا;
                  آزاد (p);
                }
            }

          آزاد (dst_name);
        }
    }
  ورنہ /* !target_directory */
    {
      اگر (parents_option)
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

      حرف ثابت *ماخذ_2 = file[0];
      حرف ثابت *dest = file[1];
      اگر (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (ماخذ_2, dest)
          && !new_dst
          && (sb.st_منوال != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_منوال))
        {
          ساکن ساخت cp_options x_tmp;

          dest = ڈھونڈو_backup_file_name (AT_FDCWD, dest, x->backup_type);
          /* Set x->backup_type to 'no_backups' so that the normal backup
             mechanism is not used when performing the actual copy.
             backup_type must be set to 'no_backups' only *after* the above
             call to find_backup_file_name -- that function uses
             backup_type to determine the suffix it applies.  */
          x_tmp = *x;
          x_tmp.backup_type = no_backups;
          x = &x_tmp;
        }

      bool غیر_مستعمل;
      ok = copy (ماخذ_2, dest, AT_FDCWD, dest, -new_dst, x, &غیر_مستعمل, تہی);
    }

  واپس ok;
}

ساکن خالی
cp_option_init (ساخت cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->dereference = DEREF_UNDEFINED;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_منوال = false;
  x->install_منوال = false;
  x->one_file_system = false;
  x->reflink_منوال = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_منوال = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_منوال = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->سیٹ_security_context = تہی;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->ڈیٹا_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_منوال = SPARSE_AUTO;
  x->symbolic_link = false;
  x->سیٹ_منوال = false;
  x->منوال_2 = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != تہی;

  x->dest_info = تہی;
  x->src_info = تہی;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
ساکن خالی
decode_preserve_arg (حرف ثابت *arg, ساخت cp_options *x, bool on_off)
{
  شمار File_attribute
    {
      PRESERVE_منوال,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  ساکن شمار File_attribute ثابت preserve_vals[] =
    {
      PRESERVE_منوال, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  ساکن حرف ثابت *ثابت preserve_دلائل[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", تہی
    };
  ARGMATCH_VERIFY (preserve_دلائل, preserve_vals);

  حرف *arg_writable = xstrdup (arg);
  حرف *s = arg_writable;
  کرو
    {
      /* find next comma */
      حرف *comma = strchr (s, ',');
      شمار File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      اگر (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_دلائل, preserve_vals);
      انتخاب (val)
        {
        حالت PRESERVE_منوال:
          x->preserve_منوال = on_off;
          x->explicit_no_preserve_منوال = !on_off;
          توڑو;

        حالت PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          توڑو;

        حالت PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          توڑو;

        حالت PRESERVE_LINK:
          x->preserve_links = on_off;
          توڑو;

        حالت PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          توڑو;

        حالت PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          توڑو;

        حالت PRESERVE_ALL:
          x->preserve_منوال = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_منوال = !on_off;
          اگر (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          توڑو;

        طےشدہ:
          affirm (false);
        }
      s = comma;
    }
  جب (s);

  آزاد (arg_writable);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool make_backups = false;
  حرف ثابت *backup_suffix = تہی;
  حرف *نسخہ_control_string = تہی;
  ساخت cp_options x;
  bool copy_contents = false;
  حرف *ہدف_directory = تہی;
  bool no_ہدف_directory = false;
  حرف ثابت *scontext = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  صحیح c;
  جب ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت SPARSE_OPTION:
          x.sparse_منوال = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          توڑو;

        حالت REFLINK_OPTION:
          اگر (optarg == تہی)
            x.reflink_منوال = REFLINK_ALWAYS;
          ورنہ
            x.reflink_منوال = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          توڑو;

        حالت 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_منوال = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          اگر (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          توڑو;

        حالت 'b':
          make_backups = true;
          اگر (optarg)
            نسخہ_control_string = optarg;
          توڑو;

        حالت اوصاف_ONLY_OPTION:
          x.ڈیٹا_copy_required = false;
          توڑو;

        حالت DEBUG_OPTION:
          x.debug = x.verbose = true;
          توڑو;

        حالت COPY_CONTENTS_OPTION:
          copy_contents = true;
          توڑو;

        حالت 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          توڑو;

        حالت 'f':
          x.unlink_dest_after_failed_open = true;
          توڑو;

        حالت 'H':
          x.dereference = DEREF_COMMAND_سطر_ARGUMENTS;
          توڑو;

        حالت 'i':
          x.interactive = I_ASK_USER;
          توڑو;

        حالت 'l':
          x.hard_link = true;
          توڑو;

        حالت 'L':
          x.dereference = DEREF_ALWAYS;
          توڑو;

        حالت 'n':
          x.interactive = I_ALWAYS_SKIP;
          توڑو;

        حالت 'P':
          x.dereference = DEREF_NEVER;
          توڑو;

        حالت NO_PRESERVE_اوصاف_OPTION:
          decode_preserve_arg (optarg, &x, false);
          توڑو;

        حالت PRESERVE_اوصاف_OPTION:
          اگر (optarg == تہی)
            {
              /* Fall through to the case for 'p' below.  */
            }
          ورنہ
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              توڑو;
            }
          FALLTHROUGH;

        حالت 'p':
          x.preserve_ownership = true;
          x.preserve_منوال = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          توڑو;

        حالت PARENTS_OPTION:
          parents_option = true;
          توڑو;

        حالت 'r':
        حالت 'R':
          x.recursive = true;
          توڑو;

        حالت UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          توڑو;

        حالت STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          توڑو;

        حالت 's':
          x.symbolic_link = true;
          توڑو;

        حالت 't':
          اگر (ہدف_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          ہدف_directory = optarg;
          توڑو;

        حالت 'T':
          no_ہدف_directory = true;
          توڑو;

        حالت 'u':
          x.update = UPDATE_OLDER;
          اگر (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          توڑو;

        حالت 'v':
          x.verbose = true;
          توڑو;

        حالت KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          توڑو;

        حالت 'x':
          x.one_file_system = true;
          توڑو;

        حالت 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          اگر (selinux_enabled)
            {
              اگر (optarg)
                scontext = optarg;
              ورنہ
                {
                  x.سیٹ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         تہی, 0);
                  اگر (! x.سیٹ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ورنہ اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          توڑو;

        حالت 'S':
          make_backups = true;
          backup_suffix = optarg;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  اگر (x.reflink_منوال == REFLINK_AUTO && x.sparse_منوال == SPARSE_NEVER)
    x.reflink_منوال = REFLINK_NEVER;

  اگر (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  اگر (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  اگر (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  اگر (x.reflink_منوال == REFLINK_ALWAYS && x.sparse_منوال != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_نسخہ (_("backup type"),
                                   نسخہ_control_string)
                   : no_backups);
  سیٹ_simple_backup_suffix (backup_suffix);

  اگر (x.dereference == DEREF_UNDEFINED)
    {
      اگر (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      ورنہ
        x.dereference = DEREF_ALWAYS;
    }

  اگر (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  اگر ((x.سیٹ_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  اگر (x.preserve_security_context && (x.سیٹ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  اگر (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  اگر (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#اگر !USE_XATTR
  اگر (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#اگر_ختم

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     ہدف_directory, no_ہدف_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
