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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>
#અન્તર્ભાવ <selinux/label.h>
#અન્તર્ભાવ <spawn.h>
#અન્તર્ભાવ <sys/wait.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "backupfile.h"
#અન્તર્ભાવ "cp-hash.h"
#અન્તર્ભાવ "copy.h"
#અન્તર્ભાવ "filenamecat.h"
#અન્તર્ભાવ "full-read.h"
#અન્તર્ભાવ "mkancesdirs.h"
#અન્તર્ભાવ "mkdir-p.h"
#અન્તર્ભાવ "modechange.h"
#અન્તર્ભાવ "prog-fprintf.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "savewd.h"
#અન્તર્ભાવ "selinux.h"
#અન્તર્ભાવ "stat-time.h"
#અન્તર્ભાવ "targetdir.h"
#અન્તર્ભાવ "utimens.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "install"

#define AUTHORS proper_name ("David MacKenzie")

સ્થિર પૂર્ણાંક selinux_enabled = 0;
સ્થિર bool use_default_selinux_context = true;

#જો ! HAVE_ENDGRENT
# define endgrent() ((શૂન્ય) 0)
#endif

#જો ! HAVE_ENDPWENT
# define endpwent() ((શૂન્ય) 0)
#endif

/* The user name that will own the files, or NULL to make the owner
   the current user ID. */
સ્થિર અક્ષર *owner_name;

/* The user ID corresponding to 'owner_name'. */
સ્થિર uid_t owner_id;

/* The group name that will own the files, or NULL to make the group
   the current group ID. */
સ્થિર અક્ષર *group_name;

/* The group ID corresponding to 'group_name'. */
સ્થિર gid_t group_id;

#define DEFAULT_MODE (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

/* The file mode bits to which non-directory files will be set.  The umask has
   no effect. */
સ્થિર mode_t mode = DEFAULT_MODE;

/* Similar, but for directories.  */
સ્થિર mode_t dir_mode = DEFAULT_MODE;

/* The file mode bits that the user cares about.  This should be a
   superset of DIR_MODE and a subset of CHMOD_MODE_BITS.  This matters
   for directories, since otherwise directories may keep their S_ISUID
   or S_ISGID bits.  */
સ્થિર mode_t dir_mode_bits = CHMOD_MODE_BITS;

/* Compare files before installing (-C) */
સ્થિર bool copy_only_if_needed;

/* If true, strip executable files after copying them. */
સ્થિર bool strip_files;

/* If true, install a directory instead of a regular file. */
સ્થિર bool dir_arg;

/* Program used to strip binaries, "strip" is default */
સ્થિર અક્ષર અચલ *strip_program = "strip";

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  DEBUG_OPTION = CHAR_MAX + 1,
  PRESERVE_CONTEXT_OPTION,
  STRIP_PROGRAM_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"backup", optional_argument, ખાલી, 'b'},
  {"compare", no_argument, ખાલી, 'C'},
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"debug", no_argument, ખાલી, DEBUG_OPTION},
  {"directory", no_argument, ખાલી, 'd'},
  {"group", required_argument, ખાલી, 'g'},
  {"mode", required_argument, ખાલી, 'm'},
  {"no-target-directory", no_argument, ખાલી, 'T'},
  {"owner", required_argument, ખાલી, 'o'},
  {"preserve-timestamps", no_argument, ખાલી, 'p'},
  {"preserve-context", no_argument, ખાલી, PRESERVE_CONTEXT_OPTION},
  {"strip", no_argument, ખાલી, 's'},
  {"strip-program", required_argument, ખાલી, STRIP_PROGRAM_OPTION},
  {"suffix", required_argument, ખાલી, 'S'},
  {"target-directory", required_argument, ખાલી, 't'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Compare content of opened files using file descriptors A_FD and B_FD. Return
   true if files are equal. */
સ્થિર bool
have_same_content (પૂર્ણાંક a_fd, પૂર્ણાંક b_fd)
{
  ગણના { CMP_BLOCK_SIZE = 4096 };
  સ્થિર અક્ષર a_buff[CMP_BLOCK_SIZE];
  સ્થિર અક્ષર b_buff[CMP_BLOCK_SIZE];

  idx_t size;
  જ્યારે (0 < (size = full_read (a_fd, a_buff, કદ a_buff))) {
    જો (size != full_read (b_fd, b_buff, કદ b_buff))
      પાછા false;

    જો (!memeq (a_buff, b_buff, size))
      પાછા false;
  }

  પાછા size == 0;
}

/* Return true for mode with non-permission bits. */
સ્થિર bool
extra_mode (mode_t આગત)
{
  mode_t mask = S_IRWXUGO | S_IFMT;
  પાછા !! (આગત & ~ mask);
}

/* Return true if copy of file SRC_NAME to file DEST_NAME aka
   DEST_DIRFD+DEST_RELNAME is necessary. */
સ્થિર bool
need_copy (અક્ષર અચલ *src_name, અક્ષર અચલ *dest_name,
           પૂર્ણાંક dest_dirfd, અક્ષર અચલ *dest_relname,
           અચલ સંરચના cp_options *x)
{
  જો (extra_mode (mode))
    પાછા true;

  /* compare files using stat */
  સંરચના stat src_sb;
  જો (stat (src_name, &src_sb) != 0)
    પાછા true;

  સંરચના stat dest_sb;
  જો (fstatat (dest_dirfd, dest_relname, &dest_sb, AT_SYMLINK_NOFOLLOW) != 0)
    પાછા true;

  જો (!S_ISREG (src_sb.st_mode) || !S_ISREG (dest_sb.st_mode)
      || extra_mode (src_sb.st_mode) || extra_mode (dest_sb.st_mode))
    પાછા true;

  જો (src_sb.st_size != dest_sb.st_size
      || (dest_sb.st_mode & CHMOD_MODE_BITS) != mode)
    પાછા true;

  જો (owner_id == (uid_t) -1)
    {
      errno = 0;
      uid_t ruid = getuid ();
      જો ((ruid == (uid_t) -1 && errno) || dest_sb.st_uid != ruid)
        પાછા true;
    }
  નહીંતર જો (dest_sb.st_uid != owner_id)
    પાછા true;

  જો (group_id == (uid_t) -1)
    {
      errno = 0;
      gid_t rgid = getgid ();
      જો ((rgid == (uid_t) -1 && errno) || dest_sb.st_gid != rgid)
        પાછા true;
    }
  નહીંતર જો (dest_sb.st_gid != group_id)
    પાછા true;

  /* compare SELinux context if preserving */
  જો (selinux_enabled && x->preserve_security_context)
    {
      અક્ષર *file_scontext_raw = ખાલી;
      જો (getfilecon_raw (src_name, &file_scontext_raw) == -1)
        પાછા true;

      અક્ષર *to_scontext_raw = ખાલી;
      જો (getfilecon_raw (dest_name, &to_scontext_raw) == -1)
        {
          freecon (file_scontext_raw);
          પાછા true;
        }

      bool scontext_match = streq (file_scontext_raw, to_scontext_raw);

      freecon (file_scontext_raw);
      freecon (to_scontext_raw);
      જો (!scontext_match)
        પાછા true;
    }

  /* compare files content */
  પૂર્ણાંક src_fd = open (src_name, O_RDONLY | O_દ્વિઅંકી);
  જો (src_fd < 0)
    પાછા true;

  પૂર્ણાંક dest_fd = openat (dest_dirfd, dest_relname, O_RDONLY | O_દ્વિઅંકી);
  જો (dest_fd < 0)
    {
      close (src_fd);
      પાછા true;
    }

  bool content_match = have_same_content (src_fd, dest_fd);

  close (src_fd);
  close (dest_fd);
  પાછા !content_match;
}

સ્થિર શૂન્ય
cp_option_init (સંરચના cp_options *x)
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
  x->set_security_context = ખાલી; /* Whether to set sys default context.  */
  x->preserve_xattr = false;
  x->verbose = false;
  x->dest_info = ખાલી;
  x->src_info = ખાલી;
}

સ્થિર સંરચના selabel_handle *
get_labeling_handle (શૂન્ય)
{
  સ્થિર bool initialized;
  સ્થિર સંરચના selabel_handle *hnd;
  જો (!initialized)
    {
      initialized = true;
      hnd = selabel_open (SELABEL_CTX_FILE, ખાલી, 0);
      જો (!hnd)
        error (0, errno, _("warning: security labeling handle failed"));
    }
  પાછા hnd;
}

/* Modify file context to match the specified policy.
   If an error occurs the file will remain with the default directory
   context.  Note this sets the context to that returned by selabel_lookup
   and thus discards MLS levels and user identity of the FILE.  */
સ્થિર શૂન્ય
setdefaultfilecon (અક્ષર અચલ *file)
{
  જો (selinux_enabled != 1)
    {
      /* Indicate no context found. */
      પાછા;
    }

  સંરચના stat st;
  જો (lstat (file, &st) != 0)
    પાછા;

  સંરચના selabel_handle *hnd = get_labeling_handle ();
  જો (!hnd)
    પાછા;

  અક્ષર *scontext_raw = ખાલી;
  જો (selabel_lookup_raw (hnd, &scontext_raw, file, st.st_mode) != 0)
    {
      જો (errno != ENOENT && ! ignorable_ctx_err (errno))
        error (0, errno, _("warning: %s: context lookup failed"),
               quotef (file));
      પાછા;
    }

  જો (lsetfilecon_raw (file, scontext_raw) < 0 && errno != ENOTSUP)
    error (0, errno,
           _("warning: %s: failed to change context to %s"),
           quotef_n (0, file), quote_n (1, scontext_raw));

  freecon (scontext_raw);
}

/* Report that directory DIR was made, if OPTIONS requests this.  */
સ્થિર શૂન્ય
announce_mkdir (અક્ષર અચલ *dir, શૂન્ય *options)
{
  સંરચના cp_options અચલ *x = options;
  જો (x->verbose)
    prog_fprintf (stdout, _("creating directory %s"), quoteaf (dir));
}

/* Make ancestor directory DIR, whose last file name component is
   COMPONENT, with options OPTIONS.  Assume the working directory is
   COMPONENT's parent.  */
સ્થિર પૂર્ણાંક
make_ancestor (અક્ષર અચલ *dir, અક્ષર અચલ *component, શૂન્ય *options)
{
  સંરચના cp_options અચલ *x = options;
  જો (x->set_security_context
      && defaultcon (x->set_security_context, component, S_IFDIR) < 0
      && ! ignorable_ctx_err (errno))
    error (0, errno, _("failed to set default creation context for %s"),
           quoteaf (dir));

  પૂર્ણાંક r = mkdir (component, DEFAULT_MODE);
  જો (r == 0)
    announce_mkdir (dir, options);
  પાછા r;
}

/* Process a command-line file name, for the -d option.  */
સ્થિર પૂર્ણાંક
process_dir (અક્ષર *dir, સંરચના savewd *wd, શૂન્ય *options)
{
  સંરચના cp_options અચલ *x = options;

  પૂર્ણાંક ret = (make_dir_parents (dir, wd, make_ancestor, options,
                               dir_mode, announce_mkdir,
                               dir_mode_bits, owner_id, group_id, false)
          ? EXIT_SUCCESS
          : EXIT_FAILURE);

  /* FIXME: Due to the current structure of make_dir_parents()
     we don't have the facility to call defaultcon() before the
     final component of DIR is created.  So for now, create the
     final component with the context from previous component
     and here we set the context for the final component. */
  જો (ret == EXIT_SUCCESS && x->set_security_context)
    {
      જો (! restorecon (x->set_security_context, last_component (dir), false)
          && ! ignorable_ctx_err (errno))
        error (0, errno, _("failed to restore context for %s"),
               quoteaf (dir));
    }

  પાછા ret;
}

ગણના copy_status { COPY_FAILED, COPY_OK, COPY_SKIPPED };

/* Copy file FROM onto file TO aka TO_DIRFD+TO_RELNAME, creating TO if
   necessary.  Return COPY_OK if successful or COPY_SKIPPED if the copy
   was skipped.  */

સ્થિર ગણના copy_status
copy_file (અક્ષર અચલ *from, અક્ષર અચલ *to,
           પૂર્ણાંક to_dirfd, અક્ષર અચલ *to_relname, અચલ સંરચના cp_options *x)
{
  જો (copy_only_if_needed && !need_copy (from, to, to_dirfd, to_relname, x))
    પાછા COPY_SKIPPED;

  /* Allow installing from non-regular files like /dev/null.
     Charles Karney reported that some Sun version of install allows that
     and that sendmail's installation process relies on the behavior.
     However, since !x->recursive, the call to "copy" will fail if FROM
     is a directory.  */

  bool copy_into_self;
  પાછા (copy (from, to, to_dirfd, to_relname, 0, x, &copy_into_self, ખાલી)
          ? COPY_OK : COPY_FAILED);
}

/* Set the attributes of file or directory NAME aka DIRFD+RELNAME.
   Return true if successful.  */

સ્થિર bool
change_attributes (અક્ષર અચલ *name, પૂર્ણાંક dirfd, અક્ષર અચલ *relname)
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

  જો (! (owner_id == (uid_t) -1 && group_id == (gid_t) -1)
      && lchownat (dirfd, relname, owner_id, group_id) != 0)
    error (0, errno, _("cannot change ownership of %s"), quoteaf (name));
  નહીંતર જો (chmodat (dirfd, relname, mode) != 0)
    error (0, errno, _("cannot change permissions of %s"), quoteaf (name));
  નહીંતર
    ok = true;

  જો (use_default_selinux_context)
    setdefaultfilecon (name);

  પાછા ok;
}

/* Set the timestamps of file DEST aka DIRFD+RELNAME to match those of SRC_SB.
   Return true if successful.  */

સ્થિર bool
change_timestamps (સંરચના stat અચલ *src_sb, અક્ષર અચલ *dest,
                   પૂર્ણાંક dirfd, અક્ષર અચલ *relname)
{
  સંરચના timespec timespec[2] = { get_stat_atime (src_sb),
                                  get_stat_mtime (src_sb) };

  જો (utimensat (dirfd, relname, timespec, 0))
    {
      error (0, errno, _("cannot set timestamps for %s"), quoteaf (dest));
      પાછા false;
    }
  પાછા true;
}

/* Strip the symbol table from the file NAME.
   We could dig the magic number out of the file first to
   determine whether to strip it, but the header files and
   magic numbers vary so much from system to system that making
   it portable would be very difficult.  Not worth the effort. */

સ્થિર bool
strip (અક્ષર અચલ *name)
{
  posix_spawnattr_t attr;
  posix_spawnattr_t *attrp = ખાલી;

  /* Try to use vfork for systems where it matters.  */
  જો (posix_spawnattr_init (&attr) == 0)
    {
      જો (posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK) == 0)
        attrp = &attr;
      નહીંતર
        posix_spawnattr_destroy (&attr);
    }

  /* Construct the arguments to 'strip'.  */
  અક્ષર *concat_name = ખાલી;
  અક્ષર અચલ *safe_name = name;
  જો (name && *name == '-')
    safe_name = concat_name = file_name_concat (".", name, ખાલી);
  અક્ષર અચલ *અચલ argv[] = { strip_program, safe_name, ખાલી };

  /* Run 'strip'.  */
  pid_t pid;
  પૂર્ણાંક result = posix_spawnp (&pid, strip_program, ખાલી, attrp,
                             (અક્ષર * અચલ *) argv, environ);

  bool ok = false;
  જો (result != 0)
    {
      error (0, result,
             streq (strip_program, "strip")
               ? _("cannot run %s")
               : _("cannot run strip program %s"),
             quoteaf (strip_program));
    }
  નહીંતર
    {
      /* Wait for 'strip' to complete, and emit a warning message on failure  */
      પૂર્ણાંક status;
      જો (waitpid (pid, &status, 0) < 0)
        error (0, errno, _("waiting for strip"));
      નહીંતર જો (! WIFEXITED (status) || WEXITSTATUS (status))
        error (0, 0, _("strip process terminated abnormally"));
      નહીંતર
        ok = true;
    }

  મુક્ત (concat_name);
  જો (attrp)
    posix_spawnattr_destroy (attrp);

  પાછા ok;
}

/* Initialize the user and group ownership of the files to install. */

સ્થિર શૂન્ય
get_ids (શૂન્ય)
{
  જો (owner_name)
    {
      સંરચના passwd *pw = getpwnam (owner_name);
      જો (pw == ખાલી)
        {
          uintmax_t tmp;
          જો (xstrtoumax (owner_name, ખાલી, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&owner_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid user %s"),
                   quoteaf (owner_name));
        }
      નહીંતર
        owner_id = pw->pw_uid;
      endpwent ();
    }
  નહીંતર
    owner_id = (uid_t) -1;

  જો (group_name)
    {
      સંરચના group *gr = getgrnam (group_name);
      જો (gr == ખાલી)
        {
          uintmax_t tmp;
          જો (xstrtoumax (group_name, ખાલી, 0, &tmp, "") != LONGINT_OK
              || ckd_add (&group_id, tmp, 0))
            error (EXIT_FAILURE, 0, _("invalid group %s"),
                   quoteaf (group_name));
        }
      નહીંતર
        group_id = gr->gr_gid;
      endgrent ();
    }
  નહીંતર
    group_id = (gid_t) -1;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
the existing DIRECTORY, જ્યારે setting permission modes and owner/group.\n\
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
         and જો no change to content, ownership, and permissions,\n\
         કરો not modify the destination at all\n\
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
         strip પ્રતીક tables\n\
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
         and each created directory, to મૂળભૂત type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or જો CTX is specified then set the\n\
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

સ્થિર bool
install_file_in_file (અક્ષર અચલ *from, અક્ષર અચલ *to,
                      પૂર્ણાંક to_dirfd, અક્ષર અચલ *to_relname,
                      અચલ સંરચના cp_options *x)
{
  સંરચના stat from_sb;
  જો (x->preserve_timestamps && stat (from, &from_sb) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (from));
      પાછા false;
    }
  ગણના copy_status copy_status = copy_file (from, to, to_dirfd, to_relname, x);
  જો (copy_status == COPY_FAILED)
    પાછા false;
  જો (copy_status == COPY_OK && strip_files && ! strip (to))
    {
      જો (unlinkat (to_dirfd, to_relname, 0) != 0)  /* Cleanup.  */
        error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (to));
      પાછા false;
    }
  જો (x->preserve_timestamps && (copy_status == COPY_SKIPPED || strip_files
                                 || ! S_ISREG (from_sb.st_mode))
      && ! change_timestamps (&from_sb, to, to_dirfd, to_relname))
    પાછા false;

  પાછા copy_status == COPY_SKIPPED
         || change_attributes (to, to_dirfd, to_relname);
}

/* Create any missing parent directories of TO,
   while maintaining the current Working Directory.
   Return true if successful.  */

સ્થિર bool
mkancesdirs_safe_wd (અક્ષર અચલ *from, અક્ષર *to, સંરચના cp_options *x,
                     bool save_always)
{
  bool save_working_directory =
    save_always
    || ! (IS_ABSOLUTE_FILE_NAME (from) && IS_ABSOLUTE_FILE_NAME (to));

  સંરચના savewd wd;
  savewd_init (&wd);
  જો (! save_working_directory)
    savewd_finish (&wd);

  પૂર્ણાંક status = EXIT_SUCCESS;
  જો (mkancesdirs (to, &wd, make_ancestor, x) == -1)
    {
      error (0, errno, _("cannot create directory %s"), quoteaf (to));
      status = EXIT_FAILURE;
    }

  જો (save_working_directory)
    {
      પૂર્ણાંક restore_result = savewd_restore (&wd, status);
      પૂર્ણાંક restore_errno = errno;
      savewd_finish (&wd);
      જો (EXIT_SUCCESS < restore_result)
        પાછા false;
      જો (restore_result < 0 && status == EXIT_SUCCESS)
        {
          error (0, restore_errno, _("cannot create directory %s"),
                 quoteaf (to));
          પાછા false;
        }
    }
  પાછા status == EXIT_SUCCESS;
}

/* Copy file FROM onto file TO, creating any missing parent directories of TO.
   Return true if successful.  */

સ્થિર bool
install_file_in_file_parents (અક્ષર અચલ *from, અક્ષર *to,
                              અચલ સંરચના cp_options *x)
{
  પાછા (mkancesdirs_safe_wd (from, to, (સંરચના cp_options *)x, false)
          && install_file_in_file (from, to, AT_FDCWD, to, x));
}

/* Copy file FROM into directory TO_DIR, keeping its same name,
   and give the copy the appropriate attributes.
   Return true if successful.  */

સ્થિર bool
install_file_in_dir (અક્ષર અચલ *from, અક્ષર અચલ *to_dir,
                     અચલ સંરચના cp_options *x, bool mkdir_and_install,
                     પૂર્ણાંક *target_dirfd)
{
  અક્ષર અચલ *from_base = last_component (from);
  અક્ષર *to_relname;
  અક્ષર *to = file_name_concat (to_dir, from_base, &to_relname);
  bool ret = true;

  જો (!target_dirfd_valid (*target_dirfd)
      && (ret = mkdir_and_install)
      && (ret = mkancesdirs_safe_wd (from, to, (સંરચના cp_options *) x, true)))
    {
      પૂર્ણાંક fd = open (to_dir, O_PATHSEARCH | O_DIRECTORY);
      જો (fd < 0)
        {
          error (0, errno, _("cannot open %s"), quoteaf (to));
          ret = false;
        }
      નહીંતર
        *target_dirfd = fd;
    }

  જો (ret)
    {
      પૂર્ણાંક to_dirfd = *target_dirfd;
      જો (!target_dirfd_valid (to_dirfd))
        {
          to_dirfd = AT_FDCWD;
          to_relname = to;
        }
      ret = install_file_in_file (from, to, to_dirfd, to_relname, x);
    }

  મુક્ત (to);
  પાછા ret;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર અચલ *specified_mode = ખાલી;
  bool make_backups = false;
  અક્ષર અચલ *backup_suffix = ખાલી;
  અક્ષર *version_control_string = ખાલી;
  bool mkdir_and_install = false;
  સંરચના cp_options x;
  અક્ષર અચલ *target_directory = ખાલી;
  bool no_target_directory = false;
  bool strip_program_specified = false;
  અક્ષર અચલ *scontext = ખાલી;
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

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "bcCsDdg:m:o:pt:TvS:Z", long_options,
                              ખાલી))
         != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'b':
          make_backups = true;
          જો (optarg)
            version_control_string = optarg;
          તોડો;
        સ્થિતિ 'c':
          તોડો;
        સ્થિતિ 'C':
          copy_only_if_needed = true;
          તોડો;
        સ્થિતિ 's':
          strip_files = true;
#ifdef SIGCHLD
          /* System V fork+wait does not work if SIGCHLD is ignored.  */
          સંકેત (SIGCHLD, SIG_DFL);
#endif
          તોડો;
        સ્થિતિ DEBUG_OPTION:
          x.debug = x.verbose = true;
          તોડો;
        સ્થિતિ STRIP_PROGRAM_OPTION:
          strip_program = xstrdup (optarg);
          strip_program_specified = true;
          તોડો;
        સ્થિતિ 'd':
          dir_arg = true;
          તોડો;
        સ્થિતિ 'D':
          mkdir_and_install = true;
          તોડો;
        સ્થિતિ 'v':
          x.verbose = true;
          તોડો;
        સ્થિતિ 'g':
          group_name = optarg;
          તોડો;
        સ્થિતિ 'm':
          specified_mode = optarg;
          તોડો;
        સ્થિતિ 'o':
          owner_name = optarg;
          તોડો;
        સ્થિતિ 'p':
          x.preserve_timestamps = true;
          તોડો;
        સ્થિતિ 'S':
          make_backups = true;
          backup_suffix = optarg;
          તોડો;
        સ્થિતિ 't':
          જો (target_directory)
            error (EXIT_FAILURE, 0,
                   _("multiple target directories specified"));
          target_directory = optarg;
          તોડો;
        સ્થિતિ 'T':
          no_target_directory = true;
          તોડો;

        સ્થિતિ PRESERVE_CONTEXT_OPTION:
          જો (! selinux_enabled)
            {
              error (0, 0, _("WARNING: ignoring --preserve-context; "
                             "this kernel is not SELinux-enabled"));
              તોડો;
            }
          x.preserve_security_context = true;
          use_default_selinux_context = false;
          તોડો;
        સ્થિતિ 'Z':
          જો (selinux_enabled)
            {
              /* Disable use of the install(1) specific setdefaultfilecon().
                 Note setdefaultfilecon() is different from the newer and more
                 generic restorecon() in that the former sets the context of
                 the dest files to that returned by selabel_lookup directly,
                 thus discarding MLS level and user identity of the file.
                 TODO: consider removing setdefaultfilecon() in future.  */
              use_default_selinux_context = false;

              જો (optarg)
                scontext = optarg;
              નહીંતર
                x.set_security_context = get_labeling_handle ();
            }
          નહીંતર જો (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux-enabled kernel"));
            }
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  /* Check for invalid combinations of arguments. */
  જો (dir_arg && strip_files)
    error (EXIT_FAILURE, 0,
           _("the strip option may not be used when installing a directory"));
  જો (dir_arg && target_directory)
    error (EXIT_FAILURE, 0,
           _("target directory not allowed when installing a directory"));

  x.backup_type = (make_backups
                   ? xget_version (_("backup type"),
                                   version_control_string)
                   : no_backups);
  set_simple_backup_suffix (backup_suffix);

  જો (x.preserve_security_context && (x.set_security_context || scontext))
    error (EXIT_FAILURE, 0,
           _("cannot set target context and preserve it"));

  જો (scontext && setfscreatecon (scontext) < 0)
    error (EXIT_FAILURE, errno,
           _("failed to set default file creation context to %s"),
         quote (scontext));

  પૂર્ણાંક n_files = argc - optind;
  અક્ષર **file = argv + optind;

  જો (n_files <= ! (dir_arg || target_directory))
    {
      જો (n_files <= 0)
        error (0, 0, _("missing file operand"));
      નહીંતર
        error (0, 0, _("missing destination file operand after %s"),
               quoteaf (file[0]));
      usage (EXIT_FAILURE);
    }

  સંરચના stat sb;
  પૂર્ણાંક target_dirfd = AT_FDCWD;
  જો (no_target_directory)
    {
      જો (target_directory)
        error (EXIT_FAILURE, 0,
               _("cannot combine --target-directory (-t) "
                 "and --no-target-directory (-T)"));
      જો (2 < n_files)
        {
          error (0, 0, _("extra operand %s"), quoteaf (file[2]));
          usage (EXIT_FAILURE);
        }
    }
  નહીંતર જો (target_directory)
    {
      target_dirfd = target_directory_operand (target_directory, &sb);
      જો (! (target_dirfd_valid (target_dirfd)
             || (mkdir_and_install && errno == ENOENT)))
        error (EXIT_FAILURE, errno, _("failed to access %s"),
               quoteaf (target_directory));
    }
  નહીંતર જો (!dir_arg)
    {
      અક્ષર અચલ *lastfile = file[n_files - 1];
      પૂર્ણાંક fd = target_directory_operand (lastfile, &sb);
      જો (target_dirfd_valid (fd))
        {
          target_dirfd = fd;
          target_directory = lastfile;
          n_files--;
        }
      નહીંતર જો (2 < n_files)
        error (EXIT_FAILURE, errno, _("target %s"), quoteaf (lastfile));
    }

  જો (specified_mode)
    {
      સંરચના mode_change *change = mode_compile (specified_mode);
      જો (!change)
        error (EXIT_FAILURE, 0, _("invalid mode %s"), quote (specified_mode));
      mode = mode_adjust (0, false, 0, change, ખાલી);
      dir_mode = mode_adjust (0, true, 0, change, &dir_mode_bits);
      મુક્ત (change);
    }

  જો (strip_program_specified && !strip_files)
    error (0, 0, _("WARNING: ignoring --strip-program option as -s option was "
                   "not specified"));

  જો (copy_only_if_needed && strip_files)
    {
      error (0, 0, _("options --compare (-C) and --strip are mutually "
                     "exclusive"));
      usage (EXIT_FAILURE);
    }

  જો (copy_only_if_needed && extra_mode (mode))
    error (0, 0, _("the --compare (-C) option is ignored when you"
                   " specify a mode with non-permission bits"));

  get_ids ();

  પૂર્ણાંક exit_status = EXIT_SUCCESS;
  જો (dir_arg)
    exit_status = savewd_process_files (n_files, file, process_dir, &x);
  નહીંતર
    {
      /* FIXME: it's a little gross that this initialization is
         required by copy.c::copy. */
      hash_init ();

      જો (!target_directory)
        {
          જો (! (mkdir_and_install
                 ? install_file_in_file_parents (file[0], file[1], &x)
                 : install_file_in_file (file[0], file[1], AT_FDCWD,
                                         file[1], &x)))
            exit_status = EXIT_FAILURE;
        }
      નહીંતર
        {
          dest_info_init (&x);
          માટે (પૂર્ણાંક i = 0; i < n_files; i++)
            જો (! install_file_in_dir (file[i], target_directory, &x,
                                       i == 0 && mkdir_and_install,
                                       &target_dirfd))
              exit_status = EXIT_FAILURE;
        }
    }

  main_exit (exit_status);
}
