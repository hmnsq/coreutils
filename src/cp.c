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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <selinux/label.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "backupfile.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "cp-hash.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "targetdir.h"
#అన్తర్భావ "utimens.h"
#అన్తర్భావ "acl.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "cp"

#నిర్వచించు AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* Used by do_copy, make_dir_parents_private, and re_protect
   to keep a list of leading directories whose protections
   need to be fixed after copying. */
నిర్మాణం dir_attr
{
  నిర్మాణం stat st;
  bool restore_బహుళకం;
  size_t slash_స్థానభ్రంశం;
  నిర్మాణం dir_attr *తదుపరి;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  లక్షణాలు_ONLY_OPTION = CHAR_MAX + 1,
  COPY_CONTENTS_OPTION,
  DEBUG_OPTION,
  NO_PRESERVE_లక్షణాలు_OPTION,
  PARENTS_OPTION,
  PRESERVE_లక్షణాలు_OPTION,
  REFLINK_OPTION,
  SPARSE_OPTION,
  STRIP_TRAILING_SLASHES_OPTION,
  UNLINK_DEST_BEFORE_OPENING,
  KEEP_DIRECTORY_SYMLINK_OPTION
};

/* True if the kernel is SELinux enabled.  */
స్థిర bool selinux_enabled;

/* If true, the command "cp x/e_file e_dir" uses "e_dir/x/e_file"
   as its destination instead of the usual "e_dir/e_file." */
స్థిర bool parents_option = false;

/* Remove any trailing slashes from each SOURCE argument.  */
స్థిర bool remove_trailing_slashes;

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి sparse_type_string[] =
{
  "never", "auto", "always", ఖాళీ
};
స్థిర గణన Sparse_type స్థిరరాశి sparse_type[] =
{
  SPARSE_NEVER, SPARSE_AUTO, SPARSE_ALWAYS
};
ARGMATCH_VERIFY (sparse_type_string, sparse_type);

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి reflink_type_string[] =
{
  "auto", "always", "never", ఖాళీ
};
స్థిర గణన Reflink_type స్థిరరాశి reflink_type[] =
{
  REFLINK_AUTO, REFLINK_ALWAYS, REFLINK_NEVER
};
ARGMATCH_VERIFY (reflink_type_string, reflink_type);

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి update_type_string[] =
{
  "all", "none", "none-fail", "older", ఖాళీ
};
స్థిర గణన Update_type స్థిరరాశి update_type[] =
{
  UPDATE_ALL, UPDATE_NONE, UPDATE_NONE_FAIL, UPDATE_OLDER,
};
ARGMATCH_VERIFY (update_type_string, update_type);

స్థిర నిర్మాణం option స్థిరరాశి long_opts[] =
{
  {"archive", no_వాదం, ఖాళీ, 'a'},
  {"attributes-only", no_వాదం, ఖాళీ, లక్షణాలు_ONLY_OPTION},
  {"backup", optional_వాదం, ఖాళీ, 'b'},
  {"copy-contents", no_వాదం, ఖాళీ, COPY_CONTENTS_OPTION},
  {"debug", no_వాదం, ఖాళీ, DEBUG_OPTION},
  {"dereference", no_వాదం, ఖాళీ, 'L'},
  {"force", no_వాదం, ఖాళీ, 'f'},
  {"interactive", no_వాదం, ఖాళీ, 'i'},
  {"link", no_వాదం, ఖాళీ, 'l'},
  {"no-clobber", no_వాదం, ఖాళీ, 'n'},   /* Deprecated.  */
  {"no-dereference", no_వాదం, ఖాళీ, 'P'},
  {"no-preserve", required_వాదం, ఖాళీ, NO_PRESERVE_లక్షణాలు_OPTION},
  {"no-target-directory", no_వాదం, ఖాళీ, 'T'},
  {"one-file-system", no_వాదం, ఖాళీ, 'x'},
  {"parents", no_వాదం, ఖాళీ, PARENTS_OPTION},
  {"path", no_వాదం, ఖాళీ, PARENTS_OPTION},   /* Deprecated.  */
  {"preserve", optional_వాదం, ఖాళీ, PRESERVE_లక్షణాలు_OPTION},
  {"recursive", no_వాదం, ఖాళీ, 'R'},
  {"remove-destination", no_వాదం, ఖాళీ, UNLINK_DEST_BEFORE_OPENING},
  {"sparse", required_వాదం, ఖాళీ, SPARSE_OPTION},
  {"reflink", optional_వాదం, ఖాళీ, REFLINK_OPTION},
  {"strip-trailing-slashes", no_వాదం, ఖాళీ,
   STRIP_TRAILING_SLASHES_OPTION},
  {"suffix", required_వాదం, ఖాళీ, 'S'},
  {"symbolic-link", no_వాదం, ఖాళీ, 's'},
  {"target-directory", required_వాదం, ఖాళీ, 't'},
  {"update", optional_వాదం, ఖాళీ, 'u'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {"keep-directory-symlink", no_వాదం, ఖాళీ,
    KEEP_DIRECTORY_SYMLINK_OPTION},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [-T] ఆధారం DEST\n\
  or:  %s [OPTION]... ఆధారం... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY ఆధారం...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Copy ఆధారం గమ్యం DEST, or గుణిజం ఆధారం(s) గమ్యం DIRECTORY.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --archive\n\
         same as -dR --preserve=all\n\
"));
      oputs (_("\
      --లక్షణాలు-only\n\
         don't copy the file దత్తాంశం, just the లక్షణాలు\n\
"));
      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup యొక్క each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an వాదం\n\
"));
      oputs (_("\
      --copy-contents\n\
         copy contents యొక్క special files when recursive\n\
"));
      oputs (_("\
  -d\n\
         same as --no-dereference --preserve=links\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  సూచిస్తుంది_2 -v\n\
"));
      oputs (_("\
  -f, --force\n\
         ఒకవేళ an existing destination file cannot be opened, remove it and try\n\
         again (this option is ignored when the -n option is also వాడిన)\n\
"));
      oputs (_("\
  -i, --interactive\n\
         prompt before overwrite (overrides a మునుపటి -n option)\n\
"));
      oputs (_("\
  -H\n\
         follow command-పంక్తి symbolic links లోపల ఆధారం\n\
"));
      oputs (_("\
  -L, --dereference\n\
         always follow symbolic links లోపల ఆధారం\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         never follow symbolic links లోపల ఆధారం\n\
"));
      oputs (_("\
      --keep-directory-symlink\n\
         follow existing symlinks గమ్యం directories\n\
"));
      oputs (_("\
  -l, --link\n\
         hard link files instead యొక్క copying\n\
"));
      oputs (_("\
  -n, --no-clobber\n\
         (deprecated) silently skip existing files.  See also --update\n\
"));
      oputs (_("\
  -p\n\
         same as --preserve=బహుళకం_2,ownership,timestamps\n\
"));
      oputs (_("\
      --preserve[=ATTR_LIST]\n\
         preserve the specified లక్షణాలు\n\
"));
      oputs (_("\
      --no-preserve=ATTR_LIST\n\
         don't preserve the specified లక్షణాలు\n\
"));
      oputs (_("\
      --parents\n\
         use నిండిన ఆధారం_2 file name under DIRECTORY\n\
"));
      oputs (_("\
  -R, -r, --recursive\n\
         copy directories recursively\n\
"));
      oputs (_("\
      --reflink[=WHEN]\n\
         control clone/CoW copies. See క్రింద\n\
"));
      oputs (_("\
      --remove-destination\n\
         remove each existing destination file before attempting గమ్యం open it\n\
         (contrast with --force)\n\
"));
      oputs (_("\
      --sparse=WHEN\n\
         control creation యొక్క sparse files. See క్రింద\n\
"));
      oputs (_("\
      --strip-trailing-slashes\n\
         remove any trailing slashes from each ఆధారం వాదం\n\
"));
      oputs (_("\
  -s, --symbolic-link\n\
         make symbolic links instead యొక్క copying\n\
"));
      oputs (_("\
  -S, --suffix=SUFFIX\n\
         override the usual backup suffix\n\
"));
      oputs (_("\
  -t, --లక్ష్యం-directory=DIRECTORY\n\
         copy all ఆధారం arguments into DIRECTORY\n\
"));
      oputs (_("\
  -T, --no-లక్ష్యం-directory\n\
         treat DEST as a సాధారణ file\n\
"));
      oputs (_("\
      --update[=UPDATE]\n\
         control which existing files are updated;\n\
         UPDATE={all,none,none-fail,older(అప్రమేయ)}\n\
"));
      oputs (_("\
  -u\n\
         equivalent గమ్యం --update[=older].  See క్రింద\n\
"));
      oputs (_("\
  -v, --verbose\n\
         explain what is being పూర్తి\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         stay on this file system\n\
"));
      oputs (_("\
  -Z\n\
         సమితి_2 SELinux security context యొక్క destination file గమ్యం అప్రమేయ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ఒకవేళ CTX is specified then సమితి_2 the\n\
         SELinux or SMACK security context గమ్యం CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
ATTR_LIST is a comma-separated list యొక్క లక్షణాలు. లక్షణాలు_2 are 'mode' కొరకు\n\
permissions (including any ACL and xattr permissions), 'ownership' కొరకు user\n\
and సమూహం, 'timestamps' కొరకు file timestamps, 'links' కొరకు hard links, 'context'\
\nfor security context, 'xattr' కొరకు extended లక్షణాలు, and 'all' కొరకు all\n\
లక్షణాలు.\n\
"), stdout);
      fputs (_("\
\n\
By అప్రమేయ, sparse ఆధారం files are detected by a crude heuristic and the\n\
corresponding DEST file is made sparse as well.  That is the behavior\n\
selected by --sparse=స్వయంచాలక.  Specify --sparse=always గమ్యం create a sparse DEST\n\
file whenever the ఆధారం file కలిగియుంది a దీర్ఘ enough sequence యొక్క zero బైట్లు.\n\
Use --sparse=never గమ్యం inhibit creation యొక్క sparse files.\n\
"), stdout);
      emit_update_parameters_note ();
      fputs (_("\
\n\
By అప్రమేయ or with --reflink=స్వయంచాలక, cp will try a lightweight copy, where the\n\
దత్తాంశం ఖండాలు are copied only when modified, falling వెనుక గమ్యం a standard copy\n\
ఒకవేళ this is not possible.  With --reflink[=always] cp will fail ఒకవేళ CoW is not\n\
supported, అయితే --reflink=never ensures a standard copy is performed.\n\
"), stdout);
      emit_backup_suffix_note ();
      fputs (_("\
\n\
As a special స్థితి, cp makes a backup యొక్క ఆధారం when the force and backup\n\
options are given and ఆధారం and DEST are the same name కొరకు an existing,\n\
regular file.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
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

స్థిర bool
re_protect (అక్షరం స్థిరరాశి *const_dst_name, అక్షరం స్థిరరాశి *dst_src_name,
            పూర్ణాంకం dst_dirfd, అక్షరం స్థిరరాశి *dst_relname,
            నిర్మాణం dir_attr *attr_list, స్థిరరాశి నిర్మాణం cp_options *x)
{
  అక్షరం *dst_name;		/* A copy of CONST_DST_NAME we can change. */

  కేటాయించు_STRDUPA (dst_name, const_dst_name);

  /* The suffix of DST_NAME that is a copy of the source file name,
     possibly truncated to name a parent directory.  */
  అక్షరం స్థిరరాశి *src_name = dst_name + (dst_src_name - const_dst_name);

  /* Likewise, but with any leading '/'s skipped.  */
  అక్షరం స్థిరరాశి *relname = dst_name + (dst_relname - const_dst_name);

  కొరకు (నిర్మాణం dir_attr *p = attr_list; p; p = p->తదుపరి)
    {
      dst_name[p->slash_స్థానభ్రంశం] = '\0';

      /* Adjust the times (and if possible, ownership) for the copy.
         chown turns off set[ug]id bits for non-root,
         so do the chmod last.  */

      ఒకవేళ (x->preserve_timestamps)
        {
          నిర్మాణం timespec timespec[2] = { get_stat_atime (&p->st),
                                          get_stat_mtime (&p->st) };

          ఒకవేళ (utimensat (dst_dirfd, relname, timespec, 0))
            {
              error (0, errno, _("failed to preserve times for %s"),
                     quoteaf (dst_name));
              తిరిగి false;
            }
        }

      ఒకవేళ (x->preserve_ownership)
        {
          ఒకవేళ (lchownat (dst_dirfd, relname, p->st.st_uid, p->st.st_gid)
              != 0)
            {
              ఒకవేళ (! chown_failure_ok (x))
                {
                  error (0, errno, _("failed to preserve ownership for %s"),
                         quoteaf (dst_name));
                  తిరిగి false;
                }
              /* Failing to preserve ownership is OK. Still, try to preserve
                 the group, but ignore the possible error. */
              ignore_విలువ (lchownat (dst_dirfd, relname, -1, p->st.st_gid));
            }
        }

      ఒకవేళ (x->preserve_బహుళకం)
        {
          ఒకవేళ (xcopy_acl (src_name, -1, dst_name, -1, p->st.st_బహుళకం) != 0)
            తిరిగి false;
        }
      లేకపోతే ఒకవేళ (p->restore_బహుళకం)
        {
          ఒకవేళ (lchmodat (dst_dirfd, relname, p->st.st_బహుళకం) != 0)
            {
              error (0, errno, _("failed to preserve permissions for %s"),
                     quoteaf (dst_name));
              తిరిగి false;
            }
        }

      dst_name[p->slash_స్థానభ్రంశం] = '/';
    }
  తిరిగి true;
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

స్థిర bool
make_dir_parents_వ్యక్తిగత (అక్షరం స్థిరరాశి *const_dir, size_t src_స్థానభ్రంశం,
                          పూర్ణాంకం dst_dirfd,
                          అక్షరం స్థిరరాశి *verbose_fmt_string,
                          నిర్మాణం dir_attr **attr_list, bool *new_dst,
                          స్థిరరాశి నిర్మాణం cp_options *x)
{
  idx_t dirlen = dir_len (const_dir);

  *attr_list = ఖాళీ;

  /* Succeed immediately if the parent of CONST_DIR must already exist,
     as the target directory has already been checked.  */
  ఒకవేళ (dirlen <= src_స్థానభ్రంశం)
    తిరిగి true;

  /* A copy of CONST_DIR we can change.  */
  అక్షరం *dir;
  కేటాయించు_STRDUPA (dir, const_dir);

  /* Source name in DIR.  */
  అక్షరం *src = dir + src_స్థానభ్రంశం;

  /* Leading directory of DIR.  */
  అక్షరం *dst_dir = alloca (dirlen + 1);
  జ్ఞాపకం_నకలు (dst_dir, dir, dirlen);
  dst_dir[dirlen] = '\0';
  అక్షరం స్థిరరాశి *dst_reldir = dst_dir + src_స్థానభ్రంశం;
  అయితే (*dst_reldir == '/')
    dst_reldir++;

  /* XXX: If all dirs are present at the destination,
     no permissions or security contexts will be updated.  */
  నిర్మాణం stat stats;
  ఒకవేళ (fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0)
    {
      /* A parent of CONST_DIR does not exist.
         Make all missing intermediate directories. */
      అక్షరం *slash = src;

      అయితే (*slash == '/')
        slash++;
      dst_reldir = slash;

      అయితే ((slash = strchr (slash, '/')))
        {
          నిర్మాణం dir_attr *new;

          *slash = '\0';
          bool missing_dir = fstatat (dst_dirfd, dst_reldir, &stats, 0) != 0;

          ఒకవేళ (missing_dir || x->preserve_ownership || x->preserve_బహుళకం
              || x->preserve_timestamps)
            {
              /* Add this directory to the list of directories whose
                 modes might need fixing later. */
              నిర్మాణం stat src_st;
              పూర్ణాంకం src_errno = (stat (src, &src_st) != 0
                               ? errno
                               : S_ISDIR (src_st.st_బహుళకం)
                               ? 0
                               : ENOTDIR);
              ఒకవేళ (src_errno)
                {
                  error (0, src_errno, _("failed to get attributes of %s"),
                         quoteaf (src));
                  తిరిగి false;
                }

              new = xmalloc (పరిమాణం *new);
              new->st = src_st;
              new->slash_స్థానభ్రంశం = slash - dir;
              new->restore_బహుళకం = false;
              new->తదుపరి = *attr_list;
              *attr_list = new;
            }

          /* If required set the default context for created dirs.  */
          ఒకవేళ (! సమితి_process_security_ctx (src, dir,
                                          missing_dir ? new->st.st_బహుళకం : 0,
                                          missing_dir, x))
            తిరిగి false;

          ఒకవేళ (missing_dir)
            {
              /* This component does not exist.  We must set
                 *new_dst and new->st.st_mode inside this loop because,
                 for example, in the command 'cp --parents ../a/../b/c e_dir',
                 make_dir_parents_private creates only e_dir/../a if
                 ./b already exists. */
              *new_dst = true;
              mode_t src_బహుళకం = new->st.st_బహుళకం;

              /* If the ownership or special mode bits might change,
                 omit some permissions at first, so unauthorized users
                 cannot nip in before the file is ready.  */
              mode_t omitted_permissions = (src_బహుళకం
                                            & (x->preserve_ownership
                                               ? S_IRWXG | S_IRWXO
                                               : x->preserve_బహుళకం
                                               ? S_IWGRP | S_IWOTH
                                               : 0));

              /* POSIX says mkdir's behavior is implementation-defined when
                 (src_mode & ~S_IRWXUGO) != 0.  However, common practice is
                 to ask mkdir to copy all the CHMOD_MODE_BITS, letting mkdir
                 decide what to do with S_ISUID | S_ISGID | S_ISVTX.  */
              mode_t mkdir_బహుళకం = (x->explicit_no_preserve_బహుళకం
                                   ? S_IRWXUGO : src_బహుళకం);
              mkdir_బహుళకం &= CHMOD_బహుళకం_బిట్లు & ~omitted_permissions;
              ఒకవేళ (mkdirat (dst_dirfd, dst_reldir, mkdir_బహుళకం) != 0)
                {
                  error (0, errno, _("cannot make directory %s"),
                         quoteaf (dir));
                  తిరిగి false;
                }
              లేకపోతే
                {
                  ఒకవేళ (verbose_fmt_string != ఖాళీ)
                    రాయి (verbose_fmt_string, src, dir);
                }

              /* We need search and write permissions to the new directory
                 for writing the directory's contents. Check if these
                 permissions are there.  */

              ఒకవేళ (fstatat (dst_dirfd, dst_reldir, &stats, AT_SYMLINK_NOFOLLOW))
                {
                  error (0, errno, _("failed to get attributes of %s"),
                         quoteaf (dir));
                  తిరిగి false;
                }


              ఒకవేళ (! x->preserve_బహుళకం)
                {
                  ఒకవేళ (omitted_permissions & ~stats.st_బహుళకం)
                    omitted_permissions &= ~ cached_umask ();
                  ఒకవేళ (omitted_permissions & ~stats.st_బహుళకం
                      || (stats.st_బహుళకం & S_IRWXU) != S_IRWXU)
                    {
                      new->st.st_బహుళకం = stats.st_బహుళకం | omitted_permissions;
                      new->restore_బహుళకం = true;
                    }
                }

              mode_t accessible = stats.st_బహుళకం | S_IRWXU;
              ఒకవేళ (stats.st_బహుళకం != accessible)
                {
                  /* Make the new directory searchable and writable.
                     The original permissions will be restored later.  */

                  ఒకవేళ (lchmodat (dst_dirfd, dst_reldir, accessible) != 0)
                    {
                      error (0, errno, _("setting permissions for %s"),
                             quoteaf (dir));
                      తిరిగి false;
                    }
                }
            }
          లేకపోతే ఒకవేళ (!S_ISDIR (stats.st_బహుళకం))
            {
              error (0, 0, _("%s exists but is not a directory"),
                     quoteaf (dir));
              తిరిగి false;
            }
          లేకపోతే
            *new_dst = false;

          /* For existing dirs, set the security context as per that already
             set for the process global context.  */
          ఒకవేళ (! *new_dst
              && (x->సమితి_security_context || x->preserve_security_context))
            {
              ఒకవేళ (! సమితి_file_security_ctx (dir, false, x)
                  && x->require_preserve_context)
                తిరిగి false;
            }

          *slash++ = '/';

          /* Avoid unnecessary calls to 'stat' when given
             file names containing multiple adjacent slashes.  */
          అయితే (*slash == '/')
            slash++;
        }
    }

  /* We get here if the parent of DIR already exists.  */

  లేకపోతే ఒకవేళ (!S_ISDIR (stats.st_బహుళకం))
    {
      error (0, 0, _("%s exists but is not a directory"), quoteaf (dst_dir));
      తిరిగి false;
    }
  లేకపోతే
    {
      *new_dst = false;
    }
  తిరిగి true;
}

/* Scan the arguments, and copy each by calling copy.
   Return true if successful.  */

స్థిర bool
do_copy (పూర్ణాంకం n_files, అక్షరం **file, అక్షరం స్థిరరాశి *లక్ష్యం_directory,
         bool no_లక్ష్యం_directory, నిర్మాణం cp_options *x)
{
  ఒకవేళ (n_files <= !లక్ష్యం_directory)
    {
      ఒకవేళ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      లేకపోతే
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  నిర్మాణం stat sb;
  sb.st_బహుళకం = 0;
  పూర్ణాంకం లక్ష్యం_dirfd = AT_FDCWD;
  bool new_dst = false;
  bool ok = true;
  ఒకవేళ (no_లక్ష్యం_directory)
    {
      ఒకవేళ (లక్ష్యం_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ఒకవేళ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  లేకపోతే ఒకవేళ (లక్ష్యం_directory)
    {
      లక్ష్యం_dirfd = లక్ష్యం_directory_నిర్వాహ్యం (లక్ష్యం_directory, &sb);
      ఒకవేళ (! లక్ష్యం_dirfd_valid (లక్ష్యం_dirfd))
        error (EXIT_FAILURE, errno, _("target directory %s"),
               quoteaf (లక్ష్యం_directory));
    }
  లేకపోతే
    {
      అక్షరం స్థిరరాశి *lastfile = file[n_files - 1];
      పూర్ణాంకం fd = లక్ష్యం_directory_నిర్వాహ్యం (lastfile, &sb);
      ఒకవేళ (లక్ష్యం_dirfd_valid (fd))
        {
          లక్ష్యం_dirfd = fd;
          లక్ష్యం_directory = lastfile;
          n_files--;
        }
      లేకపోతే
        {
          పూర్ణాంకం err = errno;
          ఒకవేళ (err == ENOENT)
            new_dst = true;

          /* The last operand LASTFILE cannot be opened as a directory.
             If there are more than two operands, report an error.

             Also, report an error if LASTFILE is known to be a directory
             even though it could not be opened, which can happen if
             opening failed with EACCES on a platform lacking O_PATH.
             In this case use stat to test whether LASTFILE is a
             directory, in case opening a non-directory with (O_SEARCH
             | O_DIRECTORY) failed with EACCES not ENOTDIR.  */
          ఒకవేళ (2 < n_files
              || (O_PATHSEARCH == O_శోధన && err == EACCES
                  && (sb.st_బహుళకం || stat (lastfile, &sb) == 0)
                  && S_ISDIR (sb.st_బహుళకం)))
            error (EXIT_FAILURE, err, _("target %s"), quoteaf (lastfile));
        }
    }

  ఒకవేళ (లక్ష్యం_directory)
    {
      /* cp file1...filen edir
         Copy the files 'file1' through 'filen'
         to the existing directory 'edir'. */

      /* Initialize these hash tables only if we'll need them.
         The problems they're used to detect can arise only if
         there are two or more files to copy.  */
      ఒకవేళ (2 <= n_files)
        {
          dest_info_init (x);
          src_info_init (x);
        }

      కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
        {
          అక్షరం *dst_name;
          bool మాతృ_exists = true;  /* True if dir_name (dst_name) exists. */
          నిర్మాణం dir_attr *attr_list;
          అక్షరం *arg_లోపల_concat;
          అక్షరం *arg = file[i];

          /* Trailing slashes are meaningful (i.e., maybe worth preserving)
             only in the source file names.  */
          ఒకవేళ (remove_trailing_slashes)
            strip_trailing_slashes (arg);

          ఒకవేళ (parents_option)
            {
              అక్షరం *arg_no_trailing_slash;

              /* Use 'arg' without trailing slashes in constructing destination
                 file names.  Otherwise, we can end up trying to create a
                 directory using a name with trailing slash, which fails on
                 NetBSD 1.[34] systems.  */
              కేటాయించు_STRDUPA (arg_no_trailing_slash, arg);
              strip_trailing_slashes (arg_no_trailing_slash);

              /* Append all of 'arg' (minus any trailing slash) to 'dest'.  */
              dst_name = file_name_concat (లక్ష్యం_directory,
                                           arg_no_trailing_slash,
                                           &arg_లోపల_concat);

              /* For --parents, we have to make sure that the directory
                 dir_name (dst_name) exists.  We may have to create a few
                 leading directories. */
              మాతృ_exists =
                (make_dir_parents_వ్యక్తిగత
                 (dst_name, arg_లోపల_concat - dst_name, లక్ష్యం_dirfd,
                  (x->verbose ? "%s -> %s\n" : ఖాళీ),
                  &attr_list, &new_dst, x));
            }
          లేకపోతే
            {
              అక్షరం *arg_పునాది;
              /* Append the last component of 'arg' to 'target_directory'.  */
              కేటాయించు_STRDUPA (arg_పునాది, చివరి_component (arg));
              strip_trailing_slashes (arg_పునాది);
              /* For 'cp -R source/.. dest', don't copy into 'dest/..'. */
              arg_పునాది += streq (arg_పునాది, "..");
              dst_name = file_name_concat (లక్ష్యం_directory, arg_పునాది,
                                           &arg_లోపల_concat);
            }

          ఒకవేళ (!మాతృ_exists)
            {
              /* make_dir_parents_private failed, so don't even
                 attempt the copy.  */
              ok = false;
            }
          లేకపోతే
            {
              అక్షరం స్థిరరాశి *dst_relname = arg_లోపల_concat;
              అయితే (*dst_relname == '/')
                dst_relname++;

              bool copy_into_self;
              ok &= copy (arg, dst_name, లక్ష్యం_dirfd, dst_relname,
                          new_dst, x, &copy_into_self, ఖాళీ);

              ఒకవేళ (parents_option)
                ok &= re_protect (dst_name, arg_లోపల_concat, లక్ష్యం_dirfd,
                                  dst_relname, attr_list, x);
            }

          ఒకవేళ (parents_option)
            {
              అయితే (attr_list)
                {
                  నిర్మాణం dir_attr *p = attr_list;
                  attr_list = attr_list->తదుపరి;
                  విడుదల (p);
                }
            }

          విడుదల (dst_name);
        }
    }
  లేకపోతే /* !target_directory */
    {
      ఒకవేళ (parents_option)
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

      అక్షరం స్థిరరాశి *ఆధారం_2 = file[0];
      అక్షరం స్థిరరాశి *dest = file[1];
      ఒకవేళ (x->unlink_dest_after_failed_open
          && x->backup_type != no_backups
          && streq (ఆధారం_2, dest)
          && !new_dst
          && (sb.st_బహుళకం != 0 || stat (dest, &sb) == 0) && S_ISREG (sb.st_బహుళకం))
        {
          స్థిర నిర్మాణం cp_options x_tmp;

          dest = కనుగొను_backup_file_name (AT_FDCWD, dest, x->backup_type);
          /* Set x->backup_type to 'no_backups' so that the normal backup
             mechanism is not used when performing the actual copy.
             backup_type must be set to 'no_backups' only *after* the above
             call to find_backup_file_name -- that function uses
             backup_type to determine the suffix it applies.  */
          x_tmp = *x;
          x_tmp.backup_type = no_backups;
          x = &x_tmp;
        }

      bool వాడని;
      ok = copy (ఆధారం_2, dest, AT_FDCWD, dest, -new_dst, x, &వాడని, ఖాళీ);
    }

  తిరిగి ok;
}

స్థిర శూన్య
cp_option_init (నిర్మాణం cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->dereference = DEREF_UNDEFINED;
  x->unlink_dest_before_opening = false;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_బహుళకం = false;
  x->install_బహుళకం = false;
  x->one_file_system = false;
  x->reflink_బహుళకం = REFLINK_AUTO;

  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_బహుళకం = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_బహుళకం = false;
  x->preserve_security_context = false; /* -a or --preserve=context.  */
  x->require_preserve_context = false;  /* --preserve=context.  */
  x->సమితి_security_context = ఖాళీ;       /* -Z, set sys default context. */
  x->preserve_xattr = false;
  x->reduce_diagnostics = false;
  x->require_preserve_xattr = false;

  x->దత్తాంశం_copy_required = true;
  x->require_preserve = false;
  x->recursive = false;
  x->sparse_బహుళకం = SPARSE_AUTO;
  x->symbolic_link = false;
  x->సమితి_బహుళకం = false;
  x->బహుళకం_2 = 0;

  /* Not used.  */
  x->stdin_tty = false;

  x->update = UPDATE_ALL;
  x->verbose = false;
  x->keep_directory_symlink = false;

  /* By default, refuse to open a dangling destination symlink, because
     in general one cannot do that safely, give the current semantics of
     open's O_EXCL flag, (which POSIX doesn't even allow cp to use, btw).
     But POSIX requires it.  */
  x->open_dangling_dest_symlink = getenv ("POSIXLY_CORRECT") != ఖాళీ;

  x->dest_info = ఖాళీ;
  x->src_info = ఖాళీ;
}

/* Given a string, ARG, containing a comma-separated list of arguments
   to the --preserve option, set the appropriate fields of X to ON_OFF.  */
స్థిర శూన్య
decode_preserve_arg (అక్షరం స్థిరరాశి *arg, నిర్మాణం cp_options *x, bool on_off)
{
  గణన File_attribute
    {
      PRESERVE_బహుళకం,
      PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP,
      PRESERVE_LINK,
      PRESERVE_CONTEXT,
      PRESERVE_XATTR,
      PRESERVE_ALL
    };
  స్థిర గణన File_attribute స్థిరరాశి preserve_vals[] =
    {
      PRESERVE_బహుళకం, PRESERVE_TIMESTAMPS,
      PRESERVE_OWNERSHIP, PRESERVE_LINK, PRESERVE_CONTEXT, PRESERVE_XATTR,
      PRESERVE_ALL
    };
  /* Valid arguments to the '--preserve' option. */
  స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి preserve_వాదనలు[] =
    {
      "mode", "timestamps",
      "ownership", "links", "context", "xattr", "all", ఖాళీ
    };
  ARGMATCH_VERIFY (preserve_వాదనలు, preserve_vals);

  అక్షరం *arg_writable = xstrdup (arg);
  అక్షరం *s = arg_writable;
  చేయి
    {
      /* find next comma */
      అక్షరం *comma = strchr (s, ',');
      గణన File_attribute val;

      /* If we found a comma, put a NUL in its place and advance.  */
      ఒకవేళ (comma)
        *comma++ = 0;

      /* process S.  */
      val = XARGMATCH (on_off ? "--preserve" : "--no-preserve",
                       s, preserve_వాదనలు, preserve_vals);
      ఎంపిక (val)
        {
        స్థితి PRESERVE_బహుళకం:
          x->preserve_బహుళకం = on_off;
          x->explicit_no_preserve_బహుళకం = !on_off;
          ఆపు;

        స్థితి PRESERVE_TIMESTAMPS:
          x->preserve_timestamps = on_off;
          ఆపు;

        స్థితి PRESERVE_OWNERSHIP:
          x->preserve_ownership = on_off;
          ఆపు;

        స్థితి PRESERVE_LINK:
          x->preserve_links = on_off;
          ఆపు;

        స్థితి PRESERVE_CONTEXT:
          x->require_preserve_context = on_off;
          x->preserve_security_context = on_off;
          ఆపు;

        స్థితి PRESERVE_XATTR:
          x->preserve_xattr = on_off;
          x->require_preserve_xattr = on_off;
          ఆపు;

        స్థితి PRESERVE_ALL:
          x->preserve_బహుళకం = on_off;
          x->preserve_timestamps = on_off;
          x->preserve_ownership = on_off;
          x->preserve_links = on_off;
          x->explicit_no_preserve_బహుళకం = !on_off;
          ఒకవేళ (selinux_enabled)
            x->preserve_security_context = on_off;
          x->preserve_xattr = on_off;
          ఆపు;

        అప్రమేయ:
          affirm (false);
        }
      s = comma;
    }
  అయితే (s);

  విడుదల (arg_writable);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool make_backups = false;
  అక్షరం స్థిరరాశి *backup_suffix = ఖాళీ;
  అక్షరం *సంస్కరణ_control_string = ఖాళీ;
  నిర్మాణం cp_options x;
  bool copy_contents = false;
  అక్షరం *లక్ష్యం_directory = ఖాళీ;
  bool no_లక్ష్యం_directory = false;
  అక్షరం స్థిరరాశి *scontext = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  selinux_enabled = (0 < is_selinux_enabled ());
  cp_option_init (&x);

  పూర్ణాంకం c;
  అయితే ((c = getopt_long (argc, argv, "abdfHilLnprst:uvxPRS:TZ",
                           long_opts, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి SPARSE_OPTION:
          x.sparse_బహుళకం = XARGMATCH ("--sparse", optarg,
                                     sparse_type_string, sparse_type);
          ఆపు;

        స్థితి REFLINK_OPTION:
          ఒకవేళ (optarg == ఖాళీ)
            x.reflink_బహుళకం = REFLINK_ALWAYS;
          లేకపోతే
            x.reflink_బహుళకం = XARGMATCH ("--reflink", optarg,
                                       reflink_type_string, reflink_type);
          ఆపు;

        స్థితి 'a':
          /* Like -dR --preserve=all with reduced failure diagnostics.  */
          x.dereference = DEREF_NEVER;
          x.preserve_links = true;
          x.preserve_ownership = true;
          x.preserve_బహుళకం = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ఒకవేళ (selinux_enabled)
             x.preserve_security_context = true;
          x.preserve_xattr = true;
          x.reduce_diagnostics = true;
          x.recursive = true;
          ఆపు;

        స్థితి 'b':
          make_backups = true;
          ఒకవేళ (optarg)
            సంస్కరణ_control_string = optarg;
          ఆపు;

        స్థితి లక్షణాలు_ONLY_OPTION:
          x.దత్తాంశం_copy_required = false;
          ఆపు;

        స్థితి DEBUG_OPTION:
          x.debug = x.verbose = true;
          ఆపు;

        స్థితి COPY_CONTENTS_OPTION:
          copy_contents = true;
          ఆపు;

        స్థితి 'd':
          x.preserve_links = true;
          x.dereference = DEREF_NEVER;
          ఆపు;

        స్థితి 'f':
          x.unlink_dest_after_failed_open = true;
          ఆపు;

        స్థితి 'H':
          x.dereference = DEREF_COMMAND_పంక్తి_ARGUMENTS;
          ఆపు;

        స్థితి 'i':
          x.interactive = I_ASK_USER;
          ఆపు;

        స్థితి 'l':
          x.hard_link = true;
          ఆపు;

        స్థితి 'L':
          x.dereference = DEREF_ALWAYS;
          ఆపు;

        స్థితి 'n':
          x.interactive = I_ALWAYS_SKIP;
          ఆపు;

        స్థితి 'P':
          x.dereference = DEREF_NEVER;
          ఆపు;

        స్థితి NO_PRESERVE_లక్షణాలు_OPTION:
          decode_preserve_arg (optarg, &x, false);
          ఆపు;

        స్థితి PRESERVE_లక్షణాలు_OPTION:
          ఒకవేళ (optarg == ఖాళీ)
            {
              /* Fall through to the case for 'p' below.  */
            }
          లేకపోతే
            {
              decode_preserve_arg (optarg, &x, true);
              x.require_preserve = true;
              ఆపు;
            }
          FALLTHROUGH;

        స్థితి 'p':
          x.preserve_ownership = true;
          x.preserve_బహుళకం = true;
          x.preserve_timestamps = true;
          x.require_preserve = true;
          ఆపు;

        స్థితి PARENTS_OPTION:
          parents_option = true;
          ఆపు;

        స్థితి 'r':
        స్థితి 'R':
          x.recursive = true;
          ఆపు;

        స్థితి UNLINK_DEST_BEFORE_OPENING:
          x.unlink_dest_before_opening = true;
          ఆపు;

        స్థితి STRIP_TRAILING_SLASHES_OPTION:
          remove_trailing_slashes = true;
          ఆపు;

        స్థితి 's':
          x.symbolic_link = true;
          ఆపు;

        స్థితి 't':
          ఒకవేళ (లక్ష్యం_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          లక్ష్యం_directory = optarg;
          ఆపు;

        స్థితి 'T':
          no_లక్ష్యం_directory = true;
          ఆపు;

        స్థితి 'u':
          x.update = UPDATE_OLDER;
          ఒకవేళ (optarg)
            x.update = XARGMATCH ("--update", optarg,
                                  update_type_string, update_type);
          ఆపు;

        స్థితి 'v':
          x.verbose = true;
          ఆపు;

        స్థితి KEEP_DIRECTORY_SYMLINK_OPTION:
          x.keep_directory_symlink = true;
          ఆపు;

        స్థితి 'x':
          x.one_file_system = true;
          ఆపు;

        స్థితి 'Z':
          /* politely decline if we're not on a selinux-enabled kernel.  */
          ఒకవేళ (selinux_enabled)
            {
              ఒకవేళ (optarg)
                scontext = optarg;
              లేకపోతే
                {
                  x.సమితి_security_context = selabel_open (SELABEL_CTX_FILE,
                                                         ఖాళీ, 0);
                  ఒకవేళ (! x.సమితి_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          లేకపోతే ఒకవేళ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ఆపు;

        స్థితి 'S':
          make_backups = true;
          backup_suffix = optarg;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  /* With --sparse=never, disable reflinking so we create a non sparse copy.
     This will also have the effect of disabling copy offload as that may
     propagate holes.  For e.g. FreeBSD documents that copy_file_range()
     will try to propagate holes.  */
  ఒకవేళ (x.reflink_బహుళకం == REFLINK_AUTO && x.sparse_బహుళకం == SPARSE_NEVER)
    x.reflink_బహుళకం = REFLINK_NEVER;

  ఒకవేళ (x.hard_link && x.symbolic_link)
    {
      error (0, 0, _("cannot make both hard and symbolic links"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (x.interactive == I_ALWAYS_SKIP)
    x.update = UPDATE_NONE;

  ఒకవేళ (make_backups
      && (x.update == UPDATE_NONE
          || x.update == UPDATE_NONE_FAIL))
    {
      error (0, 0,
             _("--backup is mutually exclusive with -n or --update=none-fail"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (x.reflink_బహుళకం == REFLINK_ALWAYS && x.sparse_బహుళకం != SPARSE_AUTO)
    {
      error (0, 0, _("--reflink can be used only with --sparse=auto"));
      usage (EXIT_FAILURE);
    }

  x.backup_type = (make_backups
                   ? xget_సంస్కరణ (_("backup type"),
                                   సంస్కరణ_control_string)
                   : no_backups);
  సమితి_simple_backup_suffix (backup_suffix);

  ఒకవేళ (x.dereference == DEREF_UNDEFINED)
    {
      ఒకవేళ (x.recursive && ! x.hard_link)
        /* This is compatible with FreeBSD.  */
        x.dereference = DEREF_NEVER;
      లేకపోతే
        x.dereference = DEREF_ALWAYS;
    }

  ఒకవేళ (x.recursive)
    x.copy_as_regular = copy_contents;

  /* Ensure -Z overrides -a.  */
  ఒకవేళ ((x.సమితి_security_context || scontext)
      && ! x.require_preserve_context)
    x.preserve_security_context = false;

  ఒకవేళ (x.preserve_security_context && (x.సమితి_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ఒకవేళ (x.require_preserve_context && ! selinux_enabled)
    error (EXIT_FAILURE, 0,
           _("cannot preserve security context "
             "without an SELinux-enabled kernel"));

  /* FIXME: This handles new files.  But what about existing files?
     I.e., if updating a tree, new files would have the specified context,
     but shouldn't existing files be updated for consistency like this?
       if (scontext && !restorecon (NULL, dst_path, 0))
          error (...);
   */
  ఒకవేళ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
           quote (scontext));

#ఒకవేళ !USE_XATTR
  ఒకవేళ (x.require_preserve_xattr)
    error (EXIT_FAILURE, 0, _("cannot preserve extended attributes, cp is "
                              "built without xattr support"));
#అయితే_ముగింపు

  /* Allocate space for remembering copied and created files.  */

  hash_init ();

  bool ok = do_copy (argc - optind, argv + optind,
                     లక్ష్యం_directory, no_లక్ష్యం_directory, &x);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
