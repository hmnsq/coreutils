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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <selinux/label.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "backupfile.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "cp-hash.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "targetdir.h"
#अन्तर्भाव "utimens.h"
#अन्तर्भाव "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "cp"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
संरचना dir_attr
{
  संरचना stat st;
  bool restore_mode;
  माप_प्रकार slash_offset;
  संरचना dir_attr *next;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  ATTRIBUTES_ONLY_OPTION = अक्षर_उच्च + 1,
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
स्थिर bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
स्थिर bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
स्थिर bool remove_trailing_slashes;

स्थिर वर्ण ध्रुव *ध्रुव sparse_type_string[] =
{
  "never", "auto", "always", रिक्त
};
स्थिर गणना Sparse_type ध्रुव sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

स्थिर वर्ण ध्रुव *ध्रुव reflink_type_string[] =
{
  "auto", "always", "never", रिक्त
};
स्थिर गणना Reflink_type ध्रुव reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

स्थिर वर्ण ध्रुव *ध्रुव update_type_string[] =
{
  "all", "none", "none-fail", "older", रिक्त
};
स्थिर गणना Update_type ध्रुव update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

स्थिर संरचना option ध्रुव long_opts[] =
{
  {"archive", no_argument, रिक्त, 'a'},
  {"attributes-only", no_argument, रिक्त, ATTRIBUTES_ONLY_OPTION},
  {"backup", optional_argument, रिक्त, 'b'},
  {"copy-contents", no_argument, रिक्त, COPY_CONTENTS_OPTION},
  {"debug", no_argument, रिक्त, DEBUG_OPTION},
  {"dereference", no_argument, रिक्त, 'L'},
  {"force", no_argument, रिक्त, 'f'},
  {"interactive", no_argument, रिक्त, 'i'},
  {"link", no_argument, रिक्त, 'l'},
  {"no-clobber", no_argument, रिक्त, 'n'},   /* Deprecated.  */
  {"no-dereference", no_argument, रिक्त, 'P'},
  {"no-preserve", required_argument, रिक्त, NO_PRESERVE_ATTRIBUTES_OPTION},
  {"no-target-directory", no_argument, रिक्त, 'T'},
  {"one-file-system", no_argument, रिक्त, 'x'},
  {"parents", no_argument, रिक्त, PARENTS_OPTION},
  {"path", no_argument, रिक्त, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_argument, रिक्त, PRESERVE_ATTRIBUTES_OPTION},
  {"recursive", no_argument, रिक्त, 'R'},
  {"remove-destination", no_argument, रिक्त, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_argument, रिक्त, SPARSE_OPTION},
  {"reflink", optional_argument, रिक्त, REFLINK_OPTION},
  {"strip-trailing-slashes", no_argument, रिक्त,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_argument, रिक्त, 'S'},
  {"symbolic-link", no_argument, रिक्त, 's'},
  {"target-directory", required_argument, रिक्त, 't'},
  {"update", optional_argument, रिक्त, 'u'},
  {"verbose", no_argument, रिक्त, 'v'},
  {"keep-directory-symlink", no_argument, रिक्त,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
"),
              program_name, program_name, program_name);
      ख_माला_दो (_("\
Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n\
"), मानक_निकास);

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
         यदि an existing destination file cannot be opened, हटाओ it and try\n\
         again (this option is ignored when the -n option is also used)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a previous -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-पंक्ति symbolic links in SOURCE\n\
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
      --हटाओ-destination\n\
         हटाओ each existing destination file before attempting to खोलो it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation of sparse files. See below\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         हटाओ any trailing slashes from each SOURCE argument\n\
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
         UPDATE={all,none,none-fail,older(स्वभाव)}\n\
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
  -x, --one-file-प्रणाली\n\
         stay on this file प्रणाली\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of destination file to स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or यदि CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
ATTR_LIST is a comma-separated list of attributes. Attributes are 'mode' प्रति\n\
permissions (including any ACL and xattr permissions), 'ownership' प्रति user\n\
and group, 'timestamps' प्रति file timestamps, 'links' प्रति hard links, 'context'\
\nfor security context, 'xattr' प्रति extended attributes, and 'all' प्रति all\n\
attributes.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
By स्वभाव, sparse SOURCE files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=स्वतः.  Specify --sparse=always to create a sparse DEST\n\
file whenever the SOURCE file contains a दीर्घ enough sequence of zero bytes.\n\
Use --sparse=never to inhibit creation of sparse files.\n\
"), मानक_निकास);
      emit_update_parameters_note ();
      ख_माला_दो (_("\
\n\
By स्वभाव or with --reflink=स्वतः, cp will try a lightweight copy, where the\n\
data blocks are copied only when modified, falling back to a standard copy\n\
यदि this is not possible.  With --reflink[=always] cp will fail यदि CoW is not\n\
supported, यावत् --reflink=never ensures a standard copy is performed.\n\
"), मानक_निकास);
      emit_backup_suffix_note ();
      ख_माला_दो (_("\
\n\
As a special पक्ष, cp makes a backup of SOURCE when the force and backup\n\
options are given and SOURCE and DEST are the same name प्रति an existing,\n\
regular file.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
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

स्थिर bool
re_protect (वर्ण ध्रुव *const_dst_name, वर्ण ध्रुव *dst_src_name,
            पूर्णाङ्क dst_dirfd, वर्ण ध्रुव *dst_relname,
            संरचना dir_attr *attr_list, ध्रुव संरचना cp_options *x)
{
  वर्ण *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  ASSIGN_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  वर्ण ध्रुव *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  वर्ण ध्रुव *relname = dst_name + (dst_relname - const_dst_name);

  प्रति (संरचना dir_attr *p = attr_list; p; p = p->next)
    {
      dst_name[p->slash_offset] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      यदि (x->preserve_timestamps)
        {
          संरचना timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          यदि (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, त्रुटि_सं, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              प्रत्यागम false;
            }
        }

      यदि (x->preserve_ownership)
        {
          यदि (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              यदि (! chown_failure_ok (x))
                {
                  error (0, त्रुटि_सं, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  प्रत्यागम false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_मूल्य (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      यदि (x->preserve_mode)
        {
          यदि (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_mode) != 0)
            प्रत्यागम false;
        }
      अन्यथा यदि (p->restore_mode)
        {
          यदि (lchmodat (dst_dirfd, relname, p->st.st_mode) != 0)
            {
              error (0, त्रुटि_सं, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              प्रत्यागम false;
            }
        }

      dst_name[p->slash_offset] = '/';
    }
  प्रत्यागम true;
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

स्थिर bool
make_dir_parents_private (वर्ण ध्रुव *const_dir, माप_प्रकार src_offset,
                          पूर्णाङ्क dst_dirfd,
                          वर्ण ध्रुव *verbose_fmt_string,
                          संरचना dir_attr **attr_list, bool *new_dst,
                          ध्रुव संरचना cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = रिक्त;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  यदि (dirlen <= src_offset)
    प्रत्यागम true;

  /* A copy of CONST_DIR we can change.  */
  वर्ण *dir;
  ASSIGN_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  वर्ण *src = dir + src_offset;

  /* Leading directory of DIR.  */
  वर्ण *dst_dir = alloca (dirlen + 1);
  स_नकल (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  वर्ण ध्रुव *dst_reldir = dst_dir + src_offset;
  यावत् (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  संरचना stat stats;
  यदि (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      वर्ण *slash = src;

      यावत् (*slash == '/')
        slash++;
      dst_reldir = slash;

      यावत् ((slash = म_अक्षर (slash, '/')))
        {
          संरचना dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          यदि (missing_dir || x->preserve_ownership || x->preserve_mode
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              संरचना stat src_st;
              पूर्णाङ्क src_errno = (stat (src, &src_st) != 0
                               ? त्रुटि_सं
                               : S_ISDIR (src_st.st_mode)
                               ? 0
                               : ENOTDIR);
              यदि (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  प्रत्यागम false;
                }

              new = xmalloc (परिमाण *new);
              new->st = src_st;
              new->slash_offset = slash - dir;
              new->restore_mode = false;
              new->next = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          यदि (! set_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_mode : 0,
                                          missing_dir, x))
            प्रत्यागम false;

          यदि (missing_dir)
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
              यदि (mkdirat (dst_dirfd, dst_reldir, mkdir_mode) != 0)
                {
                  error (0, त्रुटि_सं, _("cannot make directory %s"),
                         quoteaf (dir));
                  प्रत्यागम false;
                }
              अन्यथा
                {
                  यदि (verbose_fmt_string != रिक्त)
                    म_लिखो (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              यदि (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, त्रुटि_सं, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  प्रत्यागम false;
                }


              यदि (! x->preserve_mode)
                {
                  यदि (omitted_permissions & ~stats.st_mode)
                    omitted_permissions &= ~ cached_umask ();
                  यदि (omitted_permissions & ~stats.st_mode
                      || (stats.st_mode & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_mode = stats.st_mode | omitted_permissions;
                      new->restore_mode = true;
                    }
                }

              mode_t accessible = stats.st_mode | S_IRWXU;
              यदि (stats.st_mode != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  यदि (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, त्रुटि_सं, _("setting permissions for %s"),
                             quoteaf (dir));
                      प्रत्यागम false;
                    }
                }
            }
          अन्यथा यदि (!S_ISDIR (stats.st_mode))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              प्रत्यागम false;
            }
          अन्यथा
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          यदि (! *new_dst
              && (x->set_security_context || x->preserve_security_context))
            {
              यदि (! set_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                प्रत्यागम false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          यावत् (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  अन्यथा यदि (!S_ISDIR (stats.st_mode))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      प्रत्यागम false;
    }
  अन्यथा
    {
      *new_dst = false;
    }
  प्रत्यागम true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

स्थिर bool
do_copy (पूर्णाङ्क n_files, वर्ण **file, वर्ण ध्रुव *target_directory,
         bool no_target_directory, संरचना cp_options *x)
{
  यदि (n_files <= !target_directory)
    {
      यदि (n_files <= 0)
        error (0, 0, _("missing file operand"));
      अन्यथा
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (निकास_त्रुटि);
    }

  संरचना stat sb;
  sb.st_mode = 0;
  पूर्णाङ्क target_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  यदि (no_target_directory)
    {
      यदि (target_directory)
        error (निकास_त्रुटि, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      यदि (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (निकास_त्रुटि);
        }
    }
  अन्यथा यदि (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      यदि (! target_dirfd_valid (target_dirfd))
        error (निकास_त्रुटि, त्रुटि_सं, _("target directory %s"),
               quoteaf (target_directory));
    }
  अन्यथा
    {
      वर्ण ध्रुव *lastfile = file[n_files - 1];
      पूर्णाङ्क fd = target_directory_operand (lastfile, &sb);
      यदि (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      अन्यथा
        {
          पूर्णाङ्क err = त्रुटि_सं;
          यदि (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          यदि (2 < n_files
              || (O_PATHSEARCH == O_खोज && err == EACCES
                  && (sb.st_mode || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_mode)))
            error (निकास_त्रुटि, err, _("target %s"), quoteaf (lastfile));
        }
    }

  यदि (target_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      यदि (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
        {
          वर्ण *dst_name;
          bool parent_exists = true;  /* True if dir_name (dst_name) exists. */
          संरचना dir_attr *attr_list;
          वर्ण *arg_in_concat;
          वर्ण *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          यदि (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          यदि (parents_option)
            {
              वर्ण *arg_no_trailing_slash;

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
                  (x->verbose ? "%s -> %s\n" : रिक्त),
                  &attr_list, &new_dst, x));
            }
          अन्यथा
            {
              वर्ण *arg_base;
              /* Append the last component of 'arg' to 'target_directory'.  */
              ASSIGN_STRDUPA (arg_base, last_component (arg));
              strip_trailing_slashes (arg_base);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_base += streq (arg_base, "..");
              dst_name = file_name_concat (target_directory, arg_base,
                                           &arg_in_concat);
            }

          यदि (!parent_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          अन्यथा
            {
              वर्ण ध्रुव *dst_relname = arg_in_concat;
              यावत् (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, target_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, रिक्त);

              यदि (parents_option)
                ok &= re_protect (dst_name, arg_in_concat, target_dirfd,
                                  dst_relname, attr_list, x);
            }

          यदि (parents_option)
            {
              यावत् (attr_list)
                {
                  संरचना dir_attr *p = attr_list;
                  attr_list = attr_list->next;
                  मुक्त (p);
                }
            }

          मुक्त (dst_name);
        }
    }
  अन्यथा /* !target_directory */
    {
      यदि (parents_option)
        {
          error (0, 0,
                 _("with --parents, the destination must be a directory"));
          usage (निकास_त्रुटि);
        }

      /* When the force and backup options have been specified and
         the source and destination are the same name for an existing
         regular file, convert the user's command, e.g.,
         'cp --force --backup foo foo' to 'cp --force foo fooSUFFIX'
         where SUFFIX is determined by any version control options used.  */

      वर्ण ध्रुव *source = file[0];
      वर्ण ध्रुव *dest = file[1];
      यदि (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (source, dest)
          && !new_dst
          && (sb.st_mode != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_mode))
        {
          स्थिर संरचना cp_options x_tmp;

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
      ok = copy (source, dest, AT_FDCWD, dest, -new_dst, x, &unused, रिक्त);
    }

  प्रत्यागम ok;
}

स्थिर शून्य
cp_option_init (संरचना cp_options *x)
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
  x->set_security_context = रिक्त;       /* -Z, set sys default context. */
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
  x->open_dangling_dest_symlink = दो_पर्या ("POSIXLY_CORRECT") != रिक्त;

  x->dest_info = रिक्त;
  x->src_info = रिक्त;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
स्थिर शून्य
decode_preserve_arg (वर्ण ध्रुव *arg, संरचना cp_options *x, bool on_off)
{
  गणना File_attribute
    {
      PRESERVE_MODE,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  स्थिर गणना File_attribute ध्रुव preserve_vals[] =
    {
      PRESERVE_MODE, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  स्थिर वर्ण ध्रुव *ध्रुव preserve_args[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", रिक्त
    };
  ARGMATCH_VERIFY (preserve_args, preserve_vals);

  वर्ण *arg_writable = xstrdup (arg);
  वर्ण *s = arg_writable;
  कुरु
    {
      /* find next comma */
      वर्ण *comma = म_अक्षर (s, ',');
      गणना File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      यदि (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_args, preserve_vals);
      विकल्प (val)
        {
        पक्ष PRESERVE_MODE:
          x->preserve_mode = on_off;
          x->explicit_no_preserve_mode = !on_off;
          भङ्ग;

        पक्ष PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          भङ्ग;

        पक्ष PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          भङ्ग;

        पक्ष PRESERVE_LINK:
          x->preserve_links = on_off;
          भङ्ग;

        पक्ष PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          भङ्ग;

        पक्ष PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          भङ्ग;

        पक्ष PRESERVE_ALL:
          x->preserve_mode = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_mode = !on_off;
          यदि (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          भङ्ग;

        स्वभाव:
          affirm (false);
        }
      s = comma;
    }
  यावत् (s);

  मुक्त (arg_writable);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool make_backups = false;
  वर्ण ध्रुव *backup_suffix = रिक्त;
  वर्ण *version_control_string = रिक्त;
  संरचना cp_options x;
  bool copy_contents = false;
  वर्ण *target_directory = रिक्त;
  bool no_target_directory = false;
  वर्ण ध्रुव *scontext = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  पूर्णाङ्क c;
  यावत् ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष SPARSE_OPTION:
          x.sparse_mode = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          भङ्ग;

        पक्ष REFLINK_OPTION:
          यदि (optarg == रिक्त)
            x.reflink_mode = REFLINK_ALWAYS;
          अन्यथा
            x.reflink_mode = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          भङ्ग;

        पक्ष 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_mode = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          यदि (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          भङ्ग;

        पक्ष 'b':
          make_backups = true;
          यदि (optarg)
            version_control_string = optarg;
          भङ्ग;

        पक्ष ATTRIBUTES_ONLY_OPTION:
          x.data_copy_required = false;
          भङ्ग;

        पक्ष DEBUG_OPTION:
          x.debug = x.verbose = true;
          भङ्ग;

        पक्ष COPY_CONTENTS_OPTION:
          copy_contents = true;
          भङ्ग;

        पक्ष 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          भङ्ग;

        पक्ष 'f':
          x.unlink_dest_after_failed_open = true;
          भङ्ग;

        पक्ष 'H':
          x.dereference = DEREF_COMMAND_पंक्ति_ARGUMENTS;
          भङ्ग;

        पक्ष 'i':
          x.interactive = I_ASK_USER;
          भङ्ग;

        पक्ष 'l':
          x.hard_link = true;
          भङ्ग;

        पक्ष 'L':
          x.dereference = DEREF_ALWAYS;
          भङ्ग;

        पक्ष 'n':
          x.interactive = I_ALWAYS_SKIP;
          भङ्ग;

        पक्ष 'P':
          x.dereference = DEREF_NEVER;
          भङ्ग;

        पक्ष NO_PRESERVE_ATTRIBUTES_OPTION:
          decode_preserve_arg (optarg, &x, false);
          भङ्ग;

        पक्ष PRESERVE_ATTRIBUTES_OPTION:
          यदि (optarg == रिक्त)
            {
              /* Fall through to the case for 'p' below.  */
            }
          अन्यथा
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              भङ्ग;
            }
          FALLTHROUGH;

        पक्ष 'p':
          x.preserve_ownership = true;
          x.preserve_mode = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          भङ्ग;

        पक्ष PARENTS_OPTION:
          parents_option = true;
          भङ्ग;

        पक्ष 'r':
        पक्ष 'R':
          x.recursive = true;
          भङ्ग;

        पक्ष UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          भङ्ग;

        पक्ष STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          भङ्ग;

        पक्ष 's':
          x.symbolic_link = true;
          भङ्ग;

        पक्ष 't':
          यदि (target_directory)
            error (निकास_त्रुटि, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          भङ्ग;

        पक्ष 'T':
          no_target_directory = true;
          भङ्ग;

        पक्ष 'u':
          x.update = UPDATE_OLDER;
          यदि (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          भङ्ग;

        पक्ष 'v':
          x.verbose = true;
          भङ्ग;

        पक्ष KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          भङ्ग;

        पक्ष 'x':
          x.one_file_system = true;
          भङ्ग;

        पक्ष 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          यदि (selinux_enabled)
            {
              यदि (optarg)
                scontext = optarg;
              अन्यथा
                {
                  x.set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         रिक्त, 0);
                  यदि (! x.set_security_context)
                    error (0, त्रुटि_सं, _("warning: ignoring --context"));
                }
            }
          अन्यथा यदि (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          भङ्ग;

        पक्ष 'S':
          make_backups = true;
          backup_suffix = optarg;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  यदि (x.reflink_mode == REFLINK_AUTO && x.sparse_mode == SPARSE_NEVER)
    x.reflink_mode = REFLINK_NEVER;

  यदि (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (निकास_त्रुटि);
    }

  यदि (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  यदि (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (निकास_त्रुटि);
    }

  यदि (x.reflink_mode == REFLINK_ALWAYS && x.sparse_mode != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (निकास_त्रुटि);
    }

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  यदि (x.dereference == DEREF_UNDEFINED)
    {
      यदि (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      अन्यथा
        x.dereference = DEREF_ALWAYS;
    }

  यदि (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  यदि ((x.set_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  यदि (x.preserve_security_context && (x.set_security_context || scontext))
    error (निकास_त्रुटि, 0,
           _("cannot set target context and preserve it"));

  यदि (x.require_preserve_context && ! selinux_enabled)
    error (निकास_त्रुटि, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  यदि (scontext && setfscreatecon (scontext) < 0)
    error (निकास_त्रुटि, त्रुटि_सं,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#यदि !USE_XATTR
  यदि (x.require_preserve_xattr)
    error (निकास_त्रुटि, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#endif

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     target_directory, no_target_directory, &x);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
