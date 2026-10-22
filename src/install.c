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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <selinux/label.h>
#అన్తర్భావ <spawn.h>
#అన్తర్భావ <sys/wait.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "backupfile.h"
#అన్తర్భావ "cp-hash.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "full-read.h"
#అన్తర్భావ "mkancesdirs.h"
#అన్తర్భావ "mkdir-p.h"
#అన్తర్భావ "modechange.h"
#అన్తర్భావ "prog-fprintf.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "savewd.h"
#అన్తర్భావ "selinux.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "targetdir.h"
#అన్తర్భావ "utimens.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "install"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

స్థిర పూర్ణాంకం selinux_enabled = 0;
స్థిర bool use_default_selinux_context = true;

#ఒకవేళ ! HAVE_ENDGRENT
# define endgrent() ((శూన్య) 0)
#అయితే_ముగింపు

#ఒకవేళ ! HAVE_ENDPWENT
# define endpwent() ((శూన్య) 0)
#అయితే_ముగింపు

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
స్థిర అక్షరం *owner_name;

/* The user ID corresponding to 'owner_name'. */
స్థిర uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
స్థిర అక్షరం *సమూహం_name;

/* The group ID corresponding to 'group_name'. */
స్థిర gid_t సమూహం_id;

#నిర్వచించు DEFAULT_బహుళకం (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
స్థిర mode_t బహుళకం_2 = DEFAULT_బహుళకం;

/* Similar, but for directories.  */
స్థిర mode_t dir_బహుళకం = DEFAULT_బహుళకం;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
స్థిర mode_t dir_బహుళకం_బిట్లు = CHMOD_బహుళకం_బిట్లు;

/* Compare files before installing (-C) */
స్థిర bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
స్థిర bool strip_files;

/* If true, install a directory instead of a regular file. */
స్థిర bool dir_arg;

/* Program used to strip binaries, "strip" is default */
స్థిర అక్షరం స్థిరరాశి *strip_కార్యక్రమం = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_కార్యక్రమం_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"backup", optional_వాదం, ఖాళీ, 'b'},
  {"compare", no_వాదం, ఖాళీ, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_వాదం, ఖాళీ, DEBUG_OPTION},
  {"directory", no_వాదం, ఖాళీ, 'd'},
  {"group", required_వాదం, ఖాళీ, 'g'},
  {"mode", required_వాదం, ఖాళీ, 'm'},
  {"no-target-directory", no_వాదం, ఖాళీ, 'T'},
  {"owner", required_వాదం, ఖాళీ, 'o'},
  {"preserve-timestamps", no_వాదం, ఖాళీ, 'p'},
  {"preserve-context", no_వాదం, ఖాళీ, PRESERVE_CONTEXT_OPTION},
  {"strip", no_వాదం, ఖాళీ, 's'},
  {"strip-program", required_వాదం, ఖాళీ, STRIP_కార్యక్రమం_OPTION},
  {"suffix", required_వాదం, ఖాళీ, 'S'},
  {"target-directory", required_వాదం, ఖాళీ, 't'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
స్థిర bool
have_same_content (పూర్ణాంకం a_fd, పూర్ణాంకం b_fd)
{
  గణన { CMP_ఖండం_కొలత = 4096 };
  స్థిర అక్షరం a_buff[CMP_ఖండం_కొలత];
  స్థిర అక్షరం b_buff[CMP_ఖండం_కొలత];

  idx_t కొలత;
  అయితే (0 < (కొలత = నిండిన_read (a_fd, a_buff, పరిమాణం a_buff))) {
    ఒకవేళ (కొలత != నిండిన_read (b_fd, b_buff, పరిమాణం b_buff))
      తిరిగి false;

    ఒకవేళ (!memeq (a_buff, b_buff, కొలత))
      తిరిగి false;
  }

  తిరిగి కొలత == 0;
}

/* Return true for mode with non-permission bits. */
స్థిర bool
extra_బహుళకం (mode_t ఆదానం)
{
  mode_t ముసుగు = S_IRWXUGO | S_IFMT;
  తిరిగి !! (ఆదానం & ~ ముసుగు);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
స్థిర bool
need_copy (అక్షరం స్థిరరాశి *src_name, అక్షరం స్థిరరాశి *dest_name,
           పూర్ణాంకం dest_dirfd, అక్షరం స్థిరరాశి *dest_relname,
           స్థిరరాశి నిర్మాణం cp_options *x)
{
  ఒకవేళ (extra_బహుళకం (బహుళకం_2))
    తిరిగి true;

  /* compare files using stat */
  నిర్మాణం stat src_sb;
  ఒకవేళ (stat (src_name, &src_sb) != 0)
    తిరిగి true;

  నిర్మాణం stat dest_sb;
  ఒకవేళ (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    తిరిగి true;

  ఒకవేళ (!S_ISREG (src_sb.st_బహుళకం) || !S_ISREG (dest_sb.st_బహుళకం)
      || extra_బహుళకం (src_sb.st_బహుళకం) || extra_బహుళకం (dest_sb.st_బహుళకం))
    తిరిగి true;

  ఒకవేళ (src_sb.st_కొలత != dest_sb.st_కొలత
      || (dest_sb.st_బహుళకం & CHMOD_బహుళకం_బిట్లు) != బహుళకం_2)
    తిరిగి true;

  ఒకవేళ (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      ఒకవేళ ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        తిరిగి true;
    }
  లేకపోతే ఒకవేళ (dest_sb.st_uid != owner_id)
    తిరిగి true;

  ఒకవేళ (సమూహం_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      ఒకవేళ ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        తిరిగి true;
    }
  లేకపోతే ఒకవేళ (dest_sb.st_gid != సమూహం_id)
    తిరిగి true;

  /* compare SELinux context if preserving */
  ఒకవేళ (selinux_enabled && x->preserve_security_context)
    {
      అక్షరం *file_scontext_raw = ఖాళీ;
      ఒకవేళ (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        తిరిగి true;

      అక్షరం *గమ్యం_scontext_raw = ఖాళీ;
      ఒకవేళ (getfilecon_raw (dest_name, &గమ్యం_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          తిరిగి true;
        }

      bool scontext_సరిపోలిక = streq (file_scontext_raw, గమ్యం_scontext_raw);

      freecon (file_scontext_raw);
      freecon (గమ్యం_scontext_raw);
      ఒకవేళ (!scontext_సరిపోలిక)
        తిరిగి true;
    }

  /* compare files content */
  పూర్ణాంకం src_fd = open (src_name, O_RDONLY | O_ద్విమాన);
  ఒకవేళ (src_fd < 0)
    తిరిగి true;

  పూర్ణాంకం dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_ద్విమాన);
  ఒకవేళ (dest_fd < 0)
    {
      close (src_fd);
      తిరిగి true;
    }

  bool content_సరిపోలిక = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  తిరిగి !content_సరిపోలిక;
}

స్థిర శూన్య
cp_option_init (నిర్మాణం cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_బహుళకం = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_బహుళకం = false;
  x->install_బహుళకం = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_బహుళకం = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_బహుళకం = false;
  x->reduce_diagnostics=false;
  x->దత్తాంశం_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_బహుళకం = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->సమితి_బహుళకం = true;
  x->బహుళకం_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->సమితి_security_context = ఖాళీ; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ఖాళీ;
  x->src_info = ఖాళీ;
}

స్థిర నిర్మాణం selabel_handle *
get_labeling_handle (శూన్య)
{
  స్థిర bool initialized;
  స్థిర నిర్మాణం selabel_handle *hnd;
  ఒకవేళ (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ఖాళీ, 0);
      ఒకవేళ (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  తిరిగి hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
స్థిర శూన్య
setdefaultfilecon (అక్షరం స్థిరరాశి *file)
{
  ఒకవేళ (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      తిరిగి;
    }

  నిర్మాణం stat st;
  ఒకవేళ (lstat (file, &st) != 0)
    తిరిగి;

  నిర్మాణం selabel_handle *hnd = get_labeling_handle ();
  ఒకవేళ (!hnd)
    తిరిగి;

  అక్షరం *scontext_raw = ఖాళీ;
  ఒకవేళ (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_బహుళకం) != 0)
    {
      ఒకవేళ (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      తిరిగి;
    }

  ఒకవేళ (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
స్థిర శూన్య
announce_mkdir (అక్షరం స్థిరరాశి *dir, శూన్య *options)
{
  నిర్మాణం cp_options స్థిరరాశి *x = options;
  ఒకవేళ (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
స్థిర పూర్ణాంకం
make_ancestor (అక్షరం స్థిరరాశి *dir, అక్షరం స్థిరరాశి *component, శూన్య *options)
{
  నిర్మాణం cp_options స్థిరరాశి *x = options;
  ఒకవేళ (x->సమితి_security_context
      && defaultcon (x->సమితి_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  పూర్ణాంకం r = mkdir (component, DEFAULT_బహుళకం);
  ఒకవేళ (r == 0)
    announce_mkdir (dir, options);
  తిరిగి r;
}

/* Process a command-line file name, for the -d option.  */
స్థిర పూర్ణాంకం
process_dir (అక్షరం *dir, నిర్మాణం savewd *wd, శూన్య *options)
{
  నిర్మాణం cp_options స్థిరరాశి *x = options;

  పూర్ణాంకం ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_బహుళకం, announce_mkdir,
                               dir_బహుళకం_బిట్లు, owner_id, సమూహం_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  ఒకవేళ (ret == EXIT_SUCCESS && x->సమితి_security_context)
    {
      ఒకవేళ (! restorecon (x->సమితి_security_context, చివరి_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  తిరిగి ret;
}

గణన copy_స్థితి { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

స్థిర గణన copy_స్థితి
copy_file (అక్షరం స్థిరరాశి *from, అక్షరం స్థిరరాశి *గమ్యం,
           పూర్ణాంకం గమ్యం_dirfd, అక్షరం స్థిరరాశి *గమ్యం_relname, స్థిరరాశి నిర్మాణం cp_options *x)
{
  ఒకవేళ (copy_only_if_needed && !need_copy (from, గమ్యం, గమ్యం_dirfd, గమ్యం_relname, x))
    తిరిగి COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  తిరిగి (copy (from, గమ్యం, గమ్యం_dirfd, గమ్యం_relname, 0, x, &copy_into_self, ఖాళీ)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

స్థిర bool
change_లక్షణాలు (అక్షరం స్థిరరాశి *name, పూర్ణాంకం dirfd, అక్షరం స్థిరరాశి *relname)
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

  ఒకవేళ (! (owner_id == (uid_t) -1 && సమూహం_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, సమూహం_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  లేకపోతే ఒకవేళ (chmodat (dirfd, relname, బహుళకం_2) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  లేకపోతే
    ok = true;

  ఒకవేళ (use_default_selinux_context)
    setdefaultfilecon (name);

  తిరిగి ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

స్థిర bool
change_timestamps (నిర్మాణం stat స్థిరరాశి *src_sb, అక్షరం స్థిరరాశి *dest,
                   పూర్ణాంకం dirfd, అక్షరం స్థిరరాశి *relname)
{
  నిర్మాణం timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  ఒకవేళ (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      తిరిగి false;
    }
  తిరిగి true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

స్థిర bool
strip (అక్షరం స్థిరరాశి *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ఖాళీ;

  /* Try to use vfork for systems where it matters.  */
  ఒకవేళ (posix_spawnattr_init (&attr) == 0)
    {
      ఒకవేళ (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      లేకపోతే
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  అక్షరం *concat_name = ఖాళీ;
  అక్షరం స్థిరరాశి *సురక్షిత_name = name;
  ఒకవేళ (name && *name == '-')
    సురక్షిత_name = concat_name = file_name_concat (".", name, ఖాళీ);
  అక్షరం స్థిరరాశి *స్థిరరాశి argv[] = { strip_కార్యక్రమం, సురక్షిత_name, ఖాళీ };

  /* Run 'strip'.  */
  pid_t pid;
  పూర్ణాంకం ఫలితం = posix_spawnp (&pid, strip_కార్యక్రమం, ఖాళీ, attrp,
                             (అక్షరం * స్థిరరాశి *) argv, environ);

  bool ok = false;
  ఒకవేళ (ఫలితం != 0)
    {
      error (0, ఫలితం,
             streq (strip_కార్యక్రమం, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_కార్యక్రమం));
    }
  లేకపోతే
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      పూర్ణాంకం స్థితి_2;
      ఒకవేళ (waitpid (pid, &స్థితి_2, 0) < 0)
        error (0, errno, _("waiting for strip"));
      లేకపోతే ఒకవేళ (! WIFEXITED (స్థితి_2) || WEXITSTATUS (స్థితి_2))
        error (0, 0, _("strip process terminated abnormally"));
      లేకపోతే
        ok = true;
    }

  విడుదల (concat_name);
  ఒకవేళ (attrp)
    posix_spawnattr_destroy (attrp);

  తిరిగి ok;
}

/* Initialize the user and group ownership of the files to install. */

స్థిర శూన్య
get_ids (శూన్య)
{
  ఒకవేళ (owner_name)
    {
      నిర్మాణం passwd *pw = getpwnam (owner_name);
      ఒకవేళ (pw == ఖాళీ)
        {
          uintmax_t tmp;
          ఒకవేళ (xstrtoumax (owner_name, ఖాళీ, 0, &tmp, "") != LONGINT_OK
              || ckd_కూడు (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      లేకపోతే
        owner_id = pw->pw_uid;
      endpwent ();
    }
  లేకపోతే
    owner_id = (uid_t) -1;

  ఒకవేళ (సమూహం_name)
    {
      నిర్మాణం సమూహం *gr = getgrnam (సమూహం_name);
      ఒకవేళ (gr == ఖాళీ)
        {
          uintmax_t tmp;
          ఒకవేళ (xstrtoumax (సమూహం_name, ఖాళీ, 0, &tmp, "") != LONGINT_OK
              || ckd_కూడు (&సమూహం_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (సమూహం_name));
        }
      లేకపోతే
        సమూహం_id = gr->gr_gid;
      endgrent ();
    }
  లేకపోతే
    సమూహం_id = (gid_t) -1;
}

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
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
\n\
This install కార్యక్రమం copies files (often just compiled) into destination\n\
locations you ఎంచుకో.  If you want గమ్యం download and install a ready-గమ్యం-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
లోపల_2 the మొదటి three forms, copy ఆధారం గమ్యం DEST or గుణిజం ఆధారం(s) గమ్యం\n\
the existing DIRECTORY, అయితే setting permission modes and owner/సమూహం.\n\
లోపల_2 the 4th form, create all components యొక్క the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup యొక్క each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an వాదం\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content యొక్క ఆధారం_2 and destination files,\n\
         and ఒకవేళ no change గమ్యం content, ownership, and permissions,\n\
         చేయి not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory పేర్లు;\n\
         create all components యొక్క the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components యొక్క DEST except the చివరి,\n\
         or all components యొక్క --లక్ష్యం-directory,\n\
         then copy ఆధారం గమ్యం DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  సూచిస్తుంది_2 -v\n\
"));
      oputs (_("\
  -g, --సమూహం=సమూహం_2\n\
         సమితి_2 సమూహం ownership, instead యొక్క process' current సమూహం\n\
"));
      oputs (_("\
  -m, --బహుళకం_2=బహుళకం\n\
         సమితి_2 permission బహుళకం_2 (as లోపల chmod), instead యొక్క rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         సమితి_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times యొక్క ఆధారం files\n\
         గమ్యం corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip చిహ్నం tables\n\
"));
      oputs (_("\
      --strip-కార్యక్రమం=కార్యక్రమం_2\n\
         కార్యక్రమం వాడిన గమ్యం strip binaries\n\
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
  -v, --verbose\n\
         print the name యొక్క each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         సమితి_2 SELinux security context యొక్క destination file\n\
         and each created directory, గమ్యం అప్రమేయ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ఒకవేళ CTX is specified then సమితి_2 the\n\
         SELinux or SMACK security context గమ్యం CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

స్థిర bool
install_file_లోపల_file (అక్షరం స్థిరరాశి *from, అక్షరం స్థిరరాశి *గమ్యం,
                      పూర్ణాంకం గమ్యం_dirfd, అక్షరం స్థిరరాశి *గమ్యం_relname,
                      స్థిరరాశి నిర్మాణం cp_options *x)
{
  నిర్మాణం stat from_sb;
  ఒకవేళ (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      తిరిగి false;
    }
  గణన copy_స్థితి copy_స్థితి = copy_file (from, గమ్యం, గమ్యం_dirfd, గమ్యం_relname, x);
  ఒకవేళ (copy_స్థితి == COPY_FAILED)
    తిరిగి false;
  ఒకవేళ (copy_స్థితి == COPY_OK && strip_files && ! strip (గమ్యం))
    {
      ఒకవేళ (unlinkat (గమ్యం_dirfd, గమ్యం_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (గమ్యం));
      తిరిగి false;
    }
  ఒకవేళ (x->preserve_timestamps && (copy_స్థితి == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_బహుళకం))
      && ! change_timestamps (&from_sb, గమ్యం, గమ్యం_dirfd, గమ్యం_relname))
    తిరిగి false;

  తిరిగి copy_స్థితి == COPY_SKIPPED
         || change_లక్షణాలు (గమ్యం, గమ్యం_dirfd, గమ్యం_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

స్థిర bool
mkancesdirs_సురక్షిత_wd (అక్షరం స్థిరరాశి *from, అక్షరం *గమ్యం, నిర్మాణం cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (గమ్యం));

  నిర్మాణం savewd wd;
  savewd_init (&wd);
  ఒకవేళ (! save_working_directory)
    savewd_ముగింపు (&wd);

  పూర్ణాంకం స్థితి_2 = EXIT_SUCCESS;
  ఒకవేళ (mkancesdirs (గమ్యం, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (గమ్యం));
      స్థితి_2 = EXIT_FAILURE;
    }

  ఒకవేళ (save_working_directory)
    {
      పూర్ణాంకం restore_ఫలితం = savewd_restore (&wd, స్థితి_2);
      పూర్ణాంకం restore_errno = errno;
      savewd_ముగింపు (&wd);
      ఒకవేళ (EXIT_SUCCESS < restore_ఫలితం)
        తిరిగి false;
      ఒకవేళ (restore_ఫలితం < 0 && స్థితి_2 == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (గమ్యం));
          తిరిగి false;
        }
    }
  తిరిగి స్థితి_2 == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

స్థిర bool
install_file_లోపల_file_parents (అక్షరం స్థిరరాశి *from, అక్షరం *గమ్యం,
                              స్థిరరాశి నిర్మాణం cp_options *x)
{
  తిరిగి (mkancesdirs_సురక్షిత_wd (from, గమ్యం, (నిర్మాణం cp_options *)x, false)
          && install_file_లోపల_file (from, గమ్యం, AT_FDCWD, గమ్యం, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

స్థిర bool
install_file_లోపల_dir (అక్షరం స్థిరరాశి *from, అక్షరం స్థిరరాశి *గమ్యం_dir,
                     స్థిరరాశి నిర్మాణం cp_options *x, bool mkdir_and_install,
                     పూర్ణాంకం *లక్ష్యం_dirfd)
{
  అక్షరం స్థిరరాశి *from_పునాది = చివరి_component (from);
  అక్షరం *గమ్యం_relname;
  అక్షరం *గమ్యం = file_name_concat (గమ్యం_dir, from_పునాది, &గమ్యం_relname);
  bool ret = true;

  ఒకవేళ (!లక్ష్యం_dirfd_valid (*లక్ష్యం_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_సురక్షిత_wd (from, గమ్యం, (నిర్మాణం cp_options *) x, true)))
    {
      పూర్ణాంకం fd = open (గమ్యం_dir, O_PATHSEARCH | O_DIRECTORY);
      ఒకవేళ (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (గమ్యం));
          ret = false;
        }
      లేకపోతే
        *లక్ష్యం_dirfd = fd;
    }

  ఒకవేళ (ret)
    {
      పూర్ణాంకం గమ్యం_dirfd = *లక్ష్యం_dirfd;
      ఒకవేళ (!లక్ష్యం_dirfd_valid (గమ్యం_dirfd))
        {
          గమ్యం_dirfd = AT_FDCWD;
          గమ్యం_relname = గమ్యం;
        }
      ret = install_file_లోపల_file (from, గమ్యం, గమ్యం_dirfd, గమ్యం_relname, x);
    }

  విడుదల (గమ్యం);
  తిరిగి ret;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం స్థిరరాశి *specified_బహుళకం = ఖాళీ;
  bool make_backups = false;
  అక్షరం స్థిరరాశి *backup_suffix = ఖాళీ;
  అక్షరం *సంస్కరణ_control_string = ఖాళీ;
  bool mkdir_and_install = false;
  నిర్మాణం cp_options x;
  అక్షరం స్థిరరాశి *లక్ష్యం_directory = ఖాళీ;
  bool no_లక్ష్యం_directory = false;
  bool strip_కార్యక్రమం_specified = false;
  అక్షరం స్థిరరాశి *scontext = ఖాళీ;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ఖాళీ))
         != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'b':
          make_backups = true;
          ఒకవేళ (optarg)
            సంస్కరణ_control_string = optarg;
          ఆపు;
        స్థితి 'c':
          ఆపు;
        స్థితి 'C':
          copy_only_if_needed = true;
          ఆపు;
        స్థితి 's':
          strip_files = true;
#నిర్వచితమైతే SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          సంకేతం (SIGCHLD, SIG_DFL);
#అయితే_ముగింపు
          ఆపు;
        స్థితి DEBUG_OPTION:
          x.debug = x.verbose = true;
          ఆపు;
        స్థితి STRIP_కార్యక్రమం_OPTION:
          strip_కార్యక్రమం = xstrdup (optarg);
          strip_కార్యక్రమం_specified = true;
          ఆపు;
        స్థితి 'd':
          dir_arg = true;
          ఆపు;
        స్థితి 'D':
          mkdir_and_install = true;
          ఆపు;
        స్థితి 'v':
          x.verbose = true;
          ఆపు;
        స్థితి 'g':
          సమూహం_name = optarg;
          ఆపు;
        స్థితి 'm':
          specified_బహుళకం = optarg;
          ఆపు;
        స్థితి 'o':
          owner_name = optarg;
          ఆపు;
        స్థితి 'p':
          x.preserve_timestamps = true;
          ఆపు;
        స్థితి 'S':
          make_backups = true;
          backup_suffix = optarg;
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

        స్థితి PRESERVE_CONTEXT_OPTION:
          ఒకవేళ (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              ఆపు;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          ఆపు;
        స్థితి 'Z':
          ఒకవేళ (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              ఒకవేళ (optarg)
                scontext = optarg;
              లేకపోతే
                x.సమితి_security_context = get_labeling_handle ();
            }
          లేకపోతే ఒకవేళ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  ఒకవేళ (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  ఒకవేళ (dir_arg && లక్ష్యం_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_సంస్కరణ (_("backup type"),
                                   సంస్కరణ_control_string)
                   : no_backups);
  సమితి_simple_backup_suffix (backup_suffix);

  ఒకవేళ (x.preserve_security_context && (x.సమితి_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  ఒకవేళ (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  పూర్ణాంకం n_files = argc - optind;
  అక్షరం **file = argv + optind;

  ఒకవేళ (n_files <= ! (dir_arg || లక్ష్యం_directory))
    {
      ఒకవేళ (n_files <= 0)
        error (0, 0, _("missing file operand"));
      లేకపోతే
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  నిర్మాణం stat sb;
  పూర్ణాంకం లక్ష్యం_dirfd = AT_FDCWD;
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
      ఒకవేళ (! (లక్ష్యం_dirfd_valid (లక్ష్యం_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (లక్ష్యం_directory));
    }
  లేకపోతే ఒకవేళ (!dir_arg)
    {
      అక్షరం స్థిరరాశి *lastfile = file[n_files - 1];
      పూర్ణాంకం fd = లక్ష్యం_directory_నిర్వాహ్యం (lastfile, &sb);
      ఒకవేళ (లక్ష్యం_dirfd_valid (fd))
        {
          లక్ష్యం_dirfd = fd;
          లక్ష్యం_directory = lastfile;
          n_files--;
        }
      లేకపోతే ఒకవేళ (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  ఒకవేళ (specified_బహుళకం)
    {
      నిర్మాణం బహుళకం_change *change = బహుళకం_compile (specified_బహుళకం);
      ఒకవేళ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_బహుళకం));
      బహుళకం_2 = బహుళకం_adjust (0, false, 0, change, ఖాళీ);
      dir_బహుళకం = బహుళకం_adjust (0, true, 0, change, &dir_బహుళకం_బిట్లు);
      విడుదల (change);
    }

  ఒకవేళ (strip_కార్యక్రమం_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  ఒకవేళ (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (copy_only_if_needed && extra_బహుళకం (బహుళకం_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  పూర్ణాంకం exit_స్థితి = EXIT_SUCCESS;
  ఒకవేళ (dir_arg)
    exit_స్థితి = savewd_process_files (n_files, file, process_dir, &x);
  లేకపోతే
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      ఒకవేళ (!లక్ష్యం_directory)
        {
          ఒకవేళ (! (mkdir_and_install
                 ? install_file_లోపల_file_parents (file[0], file[1], &x)
                 : install_file_లోపల_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_స్థితి = EXIT_FAILURE;
        }
      లేకపోతే
        {
          dest_info_init (&x);
          కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
            ఒకవేళ (! install_file_లోపల_dir (file[i], లక్ష్యం_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &లక్ష్యం_dirfd))
              exit_స్థితి = EXIT_FAILURE;
        }
    }

  main_exit (exit_స్థితి);
}
