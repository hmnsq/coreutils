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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>
#ಅನ್ತರ್ಭಾವ <spawn.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "backupfile.h"
#ಅನ್ತರ್ಭಾವ "cp-hash.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "filenamecat.h"
#ಅನ್ತರ್ಭಾವ "full-read.h"
#ಅನ್ತರ್ಭಾವ "mkancesdirs.h"
#ಅನ್ತರ್ಭಾವ "mkdir-p.h"
#ಅನ್ತರ್ಭಾವ "modechange.h"
#ಅನ್ತರ್ಭಾವ "prog-fprintf.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "savewd.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "targetdir.h"
#ಅನ್ತರ್ಭಾವ "utimens.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ selinux_enabled = 0;
ಸ್ಥಿರ bool use_default_selinux_context = true;

#ಯದಿ ! HAVE_ENDGRENT
# define endgrent() ((ಶೂನ್ಯ) 0)
#endif

#ಯದಿ ! HAVE_ENDPWENT
# define endpwent() ((ಶೂನ್ಯ) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ಸ್ಥಿರ ವರ್ಣ *owner_name;

/* The user ID corresponding to 'owner_name'. */
ಸ್ಥಿರ uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ಸ್ಥಿರ ವರ್ಣ *group_name;

/* The group ID corresponding to 'group_name'. */
ಸ್ಥಿರ gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ಸ್ಥಿರ mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
ಸ್ಥಿರ mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ಸ್ಥಿರ mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
ಸ್ಥಿರ bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ಸ್ಥಿರ bool strip_files;

/* If true, install a directory instead of a regular file. */
ಸ್ಥಿರ bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"backup", optional_argument, ರಿಕ್ತ, 'b'},
  {"compare", no_argument, ರಿಕ್ತ, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, ರಿಕ್ತ, DEBUG_OPTION},
  {"directory", no_argument, ರಿಕ್ತ, 'd'},
  {"group", required_argument, ರಿಕ್ತ, 'g'},
  {"mode", required_argument, ರಿಕ್ತ, 'm'},
  {"no-target-directory", no_argument, ರಿಕ್ತ, 'T'},
  {"owner", required_argument, ರಿಕ್ತ, 'o'},
  {"preserve-timestamps", no_argument, ರಿಕ್ತ, 'p'},
  {"preserve-context", no_argument, ರಿಕ್ತ, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, ರಿಕ್ತ, 's'},
  {"strip-program", required_argument, ರಿಕ್ತ, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, ರಿಕ್ತ, 'S'},
  {"target-directory", required_argument, ರಿಕ್ತ, 't'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ಸ್ಥಿರ bool
have_same_content (ಪೂರ್ಣಾಙ್ಕ a_fd, ಪೂರ್ಣಾಙ್ಕ b_fd)
{
  ಗಣನಾ { CMP_BLOCK_SIZE = 4096 };
  ಸ್ಥಿರ ವರ್ಣ a_buff[CMP_BLOCK_SIZE];
  ಸ್ಥಿರ ವರ್ಣ b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  ಯಾವತ್ (0 < (size = full_read (a_fd, a_buff, ಪರಿಮಾಣ a_buff))) {
    ಯದಿ (size != full_read (b_fd, b_buff, ಪರಿಮಾಣ b_buff))
      ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (!memeq (a_buff, b_buff, size))
      ಪ್ರತ್ಯಾಗಮ false;
  }

  ಪ್ರತ್ಯಾಗಮ size == 0;
}

/* Return true for mode with non-permission bits. */
ಸ್ಥಿರ bool
extra_mode (mode_t ಒಳಹರಿವು)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  ಪ್ರತ್ಯಾಗಮ !! (ಒಳಹರಿವು & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ಸ್ಥಿರ bool
need_copy (ವರ್ಣ ಧ್ರುವ *src_name, ವರ್ಣ ಧ್ರುವ *dest_name,
           ಪೂರ್ಣಾಙ್ಕ dest_dirfd, ವರ್ಣ ಧ್ರುವ *dest_relname,
           ಧ್ರುವ ಸಂರಚನಾ cp_options *x)
{
  ಯದಿ (extra_mode (mode))
    ಪ್ರತ್ಯಾಗಮ true;

  /* compare files using stat */
  ಸಂರಚನಾ stat src_sb;
  ಯದಿ (stat (src_name, &src_sb) != 0)
    ಪ್ರತ್ಯಾಗಮ true;

  ಸಂರಚನಾ stat dest_sb;
  ಯದಿ (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ಪ್ರತ್ಯಾಗಮ true;

  ಯದಿ (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    ಪ್ರತ್ಯಾಗಮ true;

  ಯದಿ (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    ಪ್ರತ್ಯಾಗಮ true;

  ಯದಿ (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      ಯದಿ ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಅನ್ಯಥಾ ಯದಿ (dest_sb.st_uid != owner_id)
    ಪ್ರತ್ಯಾಗಮ true;

  ಯದಿ (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      ಯದಿ ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಅನ್ಯಥಾ ಯದಿ (dest_sb.st_gid != group_id)
    ಪ್ರತ್ಯಾಗಮ true;

  /* compare SELinux context if preserving */
  ಯದಿ (selinux_enabled && x->preserve_security_context)
    {
      ವರ್ಣ *file_scontext_raw = ರಿಕ್ತ;
      ಯದಿ (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ಪ್ರತ್ಯಾಗಮ true;

      ವರ್ಣ *to_scontext_raw = ರಿಕ್ತ;
      ಯದಿ (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ಪ್ರತ್ಯಾಗಮ true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      ಯದಿ (!scontext_match)
        ಪ್ರತ್ಯಾಗಮ true;
    }

  /* compare files content */
  ಪೂರ್ಣಾಙ್ಕ src_fd = open (src_name, O_RDONLY | O_ದ್ವಿಮಾನ);
  ಯದಿ (src_fd < 0)
    ಪ್ರತ್ಯಾಗಮ true;

  ಪೂರ್ಣಾಙ್ಕ dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ದ್ವಿಮಾನ);
  ಯದಿ (dest_fd < 0)
    {
      close (src_fd);
      ಪ್ರತ್ಯಾಗಮ true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ಪ್ರತ್ಯಾಗಮ !content_match;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cp_option_init (ಸಂರಚನಾ cp_options *x)
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
  x->set_security_context = ರಿಕ್ತ; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ರಿಕ್ತ;
  x->src_info = ರಿಕ್ತ;
}

ಸ್ಥಿರ ಸಂರಚನಾ selabel_handle *
get_labeling_handle (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ bool initialized;
  ಸ್ಥಿರ ಸಂರಚನಾ selabel_handle *hnd;
  ಯದಿ (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ರಿಕ್ತ, 0);
      ಯದಿ (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ಪ್ರತ್ಯಾಗಮ hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ಸ್ಥಿರ ಶೂನ್ಯ
setdefaultfilecon (ವರ್ಣ ಧ್ರುವ *file)
{
  ಯದಿ (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ಪ್ರತ್ಯಾಗಮ;
    }

  ಸಂರಚನಾ stat st;
  ಯದಿ (lstat (file, &st) != 0)
    ಪ್ರತ್ಯಾಗಮ;

  ಸಂರಚನಾ selabel_handle *hnd = get_labeling_handle ();
  ಯದಿ (!hnd)
    ಪ್ರತ್ಯಾಗಮ;

  ವರ್ಣ *scontext_raw = ರಿಕ್ತ;
  ಯದಿ (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      ಯದಿ (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ಪ್ರತ್ಯಾಗಮ;
    }

  ಯದಿ (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ಸ್ಥಿರ ಶೂನ್ಯ
announce_mkdir (ವರ್ಣ ಧ್ರುವ *dir, ಶೂನ್ಯ *options)
{
  ಸಂರಚನಾ cp_options ಧ್ರುವ *x = options;
  ಯದಿ (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
make_ancestor (ವರ್ಣ ಧ್ರುವ *dir, ವರ್ಣ ಧ್ರುವ *component, ಶೂನ್ಯ *options)
{
  ಸಂರಚನಾ cp_options ಧ್ರುವ *x = options;
  ಯದಿ (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ಪೂರ್ಣಾಙ್ಕ r = mkdir (component, DEFAULT_MODE);
  ಯದಿ (r == 0)
    announce_mkdir (dir, options);
  ಪ್ರತ್ಯಾಗಮ r;
}

/* Process a command-line file name, for the -d option.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
process_dir (ವರ್ಣ *dir, ಸಂರಚನಾ savewd *wd, ಶೂನ್ಯ *options)
{
  ಸಂರಚನಾ cp_options ಧ್ರುವ *x = options;

  ಪೂರ್ಣಾಙ್ಕ ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ಯದಿ (ret == EXIT_SUCCESS && x->set_security_context)
    {
      ಯದಿ (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಗಣನಾ copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ಸ್ಥಿರ ಗಣನಾ copy_status
copy_file (ವರ್ಣ ಧ್ರುವ *from, ವರ್ಣ ಧ್ರುವ *to,
           ಪೂರ್ಣಾಙ್ಕ to_dirfd, ವರ್ಣ ಧ್ರುವ *to_relname, ಧ್ರುವ ಸಂರಚನಾ cp_options *x)
{
  ಯದಿ (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    ಪ್ರತ್ಯಾಗಮ COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ಪ್ರತ್ಯಾಗಮ (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, ರಿಕ್ತ)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ಸ್ಥಿರ bool
change_attributes (ವರ್ಣ ಧ್ರುವ *name, ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ ಧ್ರುವ *relname)
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

  ಯದಿ (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  ಅನ್ಯಥಾ ಯದಿ (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  ಅನ್ಯಥಾ
    ok = true;

  ಯದಿ (use_default_selinux_context)
    setdefaultfilecon (name);

  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ಸ್ಥಿರ bool
change_timestamps (ಸಂರಚನಾ stat ಧ್ರುವ *src_sb, ವರ್ಣ ಧ್ರುವ *dest,
                   ಪೂರ್ಣಾಙ್ಕ dirfd, ವರ್ಣ ಧ್ರುವ *relname)
{
  ಸಂರಚನಾ timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  ಯದಿ (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ಸ್ಥಿರ bool
strip (ವರ್ಣ ಧ್ರುವ *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ರಿಕ್ತ;

  /* Try to use vfork for systems where it matters.  */
  ಯದಿ (posix_spawnattr_init (&attr) == 0)
    {
      ಯದಿ (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      ಅನ್ಯಥಾ
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  ವರ್ಣ *concat_name = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *safe_name = name;
  ಯದಿ (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, ರಿಕ್ತ);
  ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ argv[] = { strip_program, safe_name, ರಿಕ್ತ };

  /* Run 'strip'.  */
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ result = posix_spawnp (&pid, strip_program, ರಿಕ್ತ, attrp,
                             (ವರ್ಣ * ಧ್ರುವ *) argv, environ);

  bool ok = false;
  ಯದಿ (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  ಅನ್ಯಥಾ
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      ಪೂರ್ಣಾಙ್ಕ status;
      ಯದಿ (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      ಅನ್ಯಥಾ ಯದಿ (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      ಅನ್ಯಥಾ
        ok = true;
    }

  ಮುಕ್ತ (concat_name);
  ಯದಿ (attrp)
    posix_spawnattr_destroy (attrp);

  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Initialize the user and group ownership of the files to install. */

ಸ್ಥಿರ ಶೂನ್ಯ
get_ids (ಶೂನ್ಯ)
{
  ಯದಿ (owner_name)
    {
      ಸಂರಚನಾ passwd *pw = getpwnam (owner_name);
      ಯದಿ (pw == ರಿಕ್ತ)
        {
          uintmax_t tmp;
          ಯದಿ (xstrtoumax (owner_name, ರಿಕ್ತ, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      ಅನ್ಯಥಾ
        owner_id = pw->pw_uid;
      endpwent ();
    }
  ಅನ್ಯಥಾ
    owner_id = (uid_t) -1;

  ಯದಿ (group_name)
    {
      ಸಂರಚನಾ group *gr = getgrnam (group_name);
      ಯದಿ (gr == ರಿಕ್ತ)
        {
          uintmax_t tmp;
          ಯದಿ (xstrtoumax (group_name, ರಿಕ್ತ, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      ಅನ್ಯಥಾ
        group_id = gr->gr_gid;
      endgrent ();
    }
  ಅನ್ಯಥಾ
    group_id = (gid_t) -1;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
the existing DIRECTORY, ಯಾವತ್ setting permission modes and owner/group.\n\
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
         and ಯದಿ no change to content, ownership, and permissions,\n\
         ಕುರು not modify the destination at all\n\
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
         strip ಚಿಹ್ನೆ tables\n\
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
         and each created directory, to ಸ್ವಭಾವ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಯದಿ CTX is specified then set the\n\
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

ಸ್ಥಿರ bool
install_file_in_file (ವರ್ಣ ಧ್ರುವ *from, ವರ್ಣ ಧ್ರುವ *to,
                      ಪೂರ್ಣಾಙ್ಕ to_dirfd, ವರ್ಣ ಧ್ರುವ *to_relname,
                      ಧ್ರುವ ಸಂರಚನಾ cp_options *x)
{
  ಸಂರಚನಾ stat from_sb;
  ಯದಿ (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಗಣನಾ copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  ಯದಿ (copy_status == COPY_FAILED)
    ಪ್ರತ್ಯಾಗಮ false;
  ಯದಿ (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      ಯದಿ (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      ಪ್ರತ್ಯಾಗಮ false;
    }
  ಯದಿ (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ಸ್ಥಿರ bool
mkancesdirs_safe_wd (ವರ್ಣ ಧ್ರುವ *from, ವರ್ಣ *to, ಸಂರಚನಾ cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  ಸಂರಚನಾ savewd wd;
  savewd_init (&wd);
  ಯದಿ (! save_working_directory)
    savewd_finish (&wd);

  ಪೂರ್ಣಾಙ್ಕ status = EXIT_SUCCESS;
  ಯದಿ (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  ಯದಿ (save_working_directory)
    {
      ಪೂರ್ಣಾಙ್ಕ restore_result = savewd_restore (&wd, status);
      ಪೂರ್ಣಾಙ್ಕ restore_errno = errno;
      savewd_finish (&wd);
      ಯದಿ (EXIT_SUCCESS < restore_result)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }
  ಪ್ರತ್ಯಾಗಮ status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ಸ್ಥಿರ bool
install_file_in_file_parents (ವರ್ಣ ಧ್ರುವ *from, ವರ್ಣ *to,
                              ಧ್ರುವ ಸಂರಚನಾ cp_options *x)
{
  ಪ್ರತ್ಯಾಗಮ (mkancesdirs_safe_wd (from, to, (ಸಂರಚನಾ cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ಸ್ಥಿರ bool
install_file_in_dir (ವರ್ಣ ಧ್ರುವ *from, ವರ್ಣ ಧ್ರುವ *to_dir,
                     ಧ್ರುವ ಸಂರಚನಾ cp_options *x, bool mkdir_and_install,
                     ಪೂರ್ಣಾಙ್ಕ *target_dirfd)
{
  ವರ್ಣ ಧ್ರುವ *from_base = last_component (from);
  ವರ್ಣ *to_relname;
  ವರ್ಣ *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  ಯದಿ (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (ಸಂರಚನಾ cp_options *) x, true)))
    {
      ಪೂರ್ಣಾಙ್ಕ fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      ಯದಿ (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      ಅನ್ಯಥಾ
        *target_dirfd = fd;
    }

  ಯದಿ (ret)
    {
      ಪೂರ್ಣಾಙ್ಕ to_dirfd = *target_dirfd;
      ಯದಿ (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  ಮುಕ್ತ (to);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ ಧ್ರುವ *specified_mode = ರಿಕ್ತ;
  bool make_backups = false;
  ವರ್ಣ ಧ್ರುವ *backup_suffix = ರಿಕ್ತ;
  ವರ್ಣ *version_control_string = ರಿಕ್ತ;
  bool mkdir_and_install = false;
  ಸಂರಚನಾ cp_options x;
  ವರ್ಣ ಧ್ರುವ *target_directory = ರಿಕ್ತ;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  ವರ್ಣ ಧ್ರುವ *scontext = ರಿಕ್ತ;
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

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'b':
          make_backups = true;
          ಯದಿ (optarg)
            version_control_string = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'c':
          ಭಙ್ಗ;
        ಪಕ್ಷ 'C':
          copy_only_if_needed = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          ಸಙ್ಕೇತ (SIGCHLD, SIG_DFL);
#endif
          ಭಙ್ಗ;
        ಪಕ್ಷ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'd':
          dir_arg = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'D':
          mkdir_and_install = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          x.verbose = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'g':
          group_name = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm':
          specified_mode = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'o':
          owner_name = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'p':
          x.preserve_timestamps = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'S':
          make_backups = true;
          backup_suffix = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 't':
          ಯದಿ (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'T':
          no_target_directory = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ PRESERVE_CONTEXT_OPTION:
          ಯದಿ (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              ಭಙ್ಗ;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'Z':
          ಯದಿ (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              ಯದಿ (optarg)
                scontext = optarg;
              ಅನ್ಯಥಾ
                x.set_security_context = get_labeling_handle ();
            }
          ಅನ್ಯಥಾ ಯದಿ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  ಯದಿ (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  ಯದಿ (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  ಯದಿ (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ಯದಿ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  ಪೂರ್ಣಾಙ್ಕ n_files = argc - optind;
  ವರ್ಣ **file = argv + optind;

  ಯದಿ (n_files <= ! (dir_arg || target_directory))
    {
      ಯದಿ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ಸಂರಚನಾ stat sb;
  ಪೂರ್ಣಾಙ್ಕ target_dirfd = AT_FDCWD;
  ಯದಿ (no_target_directory)
    {
      ಯದಿ (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      ಯದಿ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      ಯದಿ (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  ಅನ್ಯಥಾ ಯದಿ (!dir_arg)
    {
      ವರ್ಣ ಧ್ರುವ *lastfile = file[n_files - 1];
      ಪೂರ್ಣಾಙ್ಕ fd = target_directory_operand (lastfile, &sb);
      ಯದಿ (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      ಅನ್ಯಥಾ ಯದಿ (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  ಯದಿ (specified_mode)
    {
      ಸಂರಚನಾ mode_change *change = mode_compile (specified_mode);
      ಯದಿ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, ರಿಕ್ತ);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      ಮುಕ್ತ (change);
    }

  ಯದಿ (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  ಯದಿ (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  ಪೂರ್ಣಾಙ್ಕ exit_status = EXIT_SUCCESS;
  ಯದಿ (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  ಅನ್ಯಥಾ
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      ಯದಿ (!target_directory)
        {
          ಯದಿ (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      ಅನ್ಯಥಾ
        {
          dest_info_init (&x);
          ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_files; i++)
            ಯದಿ (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
