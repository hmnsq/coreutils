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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "cp"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
ರಚನೆ dir_attr
{
  ರಚನೆ stat st;
  bool restore_ಬಹುಲಕ;
  size_t slash_ಸ್ಥಾನಾಂತರ;
  ರಚನೆ dir_attr *ಮುಂದಿನ;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  ಗುಣಲಕ್ಷಣಗಳು_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_ಗುಣಲಕ್ಷಣಗಳು_OPTION,
  PARENTS_OPTION,
  PRESERVE_ಗುಣಲಕ್ಷಣಗಳು_OPTION,
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

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ sparse_type_string[] =
{
  "never", "auto", "always", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ Sparse_type ಸ್ಥಿರಾಂಕ sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ reflink_type_string[] =
{
  "auto", "always", "never", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ Reflink_type ಸ್ಥಿರಾಂಕ reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ update_type_string[] =
{
  "all", "none", "none-fail", "older", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ Update_type ಸ್ಥಿರಾಂಕ update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_opts[] =
{
  {"archive", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"attributes-only", no_ವಾದ, ಖಾಲಿ, ಗುಣಲಕ್ಷಣಗಳು_ONLY_OPTION},
  {"backup", optional_ವಾದ, ಖಾಲಿ, 'b'},
  {"copy-contents", no_ವಾದ, ಖಾಲಿ, COPY_CONTENTS_OPTION},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_OPTION},
  {"dereference", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"force", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"interactive", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"link", no_ವಾದ, ಖಾಲಿ, 'l'},
  {"no-clobber", no_ವಾದ, ಖಾಲಿ, 'n'},   /* Deprecated.  */
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'P'},
  {"no-preserve", required_ವಾದ, ಖಾಲಿ, NO_PRESERVE_ಗುಣಲಕ್ಷಣಗಳು_OPTION},
  {"no-target-directory", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"one-file-system", no_ವಾದ, ಖಾಲಿ, 'x'},
  {"parents", no_ವಾದ, ಖಾಲಿ, PARENTS_OPTION},
  {"path", no_ವಾದ, ಖಾಲಿ, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_ವಾದ, ಖಾಲಿ, PRESERVE_ಗುಣಲಕ್ಷಣಗಳು_OPTION},
  {"recursive", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"remove-destination", no_ವಾದ, ಖಾಲಿ, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_ವಾದ, ಖಾಲಿ, SPARSE_OPTION},
  {"reflink", optional_ವಾದ, ಖಾಲಿ, REFLINK_OPTION},
  {"strip-trailing-slashes", no_ವಾದ, ಖಾಲಿ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_ವಾದ, ಖಾಲಿ, 'S'},
  {"symbolic-link", no_ವಾದ, ಖಾಲಿ, 's'},
  {"target-directory", required_ವಾದ, ಖಾಲಿ, 't'},
  {"update", optional_ವಾದ, ಖಾಲಿ, 'u'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {"keep-directory-symlink", no_ವಾದ, ಖಾಲಿ,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [-T] ಮೂಲ DEST\n\
  or:  %s [OPTION]... ಮೂಲ... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ಮೂಲ...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Copy ಮೂಲ ಗಮ್ಯ DEST, or ಅಪವರ್ತ್ಯ ಮೂಲ(s) ಗಮ್ಯ DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --ಗುಣಲಕ್ಷಣಗಳು-only\n\
         don't copy the file ದತ್ತಾಂಶ, just the ಗುಣಲಕ್ಷಣಗಳು\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup ದ each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an ವಾದ\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents ದ special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  ಸೂಚಿಸುತ್ತದೆ_2 -v\n\
"));
      oputs (_("\
  -f, --force\n\
         ಆದರೆ an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also ಬಳಸಿದ)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a ಹಿಂದಿನ -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-ಸಾಲು symbolic links ಒಳ ಮೂಲ\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links ಒಳ ಮೂಲ\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links ಒಳ ಮೂಲ\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks ಗಮ್ಯ directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead ದ copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=ಬಹುಲಕ_2,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified ಗುಣಲಕ್ಷಣಗಳು\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified ಗುಣಲಕ್ಷಣಗಳು\n\
"));
      oputs (_("\
      --parents\n\
         use ತುಂಬಿದ ಮೂಲ_2 file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See ಕೆಳಗೆ\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting ಗಮ್ಯ open it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation ದ sparse files. See ಕೆಳಗೆ\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ಮೂಲ ವಾದ\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead ದ copying\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --ಗುರಿ-directory=DIRECTORY\n\
         copy all ಮೂಲ arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-ಗುರಿ-directory\n\
         treat DEST as a ಸಾಮಾನ್ಯ file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(ಪೂರ್ವನಿಯೋಜಿತ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent ಗಮ್ಯ --update[=older].  See ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being ಮುಗಿದ\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         ಗಣ_2 SELinux security context ದ destination file ಗಮ್ಯ ಪೂರ್ವನಿಯೋಜಿತ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಆದರೆ CTX is specified then ಗಣ_2 the\n\
         SELinux or SMACK security context ಗಮ್ಯ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list ದ ಗುಣಲಕ್ಷಣಗಳು. ಗುಣಲಕ್ಷಣಗಳು_2 are 'mode' ಗಾಗಿ\n\
permissions (including any ACL and xattr permissions), 'ownership' ಗಾಗಿ user\n\
and ಗುಂಪು, 'timestamps' ಗಾಗಿ file timestamps, 'links' ಗಾಗಿ hard links, 'context'\
\nfor security context, 'xattr' ಗಾಗಿ extended ಗುಣಲಕ್ಷಣಗಳು, and 'all' ಗಾಗಿ all\n\
ಗುಣಲಕ್ಷಣಗಳು.\n\
"), stdout);
      fputs (_("\
\n\
By ಪೂರ್ವನಿಯೋಜಿತ, sparse ಮೂಲ files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=ಸ್ವಯಂಚಾಲಿತ.  Specify --sparse=always ಗಮ್ಯ create a sparse DEST\n\
file whenever the ಮೂಲ file ಒಳಗೊಂಡಿದೆ a ದೀರ್ಘ enough sequence ದ zero ಬೈಟುಗಳು.\n\
Use --sparse=never ಗಮ್ಯ inhibit creation ದ sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By ಪೂರ್ವನಿಯೋಜಿತ or with --reflink=ಸ್ವಯಂಚಾಲಿತ, cp will try a lightweight copy, where the\n\
ದತ್ತಾಂಶ ಖಂಡಗಳು are copied only when modified, falling ಹಿಂಭಾಗ ಗಮ್ಯ a standard copy\n\
ಆದರೆ this is not possible.  With --reflink[=always] cp will fail ಆದರೆ CoW is not\n\
supported, ಆಗ --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special ಸ್ಥಿತಿ, cp makes a backup ದ ಮೂಲ when the force and backup\n\
options are given and ಮೂಲ and DEST are the same name ಗಾಗಿ an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
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
re_protect (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *const_dst_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_src_name,
            ಪೂರ್ಣಾಂಕ dst_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname,
            ರಚನೆ dir_attr *attr_list, ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ಅಕ್ಷರ *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  ನಿಯೋಜಿಸು_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relname = dst_name + (dst_relname - const_dst_name);

  ಗಾಗಿ (ರಚನೆ dir_attr *p = attr_list; p; p = p->ಮುಂದಿನ)
    {
      dst_name[p->slash_ಸ್ಥಾನಾಂತರ] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      ಆದರೆ (x->preserve_timestamps)
        {
          ರಚನೆ timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          ಆದರೆ (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              ಹಿಂತಿರುಗಿ false;
            }
        }

      ಆದರೆ (x->preserve_ownership)
        {
          ಆದರೆ (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              ಆದರೆ (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  ಹಿಂತಿರುಗಿ false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_ಮೌಲ್ಯ (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      ಆದರೆ (x->preserve_ಬಹುಲಕ)
        {
          ಆದರೆ (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_ಬಹುಲಕ) != 0)
            ಹಿಂತಿರುಗಿ false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->restore_ಬಹುಲಕ)
        {
          ಆದರೆ (lchmodat (dst_dirfd, relname, p->st.st_ಬಹುಲಕ) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              ಹಿಂತಿರುಗಿ false;
            }
        }

      dst_name[p->slash_ಸ್ಥಾನಾಂತರ] = '/';
    }
  ಹಿಂತಿರುಗಿ true;
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
make_dir_parents_ಖಾಸಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *const_dir, size_t src_ಸ್ಥಾನಾಂತರ,
                          ಪೂರ್ಣಾಂಕ dst_dirfd,
                          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *verbose_fmt_string,
                          ರಚನೆ dir_attr **attr_list, bool *new_dst,
                          ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = ಖಾಲಿ;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  ಆದರೆ (dirlen <= src_ಸ್ಥಾನಾಂತರ)
    ಹಿಂತಿರುಗಿ true;

  /* A copy of CONST_DIR we can change.  */
  ಅಕ್ಷರ *dir;
  ನಿಯೋಜಿಸು_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  ಅಕ್ಷರ *src = dir + src_ಸ್ಥಾನಾಂತರ;

  /* Leading directory of DIR.  */
  ಅಕ್ಷರ *dst_dir = alloca (dirlen + 1);
  ಸ್ಮರಣೆ_ನಕಲು (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_reldir = dst_dir + src_ಸ್ಥಾನಾಂತರ;
  ಆಗ (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  ರಚನೆ stat stats;
  ಆದರೆ (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      ಅಕ್ಷರ *slash = src;

      ಆಗ (*slash == '/')
        slash++;
      dst_reldir = slash;

      ಆಗ ((slash = strchr (slash, '/')))
        {
          ರಚನೆ dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          ಆದರೆ (missing_dir || x->preserve_ownership || x->preserve_ಬಹುಲಕ
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              ರಚನೆ stat src_st;
              ಪೂರ್ಣಾಂಕ src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_ಬಹುಲಕ)
                               ? 0
                               : ENOTDIR);
              ಆದರೆ (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  ಹಿಂತಿರುಗಿ false;
                }

              new = xmalloc (ಗಾತ್ರ *new);
              new->st = src_st;
              new->slash_ಸ್ಥಾನಾಂತರ = slash - dir;
              new->restore_ಬಹುಲಕ = false;
              new->ಮುಂದಿನ = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          ಆದರೆ (! ಗಣ_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_ಬಹುಲಕ : 0,
                                          missing_dir, x))
            ಹಿಂತಿರುಗಿ false;

          ಆದರೆ (missing_dir)
            {
              /* This component does not exist.  We must set
                 *new_dst and new->st.st_mode inside this loop because,
                 for example, in the command 'cp --parents ../a/../b/c e_dir',
                 make_dir_parents_private creates only e_dir/../a if
                 ./b already exists. */
              *new_dst = true;
              mode_t src_ಬಹುಲಕ = new->st.st_ಬಹುಲಕ;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_ಬಹುಲಕ
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_ಬಹುಲಕ
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_ಬಹುಲಕ = (x->explicit_no_preserve_ಬಹುಲಕ
                                   ? S_IRWXUGO : src_ಬಹುಲಕ);
              mkdir_ಬಹುಲಕ &= CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು & ~omitted_permissions;
              ಆದರೆ (mkdirat (dst_dirfd, dst_reldir, mkdir_ಬಹುಲಕ) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  ಹಿಂತಿರುಗಿ false;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಆದರೆ (verbose_fmt_string != ಖಾಲಿ)
                    ಬರೆ (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              ಆದರೆ (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  ಹಿಂತಿರುಗಿ false;
                }


              ಆದರೆ (! x->preserve_ಬಹುಲಕ)
                {
                  ಆದರೆ (omitted_permissions & ~stats.st_ಬಹುಲಕ)
                    omitted_permissions &= ~ cached_umask ();
                  ಆದರೆ (omitted_permissions & ~stats.st_ಬಹುಲಕ
                      || (stats.st_ಬಹುಲಕ & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_ಬಹುಲಕ = stats.st_ಬಹುಲಕ | omitted_permissions;
                      new->restore_ಬಹುಲಕ = true;
                    }
                }

              mode_t accessible = stats.st_ಬಹುಲಕ | S_IRWXU;
              ಆದರೆ (stats.st_ಬಹುಲಕ != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  ಆದರೆ (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      ಹಿಂತಿರುಗಿ false;
                    }
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!S_ISDIR (stats.st_ಬಹುಲಕ))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              ಹಿಂತಿರುಗಿ false;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          ಆದರೆ (! *new_dst
              && (x->ಗಣ_security_context || x->preserve_security_context))
            {
              ಆದರೆ (! ಗಣ_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                ಹಿಂತಿರುಗಿ false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          ಆಗ (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!S_ISDIR (stats.st_ಬಹುಲಕ))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      ಹಿಂತಿರುಗಿ false;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *new_dst = false;
    }
  ಹಿಂತಿರುಗಿ true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

ಸ್ಥಿರ bool
do_copy (ಪೂರ್ಣಾಂಕ n_files, ಅಕ್ಷರ **file, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುರಿ_directory,
         bool no_ಗುರಿ_directory, ರಚನೆ cp_options *x)
{
  ಆದರೆ (n_files <= !ಗುರಿ_directory)
    {
      ಆದರೆ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ರಚನೆ stat sb;
  sb.st_ಬಹುಲಕ = 0;
  ಪೂರ್ಣಾಂಕ ಗುರಿ_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  ಆದರೆ (no_ಗುರಿ_directory)
    {
      ಆದರೆ (ಗುರಿ_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ಆದರೆ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗುರಿ_directory)
    {
      ಗುರಿ_dirfd = ಗುರಿ_directory_ಕಾರ್ಯಾಂಶ (ಗುರಿ_directory, &sb);
      ಆದರೆ (! ಗುರಿ_dirfd_valid (ಗುರಿ_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (ಗುರಿ_directory));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lastfile = file[n_files - 1];
      ಪೂರ್ಣಾಂಕ fd = ಗುರಿ_directory_ಕಾರ್ಯಾಂಶ (lastfile, &sb);
      ಆದರೆ (ಗುರಿ_dirfd_valid (fd))
        {
          ಗುರಿ_dirfd = fd;
          ಗುರಿ_directory = lastfile;
          n_files--;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ err = errno;
          ಆದರೆ (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          ಆದರೆ (2 < n_files
              || (O_PATHSEARCH == O_ಹುಡುಕು && err == EACCES
                  && (sb.st_ಬಹುಲಕ || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_ಬಹುಲಕ)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  ಆದರೆ (ಗುರಿ_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      ಆದರೆ (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
        {
          ಅಕ್ಷರ *dst_name;
          bool ಪೋಷಕ_exists = true;  /* True if dir_name (dst_name) exists. */
          ರಚನೆ dir_attr *attr_list;
          ಅಕ್ಷರ *arg_ಒಳ_concat;
          ಅಕ್ಷರ *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          ಆದರೆ (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          ಆದರೆ (parents_option)
            {
              ಅಕ್ಷರ *arg_no_trailing_slash;

              /* Use 'arg' without trailing slashes in constructing destination
                 file names.  Otherwise, we can end up trying to create a
                 directory using a name with trailing slash, which fails on
                 NetBSD 1.[34] systems.  */
              ನಿಯೋಜಿಸು_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (ಗುರಿ_directory,
                                           arg_no_trailing_slash,
                                           &arg_ಒಳ_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              ಪೋಷಕ_exists =
                (make_dir_parents_ಖಾಸಗಿ
                 (dst_name, arg_ಒಳ_concat - dst_name, ಗುರಿ_dirfd,
                  (x->verbose ? "%s -> %s\n" : ಖಾಲಿ),
                  &attr_list, &new_dst, x));
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ *arg_ತಳ;
              /* Append the last component of 'arg' to 'target_directory'.  */
              ನಿಯೋಜಿಸು_STRDUPA (arg_ತಳ, ಕೊನೆಯ_component (arg));
              strip_trailing_slashes (arg_ತಳ);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_ತಳ += streq (arg_ತಳ, "..");
              dst_name = file_name_concat (ಗುರಿ_directory, arg_ತಳ,
                                           &arg_ಒಳ_concat);
            }

          ಆದರೆ (!ಪೋಷಕ_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dst_relname = arg_ಒಳ_concat;
              ಆಗ (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, ಗುರಿ_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, ಖಾಲಿ);

              ಆದರೆ (parents_option)
                ok &= re_protect (dst_name, arg_ಒಳ_concat, ಗುರಿ_dirfd,
                                  dst_relname, attr_list, x);
            }

          ಆದರೆ (parents_option)
            {
              ಆಗ (attr_list)
                {
                  ರಚನೆ dir_attr *p = attr_list;
                  attr_list = attr_list->ಮುಂದಿನ;
                  ಬಿಡುಗಡೆ (p);
                }
            }

          ಬಿಡುಗಡೆ (dst_name);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ /* !target_directory */
    {
      ಆದರೆ (parents_option)
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

      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮೂಲ_2 = file[0];
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest = file[1];
      ಆದರೆ (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (ಮೂಲ_2, dest)
          && !new_dst
          && (sb.st_ಬಹುಲಕ != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_ಬಹುಲಕ))
        {
          ಸ್ಥಿರ ರಚನೆ cp_options x_tmp;

          dest = ಕಂಡುಹಿಡಿ_backup_file_name (AT_FDCWD, dest, x->backup_type);
          /* Set x->backup_type to 'no_backups' so that the normal backup
             mechanism is not used when performing the actual copy.
             backup_type must be set to 'no_backups' only *after* the above
             call to find_backup_file_name -- that function uses
             backup_type to determine the suffix it applies.  */
          x_tmp = *x;
          x_tmp.backup_type = no_backups;
          x = &x_tmp;
        }

      bool ಬಳಸದ;
      ok = copy (ಮೂಲ_2, dest, AT_FDCWD, dest, -new_dst, x, &ಬಳಸದ, ಖಾಲಿ);
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cp_option_init (ರಚನೆ cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->dereference = DEREF_UNDEFINED;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_ಬಹುಲಕ = false;
  x->install_ಬಹುಲಕ = false;
  x->one_file_system = false;
  x->reflink_ಬಹುಲಕ = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_ಬಹುಲಕ = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_ಬಹುಲಕ = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->ಗಣ_security_context = ಖಾಲಿ;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->ದತ್ತಾಂಶ_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_ಬಹುಲಕ = SPARSE_AUTO;
  x->symbolic_link = false;
  x->ಗಣ_ಬಹುಲಕ = false;
  x->ಬಹುಲಕ_2 = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != ಖಾಲಿ;

  x->dest_info = ಖಾಲಿ;
  x->src_info = ಖಾಲಿ;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
ಸ್ಥಿರ ಶೂನ್ಯ
decode_preserve_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg, ರಚನೆ cp_options *x, bool on_off)
{
  ಎಣಿಕೆ File_attribute
    {
      PRESERVE_ಬಹುಲಕ,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  ಸ್ಥಿರ ಎಣಿಕೆ File_attribute ಸ್ಥಿರಾಂಕ preserve_vals[] =
    {
      PRESERVE_ಬಹುಲಕ, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ preserve_ವಾದಗಳು[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", ಖಾಲಿ
    };
  ARGMATCH_VERIFY (preserve_ವಾದಗಳು, preserve_vals);

  ಅಕ್ಷರ *arg_writable = xstrdup (arg);
  ಅಕ್ಷರ *s = arg_writable;
  ಮಾಡು
    {
      /* find next comma */
      ಅಕ್ಷರ *comma = strchr (s, ',');
      ಎಣಿಕೆ File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      ಆದರೆ (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_ವಾದಗಳು, preserve_vals);
      ಆಯ್ಕೆ (val)
        {
        ಸ್ಥಿತಿ PRESERVE_ಬಹುಲಕ:
          x->preserve_ಬಹುಲಕ = on_off;
          x->explicit_no_preserve_ಬಹುಲಕ = !on_off;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_LINK:
          x->preserve_links = on_off;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_ALL:
          x->preserve_ಬಹುಲಕ = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_ಬಹುಲಕ = !on_off;
          ಆದರೆ (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (false);
        }
      s = comma;
    }
  ಆಗ (s);

  ಬಿಡುಗಡೆ (arg_writable);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool make_backups = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *backup_suffix = ಖಾಲಿ;
  ಅಕ್ಷರ *ಆವೃತ್ತಿ_control_string = ಖಾಲಿ;
  ರಚನೆ cp_options x;
  bool copy_contents = false;
  ಅಕ್ಷರ *ಗುರಿ_directory = ಖಾಲಿ;
  bool no_ಗುರಿ_directory = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *scontext = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ SPARSE_OPTION:
          x.sparse_ಬಹುಲಕ = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ REFLINK_OPTION:
          ಆದರೆ (optarg == ಖಾಲಿ)
            x.reflink_ಬಹುಲಕ = REFLINK_ALWAYS;
          ಇಲ್ಲದಿದ್ದರೆ
            x.reflink_ಬಹುಲಕ = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_ಬಹುಲಕ = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ಆದರೆ (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          make_backups = true;
          ಆದರೆ (optarg)
            ಆವೃತ್ತಿ_control_string = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗುಣಲಕ್ಷಣಗಳು_ONLY_OPTION:
          x.ದತ್ತಾಂಶ_copy_required = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ COPY_CONTENTS_OPTION:
          copy_contents = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          x.unlink_dest_after_failed_open = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'H':
          x.dereference = DEREF_COMMAND_ಸಾಲು_ARGUMENTS;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'i':
          x.interactive = I_ASK_USER;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          x.hard_link = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L':
          x.dereference = DEREF_ALWAYS;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'n':
          x.interactive = I_ALWAYS_SKIP;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'P':
          x.dereference = DEREF_NEVER;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NO_PRESERVE_ಗುಣಲಕ್ಷಣಗಳು_OPTION:
          decode_preserve_arg (optarg, &x, false);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PRESERVE_ಗುಣಲಕ್ಷಣಗಳು_OPTION:
          ಆದರೆ (optarg == ಖಾಲಿ)
            {
              /* Fall through to the case for 'p' below.  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              ನಿಲ್ಲಿಸು;
            }
          FALLTHROUGH;

        ಸ್ಥಿತಿ 'p':
          x.preserve_ownership = true;
          x.preserve_ಬಹುಲಕ = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PARENTS_OPTION:
          parents_option = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
        ಸ್ಥಿತಿ 'R':
          x.recursive = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          x.symbolic_link = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          ಆದರೆ (ಗುರಿ_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          ಗುರಿ_directory = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'T':
          no_ಗುರಿ_directory = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          x.update = UPDATE_OLDER;
          ಆದರೆ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':
          x.verbose = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'x':
          x.one_file_system = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          ಆದರೆ (selinux_enabled)
            {
              ಆದರೆ (optarg)
                scontext = optarg;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  x.ಗಣ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         ಖಾಲಿ, 0);
                  ಆದರೆ (! x.ಗಣ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  ಆದರೆ (x.reflink_ಬಹುಲಕ == REFLINK_AUTO && x.sparse_ಬಹುಲಕ == SPARSE_NEVER)
    x.reflink_ಬಹುಲಕ = REFLINK_NEVER;

  ಆದರೆ (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ಆದರೆ (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (x.reflink_ಬಹುಲಕ == REFLINK_ALWAYS && x.sparse_ಬಹುಲಕ != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_ಆವೃತ್ತಿ (_("backup type"),
                                   ಆವೃತ್ತಿ_control_string)
                   : no_backups);
  ಗಣ_simple_backup_suffix (backup_suffix);

  ಆದರೆ (x.dereference == DEREF_UNDEFINED)
    {
      ಆದರೆ (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      ಇಲ್ಲದಿದ್ದರೆ
        x.dereference = DEREF_ALWAYS;
    }

  ಆದರೆ (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  ಆದರೆ ((x.ಗಣ_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  ಆದರೆ (x.preserve_security_context && (x.ಗಣ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ಆದರೆ (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  ಆದರೆ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#ಆದರೆ !USE_XATTR
  ಆದರೆ (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#ಆದರೆ_ಅಂತ್ಯ

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     ಗುರಿ_directory, no_ಗುರಿ_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
