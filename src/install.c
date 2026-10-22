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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <spawn.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "backupfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cp-hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filenamecat.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "full-read.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mkancesdirs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mkdir-p.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "modechange.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prog-fprintf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "savewd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "selinux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "targetdir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utimens.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "install"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਸਥਿਰ ਪੂਰਨ selinux_enabled = 0;
ਸਥਿਰ bool use_default_selinux_context = true;

#ਜੇ ! HAVE_ENDGRENT
# define endgrent() ((ਖਾਲੀ) 0)
#ਜੇ_ਅੰਤ

#ਜੇ ! HAVE_ENDPWENT
# define endpwent() ((ਖਾਲੀ) 0)
#ਜੇ_ਅੰਤ

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ਸਥਿਰ ਅੱਖਰ *owner_name;

/* The user ID corresponding to 'owner_name'. */
ਸਥਿਰ uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ਸਥਿਰ ਅੱਖਰ *ਸਮੂਹ_name;

/* The group ID corresponding to 'group_name'. */
ਸਥਿਰ gid_t ਸਮੂਹ_id;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_ਬਹੁਲਕ (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ਸਥਿਰ mode_t ਬਹੁਲਕ_2 = DEFAULT_ਬਹੁਲਕ;

/* Similar, but for directories.  */
ਸਥਿਰ mode_t dir_ਬਹੁਲਕ = DEFAULT_ਬਹੁਲਕ;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ਸਥਿਰ mode_t dir_ਬਹੁਲਕ_ਬਿੱਟ = CHMOD_ਬਹੁਲਕ_ਬਿੱਟ;

/* Compare files before installing (-C) */
ਸਥਿਰ bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ਸਥਿਰ bool strip_files;

/* If true, install a directory instead of a regular file. */
ਸਥਿਰ bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *strip_ਪ੍ਰੋਗਰਾਮ = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_ਪ੍ਰੋਗਰਾਮ_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"backup", optional_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"compare", no_ਦਲੀਲ, ਨਿਰਾ, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_OPTION},
  {"directory", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"group", required_ਦਲੀਲ, ਨਿਰਾ, 'g'},
  {"mode", required_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"no-target-directory", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"owner", required_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"preserve-timestamps", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"preserve-context", no_ਦਲੀਲ, ਨਿਰਾ, PRESERVE_CONTEXT_OPTION},
  {"strip", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"strip-program", required_ਦਲੀਲ, ਨਿਰਾ, STRIP_ਪ੍ਰੋਗਰਾਮ_OPTION},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"target-directory", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ਸਥਿਰ bool
have_same_content (ਪੂਰਨ a_fd, ਪੂਰਨ b_fd)
{
  ਗਿਣਤੀ { CMP_ਖੰਡ_ਮਾਪ = 4096 };
  ਸਥਿਰ ਅੱਖਰ a_buff[CMP_ਖੰਡ_ਮਾਪ];
  ਸਥਿਰ ਅੱਖਰ b_buff[CMP_ਖੰਡ_ਮਾਪ];

  idx_t ਮਾਪ;
  ਜਦੋਂ (0 < (ਮਾਪ = ਭਰਿਆ_read (a_fd, a_buff, ਆਕਾਰ a_buff))) {
    ਜੇ (ਮਾਪ != ਭਰਿਆ_read (b_fd, b_buff, ਆਕਾਰ b_buff))
      ਵਾਪਸ false;

    ਜੇ (!memeq (a_buff, b_buff, ਮਾਪ))
      ਵਾਪਸ false;
  }

  ਵਾਪਸ ਮਾਪ == 0;
}

/* Return true for mode with non-permission bits. */
ਸਥਿਰ bool
extra_ਬਹੁਲਕ (mode_t ਨਿਵੇਸ਼)
{
  mode_t ਨਕਾਬ = S_IRWXUGO | S_IFMT;
  ਵਾਪਸ !! (ਨਿਵੇਸ਼ & ~ ਨਕਾਬ);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ਸਥਿਰ bool
need_copy (ਅੱਖਰ ਅਚਲ *src_name, ਅੱਖਰ ਅਚਲ *dest_name,
           ਪੂਰਨ dest_dirfd, ਅੱਖਰ ਅਚਲ *dest_relname,
           ਅਚਲ ਬਣਤਰ cp_options *x)
{
  ਜੇ (extra_ਬਹੁਲਕ (ਬਹੁਲਕ_2))
    ਵਾਪਸ true;

  /* compare files using stat */
  ਬਣਤਰ stat src_sb;
  ਜੇ (stat (src_name, &src_sb) != 0)
    ਵਾਪਸ true;

  ਬਣਤਰ stat dest_sb;
  ਜੇ (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ਵਾਪਸ true;

  ਜੇ (!S_ISREG (src_sb.st_ਬਹੁਲਕ) || !S_ISREG (dest_sb.st_ਬਹੁਲਕ)
      || extra_ਬਹੁਲਕ (src_sb.st_ਬਹੁਲਕ) || extra_ਬਹੁਲਕ (dest_sb.st_ਬਹੁਲਕ))
    ਵਾਪਸ true;

  ਜੇ (src_sb.st_ਮਾਪ != dest_sb.st_ਮਾਪ
      || (dest_sb.st_ਬਹੁਲਕ & CHMOD_ਬਹੁਲਕ_ਬਿੱਟ) != ਬਹੁਲਕ_2)
    ਵਾਪਸ true;

  ਜੇ (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      ਜੇ ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ਵਾਪਸ true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (dest_sb.st_uid != owner_id)
    ਵਾਪਸ true;

  ਜੇ (ਸਮੂਹ_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      ਜੇ ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ਵਾਪਸ true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (dest_sb.st_gid != ਸਮੂਹ_id)
    ਵਾਪਸ true;

  /* compare SELinux context if preserving */
  ਜੇ (selinux_enabled && x->preserve_security_context)
    {
      ਅੱਖਰ *file_scontext_raw = ਨਿਰਾ;
      ਜੇ (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ਵਾਪਸ true;

      ਅੱਖਰ *ਮੰਜ਼ਿਲ_scontext_raw = ਨਿਰਾ;
      ਜੇ (getfilecon_raw (dest_name, &ਮੰਜ਼ਿਲ_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ਵਾਪਸ true;
        }

      bool scontext_ਮੇਲ = streq (file_scontext_raw, ਮੰਜ਼ਿਲ_scontext_raw);

      freecon (file_scontext_raw);
      freecon (ਮੰਜ਼ਿਲ_scontext_raw);
      ਜੇ (!scontext_ਮੇਲ)
        ਵਾਪਸ true;
    }

  /* compare files content */
  ਪੂਰਨ src_fd = open (src_name, O_RDONLY | O_ਦੁਆਧਾਰੀ);
  ਜੇ (src_fd < 0)
    ਵਾਪਸ true;

  ਪੂਰਨ dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ਦੁਆਧਾਰੀ);
  ਜੇ (dest_fd < 0)
    {
      close (src_fd);
      ਵਾਪਸ true;
    }

  bool content_ਮੇਲ = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ਵਾਪਸ !content_ਮੇਲ;
}

ਸਥਿਰ ਖਾਲੀ
cp_option_init (ਬਣਤਰ cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_ਬਹੁਲਕ = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_ਬਹੁਲਕ = false;
  x->install_ਬਹੁਲਕ = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_ਬਹੁਲਕ = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_ਬਹੁਲਕ = false;
  x->reduce_diagnostics=false;
  x->ਅੰਕੜੇ_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_ਬਹੁਲਕ = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->ਸੈੱਟ_ਬਹੁਲਕ = true;
  x->ਬਹੁਲਕ_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->ਸੈੱਟ_security_context = ਨਿਰਾ; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ਨਿਰਾ;
  x->src_info = ਨਿਰਾ;
}

ਸਥਿਰ ਬਣਤਰ selabel_handle *
get_labeling_handle (ਖਾਲੀ)
{
  ਸਥਿਰ bool initialized;
  ਸਥਿਰ ਬਣਤਰ selabel_handle *hnd;
  ਜੇ (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ਨਿਰਾ, 0);
      ਜੇ (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ਵਾਪਸ hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ਸਥਿਰ ਖਾਲੀ
setdefaultfilecon (ਅੱਖਰ ਅਚਲ *file)
{
  ਜੇ (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ਵਾਪਸ;
    }

  ਬਣਤਰ stat st;
  ਜੇ (lstat (file, &st) != 0)
    ਵਾਪਸ;

  ਬਣਤਰ selabel_handle *hnd = get_labeling_handle ();
  ਜੇ (!hnd)
    ਵਾਪਸ;

  ਅੱਖਰ *scontext_raw = ਨਿਰਾ;
  ਜੇ (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_ਬਹੁਲਕ) != 0)
    {
      ਜੇ (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ਵਾਪਸ;
    }

  ਜੇ (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ਸਥਿਰ ਖਾਲੀ
announce_mkdir (ਅੱਖਰ ਅਚਲ *dir, ਖਾਲੀ *options)
{
  ਬਣਤਰ cp_options ਅਚਲ *x = options;
  ਜੇ (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ਸਥਿਰ ਪੂਰਨ
make_ancestor (ਅੱਖਰ ਅਚਲ *dir, ਅੱਖਰ ਅਚਲ *component, ਖਾਲੀ *options)
{
  ਬਣਤਰ cp_options ਅਚਲ *x = options;
  ਜੇ (x->ਸੈੱਟ_security_context
      && defaultcon (x->ਸੈੱਟ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ਪੂਰਨ r = mkdir (component, DEFAULT_ਬਹੁਲਕ);
  ਜੇ (r == 0)
    announce_mkdir (dir, options);
  ਵਾਪਸ r;
}

/* Process a command-line file name, for the -d option.  */
ਸਥਿਰ ਪੂਰਨ
process_dir (ਅੱਖਰ *dir, ਬਣਤਰ savewd *wd, ਖਾਲੀ *options)
{
  ਬਣਤਰ cp_options ਅਚਲ *x = options;

  ਪੂਰਨ ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_ਬਹੁਲਕ, announce_mkdir,
                               dir_ਬਹੁਲਕ_ਬਿੱਟ, owner_id, ਸਮੂਹ_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ਜੇ (ret == EXIT_SUCCESS && x->ਸੈੱਟ_security_context)
    {
      ਜੇ (! restorecon (x->ਸੈੱਟ_security_context, ਆਖਰੀ_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ਵਾਪਸ ret;
}

ਗਿਣਤੀ copy_ਸਥਿਤੀ { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ਸਥਿਰ ਗਿਣਤੀ copy_ਸਥਿਤੀ
copy_file (ਅੱਖਰ ਅਚਲ *from, ਅੱਖਰ ਅਚਲ *ਮੰਜ਼ਿਲ,
           ਪੂਰਨ ਮੰਜ਼ਿਲ_dirfd, ਅੱਖਰ ਅਚਲ *ਮੰਜ਼ਿਲ_relname, ਅਚਲ ਬਣਤਰ cp_options *x)
{
  ਜੇ (copy_only_if_needed && !need_copy (from, ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname, x))
    ਵਾਪਸ COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ਵਾਪਸ (copy (from, ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname, 0, x, &copy_into_self, ਨਿਰਾ)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ਸਥਿਰ bool
change_ਗੁਣ (ਅੱਖਰ ਅਚਲ *name, ਪੂਰਨ dirfd, ਅੱਖਰ ਅਚਲ *relname)
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

  ਜੇ (! (owner_id == (uid_t) -1 && ਸਮੂਹ_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, ਸਮੂਹ_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  ਨਹੀਂਤਾਂ ਜੇ (chmodat (dirfd, relname, ਬਹੁਲਕ_2) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  ਨਹੀਂਤਾਂ
    ok = true;

  ਜੇ (use_default_selinux_context)
    setdefaultfilecon (name);

  ਵਾਪਸ ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ਸਥਿਰ bool
change_timestamps (ਬਣਤਰ stat ਅਚਲ *src_sb, ਅੱਖਰ ਅਚਲ *dest,
                   ਪੂਰਨ dirfd, ਅੱਖਰ ਅਚਲ *relname)
{
  ਬਣਤਰ timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  ਜੇ (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ਵਾਪਸ false;
    }
  ਵਾਪਸ true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ਸਥਿਰ bool
strip (ਅੱਖਰ ਅਚਲ *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ਨਿਰਾ;

  /* Try to use vfork for systems where it matters.  */
  ਜੇ (posix_spawnattr_init (&attr) == 0)
    {
      ਜੇ (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      ਨਹੀਂਤਾਂ
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  ਅੱਖਰ *concat_name = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *ਸੁਰੱਖਿਅਤ_name = name;
  ਜੇ (name && *name == '-')
    ਸੁਰੱਖਿਅਤ_name = concat_name = file_name_concat (".", name, ਨਿਰਾ);
  ਅੱਖਰ ਅਚਲ *ਅਚਲ argv[] = { strip_ਪ੍ਰੋਗਰਾਮ, ਸੁਰੱਖਿਅਤ_name, ਨਿਰਾ };

  /* Run 'strip'.  */
  pid_t pid;
  ਪੂਰਨ ਨਤੀਜਾ = posix_spawnp (&pid, strip_ਪ੍ਰੋਗਰਾਮ, ਨਿਰਾ, attrp,
                             (ਅੱਖਰ * ਅਚਲ *) argv, environ);

  bool ok = false;
  ਜੇ (ਨਤੀਜਾ != 0)
    {
      error (0, ਨਤੀਜਾ,
             streq (strip_ਪ੍ਰੋਗਰਾਮ, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_ਪ੍ਰੋਗਰਾਮ));
    }
  ਨਹੀਂਤਾਂ
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      ਪੂਰਨ ਸਥਿਤੀ;
      ਜੇ (waitpid (pid, &ਸਥਿਤੀ, 0) < 0)
        error (0, errno, _("waiting for strip"));
      ਨਹੀਂਤਾਂ ਜੇ (! WIFEXITED (ਸਥਿਤੀ) || WEXITSTATUS (ਸਥਿਤੀ))
        error (0, 0, _("strip process terminated abnormally"));
      ਨਹੀਂਤਾਂ
        ok = true;
    }

  ਮੁਕਤ (concat_name);
  ਜੇ (attrp)
    posix_spawnattr_destroy (attrp);

  ਵਾਪਸ ok;
}

/* Initialize the user and group ownership of the files to install. */

ਸਥਿਰ ਖਾਲੀ
get_ids (ਖਾਲੀ)
{
  ਜੇ (owner_name)
    {
      ਬਣਤਰ passwd *pw = getpwnam (owner_name);
      ਜੇ (pw == ਨਿਰਾ)
        {
          uintmax_t tmp;
          ਜੇ (xstrtoumax (owner_name, ਨਿਰਾ, 0, &tmp, "") != LONGINT_OK
              || ckd_ਜੋੜੋ (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      ਨਹੀਂਤਾਂ
        owner_id = pw->pw_uid;
      endpwent ();
    }
  ਨਹੀਂਤਾਂ
    owner_id = (uid_t) -1;

  ਜੇ (ਸਮੂਹ_name)
    {
      ਬਣਤਰ ਸਮੂਹ *gr = getgrnam (ਸਮੂਹ_name);
      ਜੇ (gr == ਨਿਰਾ)
        {
          uintmax_t tmp;
          ਜੇ (xstrtoumax (ਸਮੂਹ_name, ਨਿਰਾ, 0, &tmp, "") != LONGINT_OK
              || ckd_ਜੋੜੋ (&ਸਮੂਹ_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (ਸਮੂਹ_name));
        }
      ਨਹੀਂਤਾਂ
        ਸਮੂਹ_id = gr->gr_gid;
      endgrent ();
    }
  ਨਹੀਂਤਾਂ
    ਸਮੂਹ_id = (gid_t) -1;
}

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
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
\n\
This install ਪ੍ਰੋਗਰਾਮ copies files (often just compiled) into destination\n\
locations you ਚੁਣੋ.  If you want ਮੰਜ਼ਿਲ download and install a ready-ਮੰਜ਼ਿਲ-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
ਵਿੱਚ_2 the ਪਹਿਲਾ three forms, copy ਸਰੋਤ ਮੰਜ਼ਿਲ DEST or ਗੁਣਜ ਸਰੋਤ(s) ਮੰਜ਼ਿਲ\n\
the existing DIRECTORY, ਜਦੋਂ setting permission modes and owner/ਸਮੂਹ.\n\
ਵਿੱਚ_2 the 4th form, create all components ਦਾ the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup ਦਾ each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an ਦਲੀਲ\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content ਦਾ ਸਰੋਤ_2 and destination files,\n\
         and ਜੇ no change ਮੰਜ਼ਿਲ content, ownership, and permissions,\n\
         ਕਰੋ not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory ਨਾਂ;\n\
         create all components ਦਾ the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components ਦਾ DEST except the ਆਖਰੀ,\n\
         or all components ਦਾ --ਨਿਸ਼ਾਨਾ-directory,\n\
         then copy ਸਰੋਤ ਮੰਜ਼ਿਲ DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  ਸਿੱਟਾ_ਦਿੰਦਾ_2 -v\n\
"));
      oputs (_("\
  -g, --ਸਮੂਹ=ਸਮੂਹ_2\n\
         ਸੈੱਟ_2 ਸਮੂਹ ownership, instead ਦਾ process' current ਸਮੂਹ\n\
"));
      oputs (_("\
  -m, --ਬਹੁਲਕ_2=ਬਹੁਲਕ\n\
         ਸੈੱਟ_2 permission ਬਹੁਲਕ_2 (as ਵਿੱਚ chmod), instead ਦਾ rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         ਸੈੱਟ_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times ਦਾ ਸਰੋਤ files\n\
         ਮੰਜ਼ਿਲ corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip ਚਿੰਨ੍ਹ tables\n\
"));
      oputs (_("\
      --strip-ਪ੍ਰੋਗਰਾਮ=ਪ੍ਰੋਗਰਾਮ_2\n\
         ਪ੍ਰੋਗਰਾਮ ਵਰਤਿਆ ਮੰਜ਼ਿਲ strip binaries\n\
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
  -v, --verbose\n\
         print the name ਦਾ each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         ਸੈੱਟ_2 SELinux security context ਦਾ destination file\n\
         and each created directory, ਮੰਜ਼ਿਲ ਮੂਲ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ਜੇ CTX is specified then ਸੈੱਟ_2 the\n\
         SELinux or SMACK security context ਮੰਜ਼ਿਲ CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

ਸਥਿਰ bool
install_file_ਵਿੱਚ_file (ਅੱਖਰ ਅਚਲ *from, ਅੱਖਰ ਅਚਲ *ਮੰਜ਼ਿਲ,
                      ਪੂਰਨ ਮੰਜ਼ਿਲ_dirfd, ਅੱਖਰ ਅਚਲ *ਮੰਜ਼ਿਲ_relname,
                      ਅਚਲ ਬਣਤਰ cp_options *x)
{
  ਬਣਤਰ stat from_sb;
  ਜੇ (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ਵਾਪਸ false;
    }
  ਗਿਣਤੀ copy_ਸਥਿਤੀ copy_ਸਥਿਤੀ = copy_file (from, ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname, x);
  ਜੇ (copy_ਸਥਿਤੀ == COPY_FAILED)
    ਵਾਪਸ false;
  ਜੇ (copy_ਸਥਿਤੀ == COPY_OK && strip_files && ! strip (ਮੰਜ਼ਿਲ))
    {
      ਜੇ (unlinkat (ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (ਮੰਜ਼ਿਲ));
      ਵਾਪਸ false;
    }
  ਜੇ (x->preserve_timestamps && (copy_ਸਥਿਤੀ == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_ਬਹੁਲਕ))
      && ! change_timestamps (&from_sb, ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname))
    ਵਾਪਸ false;

  ਵਾਪਸ copy_ਸਥਿਤੀ == COPY_SKIPPED
         || change_ਗੁਣ (ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ਸਥਿਰ bool
mkancesdirs_ਸੁਰੱਖਿਅਤ_wd (ਅੱਖਰ ਅਚਲ *from, ਅੱਖਰ *ਮੰਜ਼ਿਲ, ਬਣਤਰ cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (ਮੰਜ਼ਿਲ));

  ਬਣਤਰ savewd wd;
  savewd_init (&wd);
  ਜੇ (! save_working_directory)
    savewd_ਅੰਤ (&wd);

  ਪੂਰਨ ਸਥਿਤੀ = EXIT_SUCCESS;
  ਜੇ (mkancesdirs (ਮੰਜ਼ਿਲ, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (ਮੰਜ਼ਿਲ));
      ਸਥਿਤੀ = EXIT_FAILURE;
    }

  ਜੇ (save_working_directory)
    {
      ਪੂਰਨ restore_ਨਤੀਜਾ = savewd_restore (&wd, ਸਥਿਤੀ);
      ਪੂਰਨ restore_errno = errno;
      savewd_ਅੰਤ (&wd);
      ਜੇ (EXIT_SUCCESS < restore_ਨਤੀਜਾ)
        ਵਾਪਸ false;
      ਜੇ (restore_ਨਤੀਜਾ < 0 && ਸਥਿਤੀ == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (ਮੰਜ਼ਿਲ));
          ਵਾਪਸ false;
        }
    }
  ਵਾਪਸ ਸਥਿਤੀ == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ਸਥਿਰ bool
install_file_ਵਿੱਚ_file_parents (ਅੱਖਰ ਅਚਲ *from, ਅੱਖਰ *ਮੰਜ਼ਿਲ,
                              ਅਚਲ ਬਣਤਰ cp_options *x)
{
  ਵਾਪਸ (mkancesdirs_ਸੁਰੱਖਿਅਤ_wd (from, ਮੰਜ਼ਿਲ, (ਬਣਤਰ cp_options *)x, false)
          && install_file_ਵਿੱਚ_file (from, ਮੰਜ਼ਿਲ, AT_FDCWD, ਮੰਜ਼ਿਲ, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ਸਥਿਰ bool
install_file_ਵਿੱਚ_dir (ਅੱਖਰ ਅਚਲ *from, ਅੱਖਰ ਅਚਲ *ਮੰਜ਼ਿਲ_dir,
                     ਅਚਲ ਬਣਤਰ cp_options *x, bool mkdir_and_install,
                     ਪੂਰਨ *ਨਿਸ਼ਾਨਾ_dirfd)
{
  ਅੱਖਰ ਅਚਲ *from_ਅਧਾਰ = ਆਖਰੀ_component (from);
  ਅੱਖਰ *ਮੰਜ਼ਿਲ_relname;
  ਅੱਖਰ *ਮੰਜ਼ਿਲ = file_name_concat (ਮੰਜ਼ਿਲ_dir, from_ਅਧਾਰ, &ਮੰਜ਼ਿਲ_relname);
  bool ret = true;

  ਜੇ (!ਨਿਸ਼ਾਨਾ_dirfd_valid (*ਨਿਸ਼ਾਨਾ_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_ਸੁਰੱਖਿਅਤ_wd (from, ਮੰਜ਼ਿਲ, (ਬਣਤਰ cp_options *) x, true)))
    {
      ਪੂਰਨ fd = open (ਮੰਜ਼ਿਲ_dir, O_PATHSEARCH | O_DIRECTORY);
      ਜੇ (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (ਮੰਜ਼ਿਲ));
          ret = false;
        }
      ਨਹੀਂਤਾਂ
        *ਨਿਸ਼ਾਨਾ_dirfd = fd;
    }

  ਜੇ (ret)
    {
      ਪੂਰਨ ਮੰਜ਼ਿਲ_dirfd = *ਨਿਸ਼ਾਨਾ_dirfd;
      ਜੇ (!ਨਿਸ਼ਾਨਾ_dirfd_valid (ਮੰਜ਼ਿਲ_dirfd))
        {
          ਮੰਜ਼ਿਲ_dirfd = AT_FDCWD;
          ਮੰਜ਼ਿਲ_relname = ਮੰਜ਼ਿਲ;
        }
      ret = install_file_ਵਿੱਚ_file (from, ਮੰਜ਼ਿਲ, ਮੰਜ਼ਿਲ_dirfd, ਮੰਜ਼ਿਲ_relname, x);
    }

  ਮੁਕਤ (ਮੰਜ਼ਿਲ);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *specified_ਬਹੁਲਕ = ਨਿਰਾ;
  bool make_backups = false;
  ਅੱਖਰ ਅਚਲ *backup_suffix = ਨਿਰਾ;
  ਅੱਖਰ *ਸੰਸਕਰਣ_control_string = ਨਿਰਾ;
  bool mkdir_and_install = false;
  ਬਣਤਰ cp_options x;
  ਅੱਖਰ ਅਚਲ *ਨਿਸ਼ਾਨਾ_directory = ਨਿਰਾ;
  bool no_ਨਿਸ਼ਾਨਾ_directory = false;
  bool strip_ਪ੍ਰੋਗਰਾਮ_specified = false;
  ਅੱਖਰ ਅਚਲ *scontext = ਨਿਰਾ;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'b':
          make_backups = true;
          ਜੇ (optarg)
            ਸੰਸਕਰਣ_control_string = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'c':
          ਰੋਕੋ;
        ਹਾਲਤ 'C':
          copy_only_if_needed = true;
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          strip_files = true;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          ਸੰਕੇਤ (SIGCHLD, SIG_DFL);
#ਜੇ_ਅੰਤ
          ਰੋਕੋ;
        ਹਾਲਤ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ਰੋਕੋ;
        ਹਾਲਤ STRIP_ਪ੍ਰੋਗਰਾਮ_OPTION:
          strip_ਪ੍ਰੋਗਰਾਮ = xstrdup (optarg);
          strip_ਪ੍ਰੋਗਰਾਮ_specified = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'd':
          dir_arg = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'D':
          mkdir_and_install = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          x.verbose = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'g':
          ਸਮੂਹ_name = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'm':
          specified_ਬਹੁਲਕ = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'o':
          owner_name = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'p':
          x.preserve_timestamps = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'S':
          make_backups = true;
          backup_suffix = optarg;
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

        ਹਾਲਤ PRESERVE_CONTEXT_OPTION:
          ਜੇ (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              ਰੋਕੋ;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'Z':
          ਜੇ (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              ਜੇ (optarg)
                scontext = optarg;
              ਨਹੀਂਤਾਂ
                x.ਸੈੱਟ_security_context = get_labeling_handle ();
            }
          ਨਹੀਂਤਾਂ ਜੇ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  ਜੇ (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  ਜੇ (dir_arg && ਨਿਸ਼ਾਨਾ_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_ਸੰਸਕਰਣ (_("backup type"),
                                   ਸੰਸਕਰਣ_control_string)
                   : no_backups);
  ਸੈੱਟ_simple_backup_suffix (backup_suffix);

  ਜੇ (x.preserve_security_context && (x.ਸੈੱਟ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ਜੇ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  ਪੂਰਨ n_files = argc - optind;
  ਅੱਖਰ **file = argv + optind;

  ਜੇ (n_files <= ! (dir_arg || ਨਿਸ਼ਾਨਾ_directory))
    {
      ਜੇ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ਬਣਤਰ stat sb;
  ਪੂਰਨ ਨਿਸ਼ਾਨਾ_dirfd = AT_FDCWD;
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
      ਜੇ (! (ਨਿਸ਼ਾਨਾ_dirfd_valid (ਨਿਸ਼ਾਨਾ_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (ਨਿਸ਼ਾਨਾ_directory));
    }
  ਨਹੀਂਤਾਂ ਜੇ (!dir_arg)
    {
      ਅੱਖਰ ਅਚਲ *lastfile = file[n_files - 1];
      ਪੂਰਨ fd = ਨਿਸ਼ਾਨਾ_directory_ਸੰਕਾਰਜ (lastfile, &sb);
      ਜੇ (ਨਿਸ਼ਾਨਾ_dirfd_valid (fd))
        {
          ਨਿਸ਼ਾਨਾ_dirfd = fd;
          ਨਿਸ਼ਾਨਾ_directory = lastfile;
          n_files--;
        }
      ਨਹੀਂਤਾਂ ਜੇ (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  ਜੇ (specified_ਬਹੁਲਕ)
    {
      ਬਣਤਰ ਬਹੁਲਕ_change *change = ਬਹੁਲਕ_compile (specified_ਬਹੁਲਕ);
      ਜੇ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_ਬਹੁਲਕ));
      ਬਹੁਲਕ_2 = ਬਹੁਲਕ_adjust (0, false, 0, change, ਨਿਰਾ);
      dir_ਬਹੁਲਕ = ਬਹੁਲਕ_adjust (0, true, 0, change, &dir_ਬਹੁਲਕ_ਬਿੱਟ);
      ਮੁਕਤ (change);
    }

  ਜੇ (strip_ਪ੍ਰੋਗਰਾਮ_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  ਜੇ (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (copy_only_if_needed && extra_ਬਹੁਲਕ (ਬਹੁਲਕ_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  ਪੂਰਨ exit_ਸਥਿਤੀ = EXIT_SUCCESS;
  ਜੇ (dir_arg)
    exit_ਸਥਿਤੀ = savewd_process_files (n_files, file, process_dir, &x);
  ਨਹੀਂਤਾਂ
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      ਜੇ (!ਨਿਸ਼ਾਨਾ_directory)
        {
          ਜੇ (! (mkdir_and_install
                 ? install_file_ਵਿੱਚ_file_parents (file[0], file[1], &x)
                 : install_file_ਵਿੱਚ_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
      ਨਹੀਂਤਾਂ
        {
          dest_info_init (&x);
          ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
            ਜੇ (! install_file_ਵਿੱਚ_dir (file[i], ਨਿਸ਼ਾਨਾ_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &ਨਿਸ਼ਾਨਾ_dirfd))
              exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }

  main_exit (exit_ਸਥਿਤੀ);
}
