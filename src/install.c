/* install - copy files and set attributes
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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu> */

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>
#অন্তর্ভাব <selinux/label.h>
#অন্তর্ভাব <spawn.h>
#অন্তর্ভাব <sys/wait.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "backupfile.h"
#অন্তর্ভাব "cp-hash.h"
#অন্তর্ভাব "copy.h"
#অন্তর্ভাব "filenamecat.h"
#অন্তর্ভাব "full-read.h"
#অন্তর্ভাব "mkancesdirs.h"
#অন্তর্ভাব "mkdir-p.h"
#অন্তর্ভাব "modechange.h"
#অন্তর্ভাব "prog-fprintf.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "savewd.h"
#অন্তর্ভাব "selinux.h"
#অন্তর্ভাব "stat-time.h"
#অন্তর্ভাব "targetdir.h"
#অন্তর্ভাব "utimens.h"
#অন্তর্ভাব "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

স্থির পূর্ণসংখ্যা selinux_enabled = 0;
স্থির bool use_default_selinux_context = true;

#যদি ! HAVE_ENDGRENT
# define endgrent() ((শূন্য) 0)
#endif

#যদি ! HAVE_ENDPWENT
# define endpwent() ((শূন্য) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
স্থির অক্ষর *owner_name;

/* The user ID corresponding to 'owner_name'. */
স্থির uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
স্থির অক্ষর *group_name;

/* The group ID corresponding to 'group_name'. */
স্থির gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
স্থির mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
স্থির mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
স্থির mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
স্থির bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
স্থির bool strip_files;

/* If true, install a directory instead of a regular file. */
স্থির bool dir_arg;

/* Program used to strip binaries, "strip" is default */
স্থির অক্ষর ধ্রুবক *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"backup", optional_argument, ফাঁকা, 'b'},
  {"compare", no_argument, ফাঁকা, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, ফাঁকা, DEBUG_OPTION},
  {"directory", no_argument, ফাঁকা, 'd'},
  {"group", required_argument, ফাঁকা, 'g'},
  {"mode", required_argument, ফাঁকা, 'm'},
  {"no-target-directory", no_argument, ফাঁকা, 'T'},
  {"owner", required_argument, ফাঁকা, 'o'},
  {"preserve-timestamps", no_argument, ফাঁকা, 'p'},
  {"preserve-context", no_argument, ফাঁকা, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, ফাঁকা, 's'},
  {"strip-program", required_argument, ফাঁকা, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, ফাঁকা, 'S'},
  {"target-directory", required_argument, ফাঁকা, 't'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
স্থির bool
have_same_content (পূর্ণসংখ্যা a_fd, পূর্ণসংখ্যা b_fd)
{
  গণনা { CMP_BLOCK_আকার = 4096 };
  স্থির অক্ষর a_buff[CMP_BLOCK_আকার];
  স্থির অক্ষর b_buff[CMP_BLOCK_আকার];

  idx_t আকার_2;
  যখন (0 < (আকার_2 = full_read (a_fd, a_buff, আকার a_buff))) {
    যদি (আকার_2 != full_read (b_fd, b_buff, আকার b_buff))
      ফেরত false;

    যদি (!memeq (a_buff, b_buff, আকার_2))
      ফেরত false;
  }

  ফেরত আকার_2 == 0;
}

/* Return true for mode with non-permission bits. */
স্থির bool
extra_mode (mode_t প্রবেশ)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  ফেরত !! (প্রবেশ & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
স্থির bool
need_copy (অক্ষর ধ্রুবক *src_name, অক্ষর ধ্রুবক *dest_name,
           পূর্ণসংখ্যা dest_dirfd, অক্ষর ধ্রুবক *dest_relname,
           ধ্রুবক কাঠামো cp_options *x)
{
  যদি (extra_mode (mode))
    ফেরত true;

  /* compare files using stat */
  কাঠামো stat src_sb;
  যদি (stat (src_name, &src_sb) != 0)
    ফেরত true;

  কাঠামো stat dest_sb;
  যদি (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ফেরত true;

  যদি (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    ফেরত true;

  যদি (src_sb.st_আকার != dest_sb.st_আকার
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    ফেরত true;

  যদি (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      যদি ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ফেরত true;
    }
  অন্যথা যদি (dest_sb.st_uid != owner_id)
    ফেরত true;

  যদি (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      যদি ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ফেরত true;
    }
  অন্যথা যদি (dest_sb.st_gid != group_id)
    ফেরত true;

  /* compare SELinux context if preserving */
  যদি (selinux_enabled && x->preserve_security_context)
    {
      অক্ষর *file_scontext_raw = ফাঁকা;
      যদি (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ফেরত true;

      অক্ষর *to_scontext_raw = ফাঁকা;
      যদি (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ফেরত true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      যদি (!scontext_match)
        ফেরত true;
    }

  /* compare files content */
  পূর্ণসংখ্যা src_fd = open (src_name, O_RDONLY | O_দ্বিমিক);
  যদি (src_fd < 0)
    ফেরত true;

  পূর্ণসংখ্যা dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_দ্বিমিক);
  যদি (dest_fd < 0)
    {
      close (src_fd);
      ফেরত true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ফেরত !content_match;
}

স্থির শূন্য
cp_option_init (কাঠামো cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_mode = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_mode = false;
  x->install_mode = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_mode = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_mode = false;
  x->reduce_diagnostics=false;
  x->তথ্য_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_mode = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->set_mode = true;
  x->mode = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->set_security_context = ফাঁকা; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ফাঁকা;
  x->src_info = ফাঁকা;
}

স্থির কাঠামো selabel_handle *
get_labeling_handle (শূন্য)
{
  স্থির bool initialized;
  স্থির কাঠামো selabel_handle *hnd;
  যদি (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ফাঁকা, 0);
      যদি (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ফেরত hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
স্থির শূন্য
setdefaultfilecon (অক্ষর ধ্রুবক *file)
{
  যদি (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ফেরত;
    }

  কাঠামো stat st;
  যদি (lstat (file, &st) != 0)
    ফেরত;

  কাঠামো selabel_handle *hnd = get_labeling_handle ();
  যদি (!hnd)
    ফেরত;

  অক্ষর *scontext_raw = ফাঁকা;
  যদি (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      যদি (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ফেরত;
    }

  যদি (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
স্থির শূন্য
announce_mkdir (অক্ষর ধ্রুবক *dir, শূন্য *options)
{
  কাঠামো cp_options ধ্রুবক *x = options;
  যদি (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
স্থির পূর্ণসংখ্যা
make_ancestor (অক্ষর ধ্রুবক *dir, অক্ষর ধ্রুবক *component, শূন্য *options)
{
  কাঠামো cp_options ধ্রুবক *x = options;
  যদি (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  পূর্ণসংখ্যা r = mkdir (component, DEFAULT_MODE);
  যদি (r == 0)
    announce_mkdir (dir, options);
  ফেরত r;
}

/* Process a command-line file name, for the -d option.  */
স্থির পূর্ণসংখ্যা
process_dir (অক্ষর *dir, কাঠামো savewd *wd, শূন্য *options)
{
  কাঠামো cp_options ধ্রুবক *x = options;

  পূর্ণসংখ্যা ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  যদি (ret == EXIT_SUCCESS && x->set_security_context)
    {
      যদি (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ফেরত ret;
}

গণনা copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

স্থির গণনা copy_status
copy_file (অক্ষর ধ্রুবক *from, অক্ষর ধ্রুবক *to,
           পূর্ণসংখ্যা to_dirfd, অক্ষর ধ্রুবক *to_relname, ধ্রুবক কাঠামো cp_options *x)
{
  যদি (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    ফেরত COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ফেরত (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, ফাঁকা)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

স্থির bool
change_attributes (অক্ষর ধ্রুবক *name, পূর্ণসংখ্যা dirfd, অক্ষর ধ্রুবক *relname)
{
  bool ok = false;
  /* chown must precede chmod because on some systems,
     chown clears the set[ug]id bits for non-superusers,
     resulting in incorrect permissions.
     On System V, users can give away files with chown and then not
     be able to chmod them.  So don't give files away.

     We don't normally ignore errors from chown because the idea of
     the install command is that the file is supposed to end up with
     precisely the attributes that the user specified (or defaulted).
     If the file doesn't end up with the group they asked for, they'll
     want to know.  */

  যদি (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  অন্যথা যদি (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  অন্যথা
    ok = true;

  যদি (use_default_selinux_context)
    setdefaultfilecon (name);

  ফেরত ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

স্থির bool
change_timestamps (কাঠামো stat ধ্রুবক *src_sb, অক্ষর ধ্রুবক *dest,
                   পূর্ণসংখ্যা dirfd, অক্ষর ধ্রুবক *relname)
{
  কাঠামো timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  যদি (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ফেরত false;
    }
  ফেরত true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

স্থির bool
strip (অক্ষর ধ্রুবক *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ফাঁকা;

  /* Try to use vfork for systems where it matters.  */
  যদি (posix_spawnattr_init (&attr) == 0)
    {
      যদি (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      অন্যথা
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  অক্ষর *concat_name = ফাঁকা;
  অক্ষর ধ্রুবক *safe_name = name;
  যদি (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, ফাঁকা);
  অক্ষর ধ্রুবক *ধ্রুবক argv[] = { strip_program, safe_name, ফাঁকা };

  /* Run 'strip'.  */
  pid_t pid;
  পূর্ণসংখ্যা ফলাফল = posix_spawnp (&pid, strip_program, ফাঁকা, attrp,
                             (অক্ষর * ধ্রুবক *) argv, environ);

  bool ok = false;
  যদি (ফলাফল != 0)
    {
      error (0, ফলাফল,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  অন্যথা
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      পূর্ণসংখ্যা status;
      যদি (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      অন্যথা যদি (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      অন্যথা
        ok = true;
    }

  মুক্ত (concat_name);
  যদি (attrp)
    posix_spawnattr_destroy (attrp);

  ফেরত ok;
}

/* Initialize the user and group ownership of the files to install. */

স্থির শূন্য
get_ids (শূন্য)
{
  যদি (owner_name)
    {
      কাঠামো passwd *pw = getpwnam (owner_name);
      যদি (pw == ফাঁকা)
        {
          uintmax_t tmp;
          যদি (xstrtoumax (owner_name, ফাঁকা, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      অন্যথা
        owner_id = pw->pw_uid;
      endpwent ();
    }
  অন্যথা
    owner_id = (uid_t) -1;

  যদি (group_name)
    {
      কাঠামো group *gr = getgrnam (group_name);
      যদি (gr == ফাঁকা)
        {
          uintmax_t tmp;
          যদি (xstrtoumax (group_name, ফাঁকা, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      অন্যথা
        group_id = gr->gr_gid;
      endgrent ();
    }
  অন্যথা
    group_id = (gid_t) -1;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [-T] SOURCE DEST\n\
  or:  %s [OPTION]... SOURCE... DIRECTORY\n\
  or:  %s [OPTION]... -t DIRECTORY SOURCE...\n\
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              program_name, program_name, program_name, program_name);
      fputs (_("\
\n\
This install program copies files (often just compiled) into destination\n\
locations you choose.  If you want to download and install a ready-to-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
In the first three forms, copy SOURCE to DEST or multiple SOURCE(s) to\n\
the existing DIRECTORY, যখন setting permission modes and owner/group.\n\
In the 4th form, create all components of the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup of each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an argument\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content of source and destination files,\n\
         and যদি no change to content, ownership, and permissions,\n\
         করো not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory names;\n\
         create all components of the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components of DEST except the last,\n\
         or all components of --target-directory,\n\
         then copy SOURCE to DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  Implies -v\n\
"));
      oputs (_("\
  -g, --group=GROUP\n\
         set group ownership, instead of process' current group\n\
"));
      oputs (_("\
  -m, --mode=MODE\n\
         set permission mode (as in chmod), instead of rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         set ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times of SOURCE files\n\
         to corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip প্রতীক tables\n\
"));
      oputs (_("\
      --strip-program=PROGRAM\n\
         program used to strip binaries\n\
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
  -v, --verbose\n\
         print the name of each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         set SELinux security context of destination file\n\
         and each created directory, to পূর্বনির্ধারিত type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or যদি CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

স্থির bool
install_file_in_file (অক্ষর ধ্রুবক *from, অক্ষর ধ্রুবক *to,
                      পূর্ণসংখ্যা to_dirfd, অক্ষর ধ্রুবক *to_relname,
                      ধ্রুবক কাঠামো cp_options *x)
{
  কাঠামো stat from_sb;
  যদি (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ফেরত false;
    }
  গণনা copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  যদি (copy_status == COPY_FAILED)
    ফেরত false;
  যদি (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      যদি (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      ফেরত false;
    }
  যদি (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    ফেরত false;

  ফেরত copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

স্থির bool
mkancesdirs_safe_wd (অক্ষর ধ্রুবক *from, অক্ষর *to, কাঠামো cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  কাঠামো savewd wd;
  savewd_init (&wd);
  যদি (! save_working_directory)
    savewd_finish (&wd);

  পূর্ণসংখ্যা status = EXIT_SUCCESS;
  যদি (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  যদি (save_working_directory)
    {
      পূর্ণসংখ্যা restore_ফলাফল = savewd_restore (&wd, status);
      পূর্ণসংখ্যা restore_errno = errno;
      savewd_finish (&wd);
      যদি (EXIT_SUCCESS < restore_ফলাফল)
        ফেরত false;
      যদি (restore_ফলাফল < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          ফেরত false;
        }
    }
  ফেরত status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

স্থির bool
install_file_in_file_parents (অক্ষর ধ্রুবক *from, অক্ষর *to,
                              ধ্রুবক কাঠামো cp_options *x)
{
  ফেরত (mkancesdirs_safe_wd (from, to, (কাঠামো cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

স্থির bool
install_file_in_dir (অক্ষর ধ্রুবক *from, অক্ষর ধ্রুবক *to_dir,
                     ধ্রুবক কাঠামো cp_options *x, bool mkdir_and_install,
                     পূর্ণসংখ্যা *target_dirfd)
{
  অক্ষর ধ্রুবক *from_base = last_component (from);
  অক্ষর *to_relname;
  অক্ষর *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  যদি (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (কাঠামো cp_options *) x, true)))
    {
      পূর্ণসংখ্যা fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      যদি (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      অন্যথা
        *target_dirfd = fd;
    }

  যদি (ret)
    {
      পূর্ণসংখ্যা to_dirfd = *target_dirfd;
      যদি (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  মুক্ত (to);
  ফেরত ret;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  অক্ষর ধ্রুবক *specified_mode = ফাঁকা;
  bool make_backups = false;
  অক্ষর ধ্রুবক *backup_suffix = ফাঁকা;
  অক্ষর *version_control_string = ফাঁকা;
  bool mkdir_and_install = false;
  কাঠামো cp_options x;
  অক্ষর ধ্রুবক *target_directory = ফাঁকা;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  অক্ষর ধ্রুবক *scontext = ফাঁকা;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  পূর্ণসংখ্যা optc;
  যখন ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ফাঁকা))
         != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'b':
          make_backups = true;
          যদি (optarg)
            version_control_string = optarg;
          বিরতি;
        ক্ষেত্র 'c':
          বিরতি;
        ক্ষেত্র 'C':
          copy_only_if_needed = true;
          বিরতি;
        ক্ষেত্র 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          সংকেত (SIGCHLD, SIG_DFL);
#endif
          বিরতি;
        ক্ষেত্র DEBUG_OPTION:
          x.debug = x.verbose = true;
          বিরতি;
        ক্ষেত্র STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          বিরতি;
        ক্ষেত্র 'd':
          dir_arg = true;
          বিরতি;
        ক্ষেত্র 'D':
          mkdir_and_install = true;
          বিরতি;
        ক্ষেত্র 'v':
          x.verbose = true;
          বিরতি;
        ক্ষেত্র 'g':
          group_name = optarg;
          বিরতি;
        ক্ষেত্র 'm':
          specified_mode = optarg;
          বিরতি;
        ক্ষেত্র 'o':
          owner_name = optarg;
          বিরতি;
        ক্ষেত্র 'p':
          x.preserve_timestamps = true;
          বিরতি;
        ক্ষেত্র 'S':
          make_backups = true;
          backup_suffix = optarg;
          বিরতি;
        ক্ষেত্র 't':
          যদি (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          বিরতি;
        ক্ষেত্র 'T':
          no_target_directory = true;
          বিরতি;

        ক্ষেত্র PRESERVE_CONTEXT_OPTION:
          যদি (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              বিরতি;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          বিরতি;
        ক্ষেত্র 'Z':
          যদি (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              যদি (optarg)
                scontext = optarg;
              অন্যথা
                x.set_security_context = get_labeling_handle ();
            }
          অন্যথা যদি (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  যদি (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  যদি (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  যদি (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  যদি (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  পূর্ণসংখ্যা n_files = argc - optind;
  অক্ষর **file = argv + optind;

  যদি (n_files <= ! (dir_arg || target_directory))
    {
      যদি (n_files <= 0)
        error (0, 0, _("missing file operand"));
      অন্যথা
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  কাঠামো stat sb;
  পূর্ণসংখ্যা target_dirfd = AT_FDCWD;
  যদি (no_target_directory)
    {
      যদি (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      যদি (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  অন্যথা যদি (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      যদি (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  অন্যথা যদি (!dir_arg)
    {
      অক্ষর ধ্রুবক *lastfile = file[n_files - 1];
      পূর্ণসংখ্যা fd = target_directory_operand (lastfile, &sb);
      যদি (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      অন্যথা যদি (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  যদি (specified_mode)
    {
      কাঠামো mode_change *change = mode_compile (specified_mode);
      যদি (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, ফাঁকা);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      মুক্ত (change);
    }

  যদি (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  যদি (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  যদি (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  পূর্ণসংখ্যা exit_status = EXIT_SUCCESS;
  যদি (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  অন্যথা
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      যদি (!target_directory)
        {
          যদি (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      অন্যথা
        {
          dest_info_init (&x);
          জন্য (পূর্ণসংখ্যা i = 0; i < n_files; i++)
            যদি (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
