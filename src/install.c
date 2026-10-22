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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>
#അന്തര്ഭാവ <selinux/label.h>
#അന്തര്ഭാവ <spawn.h>
#അന്തര്ഭാവ <sys/wait.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "backupfile.h"
#അന്തര്ഭാവ "cp-hash.h"
#അന്തര്ഭാവ "copy.h"
#അന്തര്ഭാവ "filenamecat.h"
#അന്തര്ഭാവ "full-read.h"
#അന്തര്ഭാവ "mkancesdirs.h"
#അന്തര്ഭാവ "mkdir-p.h"
#അന്തര്ഭാവ "modechange.h"
#അന്തര്ഭാവ "prog-fprintf.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "savewd.h"
#അന്തര്ഭാവ "selinux.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "targetdir.h"
#അന്തര്ഭാവ "utimens.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

സ്ഥിതം പൂർണ്ണസംഖ്യ selinux_enabled = 0;
സ്ഥിതം bool use_default_selinux_context = true;

#എങ്കിൽ ! HAVE_ENDGRENT
# define endgrent() ((ശൂന്യം) 0)
#endif

#എങ്കിൽ ! HAVE_ENDPWENT
# define endpwent() ((ശൂന്യം) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
സ്ഥിതം അക്ഷരം *owner_name;

/* The user ID corresponding to 'owner_name'. */
സ്ഥിതം uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
സ്ഥിതം അക്ഷരം *group_name;

/* The group ID corresponding to 'group_name'. */
സ്ഥിതം gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
സ്ഥിതം mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
സ്ഥിതം mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
സ്ഥിതം mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
സ്ഥിതം bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
സ്ഥിതം bool strip_files;

/* If true, install a directory instead of a regular file. */
സ്ഥിതം bool dir_arg;

/* Program used to strip binaries, "strip" is default */
സ്ഥിതം അക്ഷരം സ്ഥിരം *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"backup", optional_argument, ശൂന്യ, 'b'},
  {"compare", no_argument, ശൂന്യ, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, ശൂന്യ, DEBUG_OPTION},
  {"directory", no_argument, ശൂന്യ, 'd'},
  {"group", required_argument, ശൂന്യ, 'g'},
  {"mode", required_argument, ശൂന്യ, 'm'},
  {"no-target-directory", no_argument, ശൂന്യ, 'T'},
  {"owner", required_argument, ശൂന്യ, 'o'},
  {"preserve-timestamps", no_argument, ശൂന്യ, 'p'},
  {"preserve-context", no_argument, ശൂന്യ, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, ശൂന്യ, 's'},
  {"strip-program", required_argument, ശൂന്യ, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, ശൂന്യ, 'S'},
  {"target-directory", required_argument, ശൂന്യ, 't'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
സ്ഥിതം bool
have_same_content (പൂർണ്ണസംഖ്യ a_fd, പൂർണ്ണസംഖ്യ b_fd)
{
  എണ്ണൽ { CMP_BLOCK_SIZE = 4096 };
  സ്ഥിതം അക്ഷരം a_buff[CMP_BLOCK_SIZE];
  സ്ഥിതം അക്ഷരം b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  ആകുമ്പോൾ (0 < (size = full_read (a_fd, a_buff, വലുപ്പം a_buff))) {
    എങ്കിൽ (size != full_read (b_fd, b_buff, വലുപ്പം b_buff))
      തിരികെ false;

    എങ്കിൽ (!memeq (a_buff, b_buff, size))
      തിരികെ false;
  }

  തിരികെ size == 0;
}

/* Return true for mode with non-permission bits. */
സ്ഥിതം bool
extra_mode (mode_t നിവേശം)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  തിരികെ !! (നിവേശം & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
സ്ഥിതം bool
need_copy (അക്ഷരം സ്ഥിരം *src_name, അക്ഷരം സ്ഥിരം *dest_name,
           പൂർണ്ണസംഖ്യ dest_dirfd, അക്ഷരം സ്ഥിരം *dest_relname,
           സ്ഥിരം ഘടന cp_options *x)
{
  എങ്കിൽ (extra_mode (mode))
    തിരികെ true;

  /* compare files using stat */
  ഘടന stat src_sb;
  എങ്കിൽ (stat (src_name, &src_sb) != 0)
    തിരികെ true;

  ഘടന stat dest_sb;
  എങ്കിൽ (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    തിരികെ true;

  എങ്കിൽ (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    തിരികെ true;

  എങ്കിൽ (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    തിരികെ true;

  എങ്കിൽ (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      എങ്കിൽ ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        തിരികെ true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (dest_sb.st_uid != owner_id)
    തിരികെ true;

  എങ്കിൽ (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      എങ്കിൽ ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        തിരികെ true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (dest_sb.st_gid != group_id)
    തിരികെ true;

  /* compare SELinux context if preserving */
  എങ്കിൽ (selinux_enabled && x->preserve_security_context)
    {
      അക്ഷരം *file_scontext_raw = ശൂന്യ;
      എങ്കിൽ (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        തിരികെ true;

      അക്ഷരം *to_scontext_raw = ശൂന്യ;
      എങ്കിൽ (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          തിരികെ true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      എങ്കിൽ (!scontext_match)
        തിരികെ true;
    }

  /* compare files content */
  പൂർണ്ണസംഖ്യ src_fd = open (src_name, O_RDONLY | O_ദ്വിമാന);
  എങ്കിൽ (src_fd < 0)
    തിരികെ true;

  പൂർണ്ണസംഖ്യ dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ദ്വിമാന);
  എങ്കിൽ (dest_fd < 0)
    {
      close (src_fd);
      തിരികെ true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  തിരികെ !content_match;
}

സ്ഥിതം ശൂന്യം
cp_option_init (ഘടന cp_options *x)
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
  x->set_security_context = ശൂന്യ; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ശൂന്യ;
  x->src_info = ശൂന്യ;
}

സ്ഥിതം ഘടന selabel_handle *
get_labeling_handle (ശൂന്യം)
{
  സ്ഥിതം bool initialized;
  സ്ഥിതം ഘടന selabel_handle *hnd;
  എങ്കിൽ (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ശൂന്യ, 0);
      എങ്കിൽ (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  തിരികെ hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
സ്ഥിതം ശൂന്യം
setdefaultfilecon (അക്ഷരം സ്ഥിരം *file)
{
  എങ്കിൽ (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      തിരികെ;
    }

  ഘടന stat st;
  എങ്കിൽ (lstat (file, &st) != 0)
    തിരികെ;

  ഘടന selabel_handle *hnd = get_labeling_handle ();
  എങ്കിൽ (!hnd)
    തിരികെ;

  അക്ഷരം *scontext_raw = ശൂന്യ;
  എങ്കിൽ (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      എങ്കിൽ (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      തിരികെ;
    }

  എങ്കിൽ (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
സ്ഥിതം ശൂന്യം
announce_mkdir (അക്ഷരം സ്ഥിരം *dir, ശൂന്യം *options)
{
  ഘടന cp_options സ്ഥിരം *x = options;
  എങ്കിൽ (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
make_ancestor (അക്ഷരം സ്ഥിരം *dir, അക്ഷരം സ്ഥിരം *component, ശൂന്യം *options)
{
  ഘടന cp_options സ്ഥിരം *x = options;
  എങ്കിൽ (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  പൂർണ്ണസംഖ്യ r = mkdir (component, DEFAULT_MODE);
  എങ്കിൽ (r == 0)
    announce_mkdir (dir, options);
  തിരികെ r;
}

/* Process a command-line file name, for the -d option.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
process_dir (അക്ഷരം *dir, ഘടന savewd *wd, ശൂന്യം *options)
{
  ഘടന cp_options സ്ഥിരം *x = options;

  പൂർണ്ണസംഖ്യ ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  എങ്കിൽ (ret == EXIT_SUCCESS && x->set_security_context)
    {
      എങ്കിൽ (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  തിരികെ ret;
}

എണ്ണൽ copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

സ്ഥിതം എണ്ണൽ copy_status
copy_file (അക്ഷരം സ്ഥിരം *from, അക്ഷരം സ്ഥിരം *to,
           പൂർണ്ണസംഖ്യ to_dirfd, അക്ഷരം സ്ഥിരം *to_relname, സ്ഥിരം ഘടന cp_options *x)
{
  എങ്കിൽ (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    തിരികെ COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  തിരികെ (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, ശൂന്യ)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

സ്ഥിതം bool
change_attributes (അക്ഷരം സ്ഥിരം *name, പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം സ്ഥിരം *relname)
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

  എങ്കിൽ (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  അല്ലെങ്കിൽ എങ്കിൽ (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  അല്ലെങ്കിൽ
    ok = true;

  എങ്കിൽ (use_default_selinux_context)
    setdefaultfilecon (name);

  തിരികെ ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

സ്ഥിതം bool
change_timestamps (ഘടന stat സ്ഥിരം *src_sb, അക്ഷരം സ്ഥിരം *dest,
                   പൂർണ്ണസംഖ്യ dirfd, അക്ഷരം സ്ഥിരം *relname)
{
  ഘടന timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  എങ്കിൽ (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      തിരികെ false;
    }
  തിരികെ true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

സ്ഥിതം bool
strip (അക്ഷരം സ്ഥിരം *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ശൂന്യ;

  /* Try to use vfork for systems where it matters.  */
  എങ്കിൽ (posix_spawnattr_init (&attr) == 0)
    {
      എങ്കിൽ (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      അല്ലെങ്കിൽ
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  അക്ഷരം *concat_name = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *safe_name = name;
  എങ്കിൽ (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, ശൂന്യ);
  അക്ഷരം സ്ഥിരം *സ്ഥിരം argv[] = { strip_program, safe_name, ശൂന്യ };

  /* Run 'strip'.  */
  pid_t pid;
  പൂർണ്ണസംഖ്യ result = posix_spawnp (&pid, strip_program, ശൂന്യ, attrp,
                             (അക്ഷരം * സ്ഥിരം *) argv, environ);

  bool ok = false;
  എങ്കിൽ (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  അല്ലെങ്കിൽ
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      പൂർണ്ണസംഖ്യ status;
      എങ്കിൽ (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      അല്ലെങ്കിൽ എങ്കിൽ (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      അല്ലെങ്കിൽ
        ok = true;
    }

  സ്വതന്ത്രം (concat_name);
  എങ്കിൽ (attrp)
    posix_spawnattr_destroy (attrp);

  തിരികെ ok;
}

/* Initialize the user and group ownership of the files to install. */

സ്ഥിതം ശൂന്യം
get_ids (ശൂന്യം)
{
  എങ്കിൽ (owner_name)
    {
      ഘടന passwd *pw = getpwnam (owner_name);
      എങ്കിൽ (pw == ശൂന്യ)
        {
          uintmax_t tmp;
          എങ്കിൽ (xstrtoumax (owner_name, ശൂന്യ, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      അല്ലെങ്കിൽ
        owner_id = pw->pw_uid;
      endpwent ();
    }
  അല്ലെങ്കിൽ
    owner_id = (uid_t) -1;

  എങ്കിൽ (group_name)
    {
      ഘടന group *gr = getgrnam (group_name);
      എങ്കിൽ (gr == ശൂന്യ)
        {
          uintmax_t tmp;
          എങ്കിൽ (xstrtoumax (group_name, ശൂന്യ, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      അല്ലെങ്കിൽ
        group_id = gr->gr_gid;
      endgrent ();
    }
  അല്ലെങ്കിൽ
    group_id = (gid_t) -1;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
the existing DIRECTORY, ആകുമ്പോൾ setting permission modes and owner/group.\n\
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
         and എങ്കിൽ no change to content, ownership, and permissions,\n\
         ചെയ്യുക not modify the destination at all\n\
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
         strip ചിഹ്നം tables\n\
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
         and each created directory, to സ്ഥിരസ്ഥിതി type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or എങ്കിൽ CTX is specified then set the\n\
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

സ്ഥിതം bool
install_file_in_file (അക്ഷരം സ്ഥിരം *from, അക്ഷരം സ്ഥിരം *to,
                      പൂർണ്ണസംഖ്യ to_dirfd, അക്ഷരം സ്ഥിരം *to_relname,
                      സ്ഥിരം ഘടന cp_options *x)
{
  ഘടന stat from_sb;
  എങ്കിൽ (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      തിരികെ false;
    }
  എണ്ണൽ copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  എങ്കിൽ (copy_status == COPY_FAILED)
    തിരികെ false;
  എങ്കിൽ (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      എങ്കിൽ (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      തിരികെ false;
    }
  എങ്കിൽ (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    തിരികെ false;

  തിരികെ copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

സ്ഥിതം bool
mkancesdirs_safe_wd (അക്ഷരം സ്ഥിരം *from, അക്ഷരം *to, ഘടന cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  ഘടന savewd wd;
  savewd_init (&wd);
  എങ്കിൽ (! save_working_directory)
    savewd_finish (&wd);

  പൂർണ്ണസംഖ്യ status = EXIT_SUCCESS;
  എങ്കിൽ (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  എങ്കിൽ (save_working_directory)
    {
      പൂർണ്ണസംഖ്യ restore_result = savewd_restore (&wd, status);
      പൂർണ്ണസംഖ്യ restore_errno = errno;
      savewd_finish (&wd);
      എങ്കിൽ (EXIT_SUCCESS < restore_result)
        തിരികെ false;
      എങ്കിൽ (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          തിരികെ false;
        }
    }
  തിരികെ status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

സ്ഥിതം bool
install_file_in_file_parents (അക്ഷരം സ്ഥിരം *from, അക്ഷരം *to,
                              സ്ഥിരം ഘടന cp_options *x)
{
  തിരികെ (mkancesdirs_safe_wd (from, to, (ഘടന cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

സ്ഥിതം bool
install_file_in_dir (അക്ഷരം സ്ഥിരം *from, അക്ഷരം സ്ഥിരം *to_dir,
                     സ്ഥിരം ഘടന cp_options *x, bool mkdir_and_install,
                     പൂർണ്ണസംഖ്യ *target_dirfd)
{
  അക്ഷരം സ്ഥിരം *from_base = last_component (from);
  അക്ഷരം *to_relname;
  അക്ഷരം *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  എങ്കിൽ (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (ഘടന cp_options *) x, true)))
    {
      പൂർണ്ണസംഖ്യ fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      എങ്കിൽ (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      അല്ലെങ്കിൽ
        *target_dirfd = fd;
    }

  എങ്കിൽ (ret)
    {
      പൂർണ്ണസംഖ്യ to_dirfd = *target_dirfd;
      എങ്കിൽ (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  സ്വതന്ത്രം (to);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം സ്ഥിരം *specified_mode = ശൂന്യ;
  bool make_backups = false;
  അക്ഷരം സ്ഥിരം *backup_suffix = ശൂന്യ;
  അക്ഷരം *version_control_string = ശൂന്യ;
  bool mkdir_and_install = false;
  ഘടന cp_options x;
  അക്ഷരം സ്ഥിരം *target_directory = ശൂന്യ;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  അക്ഷരം സ്ഥിരം *scontext = ശൂന്യ;
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

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'b':
          make_backups = true;
          എങ്കിൽ (optarg)
            version_control_string = optarg;
          നിർത്തുക;
        അവസ്ഥ 'c':
          നിർത്തുക;
        അവസ്ഥ 'C':
          copy_only_if_needed = true;
          നിർത്തുക;
        അവസ്ഥ 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          സൂചന (SIGCHLD, SIG_DFL);
#endif
          നിർത്തുക;
        അവസ്ഥ DEBUG_OPTION:
          x.debug = x.verbose = true;
          നിർത്തുക;
        അവസ്ഥ STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          നിർത്തുക;
        അവസ്ഥ 'd':
          dir_arg = true;
          നിർത്തുക;
        അവസ്ഥ 'D':
          mkdir_and_install = true;
          നിർത്തുക;
        അവസ്ഥ 'v':
          x.verbose = true;
          നിർത്തുക;
        അവസ്ഥ 'g':
          group_name = optarg;
          നിർത്തുക;
        അവസ്ഥ 'm':
          specified_mode = optarg;
          നിർത്തുക;
        അവസ്ഥ 'o':
          owner_name = optarg;
          നിർത്തുക;
        അവസ്ഥ 'p':
          x.preserve_timestamps = true;
          നിർത്തുക;
        അവസ്ഥ 'S':
          make_backups = true;
          backup_suffix = optarg;
          നിർത്തുക;
        അവസ്ഥ 't':
          എങ്കിൽ (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          നിർത്തുക;
        അവസ്ഥ 'T':
          no_target_directory = true;
          നിർത്തുക;

        അവസ്ഥ PRESERVE_CONTEXT_OPTION:
          എങ്കിൽ (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              നിർത്തുക;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          നിർത്തുക;
        അവസ്ഥ 'Z':
          എങ്കിൽ (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              എങ്കിൽ (optarg)
                scontext = optarg;
              അല്ലെങ്കിൽ
                x.set_security_context = get_labeling_handle ();
            }
          അല്ലെങ്കിൽ എങ്കിൽ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  എങ്കിൽ (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  എങ്കിൽ (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  എങ്കിൽ (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  എങ്കിൽ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  പൂർണ്ണസംഖ്യ n_files = argc - optind;
  അക്ഷരം **file = argv + optind;

  എങ്കിൽ (n_files <= ! (dir_arg || target_directory))
    {
      എങ്കിൽ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ഘടന stat sb;
  പൂർണ്ണസംഖ്യ target_dirfd = AT_FDCWD;
  എങ്കിൽ (no_target_directory)
    {
      എങ്കിൽ (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      എങ്കിൽ (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      എങ്കിൽ (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  അല്ലെങ്കിൽ എങ്കിൽ (!dir_arg)
    {
      അക്ഷരം സ്ഥിരം *lastfile = file[n_files - 1];
      പൂർണ്ണസംഖ്യ fd = target_directory_operand (lastfile, &sb);
      എങ്കിൽ (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  എങ്കിൽ (specified_mode)
    {
      ഘടന mode_change *change = mode_compile (specified_mode);
      എങ്കിൽ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, ശൂന്യ);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      സ്വതന്ത്രം (change);
    }

  എങ്കിൽ (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  എങ്കിൽ (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  പൂർണ്ണസംഖ്യ exit_status = EXIT_SUCCESS;
  എങ്കിൽ (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  അല്ലെങ്കിൽ
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      എങ്കിൽ (!target_directory)
        {
          എങ്കിൽ (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      അല്ലെങ്കിൽ
        {
          dest_info_init (&x);
          വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
            എങ്കിൽ (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
