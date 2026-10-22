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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "install"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ selinux_enabled = 0;
ಸ್ಥಿರ bool use_default_selinux_context = true;

#ಆದರೆ ! HAVE_ENDGRENT
# define endgrent() ((ಶೂನ್ಯ) 0)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ! HAVE_ENDPWENT
# define endpwent() ((ಶೂನ್ಯ) 0)
#ಆದರೆ_ಅಂತ್ಯ

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
ಸ್ಥಿರ ಅಕ್ಷರ *owner_name;

/* The user ID corresponding to 'owner_name'. */
ಸ್ಥಿರ uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
ಸ್ಥಿರ ಅಕ್ಷರ *ಗುಂಪು_name;

/* The group ID corresponding to 'group_name'. */
ಸ್ಥಿರ gid_t ಗುಂಪು_id;

#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_ಬಹುಲಕ (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
ಸ್ಥಿರ mode_t ಬಹುಲಕ_2 = DEFAULT_ಬಹುಲಕ;

/* Similar, but for directories.  */
ಸ್ಥಿರ mode_t dir_ಬಹುಲಕ = DEFAULT_ಬಹುಲಕ;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
ಸ್ಥಿರ mode_t dir_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು = CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು;

/* Compare files before installing (-C) */
ಸ್ಥಿರ bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
ಸ್ಥಿರ bool strip_files;

/* If true, install a directory instead of a regular file. */
ಸ್ಥಿರ bool dir_arg;

/* Program used to strip binaries, "strip" is default */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *strip_ಕಾರ್ಯಕ್ರಮ = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_ಕಾರ್ಯಕ್ರಮ_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"backup", optional_ವಾದ, ಖಾಲಿ, 'b'},
  {"compare", no_ವಾದ, ಖಾಲಿ, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_OPTION},
  {"directory", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"group", required_ವಾದ, ಖಾಲಿ, 'g'},
  {"mode", required_ವಾದ, ಖಾಲಿ, 'm'},
  {"no-target-directory", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"owner", required_ವಾದ, ಖಾಲಿ, 'o'},
  {"preserve-timestamps", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"preserve-context", no_ವಾದ, ಖಾಲಿ, PRESERVE_CONTEXT_OPTION},
  {"strip", no_ವಾದ, ಖಾಲಿ, 's'},
  {"strip-program", required_ವಾದ, ಖಾಲಿ, STRIP_ಕಾರ್ಯಕ್ರಮ_OPTION},
  {"suffix", required_ವಾದ, ಖಾಲಿ, 'S'},
  {"target-directory", required_ವಾದ, ಖಾಲಿ, 't'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
ಸ್ಥಿರ bool
have_same_content (ಪೂರ್ಣಾಂಕ a_fd, ಪೂರ್ಣಾಂಕ b_fd)
{
  ಎಣಿಕೆ { CMP_ಖಂಡ_ಗಾತ್ರ = 4096 };
  ಸ್ಥಿರ ಅಕ್ಷರ a_buff[CMP_ಖಂಡ_ಗಾತ್ರ];
  ಸ್ಥಿರ ಅಕ್ಷರ b_buff[CMP_ಖಂಡ_ಗಾತ್ರ];

  idx_t ಗಾತ್ರ_2;
  ಆಗ (0 < (ಗಾತ್ರ_2 = ತುಂಬಿದ_read (a_fd, a_buff, ಗಾತ್ರ a_buff))) {
    ಆದರೆ (ಗಾತ್ರ_2 != ತುಂಬಿದ_read (b_fd, b_buff, ಗಾತ್ರ b_buff))
      ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (!memeq (a_buff, b_buff, ಗಾತ್ರ_2))
      ಹಿಂತಿರುಗಿ false;
  }

  ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2 == 0;
}

/* Return true for mode with non-permission bits. */
ಸ್ಥಿರ bool
extra_ಬಹುಲಕ (mode_t ಒಳಹರಿವು)
{
  mode_t ಮುಖವಾಡ = S_IRWXUGO | S_IFMT;
  ಹಿಂತಿರುಗಿ !! (ಒಳಹರಿವು & ~ ಮುಖವಾಡ);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
ಸ್ಥಿರ bool
need_copy (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *src_name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_name,
           ಪೂರ್ಣಾಂಕ dest_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest_relname,
           ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ಆದರೆ (extra_ಬಹುಲಕ (ಬಹುಲಕ_2))
    ಹಿಂತಿರುಗಿ true;

  /* compare files using stat */
  ರಚನೆ stat src_sb;
  ಆದರೆ (stat (src_name, &src_sb) != 0)
    ಹಿಂತಿರುಗಿ true;

  ರಚನೆ stat dest_sb;
  ಆದರೆ (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (!S_ISREG (src_sb.st_ಬಹುಲಕ) || !S_ISREG (dest_sb.st_ಬಹುಲಕ)
      || extra_ಬಹುಲಕ (src_sb.st_ಬಹುಲಕ) || extra_ಬಹುಲಕ (dest_sb.st_ಬಹುಲಕ))
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (src_sb.st_ಗಾತ್ರ != dest_sb.st_ಗಾತ್ರ
      || (dest_sb.st_ಬಹುಲಕ & CHMOD_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು) != ಬಹುಲಕ_2)
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      ಆದರೆ ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        ಹಿಂತಿರುಗಿ true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dest_sb.st_uid != owner_id)
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (ಗುಂಪು_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      ಆದರೆ ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        ಹಿಂತಿರುಗಿ true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dest_sb.st_gid != ಗುಂಪು_id)
    ಹಿಂತಿರುಗಿ true;

  /* compare SELinux context if preserving */
  ಆದರೆ (selinux_enabled && x->preserve_security_context)
    {
      ಅಕ್ಷರ *file_scontext_raw = ಖಾಲಿ;
      ಆದರೆ (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        ಹಿಂತಿರುಗಿ true;

      ಅಕ್ಷರ *ಗಮ್ಯ_scontext_raw = ಖಾಲಿ;
      ಆದರೆ (getfilecon_raw (dest_name, &ಗಮ್ಯ_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          ಹಿಂತಿರುಗಿ true;
        }

      bool scontext_ಹೊಂದಿಕೆ = streq (file_scontext_raw, ಗಮ್ಯ_scontext_raw);

      freecon (file_scontext_raw);
      freecon (ಗಮ್ಯ_scontext_raw);
      ಆದರೆ (!scontext_ಹೊಂದಿಕೆ)
        ಹಿಂತಿರುಗಿ true;
    }

  /* compare files content */
  ಪೂರ್ಣಾಂಕ src_fd = open (src_name, O_RDONLY | O_ದ್ವಿಮಾನ);
  ಆದರೆ (src_fd < 0)
    ಹಿಂತಿರುಗಿ true;

  ಪೂರ್ಣಾಂಕ dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ದ್ವಿಮಾನ);
  ಆದರೆ (dest_fd < 0)
    {
      close (src_fd);
      ಹಿಂತಿರುಗಿ true;
    }

  bool content_ಹೊಂದಿಕೆ = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  ಹಿಂತಿರುಗಿ !content_ಹೊಂದಿಕೆ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cp_option_init (ರಚನೆ cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_ಬಹುಲಕ = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_ಬಹುಲಕ = false;
  x->install_ಬಹುಲಕ = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_ಬಹುಲಕ = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_ಬಹುಲಕ = false;
  x->reduce_diagnostics=false;
  x->ದತ್ತಾಂಶ_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_ಬಹುಲಕ = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->ಗಣ_ಬಹುಲಕ = true;
  x->ಬಹುಲಕ_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->ಗಣ_security_context = ಖಾಲಿ; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ಖಾಲಿ;
  x->src_info = ಖಾಲಿ;
}

ಸ್ಥಿರ ರಚನೆ selabel_handle *
get_labeling_handle (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ bool initialized;
  ಸ್ಥಿರ ರಚನೆ selabel_handle *hnd;
  ಆದರೆ (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ಖಾಲಿ, 0);
      ಆದರೆ (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  ಹಿಂತಿರುಗಿ hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
ಸ್ಥಿರ ಶೂನ್ಯ
setdefaultfilecon (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಆದರೆ (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      ಹಿಂತಿರುಗಿ;
    }

  ರಚನೆ stat st;
  ಆದರೆ (lstat (file, &st) != 0)
    ಹಿಂತಿರುಗಿ;

  ರಚನೆ selabel_handle *hnd = get_labeling_handle ();
  ಆದರೆ (!hnd)
    ಹಿಂತಿರುಗಿ;

  ಅಕ್ಷರ *scontext_raw = ಖಾಲಿ;
  ಆದರೆ (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_ಬಹುಲಕ) != 0)
    {
      ಆದರೆ (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      ಹಿಂತಿರುಗಿ;
    }

  ಆದರೆ (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
ಸ್ಥಿರ ಶೂನ್ಯ
announce_mkdir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir, ಶೂನ್ಯ *options)
{
  ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x = options;
  ಆದರೆ (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
make_ancestor (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *component, ಶೂನ್ಯ *options)
{
  ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x = options;
  ಆದರೆ (x->ಗಣ_security_context
      && defaultcon (x->ಗಣ_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  ಪೂರ್ಣಾಂಕ r = mkdir (component, DEFAULT_ಬಹುಲಕ);
  ಆದರೆ (r == 0)
    announce_mkdir (dir, options);
  ಹಿಂತಿರುಗಿ r;
}

/* Process a command-line file name, for the -d option.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
process_dir (ಅಕ್ಷರ *dir, ರಚನೆ savewd *wd, ಶೂನ್ಯ *options)
{
  ರಚನೆ cp_options ಸ್ಥಿರಾಂಕ *x = options;

  ಪೂರ್ಣಾಂಕ ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_ಬಹುಲಕ, announce_mkdir,
                               dir_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು, owner_id, ಗುಂಪು_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ಆದರೆ (ret == EXIT_SUCCESS && x->ಗಣ_security_context)
    {
      ಆದರೆ (! restorecon (x->ಗಣ_security_context, ಕೊನೆಯ_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಎಣಿಕೆ copy_ಸ್ಥಿತಿ { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

ಸ್ಥಿರ ಎಣಿಕೆ copy_ಸ್ಥಿತಿ
copy_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಮ್ಯ,
           ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಮ್ಯ_relname, ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ಆದರೆ (copy_only_if_needed && !need_copy (from, ಗಮ್ಯ, ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname, x))
    ಹಿಂತಿರುಗಿ COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  ಹಿಂತಿರುಗಿ (copy (from, ಗಮ್ಯ, ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname, 0, x, &copy_into_self, ಖಾಲಿ)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

ಸ್ಥಿರ bool
change_ಗುಣಲಕ್ಷಣಗಳು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relname)
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

  ಆದರೆ (! (owner_id == (uid_t) -1 && ಗುಂಪು_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, ಗುಂಪು_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (chmodat (dirfd, relname, ಬಹುಲಕ_2) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  ಇಲ್ಲದಿದ್ದರೆ
    ok = true;

  ಆದರೆ (use_default_selinux_context)
    setdefaultfilecon (name);

  ಹಿಂತಿರುಗಿ ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

ಸ್ಥಿರ bool
change_timestamps (ರಚನೆ stat ಸ್ಥಿರಾಂಕ *src_sb, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dest,
                   ಪೂರ್ಣಾಂಕ dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relname)
{
  ರಚನೆ timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  ಆದರೆ (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      ಹಿಂತಿರುಗಿ false;
    }
  ಹಿಂತಿರುಗಿ true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

ಸ್ಥಿರ bool
strip (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ಖಾಲಿ;

  /* Try to use vfork for systems where it matters.  */
  ಆದರೆ (posix_spawnattr_init (&attr) == 0)
    {
      ಆದರೆ (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      ಇಲ್ಲದಿದ್ದರೆ
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  ಅಕ್ಷರ *concat_name = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸುರಕ್ಷಿತ_name = name;
  ಆದರೆ (name && *name == '-')
    ಸುರಕ್ಷಿತ_name = concat_name = file_name_concat (".", name, ಖಾಲಿ);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ argv[] = { strip_ಕಾರ್ಯಕ್ರಮ, ಸುರಕ್ಷಿತ_name, ಖಾಲಿ };

  /* Run 'strip'.  */
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = posix_spawnp (&pid, strip_ಕಾರ್ಯಕ್ರಮ, ಖಾಲಿ, attrp,
                             (ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *) argv, environ);

  bool ok = false;
  ಆದರೆ (ಫಲಿತಾಂಶ != 0)
    {
      error (0, ಫಲಿತಾಂಶ,
             streq (strip_ಕಾರ್ಯಕ್ರಮ, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_ಕಾರ್ಯಕ್ರಮ));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
      ಆದರೆ (waitpid (pid, &ಸ್ಥಿತಿ_2, 0) < 0)
        error (0, errno, _("waiting for strip"));
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! WIFEXITED (ಸ್ಥಿತಿ_2) || WEXITSTATUS (ಸ್ಥಿತಿ_2))
        error (0, 0, _("strip process terminated abnormally"));
      ಇಲ್ಲದಿದ್ದರೆ
        ok = true;
    }

  ಬಿಡುಗಡೆ (concat_name);
  ಆದರೆ (attrp)
    posix_spawnattr_destroy (attrp);

  ಹಿಂತಿರುಗಿ ok;
}

/* Initialize the user and group ownership of the files to install. */

ಸ್ಥಿರ ಶೂನ್ಯ
get_ids (ಶೂನ್ಯ)
{
  ಆದರೆ (owner_name)
    {
      ರಚನೆ passwd *pw = getpwnam (owner_name);
      ಆದರೆ (pw == ಖಾಲಿ)
        {
          uintmax_t tmp;
          ಆದರೆ (xstrtoumax (owner_name, ಖಾಲಿ, 0, &tmp, "") != LONGINT_OK
              || ckd_ಕೂಡಿಸು (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        owner_id = pw->pw_uid;
      endpwent ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    owner_id = (uid_t) -1;

  ಆದರೆ (ಗುಂಪು_name)
    {
      ರಚನೆ ಗುಂಪು *gr = getgrnam (ಗುಂಪು_name);
      ಆದರೆ (gr == ಖಾಲಿ)
        {
          uintmax_t tmp;
          ಆದರೆ (xstrtoumax (ಗುಂಪು_name, ಖಾಲಿ, 0, &tmp, "") != LONGINT_OK
              || ckd_ಕೂಡಿಸು (&ಗುಂಪು_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (ಗುಂಪು_name));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಗುಂಪು_id = gr->gr_gid;
      endgrent ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗುಂಪು_id = (gid_t) -1;
}

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
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
\n\
This install ಕಾರ್ಯಕ್ರಮ copies files (often just compiled) into destination\n\
locations you ಆರಿಸು.  If you want ಗಮ್ಯ download and install a ready-ಗಮ್ಯ-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
ಒಳ_2 the ಮೊದಲ three forms, copy ಮೂಲ ಗಮ್ಯ DEST or ಅಪವರ್ತ್ಯ ಮೂಲ(s) ಗಮ್ಯ\n\
the existing DIRECTORY, ಆಗ setting permission modes and owner/ಗುಂಪು.\n\
ಒಳ_2 the 4th form, create all components ದ the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup ದ each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an ವಾದ\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content ದ ಮೂಲ_2 and destination files,\n\
         and ಆದರೆ no change ಗಮ್ಯ content, ownership, and permissions,\n\
         ಮಾಡು not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory ಹೆಸರುಗಳು;\n\
         create all components ದ the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components ದ DEST except the ಕೊನೆಯ,\n\
         or all components ದ --ಗುರಿ-directory,\n\
         then copy ಮೂಲ ಗಮ್ಯ DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  ಸೂಚಿಸುತ್ತದೆ_2 -v\n\
"));
      oputs (_("\
  -g, --ಗುಂಪು=ಗುಂಪು_2\n\
         ಗಣ_2 ಗುಂಪು ownership, instead ದ process' current ಗುಂಪು\n\
"));
      oputs (_("\
  -m, --ಬಹುಲಕ_2=ಬಹುಲಕ\n\
         ಗಣ_2 permission ಬಹುಲಕ_2 (as ಒಳ chmod), instead ದ rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         ಗಣ_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times ದ ಮೂಲ files\n\
         ಗಮ್ಯ corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip ಚಿಹ್ನೆ tables\n\
"));
      oputs (_("\
      --strip-ಕಾರ್ಯಕ್ರಮ=ಕಾರ್ಯಕ್ರಮ_2\n\
         ಕಾರ್ಯಕ್ರಮ ಬಳಸಿದ ಗಮ್ಯ strip binaries\n\
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
  -v, --verbose\n\
         print the name ದ each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         ಗಣ_2 SELinux security context ದ destination file\n\
         and each created directory, ಗಮ್ಯ ಪೂರ್ವನಿಯೋಜಿತ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಆದರೆ CTX is specified then ಗಣ_2 the\n\
         SELinux or SMACK security context ಗಮ್ಯ CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

ಸ್ಥಿರ bool
install_file_ಒಳ_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಮ್ಯ,
                      ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_dirfd, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಮ್ಯ_relname,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ರಚನೆ stat from_sb;
  ಆದರೆ (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      ಹಿಂತಿರುಗಿ false;
    }
  ಎಣಿಕೆ copy_ಸ್ಥಿತಿ copy_ಸ್ಥಿತಿ = copy_file (from, ಗಮ್ಯ, ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname, x);
  ಆದರೆ (copy_ಸ್ಥಿತಿ == COPY_FAILED)
    ಹಿಂತಿರುಗಿ false;
  ಆದರೆ (copy_ಸ್ಥಿತಿ == COPY_OK && strip_files && ! strip (ಗಮ್ಯ))
    {
      ಆದರೆ (unlinkat (ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (ಗಮ್ಯ));
      ಹಿಂತಿರುಗಿ false;
    }
  ಆದರೆ (x->preserve_timestamps && (copy_ಸ್ಥಿತಿ == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_ಬಹುಲಕ))
      && ! change_timestamps (&from_sb, ಗಮ್ಯ, ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname))
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ copy_ಸ್ಥಿತಿ == COPY_SKIPPED
         || change_ಗುಣಲಕ್ಷಣಗಳು (ಗಮ್ಯ, ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

ಸ್ಥಿರ bool
mkancesdirs_ಸುರಕ್ಷಿತ_wd (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from, ಅಕ್ಷರ *ಗಮ್ಯ, ರಚನೆ cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (ಗಮ್ಯ));

  ರಚನೆ savewd wd;
  savewd_init (&wd);
  ಆದರೆ (! save_working_directory)
    savewd_ಮುಕ್ತಾಯ (&wd);

  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = EXIT_SUCCESS;
  ಆದರೆ (mkancesdirs (ಗಮ್ಯ, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (ಗಮ್ಯ));
      ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
    }

  ಆದರೆ (save_working_directory)
    {
      ಪೂರ್ಣಾಂಕ restore_ಫಲಿತಾಂಶ = savewd_restore (&wd, ಸ್ಥಿತಿ_2);
      ಪೂರ್ಣಾಂಕ restore_errno = errno;
      savewd_ಮುಕ್ತಾಯ (&wd);
      ಆದರೆ (EXIT_SUCCESS < restore_ಫಲಿತಾಂಶ)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (restore_ಫಲಿತಾಂಶ < 0 && ಸ್ಥಿತಿ_2 == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (ಗಮ್ಯ));
          ಹಿಂತಿರುಗಿ false;
        }
    }
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

ಸ್ಥಿರ bool
install_file_ಒಳ_file_parents (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from, ಅಕ್ಷರ *ಗಮ್ಯ,
                              ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x)
{
  ಹಿಂತಿರುಗಿ (mkancesdirs_ಸುರಕ್ಷಿತ_wd (from, ಗಮ್ಯ, (ರಚನೆ cp_options *)x, false)
          && install_file_ಒಳ_file (from, ಗಮ್ಯ, AT_FDCWD, ಗಮ್ಯ, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

ಸ್ಥಿರ bool
install_file_ಒಳ_dir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗಮ್ಯ_dir,
                     ಸ್ಥಿರಾಂಕ ರಚನೆ cp_options *x, bool mkdir_and_install,
                     ಪೂರ್ಣಾಂಕ *ಗುರಿ_dirfd)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *from_ತಳ = ಕೊನೆಯ_component (from);
  ಅಕ್ಷರ *ಗಮ್ಯ_relname;
  ಅಕ್ಷರ *ಗಮ್ಯ = file_name_concat (ಗಮ್ಯ_dir, from_ತಳ, &ಗಮ್ಯ_relname);
  bool ret = true;

  ಆದರೆ (!ಗುರಿ_dirfd_valid (*ಗುರಿ_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_ಸುರಕ್ಷಿತ_wd (from, ಗಮ್ಯ, (ರಚನೆ cp_options *) x, true)))
    {
      ಪೂರ್ಣಾಂಕ fd = open (ಗಮ್ಯ_dir, O_PATHSEARCH | O_DIRECTORY);
      ಆದರೆ (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (ಗಮ್ಯ));
          ret = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        *ಗುರಿ_dirfd = fd;
    }

  ಆದರೆ (ret)
    {
      ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_dirfd = *ಗುರಿ_dirfd;
      ಆದರೆ (!ಗುರಿ_dirfd_valid (ಗಮ್ಯ_dirfd))
        {
          ಗಮ್ಯ_dirfd = AT_FDCWD;
          ಗಮ್ಯ_relname = ಗಮ್ಯ;
        }
      ret = install_file_ಒಳ_file (from, ಗಮ್ಯ, ಗಮ್ಯ_dirfd, ಗಮ್ಯ_relname, x);
    }

  ಬಿಡುಗಡೆ (ಗಮ್ಯ);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_ಬಹುಲಕ = ಖಾಲಿ;
  bool make_backups = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *backup_suffix = ಖಾಲಿ;
  ಅಕ್ಷರ *ಆವೃತ್ತಿ_control_string = ಖಾಲಿ;
  bool mkdir_and_install = false;
  ರಚನೆ cp_options x;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಗುರಿ_directory = ಖಾಲಿ;
  bool no_ಗುರಿ_directory = false;
  bool strip_ಕಾರ್ಯಕ್ರಮ_specified = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *scontext = ಖಾಲಿ;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'b':
          make_backups = true;
          ಆದರೆ (optarg)
            ಆವೃತ್ತಿ_control_string = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'c':
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'C':
          copy_only_if_needed = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          strip_files = true;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          ಸಂಕೇತ (SIGCHLD, SIG_DFL);
#ಆದರೆ_ಅಂತ್ಯ
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ DEBUG_OPTION:
          x.debug = x.verbose = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STRIP_ಕಾರ್ಯಕ್ರಮ_OPTION:
          strip_ಕಾರ್ಯಕ್ರಮ = xstrdup (optarg);
          strip_ಕಾರ್ಯಕ್ರಮ_specified = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'd':
          dir_arg = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'D':
          mkdir_and_install = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          x.verbose = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'g':
          ಗುಂಪು_name = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm':
          specified_ಬಹುಲಕ = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'o':
          owner_name = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'p':
          x.preserve_timestamps = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'S':
          make_backups = true;
          backup_suffix = optarg;
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

        ಸ್ಥಿತಿ PRESERVE_CONTEXT_OPTION:
          ಆದರೆ (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              ನಿಲ್ಲಿಸು;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'Z':
          ಆದರೆ (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              ಆದರೆ (optarg)
                scontext = optarg;
              ಇಲ್ಲದಿದ್ದರೆ
                x.ಗಣ_security_context = get_labeling_handle ();
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  ಆದರೆ (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  ಆದರೆ (dir_arg && ಗುರಿ_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_ಆವೃತ್ತಿ (_("backup type"),
                                   ಆವೃತ್ತಿ_control_string)
                   : no_backups);
  ಗಣ_simple_backup_suffix (backup_suffix);

  ಆದರೆ (x.preserve_security_context && (x.ಗಣ_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ಆದರೆ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  ಪೂರ್ಣಾಂಕ n_files = argc - optind;
  ಅಕ್ಷರ **file = argv + optind;

  ಆದರೆ (n_files <= ! (dir_arg || ಗುರಿ_directory))
    {
      ಆದರೆ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  ರಚನೆ stat sb;
  ಪೂರ್ಣಾಂಕ ಗುರಿ_dirfd = AT_FDCWD;
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
      ಆದರೆ (! (ಗುರಿ_dirfd_valid (ಗುರಿ_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (ಗುರಿ_directory));
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!dir_arg)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lastfile = file[n_files - 1];
      ಪೂರ್ಣಾಂಕ fd = ಗುರಿ_directory_ಕಾರ್ಯಾಂಶ (lastfile, &sb);
      ಆದರೆ (ಗುರಿ_dirfd_valid (fd))
        {
          ಗುರಿ_dirfd = fd;
          ಗುರಿ_directory = lastfile;
          n_files--;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  ಆದರೆ (specified_ಬಹುಲಕ)
    {
      ರಚನೆ ಬಹುಲಕ_change *change = ಬಹುಲಕ_compile (specified_ಬಹುಲಕ);
      ಆದರೆ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_ಬಹುಲಕ));
      ಬಹುಲಕ_2 = ಬಹುಲಕ_adjust (0, false, 0, change, ಖಾಲಿ);
      dir_ಬಹುಲಕ = ಬಹುಲಕ_adjust (0, true, 0, change, &dir_ಬಹುಲಕ_ಬಿಟ್ಟುಗಳು);
      ಬಿಡುಗಡೆ (change);
    }

  ಆದರೆ (strip_ಕಾರ್ಯಕ್ರಮ_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  ಆದರೆ (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (copy_only_if_needed && extra_ಬಹುಲಕ (ಬಹುಲಕ_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = EXIT_SUCCESS;
  ಆದರೆ (dir_arg)
    exit_ಸ್ಥಿತಿ = savewd_process_files (n_files, file, process_dir, &x);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      ಆದರೆ (!ಗುರಿ_directory)
        {
          ಆದರೆ (! (mkdir_and_install
                 ? install_file_ಒಳ_file_parents (file[0], file[1], &x)
                 : install_file_ಒಳ_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          dest_info_init (&x);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; i++)
            ಆದರೆ (! install_file_ಒಳ_dir (file[i], ಗುರಿ_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &ಗುರಿ_dirfd))
              exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
    }

  main_exit (exit_ಸ್ಥಿತಿ);
}
