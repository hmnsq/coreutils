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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <signal.h>
#شامل <pwd.h>
#شامل <grp.h>
#شامل <selinux/label.h>
#شامل <spawn.h>
#شامل <sys/wait.h>

#شامل "system.h"
#شامل "backupfile.h"
#شامل "cp-hash.h"
#شامل "copy.h"
#شامل "filenamecat.h"
#شامل "full-read.h"
#شامل "mkancesdirs.h"
#شامل "mkdir-p.h"
#شامل "modechange.h"
#شامل "prog-fprintf.h"
#شامل "quote.h"
#شامل "savewd.h"
#شامل "selinux.h"
#شامل "stat-time.h"
#شامل "targetdir.h"
#شامل "utimens.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

ایستا صحیح selinux_enabled = 0;
ایستا bool use_default_selinux_context = true;

#اگر ! HAVE_ENDGRENT
# define endgrent() ((تهی) 0)
#endif

#اگر ! HAVE_ENDPWENT
# define endpwent() ((تهی) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ایستا نویسه *owner_name;

/* The user ID corresponding to 'owner_name'. */
ایستا uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ایستا نویسه *group_name;

/* The group ID corresponding to 'group_name'. */
ایستا gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ایستا mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
ایستا mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ایستا mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
ایستا bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ایستا bool strip_files;

/* If true, install a directory instead of a regular file. */
ایستا bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ایستا نویسه ثابت *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

ایستا ساختار option ثابت long_options[] =
{
  {"backup", optional_argument, پوچ, 'b'},
  {"compare", no_argument, پوچ, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, پوچ, DEBUG_OPTION},
  {"directory", no_argument, پوچ, 'd'},
  {"group", required_argument, پوچ, 'g'},
  {"mode", required_argument, پوچ, 'm'},
  {"no-target-directory", no_argument, پوچ, 'T'},
  {"owner", required_argument, پوچ, 'o'},
  {"preserve-timestamps", no_argument, پوچ, 'p'},
  {"preserve-context", no_argument, پوچ, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, پوچ, 's'},
  {"strip-program", required_argument, پوچ, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, پوچ, 'S'},
  {"target-directory", required_argument, پوچ, 't'},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ایستا bool
have_same_content (صحیح a_fd, صحیح b_fd)
{
  شمارش { CMP_BLOCK_SIZE = 4096 };
  ایستا نویسه a_buff[CMP_BLOCK_SIZE];
  ایستا نویسه b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  وقتی (0 < (size = full_read (a_fd, a_buff, اندازه a_buff))) {
    اگر (size != full_read (b_fd, b_buff, اندازه b_buff))
      بازگشت false;

    اگر (!memeq (a_buff, b_buff, size))
      بازگشت false;
  }

  بازگشت size == 0;
}

/* Return true for mode with non-permission bits. */
ایستا bool
extra_mode (mode_t درون_داد)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  بازگشت !! (درون_داد & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ایستا bool
need_copy (نویسه ثابت *src_name, نویسه ثابت *dest_name,
           صحیح dest_dirfd, نویسه ثابت *dest_relname,
           ثابت ساختار cp_options *x)
{
  اگر (extra_mode (mode))
    بازگشت true;

  /* compare files using stat */
  ساختار stat src_sb;
  اگر (stat (src_name, &src_sb) != 0)
    بازگشت true;

  ساختار stat dest_sb;
  اگر (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    بازگشت true;

  اگر (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    بازگشت true;

  اگر (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    بازگشت true;

  اگر (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      اگر ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        بازگشت true;
    }
  وگرنه اگر (dest_sb.st_uid != owner_id)
    بازگشت true;

  اگر (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      اگر ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        بازگشت true;
    }
  وگرنه اگر (dest_sb.st_gid != group_id)
    بازگشت true;

  /* compare SELinux context if preserving */
  اگر (selinux_enabled && x->preserve_security_context)
    {
      نویسه *file_scontext_raw = پوچ;
      اگر (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        بازگشت true;

      نویسه *to_scontext_raw = پوچ;
      اگر (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          بازگشت true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      اگر (!scontext_match)
        بازگشت true;
    }

  /* compare files content */
  صحیح src_fd = open (src_name, O_RDONLY | O_دودویی);
  اگر (src_fd < 0)
    بازگشت true;

  صحیح dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_دودویی);
  اگر (dest_fd < 0)
    {
      close (src_fd);
      بازگشت true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  بازگشت !content_match;
}

ایستا تهی
cp_option_init (ساختار cp_options *x)
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
  x->data_copy_required = true;
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
  x->set_security_context = پوچ; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = پوچ;
  x->src_info = پوچ;
}

ایستا ساختار selabel_handle *
get_labeling_handle (تهی)
{
  ایستا bool initialized;
  ایستا ساختار selabel_handle *hnd;
  اگر (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, پوچ, 0);
      اگر (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  بازگشت hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ایستا تهی
setdefaultfilecon (نویسه ثابت *file)
{
  اگر (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      بازگشت;
    }

  ساختار stat st;
  اگر (lstat (file, &st) != 0)
    بازگشت;

  ساختار selabel_handle *hnd = get_labeling_handle ();
  اگر (!hnd)
    بازگشت;

  نویسه *scontext_raw = پوچ;
  اگر (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      اگر (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      بازگشت;
    }

  اگر (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ایستا تهی
announce_mkdir (نویسه ثابت *dir, تهی *options)
{
  ساختار cp_options ثابت *x = options;
  اگر (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ایستا صحیح
make_ancestor (نویسه ثابت *dir, نویسه ثابت *component, تهی *options)
{
  ساختار cp_options ثابت *x = options;
  اگر (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  صحیح r = mkdir (component, DEFAULT_MODE);
  اگر (r == 0)
    announce_mkdir (dir, options);
  بازگشت r;
}

/* Process a command-line file name, for the -d option.  */
ایستا صحیح
process_dir (نویسه *dir, ساختار savewd *wd, تهی *options)
{
  ساختار cp_options ثابت *x = options;

  صحیح ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  اگر (ret == EXIT_SUCCESS && x->set_security_context)
    {
      اگر (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  بازگشت ret;
}

شمارش copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ایستا شمارش copy_status
copy_file (نویسه ثابت *from, نویسه ثابت *to,
           صحیح to_dirfd, نویسه ثابت *to_relname, ثابت ساختار cp_options *x)
{
  اگر (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    بازگشت COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  بازگشت (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, پوچ)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ایستا bool
change_attributes (نویسه ثابت *name, صحیح dirfd, نویسه ثابت *relname)
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

  اگر (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  وگرنه اگر (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  وگرنه
    ok = true;

  اگر (use_default_selinux_context)
    setdefaultfilecon (name);

  بازگشت ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ایستا bool
change_timestamps (ساختار stat ثابت *src_sb, نویسه ثابت *dest,
                   صحیح dirfd, نویسه ثابت *relname)
{
  ساختار timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  اگر (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      بازگشت false;
    }
  بازگشت true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ایستا bool
strip (نویسه ثابت *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = پوچ;

  /* Try to use vfork for systems where it matters.  */
  اگر (posix_spawnattr_init (&attr) == 0)
    {
      اگر (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      وگرنه
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  نویسه *concat_name = پوچ;
  نویسه ثابت *safe_name = name;
  اگر (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, پوچ);
  نویسه ثابت *ثابت argv[] = { strip_program, safe_name, پوچ };

  /* Run 'strip'.  */
  pid_t pid;
  صحیح result = posix_spawnp (&pid, strip_program, پوچ, attrp,
                             (نویسه * ثابت *) argv, environ);

  bool ok = false;
  اگر (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  وگرنه
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      صحیح status;
      اگر (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      وگرنه اگر (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      وگرنه
        ok = true;
    }

  آزاد (concat_name);
  اگر (attrp)
    posix_spawnattr_destroy (attrp);

  بازگشت ok;
}

/* Initialize the user and group ownership of the files to install. */

ایستا تهی
get_ids (تهی)
{
  اگر (owner_name)
    {
      ساختار passwd *pw = getpwnam (owner_name);
      اگر (pw == پوچ)
        {
          uintmax_t tmp;
          اگر (xstrtoumax (owner_name, پوچ, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      وگرنه
        owner_id = pw->pw_uid;
      endpwent ();
    }
  وگرنه
    owner_id = (uid_t) -1;

  اگر (group_name)
    {
      ساختار group *gr = getgrnam (group_name);
      اگر (gr == پوچ)
        {
          uintmax_t tmp;
          اگر (xstrtoumax (group_name, پوچ, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      وگرنه
        group_id = gr->gr_gid;
      endgrent ();
    }
  وگرنه
    group_id = (gid_t) -1;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
the existing DIRECTORY, وقتی setting permission modes and owner/group.\n\
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
         and اگر no change to content, ownership, and permissions,\n\
         انجامبده not modify the destination at all\n\
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
         strip نماد tables\n\
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
         and each created directory, to پیشفرض type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then set the\n\
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

ایستا bool
install_file_in_file (نویسه ثابت *from, نویسه ثابت *to,
                      صحیح to_dirfd, نویسه ثابت *to_relname,
                      ثابت ساختار cp_options *x)
{
  ساختار stat from_sb;
  اگر (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      بازگشت false;
    }
  شمارش copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  اگر (copy_status == COPY_FAILED)
    بازگشت false;
  اگر (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      اگر (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      بازگشت false;
    }
  اگر (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    بازگشت false;

  بازگشت copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ایستا bool
mkancesdirs_safe_wd (نویسه ثابت *from, نویسه *to, ساختار cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  ساختار savewd wd;
  savewd_init (&wd);
  اگر (! save_working_directory)
    savewd_finish (&wd);

  صحیح status = EXIT_SUCCESS;
  اگر (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  اگر (save_working_directory)
    {
      صحیح restore_result = savewd_restore (&wd, status);
      صحیح restore_errno = errno;
      savewd_finish (&wd);
      اگر (EXIT_SUCCESS < restore_result)
        بازگشت false;
      اگر (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          بازگشت false;
        }
    }
  بازگشت status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ایستا bool
install_file_in_file_parents (نویسه ثابت *from, نویسه *to,
                              ثابت ساختار cp_options *x)
{
  بازگشت (mkancesdirs_safe_wd (from, to, (ساختار cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ایستا bool
install_file_in_dir (نویسه ثابت *from, نویسه ثابت *to_dir,
                     ثابت ساختار cp_options *x, bool mkdir_and_install,
                     صحیح *target_dirfd)
{
  نویسه ثابت *from_base = last_component (from);
  نویسه *to_relname;
  نویسه *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  اگر (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (ساختار cp_options *) x, true)))
    {
      صحیح fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      اگر (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      وگرنه
        *target_dirfd = fd;
    }

  اگر (ret)
    {
      صحیح to_dirfd = *target_dirfd;
      اگر (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  آزاد (to);
  بازگشت ret;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  نویسه ثابت *specified_mode = پوچ;
  bool make_backups = false;
  نویسه ثابت *backup_suffix = پوچ;
  نویسه *version_control_string = پوچ;
  bool mkdir_and_install = false;
  ساختار cp_options x;
  نویسه ثابت *target_directory = پوچ;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  نویسه ثابت *scontext = پوچ;
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

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              پوچ))
         != -1)
    {
      انتخاب (optc)
        {
        حالت 'b':
          make_backups = true;
          اگر (optarg)
            version_control_string = optarg;
          بشکن;
        حالت 'c':
          بشکن;
        حالت 'C':
          copy_only_if_needed = true;
          بشکن;
        حالت 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          سیگنال (SIGCHLD, SIG_DFL);
#endif
          بشکن;
        حالت DEBUG_OPTION:
          x.debug = x.verbose = true;
          بشکن;
        حالت STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          بشکن;
        حالت 'd':
          dir_arg = true;
          بشکن;
        حالت 'D':
          mkdir_and_install = true;
          بشکن;
        حالت 'v':
          x.verbose = true;
          بشکن;
        حالت 'g':
          group_name = optarg;
          بشکن;
        حالت 'm':
          specified_mode = optarg;
          بشکن;
        حالت 'o':
          owner_name = optarg;
          بشکن;
        حالت 'p':
          x.preserve_timestamps = true;
          بشکن;
        حالت 'S':
          make_backups = true;
          backup_suffix = optarg;
          بشکن;
        حالت 't':
          اگر (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          بشکن;
        حالت 'T':
          no_target_directory = true;
          بشکن;

        حالت PRESERVE_CONTEXT_OPTION:
          اگر (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              بشکن;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          بشکن;
        حالت 'Z':
          اگر (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              اگر (optarg)
                scontext = optarg;
              وگرنه
                x.set_security_context = get_labeling_handle ();
            }
          وگرنه اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  اگر (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  اگر (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  اگر (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  اگر (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  صحیح n_files = argc - optind;
  نویسه **file = argv + optind;

  اگر (n_files <= ! (dir_arg || target_directory))
    {
      اگر (n_files <= 0)
        error (0, 0, _("missing file operand"));
      وگرنه
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ساختار stat sb;
  صحیح target_dirfd = AT_FDCWD;
  اگر (no_target_directory)
    {
      اگر (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      اگر (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  وگرنه اگر (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      اگر (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  وگرنه اگر (!dir_arg)
    {
      نویسه ثابت *lastfile = file[n_files - 1];
      صحیح fd = target_directory_operand (lastfile, &sb);
      اگر (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      وگرنه اگر (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  اگر (specified_mode)
    {
      ساختار mode_change *change = mode_compile (specified_mode);
      اگر (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, پوچ);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      آزاد (change);
    }

  اگر (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  اگر (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  اگر (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  صحیح exit_status = EXIT_SUCCESS;
  اگر (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  وگرنه
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      اگر (!target_directory)
        {
          اگر (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      وگرنه
        {
          dest_info_init (&x);
          برای (صحیح i = 0; i < n_files; i++)
            اگر (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
