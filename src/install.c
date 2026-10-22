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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <selinux/label.h>
#अन्तर्भाव <spawn.h>
#अन्तर्भाव <sys/wait.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "backupfile.h"
#अन्तर्भाव "cp-hash.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "filenamecat.h"
#अन्तर्भाव "full-read.h"
#अन्तर्भाव "mkancesdirs.h"
#अन्तर्भाव "mkdir-p.h"
#अन्तर्भाव "modechange.h"
#अन्तर्भाव "prog-fprintf.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "savewd.h"
#अन्तर्भाव "selinux.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "targetdir.h"
#अन्तर्भाव "utimens.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "install"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

स्थिर पूर्णाङ्क selinux_enabled = 0;
स्थिर bool use_default_selinux_context = true;

#यदि ! HAVE_ENDGRENT
# define endgrent() ((शून्य) 0)
#यदि_अन्तः

#यदि ! HAVE_ENDPWENT
# define endpwent() ((शून्य) 0)
#यदि_अन्तः

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
स्थिर वर्ण *owner_name;

/* The user ID corresponding to 'owner_name'. */
स्थिर uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
स्थिर वर्ण *गणः_name;

/* The group ID corresponding to 'group_name'. */
स्थिर gid_t गणः_id;

#परिभाषय DEFAULT_बहुलकम् (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
स्थिर mode_t बहुलकम्_2 = DEFAULT_बहुलकम्;

/* Similar, but for directories.  */
स्थिर mode_t dir_बहुलकम् = DEFAULT_बहुलकम्;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
स्थिर mode_t dir_बहुलकम्_द्वयङ्काः = CHMOD_बहुलकम्_द्वयङ्काः;

/* Compare files before installing (-C) */
स्थिर bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
स्थिर bool strip_files;

/* If true, install a directory instead of a regular file. */
स्थिर bool dir_arg;

/* Program used to strip binaries, "strip" is default */
स्थिर वर्ण ध्रुव *strip_क्रमादेशः = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_क्रमादेशः_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"backup", optional_तर्कः, रिक्त, 'b'},
  {"compare", no_तर्कः, रिक्त, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_तर्कः, रिक्त, DEBUG_OPTION},
  {"directory", no_तर्कः, रिक्त, 'd'},
  {"group", required_तर्कः, रिक्त, 'g'},
  {"mode", required_तर्कः, रिक्त, 'm'},
  {"no-target-directory", no_तर्कः, रिक्त, 'T'},
  {"owner", required_तर्कः, रिक्त, 'o'},
  {"preserve-timestamps", no_तर्कः, रिक्त, 'p'},
  {"preserve-context", no_तर्कः, रिक्त, PRESERVE_CONTEXT_OPTION},
  {"strip", no_तर्कः, रिक्त, 's'},
  {"strip-program", required_तर्कः, रिक्त, STRIP_क्रमादेशः_OPTION},
  {"suffix", required_तर्कः, रिक्त, 'S'},
  {"target-directory", required_तर्कः, रिक्त, 't'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
स्थिर bool
have_same_content (पूर्णाङ्क a_fd, पूर्णाङ्क b_fd)
{
  गणना { CMP_खण्डम्_आकार = 4096 };
  स्थिर वर्ण a_buff[CMP_खण्डम्_आकार];
  स्थिर वर्ण b_buff[CMP_खण्डम्_आकार];

  idx_t आकार;
  यावत् (0 < (आकार = पूर्णम्_read (a_fd, a_buff, परिमाण a_buff))) {
    यदि (आकार != पूर्णम्_read (b_fd, b_buff, परिमाण b_buff))
      प्रत्यागम false;

    यदि (!memeq (a_buff, b_buff, आकार))
      प्रत्यागम false;
  }

  प्रत्यागम आकार == 0;
}

/* Return true for mode with non-permission bits. */
स्थिर bool
extra_बहुलकम् (mode_t निवेशः)
{
  mode_t आवरणम् = S_IRWXUGO | S_IFMT;
  प्रत्यागम !! (निवेशः & ~ आवरणम्);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
स्थिर bool
need_copy (वर्ण ध्रुव *src_name, वर्ण ध्रुव *dest_name,
           पूर्णाङ्क dest_dirfd, वर्ण ध्रुव *dest_relname,
           ध्रुव संरचना cp_options *x)
{
  यदि (extra_बहुलकम् (बहुलकम्_2))
    प्रत्यागम true;

  /* compare files using stat */
  संरचना stat src_sb;
  यदि (stat (src_name, &src_sb) != 0)
    प्रत्यागम true;

  संरचना stat dest_sb;
  यदि (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    प्रत्यागम true;

  यदि (!S_ISREG (src_sb.st_बहुलकम्) || !S_ISREG (dest_sb.st_बहुलकम्)
      || extra_बहुलकम् (src_sb.st_बहुलकम्) || extra_बहुलकम् (dest_sb.st_बहुलकम्))
    प्रत्यागम true;

  यदि (src_sb.st_आकार != dest_sb.st_आकार
      || (dest_sb.st_बहुलकम् & CHMOD_बहुलकम्_द्वयङ्काः) != बहुलकम्_2)
    प्रत्यागम true;

  यदि (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      यदि ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        प्रत्यागम true;
    }
  अन्यथा यदि (dest_sb.st_uid != owner_id)
    प्रत्यागम true;

  यदि (गणः_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      यदि ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        प्रत्यागम true;
    }
  अन्यथा यदि (dest_sb.st_gid != गणः_id)
    प्रत्यागम true;

  /* compare SELinux context if preserving */
  यदि (selinux_enabled && x->preserve_security_context)
    {
      वर्ण *file_scontext_raw = रिक्त;
      यदि (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        प्रत्यागम true;

      वर्ण *गन्तव्यम्_scontext_raw = रिक्त;
      यदि (getfilecon_raw (dest_name, &गन्तव्यम्_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          प्रत्यागम true;
        }

      bool scontext_मेलनम् = streq (file_scontext_raw, गन्तव्यम्_scontext_raw);

      freecon (file_scontext_raw);
      freecon (गन्तव्यम्_scontext_raw);
      यदि (!scontext_मेलनम्)
        प्रत्यागम true;
    }

  /* compare files content */
  पूर्णाङ्क src_fd = उद्घाटय (src_name, O_RDONLY | O_द्विमान);
  यदि (src_fd < 0)
    प्रत्यागम true;

  पूर्णाङ्क dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_द्विमान);
  यदि (dest_fd < 0)
    {
      close (src_fd);
      प्रत्यागम true;
    }

  bool content_मेलनम् = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  प्रत्यागम !content_मेलनम्;
}

स्थिर शून्य
cp_option_init (संरचना cp_options *x)
{
  cp_options_default (x);
  x->copy_as_regular = true;
  x->reflink_बहुलकम् = REFLINK_AUTO;
  x->dereference = DEREF_ALWAYS;
  x->unlink_dest_before_opening = true;
  x->unlink_dest_after_failed_open = false;
  x->hard_link = false;
  x->interactive = I_UNSPECIFIED;
  x->move_बहुलकम् = false;
  x->install_बहुलकम् = true;
  x->one_file_system = false;
  x->preserve_ownership = false;
  x->preserve_links = false;
  x->preserve_बहुलकम् = false;
  x->preserve_timestamps = false;
  x->explicit_no_preserve_बहुलकम् = false;
  x->reduce_diagnostics=false;
  x->दत्त_copy_required = true;
  x->require_preserve = false;
  x->require_preserve_xattr = false;
  x->recursive = false;
  x->sparse_बहुलकम् = SPARSE_AUTO;
  x->symbolic_link = false;
  x->backup_type = no_backups;

  /* Create destination files initially writable so we can run strip on them.
     Although GNU strip works fine on read-only files, some others
     would fail.  */
  x->समुच्चयः_बहुलकम् = true;
  x->बहुलकम्_2 = S_IRUSR | S_IWUSR;
  x->stdin_tty = false;

  x->open_dangling_dest_symlink = false;
  x->update = UPDATE_ALL;
  x->require_preserve_context = false;  /* Not used by install currently.  */
  x->preserve_security_context = false; /* Whether to copy context from src.  */
  x->समुच्चयः_security_context = रिक्त; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = रिक्त;
  x->src_info = रिक्त;
}

स्थिर संरचना selabel_handle *
get_labeling_handle (शून्य)
{
  स्थिर bool initialized;
  स्थिर संरचना selabel_handle *hnd;
  यदि (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, रिक्त, 0);
      यदि (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  प्रत्यागम hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
स्थिर शून्य
setdefaultfilecon (वर्ण ध्रुव *file)
{
  यदि (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      प्रत्यागम;
    }

  संरचना stat st;
  यदि (lstat (file, &st) != 0)
    प्रत्यागम;

  संरचना selabel_handle *hnd = get_labeling_handle ();
  यदि (!hnd)
    प्रत्यागम;

  वर्ण *scontext_raw = रिक्त;
  यदि (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_बहुलकम्) != 0)
    {
      यदि (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      प्रत्यागम;
    }

  यदि (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
स्थिर शून्य
announce_mkdir (वर्ण ध्रुव *dir, शून्य *options)
{
  संरचना cp_options ध्रुव *x = options;
  यदि (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
स्थिर पूर्णाङ्क
make_ancestor (वर्ण ध्रुव *dir, वर्ण ध्रुव *component, शून्य *options)
{
  संरचना cp_options ध्रुव *x = options;
  यदि (x->समुच्चयः_security_context
      && defaultcon (x->समुच्चयः_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  पूर्णाङ्क r = mkdir (component, DEFAULT_बहुलकम्);
  यदि (r == 0)
    announce_mkdir (dir, options);
  प्रत्यागम r;
}

/* Process a command-line file name, for the -d option.  */
स्थिर पूर्णाङ्क
process_dir (वर्ण *dir, संरचना savewd *wd, शून्य *options)
{
  संरचना cp_options ध्रुव *x = options;

  पूर्णाङ्क ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_बहुलकम्, announce_mkdir,
                               dir_बहुलकम्_द्वयङ्काः, owner_id, गणः_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  यदि (ret == EXIT_SUCCESS && x->समुच्चयः_security_context)
    {
      यदि (! restorecon (x->समुच्चयः_security_context, चरमः_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  प्रत्यागम ret;
}

गणना copy_स्थितिः { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

स्थिर गणना copy_स्थितिः
copy_file (वर्ण ध्रुव *from, वर्ण ध्रुव *गन्तव्यम्,
           पूर्णाङ्क गन्तव्यम्_dirfd, वर्ण ध्रुव *गन्तव्यम्_relname, ध्रुव संरचना cp_options *x)
{
  यदि (copy_only_if_needed && !need_copy (from, गन्तव्यम्, गन्तव्यम्_dirfd, गन्तव्यम्_relname, x))
    प्रत्यागम COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  प्रत्यागम (copy (from, गन्तव्यम्, गन्तव्यम्_dirfd, गन्तव्यम्_relname, 0, x, &copy_into_self, रिक्त)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

स्थिर bool
change_गुणाः (वर्ण ध्रुव *name, पूर्णाङ्क dirfd, वर्ण ध्रुव *relname)
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

  यदि (! (owner_id == (uid_t) -1 && गणः_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, गणः_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  अन्यथा यदि (chmodat (dirfd, relname, बहुलकम्_2) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  अन्यथा
    ok = true;

  यदि (use_default_selinux_context)
    setdefaultfilecon (name);

  प्रत्यागम ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

स्थिर bool
change_timestamps (संरचना stat ध्रुव *src_sb, वर्ण ध्रुव *dest,
                   पूर्णाङ्क dirfd, वर्ण ध्रुव *relname)
{
  संरचना timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  यदि (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      प्रत्यागम false;
    }
  प्रत्यागम true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

स्थिर bool
strip (वर्ण ध्रुव *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = रिक्त;

  /* Try to use vfork for systems where it matters.  */
  यदि (posix_spawnattr_init (&attr) == 0)
    {
      यदि (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      अन्यथा
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  वर्ण *concat_name = रिक्त;
  वर्ण ध्रुव *safe_name = name;
  यदि (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, रिक्त);
  वर्ण ध्रुव *ध्रुव argv[] = { strip_क्रमादेशः, safe_name, रिक्त };

  /* Run 'strip'.  */
  pid_t pid;
  पूर्णाङ्क परिणाम = posix_spawnp (&pid, strip_क्रमादेशः, रिक्त, attrp,
                             (वर्ण * ध्रुव *) argv, environ);

  bool ok = false;
  यदि (परिणाम != 0)
    {
      error (0, परिणाम,
             streq (strip_क्रमादेशः, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_क्रमादेशः));
    }
  अन्यथा
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      पूर्णाङ्क स्थितिः;
      यदि (waitpid (pid, &स्थितिः, 0) < 0)
        error (0, errno, _("waiting for strip"));
      अन्यथा यदि (! WIFEXITED (स्थितिः) || WEXITSTATUS (स्थितिः))
        error (0, 0, _("strip process terminated abnormally"));
      अन्यथा
        ok = true;
    }

  मुक्त (concat_name);
  यदि (attrp)
    posix_spawnattr_destroy (attrp);

  प्रत्यागम ok;
}

/* Initialize the user and group ownership of the files to install. */

स्थिर शून्य
get_ids (शून्य)
{
  यदि (owner_name)
    {
      संरचना passwd *pw = getpwnam (owner_name);
      यदि (pw == रिक्त)
        {
          uintmax_t tmp;
          यदि (xstrtoumax (owner_name, रिक्त, 0, &tmp, "") != LONGINT_OK
              || ckd_सङ्कलय (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      अन्यथा
        owner_id = pw->pw_uid;
      endpwent ();
    }
  अन्यथा
    owner_id = (uid_t) -1;

  यदि (गणः_name)
    {
      संरचना गणः *gr = getgrnam (गणः_name);
      यदि (gr == रिक्त)
        {
          uintmax_t tmp;
          यदि (xstrtoumax (गणः_name, रिक्त, 0, &tmp, "") != LONGINT_OK
              || ckd_सङ्कलय (&गणः_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (गणः_name));
        }
      अन्यथा
        गणः_id = gr->gr_gid;
      endgrent ();
    }
  अन्यथा
    गणः_id = (gid_t) -1;
}

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
  or:  %s [OPTION]... -d DIRECTORY...\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
\n\
This install क्रमादेशः copies files (often just compiled) into destination\n\
locations you वृणु.  If you want गन्तव्यम् download and install a ready-गन्तव्यम्-use\n\
package on a GNU/Linux system, you should instead be using a package manager\n\
like yum(1) or apt-get(1).\n\
\n\
अन्तर्_2 the first three forms, copy SOURCE गन्तव्यम् DEST or गुणजः SOURCE(s) गन्तव्यम्\n\
the existing DIRECTORY, यावत् setting permission modes and owner/गणः.\n\
अन्तर्_2 the 4th form, create all components सम्बन्धि the given DIRECTORY(ies).\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
      --backup[=CONTROL]\n\
         make a backup सम्बन्धि each existing destination file\n\
"));
      oputs (_("\
  -b\n\
         like --backup but does not accept an तर्कः\n\
"));
      oputs (_("\
  -c\n\
         (ignored)\n\
"));
      oputs (_("\
  -C, --compare\n\
         compare content सम्बन्धि source and destination files,\n\
         and यदि no change गन्तव्यम् content, ownership, and permissions,\n\
         कुरु not modify the destination at all\n\
"));
      oputs (_("\
  -d, --directory\n\
         treat all arguments as directory नामानि;\n\
         create all components सम्बन्धि the specified directories\n\
"));
      oputs (_("\
  -D\n\
         create all leading components सम्बन्धि DEST except the चरमः,\n\
         or all components सम्बन्धि --target-directory,\n\
         then copy SOURCE गन्तव्यम् DEST\n\
"));
      oputs (_("\
      --debug\n\
         explain how a file is copied.  उपपादयति_2 -v\n\
"));
      oputs (_("\
  -g, --गणः=गणः_2\n\
         समुच्चयः_2 गणः ownership, instead सम्बन्धि process' current गणः\n\
"));
      oputs (_("\
  -m, --बहुलकम्_2=बहुलकम्\n\
         समुच्चयः_2 permission बहुलकम्_2 (as अन्तर् chmod), instead सम्बन्धि rwxr-xr-x\n\
"));
      oputs (_("\
  -o, --owner=OWNER\n\
         समुच्चयः_2 ownership (super-user only)\n\
"));
      oputs (_("\
  -p, --preserve-timestamps\n\
         apply access/modification times सम्बन्धि SOURCE files\n\
         गन्तव्यम् corresponding destination files\n\
"));
      oputs (_("\
  -s, --strip\n\
         strip चिह्नम् tables\n\
"));
      oputs (_("\
      --strip-क्रमादेशः=क्रमादेशः_2\n\
         क्रमादेशः प्रयुक्तम् गन्तव्यम् strip binaries\n\
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
  -v, --verbose\n\
         print the name सम्बन्धि each created file or directory\n\
"));
      oputs (_("\
      --preserve-context\n\
         preserve SELinux security context\n\
"));
      oputs (_("\
  -Z\n\
         समुच्चयः_2 SELinux security context सम्बन्धि destination file\n\
         and each created directory, गन्तव्यम् स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or यदि CTX is specified then समुच्चयः_2 the\n\
         SELinux or SMACK security context गन्तव्यम् CTX\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_backup_suffix_note ();
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME and give TO the
   appropriate attributes.  X gives the command options.
   Return true if successful.  */

स्थिर bool
install_file_अन्तर्_file (वर्ण ध्रुव *from, वर्ण ध्रुव *गन्तव्यम्,
                      पूर्णाङ्क गन्तव्यम्_dirfd, वर्ण ध्रुव *गन्तव्यम्_relname,
                      ध्रुव संरचना cp_options *x)
{
  संरचना stat from_sb;
  यदि (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      प्रत्यागम false;
    }
  गणना copy_स्थितिः copy_स्थितिः = copy_file (from, गन्तव्यम्, गन्तव्यम्_dirfd, गन्तव्यम्_relname, x);
  यदि (copy_स्थितिः == COPY_FAILED)
    प्रत्यागम false;
  यदि (copy_स्थितिः == COPY_OK && strip_files && ! strip (गन्तव्यम्))
    {
      यदि (unlinkat (गन्तव्यम्_dirfd, गन्तव्यम्_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (गन्तव्यम्));
      प्रत्यागम false;
    }
  यदि (x->preserve_timestamps && (copy_स्थितिः == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_बहुलकम्))
      && ! change_timestamps (&from_sb, गन्तव्यम्, गन्तव्यम्_dirfd, गन्तव्यम्_relname))
    प्रत्यागम false;

  प्रत्यागम copy_स्थितिः == COPY_SKIPPED
         || change_गुणाः (गन्तव्यम्, गन्तव्यम्_dirfd, गन्तव्यम्_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

स्थिर bool
mkancesdirs_safe_wd (वर्ण ध्रुव *from, वर्ण *गन्तव्यम्, संरचना cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (गन्तव्यम्));

  संरचना savewd wd;
  savewd_init (&wd);
  यदि (! save_working_directory)
    savewd_finish (&wd);

  पूर्णाङ्क स्थितिः = EXIT_SUCCESS;
  यदि (mkancesdirs (गन्तव्यम्, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (गन्तव्यम्));
      स्थितिः = EXIT_FAILURE;
    }

  यदि (save_working_directory)
    {
      पूर्णाङ्क restore_परिणाम = savewd_restore (&wd, स्थितिः);
      पूर्णाङ्क restore_errno = errno;
      savewd_finish (&wd);
      यदि (EXIT_SUCCESS < restore_परिणाम)
        प्रत्यागम false;
      यदि (restore_परिणाम < 0 && स्थितिः == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (गन्तव्यम्));
          प्रत्यागम false;
        }
    }
  प्रत्यागम स्थितिः == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

स्थिर bool
install_file_अन्तर्_file_parents (वर्ण ध्रुव *from, वर्ण *गन्तव्यम्,
                              ध्रुव संरचना cp_options *x)
{
  प्रत्यागम (mkancesdirs_safe_wd (from, गन्तव्यम्, (संरचना cp_options *)x, false)
          && install_file_अन्तर्_file (from, गन्तव्यम्, AT_FDCWD, गन्तव्यम्, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

स्थिर bool
install_file_अन्तर्_dir (वर्ण ध्रुव *from, वर्ण ध्रुव *गन्तव्यम्_dir,
                     ध्रुव संरचना cp_options *x, bool mkdir_and_install,
                     पूर्णाङ्क *target_dirfd)
{
  वर्ण ध्रुव *from_आधारः = चरमः_component (from);
  वर्ण *गन्तव्यम्_relname;
  वर्ण *गन्तव्यम् = file_name_concat (गन्तव्यम्_dir, from_आधारः, &गन्तव्यम्_relname);
  bool ret = true;

  यदि (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, गन्तव्यम्, (संरचना cp_options *) x, true)))
    {
      पूर्णाङ्क fd = उद्घाटय (गन्तव्यम्_dir, O_PATHSEARCH | O_DIRECTORY);
      यदि (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (गन्तव्यम्));
          ret = false;
        }
      अन्यथा
        *target_dirfd = fd;
    }

  यदि (ret)
    {
      पूर्णाङ्क गन्तव्यम्_dirfd = *target_dirfd;
      यदि (!target_dirfd_valid (गन्तव्यम्_dirfd))
        {
          गन्तव्यम्_dirfd = AT_FDCWD;
          गन्तव्यम्_relname = गन्तव्यम्;
        }
      ret = install_file_अन्तर्_file (from, गन्तव्यम्, गन्तव्यम्_dirfd, गन्तव्यम्_relname, x);
    }

  मुक्त (गन्तव्यम्);
  प्रत्यागम ret;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण ध्रुव *specified_बहुलकम् = रिक्त;
  bool make_backups = false;
  वर्ण ध्रुव *backup_suffix = रिक्त;
  वर्ण *संस्करणम्_control_string = रिक्त;
  bool mkdir_and_install = false;
  संरचना cp_options x;
  वर्ण ध्रुव *target_directory = रिक्त;
  bool no_target_directory = false;
  bool strip_क्रमादेशः_specified = false;
  वर्ण ध्रुव *scontext = रिक्त;
  /* set iff kernel has extra selinux system calls */
  selinux_enabled = (0 < is_selinux_enabled ());

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdin);

  cp_option_init (&x);

  umask (0);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'b':
          make_backups = true;
          यदि (optarg)
            संस्करणम्_control_string = optarg;
          भङ्ग;
        पक्ष 'c':
          भङ्ग;
        पक्ष 'C':
          copy_only_if_needed = true;
          भङ्ग;
        पक्ष 's':
          strip_files = true;
#यदि_परिभाषितम् SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          सङ्केत (SIGCHLD, SIG_DFL);
#यदि_अन्तः
          भङ्ग;
        पक्ष DEBUG_OPTION:
          x.debug = x.verbose = true;
          भङ्ग;
        पक्ष STRIP_क्रमादेशः_OPTION:
          strip_क्रमादेशः = xstrdup (optarg);
          strip_क्रमादेशः_specified = true;
          भङ्ग;
        पक्ष 'd':
          dir_arg = true;
          भङ्ग;
        पक्ष 'D':
          mkdir_and_install = true;
          भङ्ग;
        पक्ष 'v':
          x.verbose = true;
          भङ्ग;
        पक्ष 'g':
          गणः_name = optarg;
          भङ्ग;
        पक्ष 'm':
          specified_बहुलकम् = optarg;
          भङ्ग;
        पक्ष 'o':
          owner_name = optarg;
          भङ्ग;
        पक्ष 'p':
          x.preserve_timestamps = true;
          भङ्ग;
        पक्ष 'S':
          make_backups = true;
          backup_suffix = optarg;
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

        पक्ष PRESERVE_CONTEXT_OPTION:
          यदि (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              भङ्ग;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          भङ्ग;
        पक्ष 'Z':
          यदि (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              यदि (optarg)
                scontext = optarg;
              अन्यथा
                x.समुच्चयः_security_context = get_labeling_handle ();
            }
          अन्यथा यदि (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  यदि (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  यदि (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_संस्करणम् (_("backup type"),
                                   संस्करणम्_control_string)
                   : no_backups);
  समुच्चयः_simple_backup_suffix (backup_suffix);

  यदि (x.preserve_security_context && (x.समुच्चयः_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  यदि (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  पूर्णाङ्क n_files = argc - optind;
  वर्ण **file = argv + optind;

  यदि (n_files <= ! (dir_arg || target_directory))
    {
      यदि (n_files <= 0)
        error (0, 0, _("missing file operand"));
      अन्यथा
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  संरचना stat sb;
  पूर्णाङ्क target_dirfd = AT_FDCWD;
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
      यदि (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  अन्यथा यदि (!dir_arg)
    {
      वर्ण ध्रुव *lastfile = file[n_files - 1];
      पूर्णाङ्क fd = target_directory_सङ्कार्यम् (lastfile, &sb);
      यदि (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      अन्यथा यदि (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  यदि (specified_बहुलकम्)
    {
      संरचना बहुलकम्_change *change = बहुलकम्_compile (specified_बहुलकम्);
      यदि (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_बहुलकम्));
      बहुलकम्_2 = बहुलकम्_adjust (0, false, 0, change, रिक्त);
      dir_बहुलकम् = बहुलकम्_adjust (0, true, 0, change, &dir_बहुलकम्_द्वयङ्काः);
      मुक्त (change);
    }

  यदि (strip_क्रमादेशः_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  यदि (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  यदि (copy_only_if_needed && extra_बहुलकम् (बहुलकम्_2))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  पूर्णाङ्क exit_स्थितिः = EXIT_SUCCESS;
  यदि (dir_arg)
    exit_स्थितिः = savewd_process_files (n_files, file, process_dir, &x);
  अन्यथा
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      यदि (!target_directory)
        {
          यदि (! (mkdir_and_install
                 ? install_file_अन्तर्_file_parents (file[0], file[1], &x)
                 : install_file_अन्तर्_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_स्थितिः = EXIT_FAILURE;
        }
      अन्यथा
        {
          dest_info_init (&x);
          प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
            यदि (! install_file_अन्तर्_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_स्थितिः = EXIT_FAILURE;
        }
    }

  main_exit (exit_स्थितिः);
}
