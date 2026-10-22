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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "backupfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cp-hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "targetdir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utimens.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "cp"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
ਬਣਤਰ dir_attr
{
  ਬਣਤਰ stat st;
  bool restore_ਬਹੁਲਕ;
  size_t slash_ਵਿੱਥ;
  ਬਣਤਰ dir_attr *ਅਗਲਾ;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  ਗੁਣ_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_ਗੁਣ_OPTION,
  PARENTS_OPTION,
  PRESERVE_ਗੁਣ_OPTION,
  REFLINK_OPTION,
  SPARSE_OPTION,
  STRIP_TRAILING_SLASHES_OPTION,
  UNLINK_DEST_BEFORE_OPENING,
  KEEP_DIRECTORY_SYMLINK_OPTION
};

/* True if the kernel is SELinux enabled.  */
ਸਥਿਰ bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
ਸਥਿਰ bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
ਸਥਿਰ bool remove_trailing_slashes;

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ sparse_type_string[] =
{
  "never", "auto", "always", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ Sparse_type ਅਚਲ sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ reflink_type_string[] =
{
  "auto", "always", "never", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ Reflink_type ਅਚਲ reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ update_type_string[] =
{
  "all", "none", "none-fail", "older", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ Update_type ਅਚਲ update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_opts[] =
{
  {"archive", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"attributes-only", no_ਦਲੀਲ, ਨਿਰਾ, ਗੁਣ_ONLY_OPTION},
  {"backup", optional_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"copy-contents", no_ਦਲੀਲ, ਨਿਰਾ, COPY_CONTENTS_OPTION},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_OPTION},
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"force", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"interactive", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"link", no_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"no-clobber", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},   /* Deprecated.  */
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'P'},
  {"no-preserve", required_ਦਲੀਲ, ਨਿਰਾ, NO_PRESERVE_ਗੁਣ_OPTION},
  {"no-target-directory", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"one-file-system", no_ਦਲੀਲ, ਨਿਰਾ, 'x'},
  {"parents", no_ਦਲੀਲ, ਨਿਰਾ, PARENTS_OPTION},
  {"path", no_ਦਲੀਲ, ਨਿਰਾ, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_ਦਲੀਲ, ਨਿਰਾ, PRESERVE_ਗੁਣ_OPTION},
  {"recursive", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"remove-destination", no_ਦਲੀਲ, ਨਿਰਾ, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_ਦਲੀਲ, ਨਿਰਾ, SPARSE_OPTION},
  {"reflink", optional_ਦਲੀਲ, ਨਿਰਾ, REFLINK_OPTION},
  {"strip-trailing-slashes", no_ਦਲੀਲ, ਨਿਰਾ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"symbolic-link", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"target-directory", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"update", optional_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"keep-directory-symlink", no_ਦਲੀਲ, ਨਿਰਾ,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [-T] ਸਰੋਤ DEST\n\
  or:  %s [OPTION]... ਸਰੋਤ... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ਸਰੋਤ...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Copy ਸਰੋਤ ਮੰਜ਼ਿਲ DEST, or ਗੁਣਜ ਸਰੋਤ(s) ਮੰਜ਼ਿਲ DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --ਗੁਣ-only\n\
         don't copy the file ਅੰਕੜੇ, just the ਗੁਣ\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup ਦਾ each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an ਦਲੀਲ\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents ਦਾ special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  ਸਿੱਟਾ_ਦਿੰਦਾ_2 -v\n\
"));
      oputs (_("\
  -f, --force\n\
         ਜੇ an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also ਵਰਤਿਆ)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a ਪਿਛਲਾ -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-ਸਤਰ symbolic links ਵਿੱਚ ਸਰੋਤ\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links ਵਿੱਚ ਸਰੋਤ\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links ਵਿੱਚ ਸਰੋਤ\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks ਮੰਜ਼ਿਲ directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead ਦਾ copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=ਬਹੁਲਕ_2,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified ਗੁਣ\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified ਗੁਣ\n\
"));
      oputs (_("\
      --parents\n\
         use ਭਰਿਆ ਸਰੋਤ_2 file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See ਹੇਠਾਂ\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting ਮੰਜ਼ਿਲ open it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation ਦਾ sparse files. See ਹੇਠਾਂ\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ਸਰੋਤ ਦਲੀਲ\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead ਦਾ copying\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ਨਿਸ਼ਾਨਾ-directory=DIRECTORY\n\
         copy all ਸਰੋਤ arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-ਨਿਸ਼ਾਨਾ-directory\n\
         treat DEST as a ਸਾਧਾਰਨ file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(ਮੂਲ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent ਮੰਜ਼ਿਲ --update[=older].  See ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being ਪੂਰਾ\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         ਸੈੱਟ_2 SELinux security context ਦਾ destination file ਮੰਜ਼ਿਲ ਮੂਲ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ਜੇ CTX is specified then ਸੈੱਟ_2 the\n\
         SELinux or SMACK security context ਮੰਜ਼ਿਲ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list ਦਾ ਗੁਣ. ਗੁਣ_2 are 'mode' ਲਈ\n\
permissions (including any ACL and xattr permissions), 'ownership' ਲਈ user\n\
and ਸਮੂਹ, 'timestamps' ਲਈ file timestamps, 'links' ਲਈ hard links, 'context'\
\nfor security context, 'xattr' ਲਈ extended ਗੁਣ, and 'all' ਲਈ all\n\
ਗੁਣ.\n\
"), stdout);
      fputs (_("\
\n\
By ਮੂਲ, sparse ਸਰੋਤ files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=ਸਵੈਚਾਲਿਤ.  Specify --sparse=always ਮੰਜ਼ਿਲ create a sparse DEST\n\
file whenever the ਸਰੋਤ file ਸ਼ਾਮਲ_ਹੈ a ਲੰਮਾ enough sequence ਦਾ zero ਬਾਈਟਾਂ.\n\
Use --sparse=never ਮੰਜ਼ਿਲ inhibit creation ਦਾ sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By ਮੂਲ or with --reflink=ਸਵੈਚਾਲਿਤ, cp will try a lightweight copy, where the\n\
ਅੰਕੜੇ ਖੰਡਾਂ are copied only when modified, falling ਪਿੱਠ ਮੰਜ਼ਿਲ a standard copy\n\
ਜੇ this is not possible.  With --reflink[=always] cp will fail ਜੇ CoW is not\n\
supported, ਜਦੋਂ --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special ਹਾਲਤ, cp makes a backup ਦਾ ਸਰੋਤ when the force and backup\n\
options are given and ਸਰੋਤ and DEST are the same name ਲਈ an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
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

ਸਥਿਰ bool
re_protect (ਅੱਖਰ ਅਚਲ *const_dst_name, ਅੱਖਰ ਅਚਲ *dst_src_name,
            ਪੂਰਨ dst_dirfd, ਅੱਖਰ ਅਚਲ *dst_relname,
            ਬਣਤਰ dir_attr *attr_list, ਅਚਲ ਬਣਤਰ cp_options *x)
{
  ਅੱਖਰ *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  ਸੌਂਪੋ_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  ਅੱਖਰ ਅਚਲ *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  ਅੱਖਰ ਅਚਲ *relname = dst_name + (dst_relname - const_dst_name);

  ਲਈ (ਬਣਤਰ dir_attr *p = attr_list; p; p = p->ਅਗਲਾ)
    {
      dst_name[p->slash_ਵਿੱਥ] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      ਜੇ (x->preserve_timestamps)
        {
          ਬਣਤਰ timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          ਜੇ (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              ਵਾਪਸ false;
            }
        }

      ਜੇ (x->preserve_ownership)
        {
          ਜੇ (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              ਜੇ (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  ਵਾਪਸ false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_ਮੁੱਲ (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      ਜੇ (x->preserve_ਬਹੁਲਕ)
        {
          ਜੇ (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_ਬਹੁਲਕ) != 0)
            ਵਾਪਸ false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (p->restore_ਬਹੁਲਕ)
        {
          ਜੇ (lchmodat (dst_dirfd, relname, p->st.st_ਬਹੁਲਕ) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              ਵਾਪਸ false;
            }
        }

      dst_name[p->slash_ਵਿੱਥ] = '/';
    }
  ਵਾਪਸ true;
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

ਸਥਿਰ bool
make_dir_parents_ਨਿੱਜੀ (ਅੱਖਰ ਅਚਲ *const_dir, size_t src_ਵਿੱਥ,
                          ਪੂਰਨ dst_dirfd,
                          ਅੱਖਰ ਅਚਲ *verbose_fmt_string,
                          ਬਣਤਰ dir_attr **attr_list, bool *new_dst,
                          ਅਚਲ ਬਣਤਰ cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = ਨਿਰਾ;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  ਜੇ (dirlen <= src_ਵਿੱਥ)
    ਵਾਪਸ true;

  /* A copy of CONST_DIR we can change.  */
  ਅੱਖਰ *dir;
  ਸੌਂਪੋ_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  ਅੱਖਰ *src = dir + src_ਵਿੱਥ;

  /* Leading directory of DIR.  */
  ਅੱਖਰ *dst_dir = alloca (dirlen + 1);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  ਅੱਖਰ ਅਚਲ *dst_reldir = dst_dir + src_ਵਿੱਥ;
  ਜਦੋਂ (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  ਬਣਤਰ stat stats;
  ਜੇ (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      ਅੱਖਰ *slash = src;

      ਜਦੋਂ (*slash == '/')
        slash++;
      dst_reldir = slash;

      ਜਦੋਂ ((slash = strchr (slash, '/')))
        {
          ਬਣਤਰ dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          ਜੇ (missing_dir || x->preserve_ownership || x->preserve_ਬਹੁਲਕ
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              ਬਣਤਰ stat src_st;
              ਪੂਰਨ src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_ਬਹੁਲਕ)
                               ? 0
                               : ENOTDIR);
              ਜੇ (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  ਵਾਪਸ false;
                }

              new = xmalloc (ਆਕਾਰ *new);
              new->st = src_st;
              new->slash_ਵਿੱਥ = slash - dir;
              new->restore_ਬਹੁਲਕ = false;
              new->ਅਗਲਾ = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          ਜੇ (! ਸੈੱਟ_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_ਬਹੁਲਕ : 0,
                                          missing_dir, x))
            ਵਾਪਸ false;

          ਜੇ (missing_dir)
            {
              /* This component does not exist.  We must set
                 *new_dst and new->st.st_mode inside this loop because,
                 for example, in the command 'cp --parents ../a/../b/c e_dir',
                 make_dir_parents_private creates only e_dir/../a if
                 ./b already exists. */
              *new_dst = true;
              mode_t src_ਬਹੁਲਕ = new->st.st_ਬਹੁਲਕ;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_ਬਹੁਲਕ
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_ਬਹੁਲਕ
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_ਬਹੁਲਕ = (x->explicit_no_preserve_ਬਹੁਲਕ
                                   ? S_IRWXUGO : src_ਬਹੁਲਕ);
              mkdir_ਬਹੁਲਕ &= CHMOD_ਬਹੁਲਕ_ਬਿੱਟ & ~omitted_permissions;
              ਜੇ (mkdirat (dst_dirfd, dst_reldir, mkdir_ਬਹੁਲਕ) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  ਵਾਪਸ false;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਜੇ (verbose_fmt_string != ਨਿਰਾ)
                    ਲਿਖੋ (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              ਜੇ (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  ਵਾਪਸ false;
                }


              ਜੇ (! x->preserve_ਬਹੁਲਕ)
                {
                  ਜੇ (omitted_permissions & ~stats.st_ਬਹੁਲਕ)
                    omitted_permissions &= ~ cached_umask ();
                  ਜੇ (omitted_permissions & ~stats.st_ਬਹੁਲਕ
                      || (stats.st_ਬਹੁਲਕ & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_ਬਹੁਲਕ = stats.st_ਬਹੁਲਕ | omitted_permissions;
                      new->restore_ਬਹੁਲਕ = true;
                    }
                }

              mode_t accessible = stats.st_ਬਹੁਲਕ | S_IRWXU;
              ਜੇ (stats.st_ਬਹੁਲਕ != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  ਜੇ (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      ਵਾਪਸ false;
                    }
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (!S_ISDIR (stats.st_ਬਹੁਲਕ))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              ਵਾਪਸ false;
            }
          ਨਹੀਂਤਾਂ
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          ਜੇ (! *new_dst
              && (x->ਸੈੱਟ_security_context || x->preserve_security_context))
            {
              ਜੇ (! ਸੈੱਟ_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                ਵਾਪਸ false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          ਜਦੋਂ (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  ਨਹੀਂਤਾਂ ਜੇ (!S_ISDIR (stats.st_ਬਹੁਲਕ))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      ਵਾਪਸ false;
    }
  ਨਹੀਂਤਾਂ
    {
      *new_dst = false;
    }
  ਵਾਪਸ true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

ਸਥਿਰ bool
do_copy (ਪੂਰਨ n_files, ਅੱਖਰ **file, ਅੱਖਰ ਅਚਲ *ਨਿਸ਼ਾਨਾ_directory,
         bool no_ਨਿਸ਼ਾਨਾ_directory, ਬਣਤਰ cp_options *x)
{
  ਜੇ (n_files <= !ਨਿਸ਼ਾਨਾ_directory)
    {
      ਜੇ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ਬਣਤਰ stat sb;
  sb.st_ਬਹੁਲਕ = 0;
  ਪੂਰਨ ਨਿਸ਼ਾਨਾ_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  ਜੇ (no_ਨਿਸ਼ਾਨਾ_directory)
    {
      ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ਜੇ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
    {
      ਨਿਸ਼ਾਨਾ_dirfd = ਨਿਸ਼ਾਨਾ_directory_ਸੰਕਾਰਜ (ਨਿਸ਼ਾਨਾ_directory, &sb);
      ਜੇ (! ਨਿਸ਼ਾਨਾ_dirfd_valid (ਨਿਸ਼ਾਨਾ_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (ਨਿਸ਼ਾਨਾ_directory));
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ ਅਚਲ *lastfile = file[n_files - 1];
      ਪੂਰਨ fd = ਨਿਸ਼ਾਨਾ_directory_ਸੰਕਾਰਜ (lastfile, &sb);
      ਜੇ (ਨਿਸ਼ਾਨਾ_dirfd_valid (fd))
        {
          ਨਿਸ਼ਾਨਾ_dirfd = fd;
          ਨਿਸ਼ਾਨਾ_directory = lastfile;
          n_files--;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ err = errno;
          ਜੇ (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          ਜੇ (2 < n_files
              || (O_PATHSEARCH == O_ਖੋਜ && err == EACCES
                  && (sb.st_ਬਹੁਲਕ || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_ਬਹੁਲਕ)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      ਜੇ (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
        {
          ਅੱਖਰ *dst_name;
          bool ਮਾਪਾ_exists = true;  /* True if dir_name (dst_name) exists. */
          ਬਣਤਰ dir_attr *attr_list;
          ਅੱਖਰ *arg_ਵਿੱਚ_concat;
          ਅੱਖਰ *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          ਜੇ (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          ਜੇ (parents_option)
            {
              ਅੱਖਰ *arg_no_trailing_slash;

              /* Use 'arg' without trailing slashes in constructing destination
                 file names.  Otherwise, we can end up trying to create a
                 directory using a name with trailing slash, which fails on
                 NetBSD 1.[34] systems.  */
              ਸੌਂਪੋ_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (ਨਿਸ਼ਾਨਾ_directory,
                                           arg_no_trailing_slash,
                                           &arg_ਵਿੱਚ_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              ਮਾਪਾ_exists =
                (make_dir_parents_ਨਿੱਜੀ
                 (dst_name, arg_ਵਿੱਚ_concat - dst_name, ਨਿਸ਼ਾਨਾ_dirfd,
                  (x->verbose ? "%s -> %s\n" : ਨਿਰਾ),
                  &attr_list, &new_dst, x));
            }
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ *arg_ਅਧਾਰ;
              /* Append the last component of 'arg' to 'target_directory'.  */
              ਸੌਂਪੋ_STRDUPA (arg_ਅਧਾਰ, ਆਖਰੀ_component (arg));
              strip_trailing_slashes (arg_ਅਧਾਰ);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_ਅਧਾਰ += streq (arg_ਅਧਾਰ, "..");
              dst_name = file_name_concat (ਨਿਸ਼ਾਨਾ_directory, arg_ਅਧਾਰ,
                                           &arg_ਵਿੱਚ_concat);
            }

          ਜੇ (!ਮਾਪਾ_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ ਅਚਲ *dst_relname = arg_ਵਿੱਚ_concat;
              ਜਦੋਂ (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, ਨਿਸ਼ਾਨਾ_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, ਨਿਰਾ);

              ਜੇ (parents_option)
                ok &= re_protect (dst_name, arg_ਵਿੱਚ_concat, ਨਿਸ਼ਾਨਾ_dirfd,
                                  dst_relname, attr_list, x);
            }

          ਜੇ (parents_option)
            {
              ਜਦੋਂ (attr_list)
                {
                  ਬਣਤਰ dir_attr *p = attr_list;
                  attr_list = attr_list->ਅਗਲਾ;
                  ਮੁਕਤ (p);
                }
            }

          ਮੁਕਤ (dst_name);
        }
    }
  ਨਹੀਂਤਾਂ /* !target_directory */
    {
      ਜੇ (parents_option)
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

      ਅੱਖਰ ਅਚਲ *ਸਰੋਤ_2 = file[0];
      ਅੱਖਰ ਅਚਲ *dest = file[1];
      ਜੇ (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (ਸਰੋਤ_2, dest)
          && !new_dst
          && (sb.st_ਬਹੁਲਕ != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_ਬਹੁਲਕ))
        {
          ਸਥਿਰ ਬਣਤਰ cp_options x_tmp;

          dest = ਲੱਭੋ_backup_file_name (AT_FDCWD, dest, x->backup_type);
          /* Set x->backup_type to 'no_backups' so that the normal backup
             mechanism is not used when performing the actual copy.
             backup_type must be set to 'no_backups' only *after* the above
             call to find_backup_file_name -- that function uses
             backup_type to determine the suffix it applies.  */
          x_tmp = *x;
          x_tmp.backup_type = no_backups;
          x = &x_tmp;
        }

      bool ਅਣਵਰਤਿਆ;
      ok = copy (ਸਰੋਤ_2, dest, AT_FDCWD, dest, -new_dst, x, &ਅਣਵਰਤਿਆ, ਨਿਰਾ);
    }

  ਵਾਪਸ ok;
}

ਸਥਿਰ ਖਾਲੀ
cp_option_init (ਬਣਤਰ cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->dereference = DEREF_UNDEFINED;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_ਬਹੁਲਕ = false;
  x->install_ਬਹੁਲਕ = false;
  x->one_file_system = false;
  x->reflink_ਬਹੁਲਕ = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_ਬਹੁਲਕ = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_ਬਹੁਲਕ = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->ਸੈੱਟ_security_context = ਨਿਰਾ;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->ਅੰਕੜੇ_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_ਬਹੁਲਕ = SPARSE_AUTO;
  x->symbolic_link = false;
  x->ਸੈੱਟ_ਬਹੁਲਕ = false;
  x->ਬਹੁਲਕ_2 = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != ਨਿਰਾ;

  x->dest_info = ਨਿਰਾ;
  x->src_info = ਨਿਰਾ;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
ਸਥਿਰ ਖਾਲੀ
decode_preserve_arg (ਅੱਖਰ ਅਚਲ *arg, ਬਣਤਰ cp_options *x, bool on_off)
{
  ਗਿਣਤੀ File_attribute
    {
      PRESERVE_ਬਹੁਲਕ,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  ਸਥਿਰ ਗਿਣਤੀ File_attribute ਅਚਲ preserve_vals[] =
    {
      PRESERVE_ਬਹੁਲਕ, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ preserve_ਦਲੀਲਾਂ[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", ਨਿਰਾ
    };
  ARGMATCH_VERIFY (preserve_ਦਲੀਲਾਂ, preserve_vals);

  ਅੱਖਰ *arg_writable = xstrdup (arg);
  ਅੱਖਰ *s = arg_writable;
  ਕਰੋ
    {
      /* find next comma */
      ਅੱਖਰ *comma = strchr (s, ',');
      ਗਿਣਤੀ File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      ਜੇ (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_ਦਲੀਲਾਂ, preserve_vals);
      ਚੋਣ (val)
        {
        ਹਾਲਤ PRESERVE_ਬਹੁਲਕ:
          x->preserve_ਬਹੁਲਕ = on_off;
          x->explicit_no_preserve_ਬਹੁਲਕ = !on_off;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_LINK:
          x->preserve_links = on_off;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_ALL:
          x->preserve_ਬਹੁਲਕ = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_ਬਹੁਲਕ = !on_off;
          ਜੇ (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          ਰੋਕੋ;

        ਮੂਲ:
          affirm (false);
        }
      s = comma;
    }
  ਜਦੋਂ (s);

  ਮੁਕਤ (arg_writable);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool make_backups = false;
  ਅੱਖਰ ਅਚਲ *backup_suffix = ਨਿਰਾ;
  ਅੱਖਰ *ਸੰਸਕਰਣ_control_string = ਨਿਰਾ;
  ਬਣਤਰ cp_options x;
  bool copy_contents = false;
  ਅੱਖਰ *ਨਿਸ਼ਾਨਾ_directory = ਨਿਰਾ;
  bool no_ਨਿਸ਼ਾਨਾ_directory = false;
  ਅੱਖਰ ਅਚਲ *scontext = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  ਪੂਰਨ c;
  ਜਦੋਂ ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ SPARSE_OPTION:
          x.sparse_ਬਹੁਲਕ = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          ਰੋਕੋ;

        ਹਾਲਤ REFLINK_OPTION:
          ਜੇ (optarg == ਨਿਰਾ)
            x.reflink_ਬਹੁਲਕ = REFLINK_ALWAYS;
          ਨਹੀਂਤਾਂ
            x.reflink_ਬਹੁਲਕ = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          ਰੋਕੋ;

        ਹਾਲਤ 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_ਬਹੁਲਕ = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ਜੇ (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          make_backups = true;
          ਜੇ (optarg)
            ਸੰਸਕਰਣ_control_string = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ ਗੁਣ_ONLY_OPTION:
          x.ਅੰਕੜੇ_copy_required = false;
          ਰੋਕੋ;

        ਹਾਲਤ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ਰੋਕੋ;

        ਹਾਲਤ COPY_CONTENTS_OPTION:
          copy_contents = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          x.unlink_dest_after_failed_open = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'H':
          x.dereference = DEREF_COMMAND_ਸਤਰ_ARGUMENTS;
          ਰੋਕੋ;

        ਹਾਲਤ 'i':
          x.interactive = I_ASK_USER;
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          x.hard_link = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'L':
          x.dereference = DEREF_ALWAYS;
          ਰੋਕੋ;

        ਹਾਲਤ 'n':
          x.interactive = I_ALWAYS_SKIP;
          ਰੋਕੋ;

        ਹਾਲਤ 'P':
          x.dereference = DEREF_NEVER;
          ਰੋਕੋ;

        ਹਾਲਤ NO_PRESERVE_ਗੁਣ_OPTION:
          decode_preserve_arg (optarg, &x, false);
          ਰੋਕੋ;

        ਹਾਲਤ PRESERVE_ਗੁਣ_OPTION:
          ਜੇ (optarg == ਨਿਰਾ)
            {
              /* Fall through to the case for 'p' below.  */
            }
          ਨਹੀਂਤਾਂ
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              ਰੋਕੋ;
            }
          FALLTHROUGH;

        ਹਾਲਤ 'p':
          x.preserve_ownership = true;
          x.preserve_ਬਹੁਲਕ = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ਰੋਕੋ;

        ਹਾਲਤ PARENTS_OPTION:
          parents_option = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
        ਹਾਲਤ 'R':
          x.recursive = true;
          ਰੋਕੋ;

        ਹਾਲਤ UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          ਰੋਕੋ;

        ਹਾਲਤ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          x.symbolic_link = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          ਜੇ (ਨਿਸ਼ਾਨਾ_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          ਨਿਸ਼ਾਨਾ_directory = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'T':
          no_ਨਿਸ਼ਾਨਾ_directory = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          x.update = UPDATE_OLDER;
          ਜੇ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          x.verbose = true;
          ਰੋਕੋ;

        ਹਾਲਤ KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'x':
          x.one_file_system = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          ਜੇ (selinux_enabled)
            {
              ਜੇ (optarg)
                scontext = optarg;
              ਨਹੀਂਤਾਂ
                {
                  x.ਸੈੱਟ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         ਨਿਰਾ, 0);
                  ਜੇ (! x.ਸੈੱਟ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  ਜੇ (x.reflink_ਬਹੁਲਕ == REFLINK_AUTO && x.sparse_ਬਹੁਲਕ == SPARSE_NEVER)
    x.reflink_ਬਹੁਲਕ = REFLINK_NEVER;

  ਜੇ (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ਜੇ (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (x.reflink_ਬਹੁਲਕ == REFLINK_ALWAYS && x.sparse_ਬਹੁਲਕ != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_ਸੰਸਕਰਣ (_("backup type"),
                                   ਸੰਸਕਰਣ_control_string)
                   : no_backups);
  ਸੈੱਟ_simple_backup_suffix (backup_suffix);

  ਜੇ (x.dereference == DEREF_UNDEFINED)
    {
      ਜੇ (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      ਨਹੀਂਤਾਂ
        x.dereference = DEREF_ALWAYS;
    }

  ਜੇ (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  ਜੇ ((x.ਸੈੱਟ_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  ਜੇ (x.preserve_security_context && (x.ਸੈੱਟ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ਜੇ (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  ਜੇ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#ਜੇ !USE_XATTR
  ਜੇ (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#ਜੇ_ਅੰਤ

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     ਨਿਸ਼ਾਨਾ_directory, no_ਨਿਸ਼ਾਨਾ_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
