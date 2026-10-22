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
#परिभाषय क्रमादेशः_NAME "cp"

#परिभाषय AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
संरचना dir_attr
{
  संरचना stat st;
  bool restore_बहुलकम्;
  size_t slash_विस्थापनम्;
  संरचना dir_attr *अग्र;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  गुणाः_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_गुणाः_OPTION,
  PARENTS_OPTION,
  PRESERVE_गुणाः_OPTION,
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
  {"archive", no_तर्कः, रिक्त, 'a'},
  {"attributes-only", no_तर्कः, रिक्त, गुणाः_ONLY_OPTION},
  {"backup", optional_तर्कः, रिक्त, 'b'},
  {"copy-contents", no_तर्कः, रिक्त, COPY_CONTENTS_OPTION},
  {"debug", no_तर्कः, रिक्त, DEBUG_OPTION},
  {"dereference", no_तर्कः, रिक्त, 'L'},
  {"force", no_तर्कः, रिक्त, 'f'},
  {"interactive", no_तर्कः, रिक्त, 'i'},
  {"link", no_तर्कः, रिक्त, 'l'},
  {"no-clobber", no_तर्कः, रिक्त, 'n'},   /* Deprecated.  */
  {"no-dereference", no_तर्कः, रिक्त, 'P'},
  {"no-preserve", required_तर्कः, रिक्त, NO_PRESERVE_गुणाः_OPTION},
  {"no-target-directory", no_तर्कः, रिक्त, 'T'},
  {"one-file-system", no_तर्कः, रिक्त, 'x'},
  {"parents", no_तर्कः, रिक्त, PARENTS_OPTION},
  {"path", no_तर्कः, रिक्त, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_तर्कः, रिक्त, PRESERVE_गुणाः_OPTION},
  {"recursive", no_तर्कः, रिक्त, 'R'},
  {"remove-destination", no_तर्कः, रिक्त, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_तर्कः, रिक्त, SPARSE_OPTION},
  {"reflink", optional_तर्कः, रिक्त, REFLINK_OPTION},
  {"strip-trailing-slashes", no_तर्कः, रिक्त,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_तर्कः, रिक्त, 'S'},
  {"symbolic-link", no_तर्कः, रिक्त, 's'},
  {"target-directory", required_तर्कः, रिक्त, 't'},
  {"update", optional_तर्कः, रिक्त, 'u'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {"keep-directory-symlink", no_तर्कः, रिक्त,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Copy SOURCE गन्तव्यम् DEST, or गुणजः SOURCE(s) गन्तव्यम् DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --गुणाः-only\n\
         don't copy the file दत्त, just the गुणाः\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup सम्बन्धि each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an तर्कः\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents सम्बन्धि special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  उपपादयति_2 -v\n\
"));
      oputs (_("\
  -f, --force\n\
         यदि an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also प्रयुक्तम्)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a previous -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-पङ्क्तिः symbolic links अन्तर् SOURCE\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links अन्तर् SOURCE\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links अन्तर् SOURCE\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks गन्तव्यम् directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead सम्बन्धि copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=बहुलकम्_2,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified गुणाः\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified गुणाः\n\
"));
      oputs (_("\
      --parents\n\
         use पूर्णम् source file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See अधः\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting गन्तव्यम् उद्घाटय it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation सम्बन्धि sparse files. See अधः\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each SOURCE तर्कः\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead सम्बन्धि copying\n\
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
         treat DEST as a प्रसामान्यम् file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(स्वभाव)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent गन्तव्यम् --update[=older].  See अधः\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being समाप्तम्\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         समुच्चयः_2 SELinux security context सम्बन्धि destination file गन्तव्यम् स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or यदि CTX is specified then समुच्चयः_2 the\n\
         SELinux or SMACK security context गन्तव्यम् CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list सम्बन्धि गुणाः. गुणाः_2 are 'mode' प्रति\n\
permissions (including any ACL and xattr permissions), 'ownership' प्रति user\n\
and गणः, 'timestamps' प्रति file timestamps, 'links' प्रति hard links, 'context'\
\nfor security context, 'xattr' प्रति extended गुणाः, and 'all' प्रति all\n\
गुणाः.\n\
"), stdout);
      fputs (_("\
\n\
By स्वभाव, sparse SOURCE files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=स्वचालित.  Specify --sparse=always गन्तव्यम् create a sparse DEST\n\
file whenever the SOURCE file अन्तर्भवति a दीर्घ enough sequence सम्बन्धि zero अष्टकानि.\n\
Use --sparse=never गन्तव्यम् inhibit creation सम्बन्धि sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By स्वभाव or with --reflink=स्वचालित, cp will try a lightweight copy, where the\n\
दत्त खण्डानि are copied only when modified, falling पृष्ठभागः गन्तव्यम् a standard copy\n\
यदि this is not possible.  With --reflink[=always] cp will fail यदि CoW is not\n\
supported, यावत् --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special पक्ष, cp makes a backup सम्बन्धि SOURCE when the force and backup\n\
options are given and SOURCE and DEST are the same name प्रति an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
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

  नियोजय_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  वर्ण ध्रुव *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  वर्ण ध्रुव *relname = dst_name + (dst_relname - const_dst_name);

  प्रति (संरचना dir_attr *p = attr_list; p; p = p->अग्र)
    {
      dst_name[p->slash_विस्थापनम्] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      यदि (x->preserve_timestamps)
        {
          संरचना timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          यदि (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
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
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  प्रत्यागम false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_मानम् (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      यदि (x->preserve_बहुलकम्)
        {
          यदि (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_बहुलकम्) != 0)
            प्रत्यागम false;
        }
      अन्यथा यदि (p->restore_बहुलकम्)
        {
          यदि (lchmodat (dst_dirfd, relname, p->st.st_बहुलकम्) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              प्रत्यागम false;
            }
        }

      dst_name[p->slash_विस्थापनम्] = '/';
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
make_dir_parents_वैयक्तिकम् (वर्ण ध्रुव *const_dir, size_t src_विस्थापनम्,
                          पूर्णाङ्क dst_dirfd,
                          वर्ण ध्रुव *verbose_fmt_string,
                          संरचना dir_attr **attr_list, bool *new_dst,
                          ध्रुव संरचना cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = रिक्त;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  यदि (dirlen <= src_विस्थापनम्)
    प्रत्यागम true;

  /* A copy of CONST_DIR we can change.  */
  वर्ण *dir;
  नियोजय_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  वर्ण *src = dir + src_विस्थापनम्;

  /* Leading directory of DIR.  */
  वर्ण *dst_dir = alloca (dirlen + 1);
  स्मृतिं_प्रतिलिख (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  वर्ण ध्रुव *dst_reldir = dst_dir + src_विस्थापनम्;
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

      यावत् ((slash = strchr (slash, '/')))
        {
          संरचना dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          यदि (missing_dir || x->preserve_ownership || x->preserve_बहुलकम्
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              संरचना stat src_st;
              पूर्णाङ्क src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_बहुलकम्)
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
              new->slash_विस्थापनम् = slash - dir;
              new->restore_बहुलकम् = false;
              new->अग्र = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          यदि (! समुच्चयः_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_बहुलकम् : 0,
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
              mode_t src_बहुलकम् = new->st.st_बहुलकम्;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_बहुलकम्
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_बहुलकम्
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_बहुलकम् = (x->explicit_no_preserve_बहुलकम्
                                   ? S_IRWXUGO : src_बहुलकम्);
              mkdir_बहुलकम् &= CHMOD_बहुलकम्_द्वयङ्काः & ~omitted_permissions;
              यदि (mkdirat (dst_dirfd, dst_reldir, mkdir_बहुलकम्) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  प्रत्यागम false;
                }
              अन्यथा
                {
                  यदि (verbose_fmt_string != रिक्त)
                    प्रारूप_लिख (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              यदि (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  प्रत्यागम false;
                }


              यदि (! x->preserve_बहुलकम्)
                {
                  यदि (omitted_permissions & ~stats.st_बहुलकम्)
                    omitted_permissions &= ~ cached_umask ();
                  यदि (omitted_permissions & ~stats.st_बहुलकम्
                      || (stats.st_बहुलकम् & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_बहुलकम् = stats.st_बहुलकम् | omitted_permissions;
                      new->restore_बहुलकम् = true;
                    }
                }

              mode_t accessible = stats.st_बहुलकम् | S_IRWXU;
              यदि (stats.st_बहुलकम् != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  यदि (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      प्रत्यागम false;
                    }
                }
            }
          अन्यथा यदि (!S_ISDIR (stats.st_बहुलकम्))
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
              && (x->समुच्चयः_security_context || x->preserve_security_context))
            {
              यदि (! समुच्चयः_file_security_ctx (dir, false, x)
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

  अन्यथा यदि (!S_ISDIR (stats.st_बहुलकम्))
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
      usage (EXIT_FAILURE);
    }

  संरचना stat sb;
  sb.st_बहुलकम् = 0;
  पूर्णाङ्क target_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  यदि (no_target_directory)
    {
      यदि (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      यदि (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  अन्यथा यदि (target_directory)
    {
      target_dirfd = target_directory_सङ्कार्यम् (target_directory, &sb);
      यदि (! target_dirfd_valid (target_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (target_directory));
    }
  अन्यथा
    {
      वर्ण ध्रुव *lastfile = file[n_files - 1];
      पूर्णाङ्क fd = target_directory_सङ्कार्यम् (lastfile, &sb);
      यदि (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      अन्यथा
        {
          पूर्णाङ्क err = errno;
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
              || (O_PATHSEARCH == O_अन्वेषणम् && err == EACCES
                  && (sb.st_बहुलकम् || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_बहुलकम्)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
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
          वर्ण *arg_अन्तर्_concat;
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
              नियोजय_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (target_directory,
                                           arg_no_trailing_slash,
                                           &arg_अन्तर्_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              parent_exists =
                (make_dir_parents_वैयक्तिकम्
                 (dst_name, arg_अन्तर्_concat - dst_name, target_dirfd,
                  (x->verbose ? "%s -> %s\n" : रिक्त),
                  &attr_list, &new_dst, x));
            }
          अन्यथा
            {
              वर्ण *arg_आधारः;
              /* Append the last component of 'arg' to 'target_directory'.  */
              नियोजय_STRDUPA (arg_आधारः, चरमः_component (arg));
              strip_trailing_slashes (arg_आधारः);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_आधारः += streq (arg_आधारः, "..");
              dst_name = file_name_concat (target_directory, arg_आधारः,
                                           &arg_अन्तर्_concat);
            }

          यदि (!parent_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          अन्यथा
            {
              वर्ण ध्रुव *dst_relname = arg_अन्तर्_concat;
              यावत् (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, target_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, रिक्त);

              यदि (parents_option)
                ok &= re_protect (dst_name, arg_अन्तर्_concat, target_dirfd,
                                  dst_relname, attr_list, x);
            }

          यदि (parents_option)
            {
              यावत् (attr_list)
                {
                  संरचना dir_attr *p = attr_list;
                  attr_list = attr_list->अग्र;
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
          usage (EXIT_FAILURE);
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
          && (sb.st_बहुलकम् != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_बहुलकम्))
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

      bool अप्रयुक्तम्;
      ok = copy (source, dest, AT_FDCWD, dest, -new_dst, x, &अप्रयुक्तम्, रिक्त);
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
  x->move_बहुलकम् = false;
  x->install_बहुलकम् = false;
  x->one_file_system = false;
  x->reflink_बहुलकम् = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_बहुलकम् = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_बहुलकम् = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->समुच्चयः_security_context = रिक्त;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->दत्त_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_बहुलकम् = SPARSE_AUTO;
  x->symbolic_link = false;
  x->समुच्चयः_बहुलकम् = false;
  x->बहुलकम्_2 = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != रिक्त;

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
      PRESERVE_बहुलकम्,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  स्थिर गणना File_attribute ध्रुव preserve_vals[] =
    {
      PRESERVE_बहुलकम्, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  स्थिर वर्ण ध्रुव *ध्रुव preserve_तर्काः[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", रिक्त
    };
  ARGMATCH_VERIFY (preserve_तर्काः, preserve_vals);

  वर्ण *arg_writable = xstrdup (arg);
  वर्ण *s = arg_writable;
  कुरु
    {
      /* find next comma */
      वर्ण *comma = strchr (s, ',');
      गणना File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      यदि (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_तर्काः, preserve_vals);
      विकल्प (val)
        {
        पक्ष PRESERVE_बहुलकम्:
          x->preserve_बहुलकम् = on_off;
          x->explicit_no_preserve_बहुलकम् = !on_off;
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
          x->preserve_बहुलकम् = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_बहुलकम् = !on_off;
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
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool make_backups = false;
  वर्ण ध्रुव *backup_suffix = रिक्त;
  वर्ण *संस्करणम्_control_string = रिक्त;
  संरचना cp_options x;
  bool copy_contents = false;
  वर्ण *target_directory = रिक्त;
  bool no_target_directory = false;
  वर्ण ध्रुव *scontext = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

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
          x.sparse_बहुलकम् = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          भङ्ग;

        पक्ष REFLINK_OPTION:
          यदि (optarg == रिक्त)
            x.reflink_बहुलकम् = REFLINK_ALWAYS;
          अन्यथा
            x.reflink_बहुलकम् = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          भङ्ग;

        पक्ष 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_बहुलकम् = true;
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
            संस्करणम्_control_string = optarg;
          भङ्ग;

        पक्ष गुणाः_ONLY_OPTION:
          x.दत्त_copy_required = false;
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
          x.dereference = DEREF_COMMAND_पङ्क्तिः_ARGUMENTS;
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

        पक्ष NO_PRESERVE_गुणाः_OPTION:
          decode_preserve_arg (optarg, &x, false);
          भङ्ग;

        पक्ष PRESERVE_गुणाः_OPTION:
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
          x.preserve_बहुलकम् = true;
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
            error (EXIT_FAILURE, 0,
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
                  x.समुच्चयः_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         रिक्त, 0);
                  यदि (! x.समुच्चयः_security_context)
                    error (0, errno, _("warning: ignoring --context"));
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

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  यदि (x.reflink_बहुलकम् == REFLINK_AUTO && x.sparse_बहुलकम् == SPARSE_NEVER)
    x.reflink_बहुलकम् = REFLINK_NEVER;

  यदि (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  यदि (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  यदि (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  यदि (x.reflink_बहुलकम् == REFLINK_ALWAYS && x.sparse_बहुलकम् != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_संस्करणम् (_("backup type"),
                                   संस्करणम्_control_string)
                   : no_backups);
  समुच्चयः_simple_backup_suffix (backup_suffix);

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
  यदि ((x.समुच्चयः_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  यदि (x.preserve_security_context && (x.समुच्चयः_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  यदि (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  यदि (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#यदि !USE_XATTR
  यदि (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#यदि_अन्तः

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     target_directory, no_target_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
